#include "inputParser/InputParser.h"
#include "mesh/MarkerContainer.hpp"
#include "mesh/Mesh.hpp"
#include "mesh/generator/MeshGenerator.hpp"
#include "mesh/generator/MeshReader.hpp"
#include "mesh/generator/MeshReaderSU2.hpp"
#include "mesh/generator/Parser.hpp"
#include "mesh/metrics/MetricsGenerator.hpp"
#include "postprocessing/Coefficient.hpp"
#include "postprocessing/TecplotWriter.hpp"
#include "solver/Solver.hpp"
#include "utils/logger/Logger.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
#include <array>

void show_usage() {
	std::cerr << "Usage: "
	          << "Options:\n"
	          << "\t-h,--help\t\tShow this help message\n"
	        //   << "\t-i,--input\tSpecify the file path for the EES2D Software Input file\n"
			  << "\t-v,--verbose\tOutput debugging information\n"
			  << "\t-vv,--veryverbose\tOutput EVERYTHING !?!?!?!?\n"
	          << std::endl;
}

int main(int argc, char *argv[]) {
	std::string inpath=argv[1];
	for (int i = 2; i < argc; i++) {
		std::string arg = argv[i];
		if ((arg == "-h") || (arg == "--help")) {
			show_usage();
			return 0;
		} else if ((arg == "-i") || (arg == "--input")) {
			inpath = argv[++i];
			Logger::getInstance()->AddLog("input mesh path = " + inpath,0);
		// } else if ((arg == "-v") || (arg == "--verbose")) {
		// 	argv[++i];
		// 	Logger::getInstance()->SetVerbosity(1);
		} else if ((arg == "-vv") || (arg == "--veryverbose")) {
			argv[++i];
			Logger::getInstance()->SetVerbosity(2);
		}
	}
	time_t timeBeg;
	time(&timeBeg);
	// Logger::getInstance()->AddLog(std::to_string(std::array::max_size())+"\n", 1);
	ees2d::io::InputParser inputParameters{inpath};
	inputParameters.parse();
	inputParameters.printAll();

	Mesh mesh = Mesh();

	MeshReaderSU2 reader(inputParameters.m_meshFile, &mesh);
	reader.ReadFile();

	clock_t start, end;
	double cpu_time_used;
	start = clock();

	MeshGenerator generator(&mesh);
	generator.BuildMesh();

	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	Logger::getInstance()->AddLog("MeshGenerator time = " + std::to_string(cpu_time_used) + " seconds\n", 1);


	MetricsGenerator metrics(&mesh);
	metrics.Solve();

	Solver solver(&mesh, &inputParameters);
	solver.SolveFc();

	std::vector<std::string> options{"volume","pressure","u","v","rho","rhoRMS"};
	TecplotWriter writer(&mesh, &solver);
	writer.DrawMesh(inputParameters.m_outputFile, options);

	// Solve coefficients
	int* wallFaces;
	int nWallFaces;
	mesh.m_markers->GetWallFaces(&wallFaces, &nWallFaces);
	Coefficient coef = Coefficient(&solver, &mesh);
	coef.Solve(wallFaces, nWallFaces);
	coef.Write(inputParameters.m_outputPressure);
	Logger::getInstance()->AddLog("CL = " + std::to_string(coef.m_CL) + "| CD = " + std::to_string(coef.m_CD), 1);

	time_t timeEnd;
	time(&timeEnd);

	int timeInterval = (intmax_t)timeEnd - (intmax_t)timeBeg;
	Logger::getInstance()->AddLog("simulation time = " + std::to_string(timeInterval) + " seconds", 1);

	return 0;
}
