#include "../../src/solver/Solver.hpp"
#include "../../src/inputParser/InputParser.h"
#include "../../src/mesh/Mesh.hpp"
#include "../../src/mesh/generator/MeshGenerator.hpp"
#include "../../src/mesh/generator/MeshReaderSU2.hpp"
#include "../../src/mesh/metrics/MetricsGenerator.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include <iostream>

class SolverTEST_Roe_EulerExplicit : public ::testing::Test {
protected:
    Solver* solver = NULL;
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
    solver->m_element2Residuals->solveRMS();
    EXPECT_NEAR(solver->m_element2Residuals->MaxRMS(),0,1e-10);
    
}