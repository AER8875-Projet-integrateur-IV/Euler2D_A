#include "../../src/solver/Solver.hpp"
#include "../../src/inputParser/InputParser.h"
#include "../../src/mesh/Mesh.hpp"
#include "../../src/mesh/generator/MeshGenerator.hpp"
#include "../../src/mesh/generator/MeshReaderSU2.hpp"
#include "../../src/mesh/metrics/MetricsGenerator.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

class SolverTEST_Roe_EulerExplicit : public ::testing::Test {
protected:
    Solver* solver;
    Mesh mesh;
    std::string path = "../../../tests/solver/testFiles/test_roe_eulerexplicit_diagonalized.ees2d";
    ees2d::io::InputParser inputParameters{path};

    SolverTEST_Roe_EulerExplicit(){
        inputParameters.parse();

        mesh = Mesh();

        MeshReaderSU2 reader(inputParameters.m_meshFile, &mesh); 
        reader.ReadFile();

        MeshGenerator generator(&mesh);
        generator.BuildMesh();

        MetricsGenerator metrics(&mesh);
        metrics.Solve();
        
        solver = new Solver(&mesh, &inputParameters);
    }

    ~SolverTEST_Roe_EulerExplicit(){
        delete solver;
    }
};

TEST_F(SolverTEST_Roe_EulerExplicit, Roe_FluxSumNull) {
    // Check that the flux sum on all elements is equal to zero on a simple mesh

    for(int iFace = 0;iFace<solver->m_mesh->m_nFaceNoBoundaries;iFace++){
        // Calcul Fc
		solver->ConvectiveFluxRoeScheme(iFace);
	}

    int start, stop;
    double R1, R2, R3, R4, x, y;
    for(int iElement=0; iElement<solver->m_mesh->m_nElement; iElement++){
        R1 = R2 = R3 = R4 = x = y = 0;
        start = solver->m_mesh->m_element2FaceStart[iElement];
        stop = solver->m_mesh->m_element2FaceStart[iElement+1];
        for (int iFace = start; iFace<stop; iFace++){
            int face = solver->m_mesh->m_element2Face[iFace];
            double dS = solver->m_mesh->m_face2Area[face];
            x += solver->m_mesh->m_face2FaceVector[2*face+0];
            y += solver->m_mesh->m_face2FaceVector[2*face+1];

            R1 += solver->m_face2Fc[face].H*dS;
            R2 += solver->m_face2Fc[face].rho*dS;
            R3 += solver->m_face2Fc[face].u*dS;
            R4 += solver->m_face2Fc[face].v*dS;
        }
        EXPECT_NEAR(x,0,1e-15);
        EXPECT_NEAR(y,0,1e-15);
        EXPECT_NEAR(R1,0,1e-15);
        EXPECT_NEAR(R2,0,1e-15);
        EXPECT_NEAR(R3,0,1e-15);
        EXPECT_NEAR(R4,0,1e-15);
    }

}