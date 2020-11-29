#include "Marker.hpp"
#include "../inputParser/InputParser.h"
#include "../postprocessing/Coefficient.hpp"
#include "../solver/Solver.hpp"
#include "../utils/logger/Logger.hpp"
#include "Mesh.hpp"
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>

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
		// Border conditions are considered farfield by default
		m_updateFunction = &Marker::Update_farfield;
	}
}

int Marker::GetNElement()
{
	return m_nElement;
}

int Marker::Check4Face(int* nodes, int nnode, int nface){
	int bNodes[2];
	// int bNodeEndIndex, bNodeIndex;
	// int bNNode;
	int match;
	for (int iElement = 0; iElement < m_nElement; iElement++)
	{
		// bNodeIndex = m_element2NodeStart[iElement];
		// bNodeEndIndex=m_element2NodeStart[iElement+1];
		// bNNode = bNodeEndIndex-bNodeIndex;
		// if (nnode != bNNode){
		// 	continue;
		// }
		bNodes[0] = m_element2Node[2*iElement+0];
		bNodes[1] = m_element2Node[2*iElement+1];

		// Check array equality
		match = this->AreArraysEqual(nodes, bNodes, 2);
		if(match == 0){
			break;
		}

	}
	if(match == 0){
		m_containingFaces[m_containingFacesNextEmptyIndex] = nface;
		m_containingFacesNextEmptyIndex++;
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
			std::cout << std::to_string(iElement1) << " : " << std::to_string(iElement2) << " - face " << std::to_string(iFace) 
			<< " - i " << std::to_string(i) << std::endl;
			throw std::logic_error("The two elements on either sides of the face are equal");
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

	// // verification that the vector is correctly alligned --------------------------------------------
	// if(iGhostElement < iElement){						// vector should go from elem2 to elem1
	// 	throw std::logic_error("marker error");
	// }
	// // ------------------------------------------------------------------------------------------------

	double nx, ny;
	// Define face vector
	nx = mesh->m_face2Normal[mesh->m_nDime*iFace+0];
	ny = mesh->m_face2Normal[mesh->m_nDime*iFace+1];

	// Make sur the face vector points outwards
	double xb, yb; // vector going from the boundary element center and one of the boundary nodes

	// first point
	double xb1, yb1;
	int nodeb1 = mesh->m_face2Node[2*iFace]; 
	xb1 = mesh->m_coor[nodeb1*nDime+0]-mesh->m_element2Center[iElement*nDime+0];
	yb1 = mesh->m_coor[nodeb1*nDime+1]-mesh->m_element2Center[iElement*nDime+1];
	
	// second point
	double xb2, yb2;
	int nodeb2 = mesh->m_face2Node[2*iFace+1]; 
	xb2 = mesh->m_coor[nodeb2*nDime+0]-mesh->m_element2Center[iElement*nDime+0];
	yb2 = mesh->m_coor[nodeb2*nDime+1]-mesh->m_element2Center[iElement*nDime+1];

	xb = (xb1+xb2)/2;
	yb = (yb1+yb2)/2;

	double faceNormalOrientation = xb*nx+yb*ny;

	// flow orientation
	double uElement, vElement;
	uElement = solver->m_element2W[iElement].u;
	vElement = solver->m_element2W[iElement].v;

	double orientation = xb*uElement+yb*vElement;
	
	// flow velocity
	double gamma = solver->m_inputParameters->m_Gamma;
	double P = solver->m_element2W[iElement].P*solver->m_inputParameters->m_Pressure;
	double rho = solver->m_element2W[iElement].rho*solver->m_inputParameters->m_Density;
	double umach = uElement;
	double vmach = vElement;
	double mach = pow((pow(umach,2)+pow(vmach,2)),0.5)*solver->m_Vref/pow(gamma*P/rho,0.5);

	// Vortex correction
	std::tuple<double, double, double, double> vortexVariables;
	vortexVariables = this->VortexCorrection(mesh, solver, solver->m_inputParameters, index);


	if(orientation<0){ 		// outflow
		if(mach>1){ 	// supersonic
			solver->m_element2W[iGhostElement] = solver->m_element2W[iElement];
		} else{				// subsonic

			double pd = solver->m_element2W[iElement].P;
			double rhod = solver->m_element2W[iElement].rho;
			double ud = uElement;
			double vd = vElement;

			double pa 	= std::get<2>(vortexVariables);//solver->m_Winf->P;
			double ua 	= std::get<0>(vortexVariables);//solver->m_Winf->u;
			double va 	= std::get<1>(vortexVariables);//solver->m_Winf->v;
			double rhoa = std::get<3>(vortexVariables);//solver->m_Winf->rho;

			double c0 = pow(gamma*pd/rhod,0.5);
			double rho0 = rhod;
			
			double pb 	= pa;
			double rhob = rhod	+(pb-pd)/pow(c0,2);
			double ub	= ud	+nx*(pd-pb)/(rho0*c0);
			double vb 	= vd	+ny*(pd-pb)/(rho0*c0);
			double Eb 	= pb/(rhob*(gamma-1))+0.5*(pow(ub,2)+pow(vb,2));
			double Hb 	= Eb +pb/rhob;
			
			solver->m_element2W[iGhostElement].P 	= pb;
			solver->m_element2W[iGhostElement].rho 	= rhob;
			solver->m_element2W[iGhostElement].u 	= ub;
			solver->m_element2W[iGhostElement].v 	= vb;
			solver->m_element2W[iGhostElement].E 	= Eb;
			solver->m_element2W[iGhostElement].H 	= Hb;

			// Logger::getInstance()->AddLog(" subsonic outflow ",1);
			// throw std::logic_error("subsonic condition not implemented");
		}
	} else{					// inflow
		if(mach>1){ 	// supersonic
			solver->m_element2W[iGhostElement] = *(solver->m_Winf);
		} else{				// subsonic
			double pd = solver->m_element2W[iElement].P;
			double rhod = solver->m_element2W[iElement].rho;
			double ud = uElement;
			double vd = vElement;

			double pa 	= std::get<2>(vortexVariables);//solver->m_Winf->P;
			double ua 	= std::get<0>(vortexVariables);//solver->m_Winf->u;
			double va 	= std::get<1>(vortexVariables);//solver->m_Winf->v;
			double rhoa = std::get<3>(vortexVariables);//solver->m_Winf->rho;

			double c0 = pow(gamma*pd/rhod,0.5);
			double rho0 = rhod;

			double pb 	= 0.5*(pa+pd-c0*rho0*(nx*(ua-ud)+ny*(va-vd)));
			double rhob = rhoa+(solver->m_element2W[iGhostElement].P-pa)/pow(c0,2);
			double ub	= ua-nx*(pa-pb)/(rho0*c0);
			double vb 	= va-ny*(pa-pb)/(rho0*c0);
			double Eb 	= pb/(rhob*(gamma-1))+0.5*(pow(ub,2)+pow(vb,2));
			double Hb 	= Eb +pb/rhob;

			solver->m_element2W[iGhostElement].P 	= pb;
			solver->m_element2W[iGhostElement].rho	= rhob;
			solver->m_element2W[iGhostElement].u	= ub;
			solver->m_element2W[iGhostElement].v	= vb;
			solver->m_element2W[iGhostElement].E	= Eb;
			solver->m_element2W[iGhostElement].H	= Hb;

			// Logger::getInstance()->AddLog(" subsonic inflow ",1);
			// solver->m_element2W[iGhostElement] = *(solver->m_Winf);				
			// throw std::logic_error("subsonic condition not implemented");
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

	// verification that the vector is correctly alligned --------------------------------------------
	// // Make sur the face vector points outwards
	// double xb, yb; // vector going from the boundary element center and one of the boundary nodes
	// int nDime = mesh->m_nDime;
	// // first point
	// double xb1, yb1;
	// int nodeb1 = mesh->m_face2Node[2*iFace]; 
	// xb1 = mesh->m_coor[nodeb1*nDime+0]-mesh->m_element2Center[iElement*nDime+0];
	// yb1 = mesh->m_coor[nodeb1*nDime+1]-mesh->m_element2Center[iElement*nDime+1];
	
	// // second point
	// double xb2, yb2;
	// int nodeb2 = mesh->m_face2Node[2*iFace+1]; 
	// xb2 = mesh->m_coor[nodeb2*nDime+0]-mesh->m_element2Center[iElement*nDime+0];
	// yb2 = mesh->m_coor[nodeb2*nDime+1]-mesh->m_element2Center[iElement*nDime+1];

	// xb = (xb1+xb2)/2;
	// yb = (yb1+yb2)/2;

	// double orientation = xb*nx+yb*ny;

	// if(orientation > 0){						// normal should point towards ghost cell
	// 	std::cout<<"out"<<std::endl;
	// 	// throw std::logic_error("marker error");
	// }	else{
	// 	std::cout<<"in"<<std::endl;
	// }
	// ------------------------------------------------------------------------------------------------	
	
	// Define velocity in border element
	uElement = solver->m_element2W[iElement].u;
	vElement = solver->m_element2W[iElement].v;

	// Define velocity in ghost element
	V2 = (uElement*nx+vElement*ny);
	uGhost = uElement-2*V2*nx;
	vGhost = vElement-2*V2*ny;
	// if(V2 < 1-3){						// normal should point towards ghost cell
	// 	std::cout<<"V=0"<<std::endl;
	// 	// throw std::logic_error("marker error");
	// }	
	// Update ghost element
	solver->m_element2W[iGhostElement] = solver->m_element2W[iElement];
	solver->m_element2W[iGhostElement].u = uGhost;
	solver->m_element2W[iGhostElement].v = vGhost;
	
	// std::cout << uGhost << " : " << uElement << "\n"
	// 		  << vGhost << " : " << vElement << "\n"
	// 		  << nx << " : " << ny << "\n"
	// 		  << "-----------------------"<< std::endl;
			  

}

std::tuple<double, double, double, double> Marker::VortexCorrection(Mesh *mesh, Solver *solver, ees2d::io::InputParser *inputParser, int index) {
	int iGhostElement 	= m_ghostElement[index];
	int iElement 		= m_containingElement[index];
	int iFace 			= m_containingFaces[index];
	int nDime 			= mesh->m_nDime;

	// 1. We need the CL from the "normal" calculation, without vortex correction
	// Solve coefficients
	int *wallFaces;
	int nWallFaces;
	mesh->m_markers->GetWallFaces(&wallFaces, &nWallFaces);
	Coefficient coef = Coefficient(solver, mesh);
	coef.Solve(wallFaces, nWallFaces);
	double CL;
	CL = coef.m_CL;

	// 2. We need the chord length = 1
	double a;
	a = 1;

	// 3. We need the previous infinite values
	double pInf = 	solver->m_Winf->P;
	double uInf = 	solver->m_Winf->u;
	double vInf = 	solver->m_Winf->v;
	double rhoInf = solver->m_Winf->rho;

	// 4. We need the geometry variables d and theta
	double d, theta;

	// first point
	double xb1, yb1;
	int nodeb1 = mesh->m_face2Node[2 * iFace];
	xb1 = mesh->m_coor[nodeb1 * nDime + 0];// - mesh->m_element2Center[iElement * nDime + 0];
	yb1 = mesh->m_coor[nodeb1 * nDime + 1];// - mesh->m_element2Center[iElement * nDime + 1];

	// second point
	double xb2, yb2;
	int nodeb2 = mesh->m_face2Node[2 * iFace + 1];
	xb2 = mesh->m_coor[nodeb2 * nDime + 0];// - mesh->m_element2Center[iElement * nDime + 0];
	yb2 = mesh->m_coor[nodeb2 * nDime + 1];// - mesh->m_element2Center[iElement * nDime + 1];

	// Middle point
	double xb, yb;
	xb = (xb1 + xb2) / 2;
	yb = (yb1 + yb2) / 2;

	// The reference point is the center of the grid (0,0)
	double xref, yref;
	xref = 0.25;// if we want 1/4 chord, needs to be changed to 0.25
	yref = 0.25;

	// Calculating the d and theta parameters
	double alpha;
	alpha = inputParser->m_aoa * M_PI / 180.0;//to be verified... needs to be in radians
	d = pow(pow(xb - xref, 2) + pow(yb - yref, 2), 0.5);
	theta = std::atan((yb - yref) / (xb - xref));

	// 5. Computing the VORTEX
	double velocityNorm, vortex;
	velocityNorm = pow(pow(uInf, 2) + pow(vInf, 2), 0.5);
	vortex = 0.5 * velocityNorm * a * CL;

	// 6. Computing the new infinite velocities
	double uInf_star, vInf_star, aInf, MachInf, gamma;
	gamma = inputParser->m_Gamma;
	// aInf = pow(gamma * pInf / rhoInf, 0.5);
	double P = solver->m_element2W[iElement].P*solver->m_inputParameters->m_Pressure;
	double rho = solver->m_element2W[iElement].rho*solver->m_inputParameters->m_Density;
	MachInf = velocityNorm*solver->m_Vref/pow(gamma*P/rho,0.5);
	uInf_star = uInf + (vortex * pow(1 - pow(MachInf, 2), 0.5) / (2 * M_PI * d) * (std::sin(theta)) / (1 - pow(MachInf, 2) * pow(std::sin(theta - alpha), 2)));
	vInf_star = vInf - (vortex * pow(1 - pow(MachInf, 2), 0.5) / (2 * M_PI * d) * (std::cos(theta)) / (1 - pow(MachInf, 2) * pow(std::sin(theta - alpha), 2)));

	// 7. Computing the new infinite pressure and density
	double pInf_star, rhoInf_star, velocity_starNorm;
	velocity_starNorm = pow(uInf_star, 2) + pow(vInf_star, 2);
	pInf_star = pow(pow(pInf, (gamma - 1) / gamma) + (gamma - 1) / gamma * rhoInf * (pow(velocityNorm, 2) - velocity_starNorm) / (2 * pow(pInf, 1 / gamma)), gamma / (gamma - 1));
	rhoInf_star = rhoInf * pow(pInf_star / pInf, 1 / gamma);

	// 8. Return into tuple
	std::tuple<double, double, double, double> result;
	result = std::make_tuple(uInf_star, vInf_star, pInf_star, rhoInf_star);

	return result;
}
