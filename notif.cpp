#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Alert {
public:
    Alert(const std::string& jsonString) {
        parseJson(jsonString);
    }

    void checkAlert(double currentValue) {
        if (direction == "UP" && currentValue > value) {
            std::cout << "Alert: " << symbol << " has crossed above " << value << std::endl;
        } else if (direction == "DOWN" && currentValue < value) {
            std::cout << "Alert: " << symbol << " has crossed below " << value << std::endl;
        } else {
            std::cout << "No alert for " << symbol << std::endl;
        }
    }

private:
    std::string symbol;
    std::string basis;
    int maLength = 5;  // default window of 5
    double value;
    std::string direction;

    void parseJson(const std::string& jsonString) {
        json j = json::parse(jsonString);

        symbol = j.at("symbol").get<std::string>();
        basis = j.at("basis").get<std::string>();
        if (j.contains("maLength") && !j.at("maLength").is_null()) {
            maLength = j.at("maLength").get<int>();
        }
        value = j.at("value").get<double>();
        direction = j.at("direction").get<std::string>();
    }
};

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE] = {0};
    int bytesRead = read(clientSocket, buffer, BUFFER_SIZE);
    if (bytesRead <= 0) {
        std::cerr << "Failed to read from client socket." << std::endl;
        close(clientSocket);
        return;
    }

    std::string jsonString(buffer, bytesRead);
    Alert alert(jsonString);

    // Example current value for the symbol
    double currentValue = 151.0;
    alert.checkAlert(currentValue);

    close(clientSocket);
}

void startServer() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == 0) {
        std::cerr << "Socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            std::cerr << "Accept failed" << std::endl;
            close(serverSocket);
            exit(EXIT_FAILURE);
        }

        handleClient(clientSocket);
    }

    close(serverSocket);
}

int main() {
    startServer();
    return 0;
}
