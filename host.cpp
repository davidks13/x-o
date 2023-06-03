#include "host.hpp"

Server::Server() {}

Server::~Server() {
    close(client_socket);
    close(server_socket);
}

void Server::launch_server(const int port_num) {
    port = port_num;
    socket_created();
    address_config();
    bind_made();
    listening();
    accepting();
    accepting();
    std::cout << "Host has been launched!" << std::endl;
}

int Server::get_socket() {
    return client_socket;
}

void Server::socket_created() {
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server_socket < 0){
        std::cerr << "socket error: " << strerror(errno) << std::endl;
        close(server_socket);
        exit(0);
    }
}

void Server::address_config() {
    memset(&server_address, 0, sizeof(sockaddr_in));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;
}

void Server::bind_made() {
    int server_bind = bind(server_socket, (struct sockaddr*)&server_address , sizeof(server_address));
    if(server_bind < 0){
        std::cerr << "bind error: " << strerror(errno) << std::endl;
        close(server_socket);
        exit(0);
    }
}

void Server::listening() {
    if(listen(server_socket, 1) < 0){
        std::cerr << "listen error: " << strerror(errno) << std::endl;
        close(server_socket);
        exit(0);
    }
}

void Server::accepting() {
    client_socket = accept(server_socket, (struct sockaddr*)NULL, NULL);
    if(client_socket < 0){
        std::cerr << "client_socket error: " << strerror(errno) << std::endl;
        close(client_socket);
        close(server_socket);
        exit(0);
    }
    std::cout << "Connected!\n";
}