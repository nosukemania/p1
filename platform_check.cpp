#include <iostream>
#include <cstdlib>
#include <memory>
#include <array>
#include <string>
#include <vector>
#include <map>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    
    if (!pipe) {
        throw std::runtime_error("コマンドの実行に失敗しました");
    }
    
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

class NetworkManager {
public:
    NetworkManager() {
        #if defined(_WIN32) || defined(_WIN64)
            commands = {
                {"interfaces", "netsh interface show interface"},
                {"ip", "ipconfig /all"},
                {"dns", "ipconfig /displaydns"},
                {"status", "netstat -n"}
            };
        #elif defined(__APPLE__) || defined(__MACH__)
            commands = {
                {"interfaces", "networksetup -listallhardwareports"},
                {"ip", "ifconfig"},
                {"dns", "scutil --dns"},
                {"status", "netstat -nr"}
            };
        #elif defined(__linux__)
            commands = {
                {"interfaces", "ip link show"},
                {"ip", "ip addr show"},
                {"dns", "cat /etc/resolv.conf"},
                {"status", "ss -tulpn"}
            };
        #endif
    }

    std::string getNetworkInterfaces() {
        return exec(commands["interfaces"].c_str());
    }

    std::string getIPConfiguration() {
        return exec(commands["ip"].c_str());
    }

    std::string getDNSSettings() {
        return exec(commands["dns"].c_str());
    }

    std::string getConnectionStatus() {
        return exec(commands["status"].c_str());
    }

    bool setDNS(const std::string& interface, const std::string& dns) {
        std::string cmd;
        #if defined(_WIN32) || defined(_WIN64)
            cmd = "netsh interface ip set dns \"" + interface + "\" static " + dns;
        #elif defined(__APPLE__) || defined(__MACH__)
            cmd = "networksetup -setdnsservers \"" + interface + "\" " + dns;
        #elif defined(__linux__)
            // Linux では直接 resolv.conf を編集する必要があるため、
            // root権限が必要です
            cmd = "echo 'nameserver " + dns + "' > /etc/resolv.conf";
        #endif
        
        try {
            exec(cmd.c_str());
            return true;
        } catch (const std::runtime_error&) {
            return false;
        }
    }

private:
    std::map<std::string, std::string> commands;
};

int main() {
    std::string os_type;
    
    #if defined(_WIN32) || defined(_WIN64)
        os_type = "Windows";
    #elif defined(__APPLE__) || defined(__MACH__)
        os_type = "Mac";
    #elif defined(__linux__)
        os_type = "Linux";
    #else
        std::cout << "不明な環境で実行されています" << std::endl;
        return 1;
    #endif

    std::cout << os_type << "環境で実行されています" << std::endl;

    NetworkManager nm;
    int choice;
    
    while (true) {
        std::cout << "\n実行したい操作を選択してください：" << std::endl;
        std::cout << "1. ネットワークインターフェース一覧の表示" << std::endl;
        std::cout << "2. IP設定の表示" << std::endl;
        std::cout << "3. DNS設定の表示" << std::endl;
        std::cout << "4. ネットワーク接続状態の表示" << std::endl;
        std::cout << "5. DNS設定の変更" << std::endl;
        std::cout << "0. 終了" << std::endl;
        std::cout << "選択: ";
        
        std::cin >> choice;

        try {
            switch (choice) {
                case 1:
                    std::cout << "\nネットワークインターフェース一覧:\n" 
                              << nm.getNetworkInterfaces() << std::endl;
                    break;
                case 2:
                    std::cout << "\nIP設定:\n" 
                              << nm.getIPConfiguration() << std::endl;
                    break;
                case 3:
                    std::cout << "\nDNS設定:\n" 
                              << nm.getDNSSettings() << std::endl;
                    break;
                case 4:
                    std::cout << "\nネットワーク接続状態:\n" 
                              << nm.getConnectionStatus() << std::endl;
                    break;
                case 5: {
                    std::string interface, dns;
                    std::cout << "インターフェース名を入力: ";
                    std::cin.ignore();
                    std::getline(std::cin, interface);
                    std::cout << "DNSサーバーのIPアドレスを入力: ";
                    std::getline(std::cin, dns);
                    
                    if (nm.setDNS(interface, dns)) {
                        std::cout << "DNS設定を更新しました" << std::endl;
                    } else {
                        std::cout << "DNS設定の更新に失敗しました" << std::endl;
                    }
                    break;
                }
                case 0:
                    return 0;
                default:
                    std::cout << "無効な選択です" << std::endl;
            }
        }
        catch (const std::runtime_error& e) {
            std::cerr << "エラー: " << e.what() << std::endl;
        }
    }

    return 0;
} 