#include "TecplotWriter.hpp"
#include "../mesh/Mesh.hpp"
#include "../solver/Solver.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

TecplotWriter::TecplotWriter(/* args */)
{
}

TecplotWriter::TecplotWriter(Mesh* mesh, Solver* solver)
{
    m_mesh = mesh;
    m_solver = solver;
}

TecplotWriter::~TecplotWriter()
{
    m_outfile.close();
}

void TecplotWriter::DrawMesh(std::string outpath){
    std::vector<std::string> options ;
    this -> DrawMesh(outpath, options);
}

void TecplotWriter::DrawMesh(std::string outpath, std::vector<std::string> options){
    m_outfile.open(outpath);
    this->WriteHeader(options);
    this->WriteNewZone(options);
    this->WriteCoord();
    this->WriteVar(options);
    this->WriteFaceConnectivity();
}

void TecplotWriter::WriteHeader(std::vector<std::string> options){
    m_outfile << "TITLE = \"Example Solution File\"\nVARIABLES =\"X\",\"Y\"";
    for(int i = 0; i < options.size(); i++){
        m_outfile << ", \"" << options[i] << "\"";
    }
    m_outfile << std::endl;  
}

void TecplotWriter::WriteNewZone(std::vector<std::string> options){
    m_outfile << "ZONE "
              << "ZONETYPE=FEPOLYGON, "
              << "NODES=" + std::to_string(m_mesh->m_nNode) + ", "
              << "ELEMENTS=" + std::to_string(m_mesh->m_nElement) + ", "
              << "FACES=" + std::to_string(m_mesh->m_nFace) + ", " 
              << "NUMCONNECTEDBOUNDARYFACES=0, TOTALNUMBOUNDARYCONNECTIONS=0\n" 
              << "DATAPACKING=BLOCK";
    if(!options.empty()){
        m_outfile << " VARLOCATION=(";
        int start = m_mesh->m_nDime+1;
        for(int i = start; i < start+options.size(); i++){
            m_outfile << "[" << std::to_string(i) << "]=CELLCENTERED";
            if(i!=start+options.size()-1){
                m_outfile << ",";
            }
        }        
        m_outfile << ")";
    }
    m_outfile << std::endl;
}


void TecplotWriter::WriteCoord(){
    // Write coordinates
    int nDime = m_mesh->m_nDime; 
    for(int iDime = 0;iDime<nDime;iDime++){ 
        for(int iNode = 0; iNode<m_mesh->m_nNode;iNode++){
            m_outfile << std::to_string(m_mesh->m_coor[iNode*nDime+iDime]) << "\n";
        } 
    }
}

void TecplotWriter::WriteVar(std::vector<std::string> options){
    std::string var;
    for(int i = 0; i < options.size(); i++){
        var = options[i];
        if(var=="volume"){
            for(int iElement = 0; iElement<m_mesh->m_nElement; iElement++){
                m_outfile << std::to_string(m_mesh->m_element2Volume[iElement]) << "\n";
            }
        } else if(var=="pressure"){
            for(int iElement = 0; iElement<m_mesh->m_nElement; iElement++){
                m_outfile << std::to_string(m_solver->m_element2W[iElement].P) << "\n";
            }
        } else if(var=="u"){
            for(int iElement = 0; iElement<m_mesh->m_nElement; iElement++){
                m_outfile << std::to_string(m_solver->m_element2W[iElement].u) << "\n";
            }
        } else if(var=="v"){
            for(int iElement = 0; iElement<m_mesh->m_nElement; iElement++){
                m_outfile << std::to_string(m_solver->m_element2W[iElement].v) << "\n";
            }
        } else if(var=="rho"){
            for(int iElement = 0; iElement<m_mesh->m_nElement; iElement++){
                m_outfile << std::to_string(m_solver->m_element2W[iElement].rho) << "\n";
            }
        } else if(var=="rhoRMS"){
            for(int iElement = 0; iElement<m_mesh->m_nElement; iElement++){
                m_outfile << std::to_string(*(m_solver->m_element2Residuals->GetRho(iElement))) << "\n";       
            }                 
        } else{
            throw;
        }   
    }
}

void TecplotWriter::WriteFaceConnectivity(){
    // writes face to node connectivity
    for(int iFace = 0;iFace<m_mesh->m_nFace;iFace++){
        int start = iFace*2;
        int stop = start+2;
        for(int index = start;index<stop;index++){
            m_outfile << std::to_string(m_mesh->m_face2Node[index]+1) << " ";
        }
        m_outfile << std::endl;
    }    

    // write list of left and right elements for each face
    int nElement;
    for(int side = 0; side<2;side++){
        for(int iFace = 0;iFace<m_mesh->m_nFace;iFace++){
            nElement = m_mesh->m_face2Element[iFace*2+side]+1;
            if(nElement > m_mesh->m_nElement){
                nElement = 0; 
            }
            m_outfile << std::to_string(nElement) << "\n";        
        } 
        m_outfile << std::endl; 
    }
}