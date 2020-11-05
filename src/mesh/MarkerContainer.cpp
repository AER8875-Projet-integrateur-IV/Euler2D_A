#include "MarkerContainer.hpp"
#include "Marker.hpp"
#include <memory>

MarkerContainer::MarkerContainer(int nMark)
{
    m_nMark = nMark;
    m_markers = std::unique_ptr<Marker[]>(new Marker[nMark]);
    m_nextEmptyMark = 0;
}

MarkerContainer::~MarkerContainer()
{
}

void MarkerContainer::AddMarker(std::string* tag, int* nElement, int* element2Node, int* element2NodeStart, int* element2VTKId)
{
    (m_markers[m_nextEmptyMark]).Set(tag, nElement, element2Node, element2NodeStart, element2VTKId);
    m_nextEmptyMark++;
}

int MarkerContainer::GetNElement()
{
    int nElement = 0;
    for(int i=0;i<m_nMark;i++){
        nElement = nElement + m_markers[i].GetNElement();
    }
    return nElement;
}

void MarkerContainer::Check4Face(int* face2Node, int* face2NodeStart, int FirstBorderFace, int LastBorderFace){
    int match;
    int nnode;
    int* nodes;
    int nodeIndexStart, nodeIndexEnd;

    for(int iFace=FirstBorderFace;iFace<LastBorderFace;iFace++){
        nodeIndexStart = face2NodeStart[iFace];
        nodeIndexEnd = face2NodeStart[iFace+1];
        nnode = nodeIndexEnd-nodeIndexStart;
        nodes = face2Node+nodeIndexStart;    
        for(int iMark = 0; iMark<m_nMark;iMark++){
            match = m_markers[iMark].Check4Face(nodes,nnode,iFace);
            if(match == 0){
                break;
            }
        }
        throw "No match was found for a border condition face in the available markers";
    }
}

void MarkerContainer::FindElements(Mesh* mesh){
    // Find the associated elements
    for(int iMark = 0; iMark<m_nMark;iMark++){
        m_markers[iMark].FindElements(mesh);
    }

}

void MarkerContainer::Update(Mesh* mesh, Solver* solver){
    for(int iMark = 0; iMark<m_nMark;iMark++){
        m_markers[iMark].Update(mesh, solver);
    }
}