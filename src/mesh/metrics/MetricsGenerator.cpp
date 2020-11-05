#include "MetricsGenerator.hpp"
#include "../../utils/logger/Logger.hpp"
#include <stdexcept>
#include <string>
#include <iostream>
#include <cmath>


MetricsGenerator::MetricsGenerator(Mesh* mesh)
{
    m_mesh = mesh;
}

MetricsGenerator::~MetricsGenerator()
{
}

void MetricsGenerator::Solve(){
	this->SolveVolume();
	this->SolveFaceVector();
	this->SolveElement2Center();
}

void MetricsGenerator::SolveVolume(){
	// Volume fonctionne seulement pour 2D
	int indexStart;
	int indexEnd;
	int numberOfNode;

	int node1;
	int node2;
	int node3;
	int node4;

	double x1, x2, x3 , x4, y1, y2, y3, y4;

	double volume;

	m_mesh->m_element2Volume = new double[m_mesh->m_nElement];
	for(int iElement = 0; iElement<m_mesh->m_nElement; iElement++){
		indexStart =	m_mesh->m_element2NodeStart[iElement];
		int VTKId = m_mesh->m_element2VTKId[iElement];

		node1 = m_mesh->m_element2Node[indexStart];
		node2 = m_mesh->m_element2Node[indexStart+1];
		node3 = m_mesh->m_element2Node[indexStart+2];

		x1 = m_mesh->m_coor[node1 * m_mesh->m_nDime + 0];
		x2 = m_mesh->m_coor[node2 * m_mesh->m_nDime + 0];
		x3 = m_mesh->m_coor[node3 * m_mesh->m_nDime + 0];

		y1 = m_mesh->m_coor[node1 * m_mesh->m_nDime + 1];
		y2 = m_mesh->m_coor[node2 * m_mesh->m_nDime + 1];
		y3 = m_mesh->m_coor[node3 * m_mesh->m_nDime + 1];

		if (VTKId==5){  // Triangle

			volume = this->GetTriangleVolume(x1,x2,x3,y1,y2,y3);
		} else if(VTKId==9){ // Quad
			node4 = m_mesh->m_element2Node[indexStart+3];
			x4 = m_mesh->m_coor[node4 * m_mesh->m_nDime + 0];
			y4 = m_mesh->m_coor[node4 * m_mesh->m_nDime + 1];

			volume = 0.5*((x1-x3)*(y2-y4)+(x4-x2)*(y1-y3));
		} else{
			throw std::invalid_argument(std::to_string(VTKId)+" is not a valid VTK element");
		}
		m_mesh->m_element2Volume[iElement] = volume;
		//printf("%f ",m_mesh->m_element2Volume[iElement]);
	}
	Logger::getInstance()->AddLog("Volumes calculated",1);
}

void MetricsGenerator::SolveFaceVector(){
	m_mesh->m_face2FaceVector = new double[m_mesh->m_nDime*m_mesh->m_nFace];
	m_mesh->m_face2Normal = new double[m_mesh->m_nDime*m_mesh->m_nFace];
	m_mesh->m_face2Area = new double[m_mesh->m_nFace];

	int node1, node2;

	double x1,x2,y1,y2,area;

	for(int iFace = 0; iFace<m_mesh->m_nFace; iFace++){
		node1 = m_mesh->m_face2Node[iFace*m_mesh->m_nDime+0];
		node2 = m_mesh->m_face2Node[iFace*m_mesh->m_nDime+1];

		x1 = m_mesh->m_coor[node1 * m_mesh->m_nDime + 0];
		x2 = m_mesh->m_coor[node2 * m_mesh->m_nDime + 0];

		y1 = m_mesh->m_coor[node1 * m_mesh->m_nDime + 1];
		y2 = m_mesh->m_coor[node2 * m_mesh->m_nDime + 1];

		m_mesh->m_face2FaceVector[iFace*m_mesh->m_nDime+0] = y2-y1;
		m_mesh->m_face2FaceVector[iFace*m_mesh->m_nDime+1] = x1-x2;

		area = pow((pow((x2-x1),2.0)+pow((y2-y1),2.0)),0.5);
		m_mesh->m_face2Area[iFace] = area;

		m_mesh->m_face2Normal[iFace*m_mesh->m_nDime+0] = (y2-y1)/area;
		m_mesh->m_face2Normal[iFace*m_mesh->m_nDime+1] = (x1-x2)/area;
	}
	Logger::getInstance()->AddLog("Face vectors calculated",1);
}

void MetricsGenerator::SolveElement2Center(){
	// Volume fonctionne seulement pour 2D
	int indexStart;
	int indexEnd;
	int numberOfNode;

	int node1;
	int node2;
	int node3;
	int node4;

	double x1, x2, x3 , x4, y1, y2, y3, y4;

	double vol1,  vol2;

	double center [2];
	double center1 [2];
	double center2 [2];

	m_mesh->m_element2Center = new double[m_mesh->m_nDime*m_mesh->m_nFace];
	for(int iElement = 0; iElement<m_mesh->m_nElement; iElement++){
		indexStart =	m_mesh->m_element2NodeStart[iElement];
		indexEnd =	m_mesh->m_element2NodeStart[iElement+1];
		numberOfNode = indexEnd-indexStart;

		node1 = m_mesh->m_element2Node[indexStart];
		node2 = m_mesh->m_element2Node[indexStart+1];
		node3 = m_mesh->m_element2Node[indexStart+2];

		x1 = m_mesh->m_coor[node1 * m_mesh->m_nDime + 0];
		x2 = m_mesh->m_coor[node2 * m_mesh->m_nDime + 0];
		x3 = m_mesh->m_coor[node3 * m_mesh->m_nDime + 0];

		y1 = m_mesh->m_coor[node1 * m_mesh->m_nDime + 1];
		y2 = m_mesh->m_coor[node2 * m_mesh->m_nDime + 1];
		y3 = m_mesh->m_coor[node3 * m_mesh->m_nDime + 1];
		if (numberOfNode==3){
			this->GetTriangleCenter(x1,x2,x3,y1,y2,y3,center);

		} else{
			node4 = m_mesh->m_element2Node[indexStart+3];
			x4 = m_mesh->m_coor[node4 * m_mesh->m_nDime + 0];
			y4 = m_mesh->m_coor[node4 * m_mesh->m_nDime + 1];

			this->GetTriangleCenter(x1,x2,x3,y1,y2,y3,center1);
			this->GetTriangleCenter(x1,x3,x4,y1,y3,y4,center2);
			vol1 = this->GetTriangleVolume(x1,x2,x3,y1,y2,y3);
			vol2 = this->GetTriangleVolume(x1,x3,x4,y1,y3,y4);
			center[0] = (vol1 * center1[0] + vol2 * center2[0]) / (vol1 + vol2);
			center[1] = (vol1 * center1[1] + vol2 * center2[1]) / (vol1 + vol2);
		}
		m_mesh->m_element2Center[iElement * m_mesh->m_nDime + 0] = center[0];
		m_mesh->m_element2Center[iElement * m_mesh->m_nDime + 1] = center[1];

	}
	Logger::getInstance()->AddLog("Element centers calculated",1);
}

void MetricsGenerator::GetTriangleCenter(double x1, double x2, double x3, double y1, double y2, double y3, double* center){
	center[0] = (x1+x2+x3)/3;
	center[1] = (y1+y2+y3)/3;
}

double MetricsGenerator::GetTriangleVolume(double x1, double x2, double x3, double y1, double y2, double y3){
	return 0.5*((x1-x2)*(y1+y2)+(x2-x3)*(y2+y3)+(x3-x1)*(y3+y1));
}
