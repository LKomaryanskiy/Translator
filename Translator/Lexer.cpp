#include "Lexer.hpp"

std::string Position::ToString() const
{
	return "(" + std::to_string(this->row) + ", " + std::to_string(this->column) + ")";
};

LexerTable Lexer::operator()(std::string& file_path)
{
	m_source_file = std::ifstream(file_path);
	if (!m_source_file.is_open())
	{
		std::cout << "#ERROR: FILE " << file_path << " IS NOT FOUND!\n";
	}
	else
	{
		m_lexer_file = std::ofstream(LexerFileName(file_path));
		Run();
		m_lexer_file << m_table;
		m_source_file.close();
		m_lexer_file.close();
	}
	
	return m_table;
};

void Lexer::Run()
{
	m_curr_position = { 1, 1 };
	char tmp;
	m_source_file >> std::noskipws >> tmp;
	do
	{
		StartState(tmp);
	} while (!m_source_file.eof());
};

std::string Lexer::LexerFileName(std::string source_file_name)
{
	const std::string lexer_extension = ".lex";
	return source_file_name.substr(0, source_file_name.find_last_of(".")) + lexer_extension;
};

void Lexer::AddLexem()
{
	switch (m_curr_lexem.type)
	{
	case identifier:
	{
		const std::string tolower_word = ToLowerString(m_curr_lexem.source_text);
		if (IsReserwed(tolower_word))
		{
			m_curr_lexem.code = RESERVED.at(tolower_word);
		}
		else if (!IsPersonalIdentifier(tolower_word))
		{
			m_identifiers[tolower_word] = m_personal_id_code;
			++m_personal_id_code;
			m_curr_lexem.code = m_identifiers.at(tolower_word);
		}
		else if (IsPersonalIdentifier(tolower_word))
		{
			m_curr_lexem.code = m_identifiers.at(tolower_word);
		}
		break;
	}
	case delimiter:
	{
		m_curr_lexem.code = RESERVED.at(m_curr_lexem.source_text);
		break;
	}
	default:
		break;
	}
	m_table.lexemes.push_back(m_curr_lexem);
};

void Lexer::PassWhiteSpaces(char& symbol)
{
	do
	{
		if (symbol == '\n')
		{
			++m_curr_position.row;
		}
		else
		{
			++m_curr_position.column;
		}
		m_source_file >> std::noskipws >> symbol;
	} while (isspace(symbol));
};

void Lexer::PassComments()
{
};

bool Lexer::IsDelimiter(char symbol)
{
	auto it = RESERVED.find(std::string(1, symbol));
	return (it != RESERVED.end()) && (it->second <= 100);
};

bool Lexer::IsReserwed(std::string word)
{
	auto it = RESERVED.find(word);
	return (it != RESERVED.end());
};

bool Lexer::IsPersonalIdentifier(std::string word)
{
	auto it = m_identifiers.find(word);
	return (it != m_identifiers.end());
};

void Lexer::StartState(char& symbol)
{
	m_curr_lexem = LexemInfo();
	if (isspace(symbol))
	{
		PassWhiteSpaces(symbol);
	}
	else if (IsDelimiter(symbol))
	{
		m_curr_lexem.type = delimiter;
		DelimiterState(symbol);
		AddLexem();
	}
	else if (isalpha(symbol))
	{
		m_curr_lexem.type = identifier;
		IdentifierState(symbol);
		AddLexem();
	}
	else
	{
		ErrorState("#LEXICAL ERROR: in" + m_curr_position.ToString() + " unexpected symbol");
	}
};

void Lexer::ErrorState(std::string error_description)
{
	m_table.lexical_errors.push_back(error_description);
	std::cout << error_description << '\n';
	char tmp;
	m_source_file >> tmp;
	++m_curr_position.column;
	while (!isspace(tmp) && !IsDelimiter(tmp) && !isalpha(tmp))
	{
		m_source_file >> tmp;
		++m_curr_position.column;
	}
};

void Lexer::DelimiterState(char& symbol)
{
	m_curr_lexem.position = m_curr_position;
	m_curr_lexem.source_text.push_back(symbol);
	++m_curr_position.column;
	++m_curr_lexem.length;
	m_source_file >> std::noskipws >> symbol;
};

void Lexer::IdentifierState(char& symbol)
{
	m_curr_lexem.position = m_curr_position;
	do
	{
		m_curr_lexem.source_text.push_back(symbol);
		++m_curr_position.column;
		++m_curr_lexem.length;
		m_source_file >> std::noskipws >> symbol;
	} while (isalpha(symbol) || isdigit(symbol));
};

std::string Lexer::ToLowerString(std::string word)
{
	std::string ret_value;
	std::locale loc;
	for (int i = 0; i < word.size(); ++i)
	{
		ret_value.push_back(std::tolower(word[i], loc));
	}
	return ret_value;
};

std::ostream& operator<<(std::ostream& stream, const LexemInfo& elem)
{
	stream.width(15);
	stream << std::left << elem.source_text;
	stream.width(10);
	stream << elem.position.ToString();
	stream.width(10);
	stream << elem.length;
	stream.width(5);
	stream << elem.code;
	return stream;
};

std::ofstream& operator<<(std::ofstream& stream, const LexerTable& elem)
{
	stream.width(15);
	stream << std::left << "LEXEM TEXT";
	stream.width(10);
	stream << "POSITION";
	stream.width(10);
	stream << "LENGTH";
	stream.width(5);
	stream << "CODE";
	stream << "\n\n";
	for (int i = 0; i < elem.lexemes.size(); ++i)
	{
		stream << elem.lexemes[i] << "\n";
	}
	stream << "\n";
	if (elem.lexical_errors.empty())
	{
		stream << "No lexical errors\n";
	}
	else
	{
		stream << "LEXICAL ERRORS:\n";
		for (int i = 0; i < elem.lexical_errors.size(); ++i)
		{
			stream << elem.lexical_errors[i] << "\n";
		}
	}

	return stream;
}
