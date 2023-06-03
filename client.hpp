#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <netdb.h>

class Client {
    public:
        Client();
        ~Client();
        void launch_client(const char* ip, const int& port_num);
        int get_socket();
    private:
        const char* server_ip;
        int port;
        struct sockaddr_in server_address;
        int client_socket;
        int status;
        void socket_created();
        void address_config();
        void connected();
};

#endif