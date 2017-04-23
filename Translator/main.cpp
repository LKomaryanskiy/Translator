#include "Lexer.hpp"

#include <stdlib.h>

int main()
{
	Lexer lexer;
	auto table = lexer(std::string("test_code.sig"));

	return EXIT_SUCCESS;
};