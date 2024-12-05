#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>

constexpr int DEFAULT_PORT = 7;  // Порт службы echo по умолчанию
constexpr size_t BUFFER_SIZE = 256; // Размер буфера для сообщения

void printHelp() {
    std::cout << "Usage: ./echo_client <server_ip> [<port>]\n";
    std::cout << "Example: ./echo_client 172.16.40.1 7\n";
    std::cout << "This program connects to the specified echo server over TCP and sends a message.\n";
    std::cout << "If no port is specified, the default port 7 will be used.\n";
}

int createSocket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Ошибка при создании сокета");
    }
    return sock;
}

void configureServerAddress(sockaddr_in& server_addr, const char* ip, int port) {
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("Ошибка преобразования IP-адреса");
    }
}

void handleCommunication(int sock) {
    char buffer[BUFFER_SIZE]{};

    std::cout << "Введите сообщение для отправки (или 'exit' для выхода): ";
    std::cin.getline(buffer, sizeof(buffer));

    while (std::string(buffer) != "exit") {
        if (send(sock, buffer, strlen(buffer), 0) < 0) {
            perror("Ошибка при отправке данных");
            return;
        }

        int recv_len = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (recv_len < 0) {
            perror("Ошибка при получении данных");
            return;
        }

        buffer[recv_len] = '\0'; // Завершение строки
        std::cout << "Ответ от сервера: " << buffer << std::endl;

        std::cout << "Введите сообщение для отправки (или 'exit' для выхода): ";
        std::cin.getline(buffer, sizeof(buffer));
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3 || std::string(argv[1]) == "-h") {
        printHelp();
        return 0;
    }

    int port = (argc == 3) ? std::atoi(argv[2]) : DEFAULT_PORT;
    sockaddr_in server_addr;

    int sock = createSocket();
    if (sock < 0) {
        return 1;
    }

    configureServerAddress(server_addr, argv[1], port);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при подключении к серверу");
        close(sock);
        return 1;
    }

    handleCommunication(sock);

    close(sock);
    return 0;
}
