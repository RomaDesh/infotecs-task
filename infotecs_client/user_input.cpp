#include "user_input.h"

#include <sstream>
#include <map>
#include <iostream>

void user_input(Buffer& buffer) {
	while (true) {
		std::string input;
		std::ostringstream ossRes;
		std::map<char, int> mapRes;
		std::getline(std::cin, input);
		if (input == "/q") {
			buffer.add(input);
			break;
		}
		try {
			if (input.size() > 64)
				throw std::invalid_argument("More than 64 characters");
			else
				for (char c : input) {
					if (c < 'A' || (c > 'Z' && c < 'a') || c > 'z')
						throw std::invalid_argument("Non-alphabet symbol");
					if (mapRes.find(c) != mapRes.end())
						mapRes[c]++;
					else
						mapRes[c] = 1;
				}
			for (const auto& pair : mapRes)
				ossRes << "'" << pair.first << "' : " << pair.second << std::endl;
			buffer.add(ossRes.str());
		}
		catch (std::exception& ex) {
			std::cerr << ex.what() << std::endl;
		}
	}
}