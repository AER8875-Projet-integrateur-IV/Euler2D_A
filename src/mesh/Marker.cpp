#include "Marker.hpp"
#include <string>
#include <memory>
#include "../utils/logger/Logger.hpp"
#include <cmath>
#include "Mesh.hpp"
#include "../solver/Solver.hpp"

Marker::Marker()
{
	m_element2Node = NULL;
	m_element2NodeStart = NULL;
	m_element2VTKId = NULL;
	m_containingFaces = NULL;
}

Marker::~Marker()
{
	delete[] m_element2Node;
	delete[] m_element2NodeStart;
	delete[] m_element2VTKId;
	delete[] m_containingFaces;
	delete[] m_containingElement;
	delete[] m_ghostElement;
}

void Marker::Set(std::string* tag, int* nElement, int* element2Node, int* element2NodeStart, int* m_element2VTKId){
	m_tag = *tag;
	m_nElement= *nElement;
	m_element2Node = element2Node;
	m_element2NodeStart = element2NodeStart;
	m_element2VTKId = m_element2VTKId;
	m_containingFaces = new int[*nElement];
	m_containingFacesNextEmptyIndex = 0;    
	m_containingElement = new int[*nElement];
    m_ghostElement = new int[*nElement];

	this->DefineUpdateFunction(m_tag);

	Logger::getInstance()->AddLog(*tag + " marker created with " + std::to_string(m_nElement) + " border elements",1);
}

void Marker::DefineUpdateFunction(std::string borderType){
	if(borderType=="farfield"){
		m_updateFunction = &Marker::Update_farfield;
	} else if(borderType=="wall"){
		m_updateFunction = &Marker::Update_wall;
	} else
	{
		// not implemented
	}
}

int Marker::GetNElement()
{
	return m_nElement;
}

int Marker::Check4Face(int* nodes, int nnode, int nface){
	int* bNodes;
	int bNodeEndIndex, bNodeIndex;
	int bNNode;
	int match;
	for (int iElement = 0; iElement < m_nElement; iElement++)
	{
		bNodeIndex = m_element2NodeStart[iElement];
		bNodeEndIndex=m_element2NodeStart[iElement+1];
		bNNode = bNodeEndIndex-bNodeIndex;
		if (nnode != bNNode){
			continue;
		}
		bNodes = m_element2Node+bNodeIndex;

		// Check array equality
		match = this->AreArraysEqual(nodes, bNodes, bNNode);
		
	}
	if(match == 0){
		m_containingFaces[m_containingFacesNextEmptyIndex] = nface;
		return 0;
	} else{
		return 1;
	}
	
}
int Marker::AreArraysEqual(int* ar1, int* ar2, int length){
	int val1, val2;
	int match = 0;
	val2 = ar2[0];
	for (int i1 = 0; i1 < length; i1++)
	{
		val1 = ar1[i1];
		if(val1==val2){
			match++;
			for (int i2 = 1; i2 < length; i2++){
				val2 = ar2[i2];
				i1++;
				if(i1 == length){
					i1 = 0;
				} 
				val1 = ar1[i1];
				if(val1 == val2){
					match++;
				}
			}
		}
	}
	if(match == length){
		return 0;
	} else{
		return 1;
	}
}

void Marker::FindElements(Mesh* mesh){
	int iFace, iElement1, iElement2;
	for(int i = 0;i<m_nElement;i++){
		iFace = m_containingFaces[i];
		iElement1 = mesh->m_face2Element[2*iFace+0];
		iElement2 = mesh->m_face2Element[2*iFace+1];
		if(iElement1 < iElement2){
			m_containingElement[i] = iElement1;
			m_ghostElement[i] = iElement2;
		} else if(iElement1 > iElement2){
			m_containingElement[i] = iElement2;
			m_ghostElement[i] = iElement1;
		} else{
			throw "The two elements on either sides of the face are equal";
		}
	}
}

void Marker::Update(Mesh* mesh, Solver* solver){
	for(int i = 0; i<m_containingFacesNextEmptyIndex;i++){
		(this->*(m_updateFunction))(mesh, solver, i);
	}
}

void Marker::Update_farfield(Mesh* mesh, Solver* solver, int index){
	int iGhostElement = m_ghostElement[index];
	int iElement = m_containingElement[index];
	int iFace = m_containingFaces[index];
	int nDime = mesh->m_nDime;

	double nx, ny;
	// Define face vector
	nx = mesh->m_face2Normal[mesh->m_nDime*iFace+0];
	ny = mesh->m_face2Normal[mesh->m_nDime*iFace+1];

	// Make sur the face vector points outwards
	double xb, yb; // vector going from the boundary element center and one of the boundary nodes
	int nodeb = mesh->m_face2Node[2*iFace]; 
	xb = mesh->m_coor[nodeb*nDime+0]-mesh->m_element2Center[iElement*nDime+0];
	yb = mesh->m_coor[nodeb*nDime+1]-mesh->m_element2Center[iElement*nDime+1];

	double faceNormalOrientation = xb*nx+yb*ny;

	// flow orientation
	double uElement, vElement;
	uElement = solver->m_element2W[iElement].u;
	vElement = solver->m_element2W[iElement].v;

	double velocityOrientation = uElement*nx+vElement*ny;

	double orientation = faceNormalOrientation*velocityOrientation;
	
	// flow velocity
	double velocity = pow((pow(uElement,2)+pow(vElement,2)),0.5);
	
	if(orientation<0){ 		// outflow
		if(velocity>1){ 	// supersonic
			solver->m_element2W[iGhostElement] = solver->m_element2W[iElement];
		} else{				// subsonic
			throw "not implemented";
		}
	} else{					// inflow
		if(velocity>1){ 	// supersonic
			solver->m_element2W[iGhostElement] = *(solver->m_Winf);
		} else{				// subsonic
			throw "not implemented";
		}
	}
}

void Marker::Update_wall(Mesh* mesh, Solver* solver, int index){
	int iGhostElement = m_ghostElement[index];
	int iElement = m_containingElement[index];
	int iFace = m_containingFaces[index];

	double nx, ny, uElement, vElement, uGhost, vGhost, V2;
	// Define face normal vector
	nx = mesh->m_face2Normal[mesh->m_nDime*iFace+0];
	ny = mesh->m_face2Normal[mesh->m_nDime*iFace+1];
	
	// Define velocity in border element
	uElement = solver->m_element2W[iElement].u;
	vElement = solver->m_element2W[iElement].v;

	// Define velocity in ghost element
	V2 = uElement*nx+vElement*ny;
	uGhost = uElement-2*V2*nx;
	vGhost = vElement-2*V2*ny;

	// Update ghost element
	solver->m_element2W[iGhostElement] = solver->m_element2W[iElement];
	solver->m_element2W[iGhostElement].u = uGhost;
	solver->m_element2W[iGhostElement].v = vGhost;
}
