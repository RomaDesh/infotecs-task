#include "ServerConnection.h"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdexcept>

ServerConnection::ServerConnection(Buffer& buffer, const std::string& serverProcessName)
    : buffer_(buffer), serverProcessName_(serverProcessName), stop_(false) {}

ServerConnection::~ServerConnection() {
    if (connection_thread_.joinable())
        connection_thread_.join();
}
void ServerConnection::start() {
    connection_thread_ = std::thread(&ServerConnection::run, this);
}

void ServerConnection::stop() {
    stop_ = true;
    if (connection_thread_.joinable()) {
        connection_thread_.join();
    }
}

void ServerConnection::run() {
    while (!stop_) {
        try {
        if (isServerRunning(serverProcessName_)) {
            if (!connectToServer()) {
                std::this_thread::sleep_for(std::chrono::seconds(5));
                continue;
            }

            std::cout << "Connected to server\n";

            resendPendingMessages();

            while (!stop_) {
                std::string data = buffer_.get();
                if (data == "/q") {
                    break;
                }
                std::cout << data;
                if (!sendMessage(data)) {
                    pending_messages_.push(data);
                    break;
                }
                char buffer[1024] = { 0 };
                int valread = read(sock_, buffer, 1024);
                if (valread <= 0) {
                    std::cerr << "Unable to read server responce: " << strerror(errno) << ". Trying reconnect...\n";
                    pending_messages_.push(data);
                    break;
                }
                std::cout << "Server responce: " << buffer << std::endl;
            }

            closeConnection();
            std::cout << "Disconnected from server\n";
        }
        else {
            std::cerr << "Server is down, retrying in 5 seconds\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        catch (std::exception &ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
            this.stop();
        }
    }
}

bool ServerConnection::isServerRunning(const std::string& serverProcessName) {
    std::string command = "ps aux | grep " + serverProcessName + " | grep -v grep";
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return !result.empty();
}

bool ServerConnection::connectToServer() {
    sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ < 0) {
        std::cerr << "Unable to create socket\n";
        return false;
    }

    serv_addr_.sin_family = AF_INET;
    serv_addr_.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr_.sin_addr) <= 0) {
        std::cerr << "Unable to transform adress\n";
        close(sock_);
        return false;
    }

    if (connect(sock_, (struct sockaddr*)&serv_addr_, sizeof(serv_addr_)) < 0) {
        std::cerr << "Unable connect to server\n";
        close(sock_);
        return false;
    }

    return true;
}

void ServerConnection::closeConnection() {
    if (sock_ >= 0) {
        close(sock_);
    }
}

bool ServerConnection::sendMessage(const std::string& message) {
    int sent = send(sock_, message.c_str(), message.size(), 0);
    if (sent <= 0)
        std::cerr << "Unable to send message to server: " << strerror(errno) << ". Trying reconnect...\n";
    return sent > 0;
}

void ServerConnection::resendPendingMessages() {
    while (!pending_messages_.empty()) {
        std::string message = pending_messages_.front();
        if (!sendMessage(message)) 
            break;
        pending_messages_.pop();
    }
}
