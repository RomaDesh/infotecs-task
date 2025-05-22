#include "ServerConnection.h"
#include "Buffer.h"
#include "user_input.h"

int main() {
	Buffer buffer;
	ServerConnection connection(buffer, "infotecs_server");
	std::thread producer_thread(user_input, std::ref(buffer));

	connection.start();
	producer_thread.join();

	connection.stop();
	return 0;
}