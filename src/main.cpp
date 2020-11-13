#include "mesh/generator/MeshReader.hpp"
#include "mesh/generator/MeshReaderSU2.hpp"
#include "mesh/generator/Parser.hpp"
#include "mesh/generator/MeshGenerator.hpp"
#include "mesh/Mesh.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "utils/logger/Logger.hpp"
#include "mesh/metrics/MetricsGenerator.hpp"
#include "postprocessing/TecplotWriter.hpp"
#include "inputParser/InputParser.h"
#include "solver/Solver.hpp"

void show_usage() {
	std::cerr << "Usage: "
	          << "Options:\n"
	          << "\t-h,--help\t\tShow this help message\n"
	          << "\t-i,--input\tSpecify the file path for the EES2D Software Input file\n"
			  << "\t-v,--verbose\tOutput debugging information\n"
			  << "\t-vv,--veryverbose\tOutput EVERYTHING !?!?!?!?\n"
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
			Logger::getInstance()->AddLog("input mesh path = " + inpath,0);
		} else if ((arg == "-v") || (arg == "--verbose")) {
			argv[++i];
			Logger::getInstance()->SetVerbosity(1);
		} else if ((arg == "-vv") || (arg == "--veryverbose")) {
			argv[++i];
			Logger::getInstance()->SetVerbosity(2);
		}
	}

	ees2d::io::InputParser inputParameters{inpath};
	inputParameters.parse();
	inputParameters.printAll();

	Mesh mesh = Mesh();

	MeshReaderSU2 reader(inputParameters.m_meshFile, &mesh);
	reader.ReadFile();

	MeshGenerator generator(&mesh);
	generator.BuildMesh();

	MetricsGenerator metrics(&mesh);
	metrics.Solve();

	Solver solver(&mesh, &inputParameters);
	solver.SolveFc();

	std::vector<std::string> options{"volume","pressure","u","v","rho"};
	TecplotWriter writer(&mesh, &solver);
	writer.DrawMesh(inputParameters.m_outputFile, options);

	return 0;
}
