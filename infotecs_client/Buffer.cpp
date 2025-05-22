#include "Buffer.h"

void Buffer::add(std::string data) {
	std::unique_lock<std::mutex> lock(mutex_);
	queue_.push(data);
	condition_.notify_one();
}
std::string Buffer::get() {
	std::unique_lock<std::mutex> lock(mutex_);
	if (queue_.empty())
		condition_.wait(lock, [this]() { return !queue_.empty(); });
	std::string data = queue_.front();
	queue_.pop();
	return data;
}