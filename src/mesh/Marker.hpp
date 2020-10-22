#pragma once
#define MARKER_H

#include <string>
#include <memory>

class Marker
{
protected:
    std::string m_tag;
    int m_nElement;
    std::unique_ptr<int[]> m_element2Node;
    std::unique_ptr<int[]> m_element2NodeStart;

public:
    Marker();
    ~Marker();

    void Set(std::string* tag, int* nElement, std::unique_ptr<int[]>* element2Node, std::unique_ptr<int[]>* element2NodeStart);
    int GetNElement();
};

