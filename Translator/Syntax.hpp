#pragma once

#include <map>
#include <string>


const std::map<std::string, int> RESERVED = 
{
	/*Delimiters code: [0, 100]*/
	{ "(", 0 },
	{ ")", 1 },
	{ ":", 2 },
	{ ";", 3 },
	{ ",", 4 },

	/*Keywords code: [401, 450]*/
	{ "program", 401 },
	{ "begin", 402 },
	{ "end", 403 },
	{ "procedure", 404 },

	/*Base type code: [451, 500]*/
	{ "signal", 451 },
	{ "complex", 452 },
	{ "integer", 453 },
	{ "float", 454 },
	{ "blockfloat", 455 },
	{ "ext", 456 }
};
