#ifndef SNIFF_HPP
#define SNIFF_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ifaddrs.h>

std::string scan_port(const std::string& target, int port) {
    std::cout << "Scanning " << target << " for open ports..." << std::endl;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;

    server.sin_addr.s_addr = inet_addr(target.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == 0) {
        std::cout << "Port " << port << " is open" << std::endl;
        return target;
    }

    close(sock);
    return "NULL";
}

std::vector<std::string> getLocalIPs() {
    std::vector<std::string> localIPs;


    struct ifaddrs* ifaddrsList = nullptr;
    struct ifaddrs* ifaddr = nullptr;

    if (getifaddrs(&ifaddrsList) == -1) {
        std::cerr << "Failed to get network interfaces" << std::endl;
        return localIPs;
    }

    for (ifaddr = ifaddrsList; ifaddr != nullptr; ifaddr = ifaddr->ifa_next) {
        if (ifaddr->ifa_addr == nullptr) {
            continue;
        }

        if (ifaddr->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in* addr = reinterpret_cast<struct sockaddr_in*>(ifaddr->ifa_addr);
            char ipBuffer[INET_ADDRSTRLEN];
            const char* result = inet_ntop(AF_INET, &(addr->sin_addr), ipBuffer, sizeof(ipBuffer));
            if (result != nullptr) {
                localIPs.push_back(ipBuffer);
            }
        }
    }

    freeifaddrs(ifaddrsList);
    return localIPs;
}

#endif