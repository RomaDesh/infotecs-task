#pragma once

#include "Buffer.h"

#include <string>
#include <atomic>
#include <thread>
#include <queue>
#include <arpa/inet.h>


class ServerConnection {
private:
    Buffer& buffer_; // Reference to a Buffer object
    std::string serverProcessName_; // Name of server process in system
    std::atomic<bool> stop_; // Stop flag, stops thread cycles
    std::thread connection_thread_; // Server connection thread | Second thread
    std::queue<std::string> pending_messages_;  // Stores data, that had been read already, but failed send
    int sock_; // Client socket
    struct sockaddr_in serv_addr_; // Adress of server
public:
    // ServerConnection constructor
    ServerConnection(Buffer& buffer, const std::string& serverProcessName);

    // ServerConnection destructor
    ~ServerConnection();

    // Starts server connection thread
    void start();

    // Stops server connection thread
    void stop();

private:
    // Server connection thread function | "Second thread" thread function
    void run();

    // Checks server process running in system
    bool isServerRunning(const std::string& serverProcessName);

    // Sets up connection with server
    bool connectToServer();

    // Closes connection to server
    void closeConnection();

    // Sends message to server
    bool sendMessage(const std::string& message);

    // Sends all pending messages
    void resendPendingMessages();

};