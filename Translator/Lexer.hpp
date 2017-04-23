#pragma once


#include "Syntax.hpp"

#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <locale>


enum lexem_types
{
	unknown,
	delimiter,
	identifier
};

struct Position
{
	unsigned int column = 0;
	unsigned int row = 0;

	std::string ToString() const;
};

struct LexemInfo
{
	lexem_types type = unknown;
	std::string source_text = "";
	int code = -1;
	Position position;
	int length;

	friend std::ostream& operator<<(std::ostream& stream, const LexemInfo& elem);
};

using Errors = std::vector<std::string>;

struct LexerTable
{
	std::vector<LexemInfo> lexemes;
	Errors lexical_errors;

	friend std::ofstream& operator<<(std::ofstream& stream, const LexerTable& elem);
};

class Lexer
{
public:
	Lexer() {};
	LexerTable operator()(std::string& file_path);


protected:
	LexerTable	m_table;
	std::ifstream m_source_file;
	std::ofstream m_lexer_file;
	std::map<std::string, int> m_identifiers;
	LexemInfo m_curr_lexem;
	Position m_curr_position;
	int m_personal_id_code = 1000;

	void Run();

	std::string LexerFileName(std::string source_file_name);

	void AddLexem();

	void PassWhiteSpaces(char& symbol);
	void PassComments();
	bool IsDelimiter(char symbol);
	bool IsReserwed(std::string word);
	bool IsPersonalIdentifier(std::string word);
	
	void StartState(char& symbol);
	void ErrorState(std::string error_description);
	void DelimiterState(char& symbol);
	void IdentifierState(char& symbol);

	std::string ToLowerString(std::string word);
};