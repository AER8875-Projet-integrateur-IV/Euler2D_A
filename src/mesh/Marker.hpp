#pragma once
#define MARKER_H

#include <string>
#include <memory>

class Marker
{
protected:
    std::string m_tag;
    int m_nElement;
    int* m_element2Node;
    int* m_element2NodeStart;

public:
    Marker();
    ~Marker();

    void Set(std::string* tag, int* nElement, int* element2Node, int* element2NodeStart);
    int GetNElement();
};

