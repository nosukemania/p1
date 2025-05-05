#include <iostream>
#include "network_manager.h"
#include "utils.h"

int main() {
    std::string os_type = getOSType();
    if (os_type == "Unknown") {
        std::cout << "不明な環境で実行されています" << std::endl;
        return 1;
    }

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
                // ... existing switch cases ...
            }
        }
        catch (const std::runtime_error& e) {
            std::cerr << "エラー: " << e.what() << std::endl;
        }
    }

    return 0;
} 