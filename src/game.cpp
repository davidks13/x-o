#include "../include/game.hpp"
#include "../include/sniff.hpp"

Game::Game() : board(std::vector<std::vector<char>>(3, std::vector<char>(3, '~'))) {}

int Game::choose_mode() {
    int mode;
    std::cout << "Are you a client or a host?\n" 
        << "If host enter : 0, client : 1\n" 
        << "P.S. host is an X, client is an O\n";
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
    std::cout << "Waiting for someone to connect...\n";
    host.launch_server(port_num);
}

void Game::client_mode() {
    int port_num;
    std::cout << "Enter number of the port: ";
    std::cin >> port_num;

    std::vector<std::string> local_ips = getLocalIPs();
    for (const auto& ip : local_ips) {
        if (scan_port(ip, port_num) != "NULL") {
            client.launch_client(ip.c_str(), port_num);
            break;
        }
    }
}

void Game::client_gameplay() {
    int row{}, col{};
    while(true) {
        print_board();

        if (win_check()) {
            break;
        }
        
        std::cout << "\nWaiting for opponent...\n";
        row = receiving(client.get_socket());
        col = receiving(client.get_socket());
        board[row][col] = 'X';

        if (win_check()) {
            break;
        }

        std::cout << "\n\n";
        print_board();

        while (true) {
            std::cout << "Enter valid coords 0-2 row: ";
            std::cin >> row;
            std::cout << std::endl;
            std::cout << "Enter valid coords 0-2 col: ";
            std::cin >> col;
            std::cout << std::endl;

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
    while (true) {
        print_board();
        while (true) {
            std::cout << "Enter valid coords 0-2 row: ";
            std::cin >> row;
            std::cout << std::endl;
            std::cout << "Enter valid coords 0-2 col: ";
            std::cin >> col;
            std::cout << std::endl;

            if (board[row][col] == '~') {
                board[row][col]= 'X';
                sending(host.get_socket(), row);
                sending(host.get_socket(), col);
                break;
            }
            std::cout << "\n\n";
        }

        print_board();

        if (win_check()) {
            break;
        }

        std::cout << "\nWaiting for opponent...\n";
        row = receiving(host.get_socket());
        col = receiving(host.get_socket());
        board[row][col] = 'O';

        if (win_check()) {
            break;
        }
        
        
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
    for (const auto& row : board) {
        if (row[0] == row[1] && row[1] == row[2] && row[0] != '~') {
            if (row[0] == 'X') {
                std::cout << "X won!" << std::endl;
                return true;
            }
            std::cout << "O won!" << std::endl;
            return true;
        }
    }

    for (int col = 0; col < 3; ++col) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col] && board[0][col] != '~') {
            if (board[0][col] == 'X') {
                std::cout << "X won!" << std::endl;
                return true;
            }
            std::cout << "O won!" << std::endl;
            return true;
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != '~') {
        if (board[0][0] == 'X') {
            std::cout << "X won!" << std::endl;
            return true;
        }
        std::cout << "O won!" << std::endl;
        return true;
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != '~') {
        if (board[0][2] == 'X') {
            std::cout << "X won!" << std::endl;
            return true;
        }
        std::cout << "O won!" << std::endl;
        return true;
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
