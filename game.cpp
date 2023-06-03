#include "game.hpp"
#include "sniff.hpp"

Game::Game() : board(std::vector<std::vector<char>>(3, std::vector<char>(3, '~'))) {}

int Game::choose_mode() {
    int mode;
    std::cout << "Are you a client or a host?\n" 
        << "If host enter : 0, client : 1\n";
    std::cin >> mode;
    std::cout << std::endl;
    while(mode != 0 && mode != 1) {
        std::cout << "Enter a valid number!\n"
            << "If host enter : 0, client : 1\n";
        std::cin >> mode;
        std::cout << mode << std::endl;
    }

    return mode;
}

bool Game::sending(int socket, int coord) {
    int buffer = coord;
    std::size_t bytes_send = send(socket, &buffer, sizeof(buffer), 0);
    if (bytes_send  == -1) {
        std::cerr << "Send error: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

int Game::receiving(int socket) {
    int buffer;
    std::size_t bytes_received = recv(socket, &buffer, sizeof(buffer), 0);
    if (bytes_received  == -1) {
        std::cerr << "Receive error: " << strerror(errno) << std::endl;
        exit(0);
    }
    return buffer;
}

void Game::host_mode() {
    int port_num;
    std::cout << "Enter number of the port: ";
    std::cin >> port_num;

    host.launch_server(port_num);
}

void Game::client_mode() {
    int port_num;
    std::cout << "Enter number of the port: ";
    std::cin >> port_num;

    std::vector<std::string> local_ips = getLocalIPs();
    for (const auto& ip : local_ips) {
        if (scan_port(ip, port_num) != "NULL") {
            std::cout << ip << std::endl;
            client.launch_client(ip.c_str(), port_num);
            break;
        }
    }
}

void Game::client_gameplay() {
    int row{}, col{};
    while(!win_check()) {
        print_board();
        row = receiving(client.get_socket());
        col = receiving(client.get_socket());
        std::cout << row << " " << col << std::endl;
        board[row][col] = 'X';
        print_board();

        while (true) {
            std::cout << "Enter coords 0-2 row\n";
            std::cin >> row;
            std::cout << "Enter coords 0-2 col\n";
            std::cin >> col;

            if (board[row][col] == '~') {
                board[row][col] = 'O';
                sending(client.get_socket(), row);
                sending(client.get_socket(), col);
                break;
            }
        }
        std::cout << std::endl;
    }
}

void Game::host_gameplay() {
    int row{}, col{};
    while (!win_check()) {
        print_board();
        while (true) {
            std::cout << "Enter coords 0-2 row\n";
            std::cin >> row;
            std::cout << "Enter coords 0-2 col\n";
            std::cin >> col;

            if (board[row][col] == '~') {
                board[row][col]= 'X';
                sending(host.get_socket(), row);
                sending(host.get_socket(), col);
                break;
            } 
        }

        row = receiving(host.get_socket());
        col = receiving(host.get_socket());
        board[row][col] = 'O';
        std::cout << std::endl;
    }
}

void Game::print_board() {
    for (auto& row : board) {
        for (auto& col : row) {
            std::cout << col << " ";
        }
        std::cout << std::endl;
    }
}

bool Game::win_check() {
    bool flag = false;
    int current;
    
    for (auto& row : board) {
        flag = true;
        current = row[0];
        for (auto& col : row) {
            if (col != current || current != '~') {
                std::cout << "in 1st if\n";
                flag = false;
                break;
            }
        }
    }

    for (int col = 0; col < board[0].size(); col++) {
        flag = true;
        current = board[col][0];
        for (int row = 0; row < board.size(); row++) {
            if (board[row][col] != current || current != '~') {
                std::cout << "in 2nd if\n";
                flag = false;
                break;
            }
        }
    }

    if (board[0][0] != board[1][1] && board[1][1] != board[2][2] || board[0][0] != '~') {
        std::cout << "in 3rd if\n";
        flag = false;
    }

    if (board[2][0] != board[1][1] && board[1][1] != board[0][2] || board[2][0] != '~') {
        std::cout << "in 4th	 if\n";
        flag = false;
    }

    if (flag) {
        if (current == 'X') {
            std::cout << "Game over, host won";
            return true; 
        }
        else if (current == 'Y') {
      	    std::cout << "Game over, client won";
            return true; 
        }
    }

    return false;
}

void Game::run() {
    int mode = choose_mode();
    if (!mode) {
        host_mode();
        host_gameplay();
    } else {
        client_mode();
        client_gameplay();
    }
}
