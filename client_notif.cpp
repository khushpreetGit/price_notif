#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

void sendAlert(const std::string& jsonString) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported" << std::endl;
        return;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return;
    }

    send(sock, jsonString.c_str(), jsonString.length(), 0);
    std::cout << "Alert sent: " << jsonString << std::endl;

    close(sock);
}

int main() {
    std::string jsonString = R"({
        "symbol": "AAPL",
        "basis": "price",
        "value": 150.0,
        "direction": "UP"
    })";

    sendAlert(jsonString);

    return 0;
}
