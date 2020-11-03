#pragma once
#define TECPLOTWRITER_H

#include "../mesh/Mesh.hpp"
#include <fstream>
#include <vector>

class TecplotWriter
{
private:
    Mesh* m_mesh;
    std::ofstream m_outfile;

public:
    TecplotWriter();
    TecplotWriter(Mesh* Mesh);

    ~TecplotWriter();

void DrawMesh(std::string outpath);
void DrawMesh(std::string outpath, std::vector<std::string> options);
void WriteHeader(std::vector<std::string> options);
void WriteNewZone(std::vector<std::string> options);
void WriteCoord();
void WriteFaceConnectivity();
void WriteVar(std::vector<std::string> options);
};


