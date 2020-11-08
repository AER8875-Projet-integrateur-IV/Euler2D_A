#include "Mesh.hpp"
#include <memory>


Mesh::Mesh(){
	m_coor = NULL;
	m_element2Node = NULL;
	m_element2NodeStart = NULL;	
	m_element2VTKId = NULL; 	// Stores type of element according to VTK format
    m_node2Element = NULL;
	m_node2ElementStart = NULL;	
	m_element2Element = NULL;
	m_element2ElementStart = NULL;	
    m_element2Face = NULL;
	m_element2FaceStart = NULL;	
    m_face2Element = NULL;
	m_face2Node = NULL;
	m_markers = NULL;
    m_element2Volume = NULL;
	m_face2FaceVector = NULL;
	m_face2Normal = NULL;
	m_face2Area = NULL;
	m_element2Center = NULL;	
}

Mesh::~Mesh(){
    delete[] m_coor;
    delete[] m_element2Node;
    delete[] m_element2NodeStart;
    delete[] m_element2VTKId;
    delete[] m_node2Element;
    delete[] m_node2ElementStart;
    delete[] m_element2Element;
    delete[] m_element2ElementStart;
    delete[] m_element2Face;
    // delete[] m_element2FaceStart;  // deja delete par m_element2ElementStart 
    delete[] m_face2Element;
    delete[] m_face2Node;
    delete m_markers;
    delete[] m_element2Volume;
    delete[] m_face2FaceVector;
    delete[] m_face2Normal;
    delete[] m_face2Area;
    delete[] m_element2Center;
}
