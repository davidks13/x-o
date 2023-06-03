#include "client.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>


Client::Client() {}

Client::~Client() {
    close(client_socket);
}

void Client::launch_client(const char* ip, const int port_num) {
    server_ip = ip;
    port = port_num;
    socket_created();
    address_config();
    connected();
    std::cout << "Connected!\n";
}

int Client::get_socket() {
    return client_socket;
}

void Client::socket_created() {
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client_socket < 0){
        std::cerr << "socket error: " << strerror(errno) << std::endl;
        close(client_socket);
        exit(0);
    }
}

void Client::address_config() {
    memset(&server_address, 0, sizeof(sockaddr_in));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &server_address.sin_addr) <= 0) {
        std::cerr << "inet_pton error: " << strerror(errno) << std::endl;
        close(client_socket);
        exit(0);
    }
}

void Client::connected() {
    status = connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if (status == -1) {
        std::cerr << "connect error: " << strerror(errno) << std::endl;
        close(client_socket);
        exit(0);
    }
}