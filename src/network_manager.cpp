#include "network_manager.h"
#include "utils.h"

NetworkManager::NetworkManager() {
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

std::string NetworkManager::getNetworkInterfaces() {
    return exec(commands["interfaces"].c_str());
}

std::string NetworkManager::getIPConfiguration() {
    return exec(commands["ip"].c_str());
}

std::string NetworkManager::getDNSSettings() {
    return exec(commands["dns"].c_str());
}

std::string NetworkManager::getConnectionStatus() {
    return exec(commands["status"].c_str());
}

bool NetworkManager::setDNS(const std::string& interface, const std::string& dns) {
    std::string cmd;
    #if defined(_WIN32) || defined(_WIN64)
        cmd = "netsh interface ip set dns \"" + interface + "\" static " + dns;
    #elif defined(__APPLE__) || defined(__MACH__)
        cmd = "networksetup -setdnsservers \"" + interface + "\" " + dns;
    #elif defined(__linux__)
        cmd = "echo 'nameserver " + dns + "' > /etc/resolv.conf";
    #endif
    
    try {
        exec(cmd.c_str());
        return true;
    } catch (const std::runtime_error&) {
        return false;
    }
} 