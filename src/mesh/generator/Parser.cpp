#include "Parser.hpp"
#include <filesystem>
#include <iostream>
#include <memory>
#include <regex>
#include <string>

Parser::Parser(std::string meshPath) {
	m_inFile.open(meshPath, std::ifstream::in);
	if (!m_inFile) {
		std::cout << "Can't open input file " << meshPath << std::endl;
		std::cout << "This is the current working directory: " << std::filesystem::current_path() << std::endl;
		exit(1);
	}
}

Parser::~Parser() {
	m_inFile.close();
}

int Parser::ExtractNextInt() {
	int val;
	std::regex e("-?[0-9]+");
	val = std::stoi(this->FindNextMatchingString(&e));
	return val;
}

double Parser::ExtractNextDouble() {
	double val;
	std::regex e("-?[0-9]+\\.?[0-9]+");
	val = std::stod(this->FindNextMatchingString(&e));
	return val;
}

std::string Parser::GetNextNonNullLine() {
	std::string line;
	std::getline(m_inFile, line);
	while (line == "") {
		// std::cout << line << std::endl;
		std::getline(m_inFile, line);
	}
	return line;
}

std::string Parser::GetNextWord() {
	std::string word;
	m_inFile >> word;
	//std::cout << word << std::endl;
	return word;
}

std::string Parser::FindNextMatchingString(std::regex *e) {
	std::string word;
	word = this->GetNextWord();
	while (!std::regex_match(word, *e)) {
		word = this->GetNextWord();
	}
	return word;
}