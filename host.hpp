#ifndef HOST_HPP
#define HOST_HPP

#include <iostream>
#include <string>
#include <netdb.h>
#include <queue>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

class Server {
    public:
        Server();
        ~Server();
        void launch_server(const int port_num);
        int get_socket();
        
    private:
        int port;
        int server_socket;
        struct sockaddr_in server_address;
        int client_socket;
        void socket_created();
        void address_config();
        void bind_made();
        void listening();
        void accepting();
};

#endif