#include "Application.hpp"

// STD
#include <iostream>

int main() {
	FFL::Application app;

	try {
		app.run();
	} catch(const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
