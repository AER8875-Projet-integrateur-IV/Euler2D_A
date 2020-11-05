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

void show_usage() {
	std::cerr << "Usage: "
	          << "Options:\n"
	          << "\t-h,--help\t\tShow this help message\n"
	          << "\t-i,--input\tSpecify the file path for the SU2 path\n"
			  << "\t-v,--verbose\tOutput debugging information\n"
			  << "\t-vv,--veryverbose\tOutput EVERYTHING !?!?!?!?\n"
	          << std::endl;
}

int main(int argc, char *argv[]) {

	std::cout << "Main is running" << std::endl;

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

	Mesh mesh = Mesh();

	MeshReaderSU2 reader(inpath, &mesh);
	reader.ReadFile();

	MeshGenerator generator(&mesh);
	generator.BuildMesh();

	MetricsGenerator metrics(&mesh);
	metrics.Solve();

	printf("nFace = %2d \n",mesh.m_nFace);
	printf("Face2Element = \n");
	for (int i=0;i<mesh.m_nFace*2;i++){
		printf("%2d ",mesh.m_face2Element[i]);
	}
	printf("\n");


	// printf("element2volume = \n" );
	// for (int i=0;i<mesh.m_nElement;i++){
	// 	printf("%f ",metrics.m_mesh->m_element2Volume[i]);
	// }
	// printf("\n");
	//
	// printf("face2FaceVector = \n");
	// for (int i=0;i<mesh.m_nFace*2;i++){
	// 	printf("%f ",metrics.m_mesh->m_face2FaceVector[i]);
	// }
	// printf("\n");
	//
	// printf("face2Normal = \n");
	// for (int i=0;i<mesh.m_nFace*2;i++){
	// 	printf("%f ",metrics.m_mesh->m_face2Normal[i]);
	// }
	// printf("\n");
	//
	// printf("face2Area = \n");
	// for (int i=0;i<mesh.m_nFace;i++){
	// 	printf("%f ",metrics.m_mesh->m_face2Area[i]);
	// }
	// printf("\n");
	//
	// printf("element2Center = \n");
	// for (int i=0;i<mesh.m_nElement*2;i++){
	// 	printf("%f ",metrics.m_mesh->m_element2Center[i]);
	// }
	// printf("\n");


	return 0;
}
