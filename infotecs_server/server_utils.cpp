#include "server_utils.h"

bool receiveMessage(int client_socket, std::vector<char>& buffer) {
    ssize_t bytes_received;
    buffer.resize(417); // Max possible buffer size == 417
    bytes_received = recv(client_socket, buffer.data(), 469, 0); // Max possible message size == 469
    if (bytes_received <= 0) {
        std::cerr << "Error getting data from client " << strerror(errno) << std::endl;
        return false;
    }
    if (bytes_received < 417)
        buffer.resize(bytes_received);
    return true;
}

bool compareStrings(const std::string& First, const std::string& Second) {
    if (stoi(First.substr(6)) != stoi(Second.substr(6)))
        return stoi(First.substr(6)) > stoi(Second.substr(6));
    else
        return First.at(1) < Second.at(1);
}

void quickSortIterative(std::vector<std::string>& vec) {
    std::stack<std::pair<int, int>> stack;

    stack.push(std::make_pair(0, vec.size() - 1));

    while (!stack.empty()) {
        int start = stack.top().first;
        int end = stack.top().second;
        stack.pop();

        if (start >= end) {
            continue;
        }

        std::string pivot = vec[end];
        int left = start;
        int right = end - 1;

        while (left <= right) {
            while (left <= right && compareStrings(vec[left], pivot)) {
                ++left;
            }
            while (left <= right && compareStrings(pivot, vec[right])) {
                --right;
            }
            if (left <= right) {
                std::swap(vec[left], vec[right]);
                ++left;
                --right;
            }
        }

        std::swap(vec[left], vec[end]);

        stack.push(std::make_pair(start, left - 1));
        stack.push(std::make_pair(left + 1, end));
    }
}
