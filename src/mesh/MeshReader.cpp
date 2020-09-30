#include "MeshReader.hpp"
// #include "Mesh.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <sstream>      // std::stringstream
#include <limits>       // std::numeric_limits

//MeshReader::MeshReader(){}
/*
MeshReader::MeshReader(std::string mesh_path)//:MeshReader::MeshReader()
{
    infile.open(mesh_path, std::ifstream::in);
    if (!infile) 
    {
        std::cout << "Can't open input file " << mesh_path << std::endl;
        exit(1);
    }
    //std::cout << "open? " << infile.is_open() << std::endl;

    ndim = MeshReader::Read_int();
    std::cout << "NDIM = " << ndim << std::endl;

    nnode = MeshReader::Read_int();
    std::cout << "NNODE = " << nnode << std::endl;

    MeshReader::Read_Coord();
    std::cout << "finished coord" << std::endl;

    nelem = MeshReader::Read_int();
    std::cout << "NELEM = " << nelem << std::endl;

    // Creation of the element to node connectivity matrix array
    inpoel = new int*[nelem];
    elem_type = new int[nelem];
    MeshReader::Read_connect(nelem, inpoel, elem_type);
    std::cout << "finished connect" << std::endl;

    MeshReader::Read_marks();
    std::cout << "finished marks" << std::endl;
    
}
*/

void MeshReader::get_Mesh(std::string mesh_path)
{
    double **COOR;
    int **CONNEC;
    int *elem_type;
    int *NPSUE; 
    int NELEM;
    int NPOIN;
    int NDIME;

    int nmark;
    int** connec_mark_node;
    int* nmark_elem;
    std::string* mark_tag;

    std::string line;
    std::ifstream infile;

   // Mesh::Mesh mesh;

    infile.open(mesh_path, std::ifstream::in);
    if (!infile) 
    {
        std::cout << "Can't open input file " << mesh_path << std::endl;
        exit(1);
    }
    //std::cout << "open? " << infile.is_open() << std::endl;

    NDIME = MeshReader::Read_int(&infile, &line);
    std::cout << "NDIM = " << NDIME << std::endl;

    NPOIN = MeshReader::Read_int(&infile, &line);
    std::cout << "NNODE = " << NPOIN << std::endl;

    MeshReader::Read_Coord(&infile, &line, &NDIME, &NPOIN, COOR);
    std::cout << "finished coord" << std::endl;

    NELEM = MeshReader::Read_int(&infile, &line);
    std::cout << "NELEM = " << NELEM << std::endl;

    // Creation of the element to node connectivity matrix array
    CONNEC = new int*[NELEM];
    elem_type = new int[NELEM];
    MeshReader::Read_connect(&infile, &line, &NELEM, CONNEC, elem_type);
    std::cout << "finished connect" << std::endl;

    MeshReader::Read_marks();
    std::cout << "finished marks" << std::endl;
    
    
}

int MeshReader::Read_int(std::ifstream* infile, std::string *line)
{
    MeshReader::Next_line(infile, line);
    int val;
    std::regex e ("([^0-9\\.]*)()"); // remove all non numeric chars
    *line = std::regex_replace(*line, e, "$2");
    val = std::stoi(*line);
    return val;
}

void MeshReader::Read_Coord(std::ifstream* infile, std::string* line, int* NDIME, int* NPOIN, double** COOR)
{
    double val;
    std::string empty_str = "";

    // Creation of the coordinates array
    COOR = new double*[*NDIME];
    for(int idim=0; idim<*NDIME; idim++)
    {
        COOR[idim] = new double[*NPOIN]; 
    }
    
    // Filling of the coordinates array
    for(int inode = 0; inode<*NPOIN;inode++)
    {  
        MeshReader::Next_line(infile, line);
        
        std::string::size_type pt = empty_str.size();

        for(int idim =0; idim<*NDIME;idim++)
        {
            //std::cout << line << std::endl;
            val = std::stod ((*line).substr(pt), &pt);
            //std::cout.precision(20);
            //std::cout << "Value" << idim << " = "<<val << std::endl;
            COOR[idim][inode] = val;
            
        }
    }
    std::cout << "end coord" << std::endl;
    
}

int MeshReader::VTK_elem_2_nnode(int VTKid)
{       
    int number_of_nodes;
    switch (VTKid)
    {
    case 3:
        number_of_nodes = 2;
        break;
    
    case 5:
        number_of_nodes = 3;
        break;

    case 9:
        number_of_nodes = 4;
        break;

    case 10:
        number_of_nodes = 4;
        break;

    case 12:
        number_of_nodes = 8;
        break;

    case 13:
        number_of_nodes = 6;
        break;

    case 14:
        number_of_nodes = 5;
        break;

    }

    return number_of_nodes;
}

void MeshReader::Read_connect(std::ifstream* infile, std::string* line, int* NELEM, int** connectivity_matrix, int* element_type)
{
    int val;
    int number_of_nodes;
    std::string empty_str = "";

    // Filling of the connectivity matrix
    for(int ielem = 0; ielem<*NELEM; ielem++)
    {  
        MeshReader::Next_line(infile, line);
        std::string::size_type pt = empty_str.size();

        val = std::stoi ((*line).substr(pt), &pt);
        element_type[ielem] = val;

        number_of_nodes = MeshReader::VTK_elem_2_nnode(val);

        connectivity_matrix[ielem] = new int[number_of_nodes]; 

        for(int inode =0; inode<number_of_nodes; inode++)
        {
            val = std::stod ((*line).substr(pt), &pt);
            //std::cout.precision(20);
            //std::cout << "Value" << idim << " = "<<val << std::endl;
            connectivity_matrix[ielem][inode] = val;
        }
    }
}

void MeshReader::Next_line(std::ifstream* infile, std::string* line)
{
    std::getline(*infile, *line);        
    //std::cout << "open? " << infile.is_open() << std::endl;
    while (*line == "")
    {
        // std::cout << line << std::endl;
        std::getline(*infile, *line);
    }
}

void MeshReader::Read_marks()
{
    // nmark = MeshReader::Read_int();
    // std::cout << "NMARK = " << nmark << std::endl;
}
