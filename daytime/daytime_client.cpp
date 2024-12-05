#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>

constexpr int DEFAULT_PORT = 13; // Порт службы daytime по умолчанию
constexpr size_t BUFFER_SIZE = 256; // Размер буфера для получения данных

void printHelp() {
    std::cout << "Usage: ./daytime_client <server_ip> [<port>]\n";
    std::cout << "Example: ./daytime_client 172.16.40.1 13\n";
    std::cout << "This program sends a request to the specified daytime server over UDP and prints the server's response.\n";
    std::cout << "If no port is specified, the default port 13 will be used.\n";
}

int createSocket() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
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

    if (sendto(sock, "", 1, 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при отправке запроса");
        close(sock);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);
    int recv_len = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&server_addr, &addr_len);
    if (recv_len < 0) {
        perror("Ошибка при получении данных");
        close(sock);
        return 1;
    }

    buffer[recv_len] = '\0'; // Завершение строки
    std::cout << "Текущее время от сервера: " << buffer << std::endl;

    close(sock);
    return 0;
}
