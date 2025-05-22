#pragma once

#include <string>
#include <mutex>
#include <queue>
#include <condition_variable>

class Buffer {
private:
	std::queue<std::string> queue_; // Stores strings from user_unput thread
	std::mutex mutex_; // Locks Buffer from simultaneous access from threads
	std::condition_variable condition_; // Synchronize threads
public:
	// Adds data in queue_, notifies second thread, that data was added
	void add(std::string data); 

	// Gets data from queue_, when empty waits until data will be updated
	std::string get(); 
};