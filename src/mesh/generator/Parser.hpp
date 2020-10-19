#pragma once
#define PARSER_H

#include <fstream>
#include <memory>
#include <regex>
#include <string>
#include <sstream>

class Parser
// Extract data from a mesh text file word by word
// a word is defined as a group of characters surrounded by whitespace
{
protected:
	std::string m_commentString; // This string indicates the start of a comment

public:
	std::ifstream m_inFile;

	//Parser();
	Parser(std::string meshPath, std::string commentString);
	//Parser() = default;
	~Parser();

	// Return the next non empty line from m_meshPath
	std::string GetNextNonNullLine();

	// Return the next word from m_meshPath
	std::string GetNextWord();

	// Return the next integer word that can be found in the file
	int ExtractNextInt();

	// Return the next double word that can be found in the file
	double ExtractNextDouble();

	// Return next word that matches e in the file
	std::string FindNextMatchingString(std::regex *e);
};