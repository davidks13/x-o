#include "game.hpp"
#include "sniff.hpp"

Game::Game() : board(std::vector<std::vector<char>>(3, std::vector<char>(3, '~'))) {}

int Game::choose_mode() {
    int mode;
    std::cout << "Are you a client or a host?\n" 
        << "If client enter : 0, host : 1\n";
    std::cin >> mode;
    std::cout << std::endl;
    while(mode != 0 && mode != 1) {
        std::cout << "Enter a valid number!\n"
            << "If client enter : 0, host : 1\n";
        std::cin >> mode;
        std::cout << mode << std::endl;
    }

    return mode;
}


bool Game::sending(int socket, std::pair<int, int>& coords) {
    int data_to_send[2];
    data_to_send[0] = coords.first;
    data_to_send[1] = coords.second;
    std::size_t bytes_send = ::send(socket, data_to_send, sizeof(data_to_send), 0);
    if (bytes_send  == -1) {
        std::cerr << "Receive error: " << strerror(errno) << std::endl;
        exit(0);
    }
    return true;
}


bool Game::receiving(int socket, std::pair<int, int>& received_coords) {
    int received_data[2];
    std::size_t bytes_received = recv(socket, received_data, sizeof(received_data), 0);
    if (bytes_received == -1) {
        std::cerr << "Receive error: " << strerror(errno) << std::endl;
        exit(0);
    }
    if (bytes_received != sizeof(received_data)) {
        std::cerr << "Received incomplete data" << std::endl;
        exit(0);
    }
    received_coords.first = received_data[0];
    received_coords.second = received_data[1];
    return true;
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
    std::pair<int, int> coords;
    while(!win_check()) {
        print_board();
        receiving(client.get_socket(), coords);
        board[coords.first][coords.second] = 'X';

        while (true) {
            std::cout << "Enter coords 0-2 row\n";
            std::cin >> coords.first;
            std::cout << "Enter coords 0-2 col\n";
            std::cin >> coords.second;

            if (board[coords.first][coords.second] == '~') {
                board[coords.first][coords.second] = 'O';
                sending(client.get_socket(), coords);
                break;
            }
        }
        std::cout << std::endl;
    }
}

void Game::host_gameplay() {
    std::pair<int, int> coords;
    while (!win_check()) {
        print_board();
        while (true) {
            std::cout << "Enter coords 0-2 row\n";
            std::cin >> coords.first;
            std::cout << "Enter coords 0-2 col\n";
            std::cin >> coords.second;

            if (board[coords.first][coords.second] == '~') {
                board[coords.first][coords.second] = 'X';
                sending(host.get_socket(), coords);
                break;
            } 
        }

        receiving(host.get_socket(), coords);
        board[coords.first][coords.second] = 'O';
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
        client_mode();
        client_gameplay();
    } else {
        host_mode();
        host_gameplay();
    }
}
