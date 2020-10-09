#include "mesh/generator/MeshReaderSU2.hpp"
#include "mesh/generator/Parser.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

void show_usage() {
	std::cerr << "Usage: "
	          << "Options:\n"
	          << "\t-h,--help\t\tShow this help message\n"
	          << "\t-i,--input\tSpecify the file path for the SU2 path\n"

	          << std::endl;
}

int main(int argc, char *argv[]) {

	std::string inpath;

	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		if ((arg == "-h") || (arg == "--help")) {
			show_usage();
			return 0;
		} else if ((arg == "-i") || (arg == "--input")) {
			inpath = argv[++i];
			std::cout << "\tinput mesh path = " << inpath << std::endl;
		}
	}

	return 0;
}