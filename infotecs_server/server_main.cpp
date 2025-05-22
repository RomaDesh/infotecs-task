#include "server_utils.h"

#include <netinet/in.h>
#include <unistd.h>

int main() {
    while (true) {
    int server_fd, client_socket;
    int opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(8080);

        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        std::cout << "Server is listening on port 8080\n";

        if ((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        std::cout << "Client connected\n";

        while (true) {
            std::vector<char> buffer;
            if (receiveMessage(client_socket, buffer)) {
                const char* response = "Data received";
                send(client_socket, response, strlen(response), 0);
            }
            else 
                break;
            std::vector<std::string> buf_as_string;
            auto itStart = buffer.begin();
            for (auto it = buffer.begin(); it != buffer.end(); it++)
            {
                if (*it == '\n')
                {
                    buf_as_string.push_back(std::string(itStart, it));
                    itStart = it + 1;
                }
            }
            quickSortIterative(buf_as_string);
            for (auto it = buf_as_string.begin(); it != buf_as_string.end(); it++)
                std::cout << *it << std::endl;
        }
        close(client_socket);
        close(server_fd);
    }
    return 0;
}
