#include "../../src/mesh/Mesh.hpp"
#include "../../src/mesh/generator/MeshGenerator.hpp"
#include "../../src/mesh/generator/MeshReaderSU2.hpp"
#include "../../src/mesh/metrics/MetricsGenerator.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

class MetricsGeneratorTest_single_triangle : public ::testing::Test {
protected:
    Mesh mesh;
    std::string path = "../../../../tests/mesh/metrics/single_triangle.su2";

    MetricsGeneratorTest_single_triangle(){
        mesh = Mesh();

        MeshReaderSU2 reader(path, &mesh); 
        reader.ReadFile();

        MeshGenerator generator(&mesh);
        generator.BuildMesh();

        MetricsGenerator metrics(&mesh);
        metrics.Solve();
    }

    ~MetricsGeneratorTest_single_triangle(){
    }
};

TEST_F(MetricsGeneratorTest_single_triangle, deltaS) {
    // Check that the face areas are correct

    double* areas = mesh.m_face2Area;
    EXPECT_NEAR(areas[1],1.414,1e-3);
    EXPECT_NEAR(areas[0],1,1e-15);
    EXPECT_NEAR(areas[2],1,1e-15);

}

TEST_F(MetricsGeneratorTest_single_triangle, FaceVectors) {
    // Check that the face areas are correct

    double sumx, sumy =0;
    for(int i =0; i<3;i++){
        sumx += mesh.m_face2FaceVector[2*i+0];
        sumy += mesh.m_face2FaceVector[2*i+1];
    }
    EXPECT_NEAR(sumx,0,1e-15);
    EXPECT_NEAR(sumy,0,1e-15);
}
