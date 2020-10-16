#include "MeshGenerator.hpp"
#include "MeshReaderSU2.hpp"
#include "../Mesh.hpp"
#include "../../utils/logger/Logger.hpp"



MeshGenerator::MeshGenerator(std::string meshPath){
    reader = std::unique_ptr<MeshReader>(new MeshReaderSU2(meshPath));
}

MeshGenerator::~MeshGenerator(){
    //empty
}

Mesh MeshGenerator::BuildMesh(){
    // Read values from input file
    reader->ReadFile();
    reader->get_values(&m_nDime, &m_nNode, &m_nElement, &m_coor, &m_element2Node, &m_element2NodeStart);

	this->CreateNode2Element();
}

void MeshGenerator::CreateNode2Element(){
    Logger::getInstance()->AddLog("______________________Debut code ESUP________________________",1);
	int startI;
	int endI;	
    int nodeI;
	int j;

    m_node2ElementStart = std::unique_ptr<int[]>(new int[m_nNode+1] ());
    // Looping over the elements to count the number of elements around each nodes
	for (int elemi=0;elemi<m_nElement;++elemi){
		// Starting and ending indices for the node connectivity
		startI =m_element2NodeStart[elemi];
		endI =m_element2NodeStart[elemi+1];

		// Looping over the nodes of elementI
		for (int i=startI;i<endI;++i){
			nodeI = m_element2Node[i];
			m_node2ElementStart[nodeI+1] += 1;
		}
	}    


	// Setting up the start offset linked list
	for (int i=1;i<m_nNode+1;++i){
		m_node2ElementStart[i] += m_node2ElementStart[i-1];
	}

	// Initializing the node to element linked list connectivity
	m_node2Element = std::unique_ptr<int[]>(new int[m_element2NodeStart[m_nElement]]());
	
	// Array to save the increment
	int store[m_nNode]={0};

	for (int elemi=0;elemi<m_nElement;++elemi){
		startI=m_node2ElementStart[elemi];
		endI=m_node2ElementStart[elemi+1];

		for (int i=startI;i<endI;++i){
			nodeI=m_element2Node[i];
			// Fetching the index for the connectivity list
			j=m_node2ElementStart[nodeI]+store[nodeI];
			// Adding the element index for nodeI
			m_node2Element[j] = elemi;
			store[nodeI]+=1;
		}
	}
}