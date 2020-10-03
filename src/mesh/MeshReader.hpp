#define MESHREADER_H

#include <string>
#include <fstream>

class MeshReader
{
protected:   
    static int     Read_int(std::ifstream* infile, std::string* line);
    static void    Read_Coord(std::ifstream* infile, std::string* line, int* NDIME, int* NPOIN, double** COOR);
    static void    Read_connect(std::ifstream* infile, std::string* line, int* NELEM, int** connectivity_matrix, int* element_type);
    static void    Next_line(std::ifstream* infile, std::string* line);
    static void    Read_marks();
    static int     VTK_elem_2_nnode(int VTKid);

public:
    static void get_Mesh(std::string mesh_path);
};



