#ifndef GAME_HPP
#define GAME_HPP

#include "client.hpp"
#include "host.hpp"
#include <iostream>
#include <vector>

class Game{
    public:
        Game();
        void run();
        int choose_mode();
        void client_mode();
        void host_mode();
        void print_board();
        void client_gameplay();
        void host_gameplay();
        bool sending(int socket, int coord);
        int receiving(int socket);
        bool win_check();
    private:
        int port;
        Server host;
        Client client;
        std::vector<std::vector<char>> board;
};


#endif