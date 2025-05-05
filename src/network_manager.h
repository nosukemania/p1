#pragma once

#include <string>
#include <map>

class NetworkManager {
public:
    NetworkManager();

    std::string getNetworkInterfaces();
    std::string getIPConfiguration();
    std::string getDNSSettings();
    std::string getConnectionStatus();
    bool setDNS(const std::string& interface, const std::string& dns);

private:
    std::map<std::string, std::string> commands;
}; 