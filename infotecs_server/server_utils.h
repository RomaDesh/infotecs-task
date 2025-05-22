#pragma once

#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <vector>
#include <stack>

// Waits message from client
// Returns true if message successfully received, false in other cases
bool receiveMessage(int client_socket, std::vector<char>& buffer);

// Comparing number of includes in strings
// Return true when First > Second
// If numbers are equal, compares letters
bool compareStrings(const std::string& First, const std::string& Second);

// Sorts vector of strings using quick sort iterative algorithm
void quickSortIterative(std::vector<std::string>& vec);