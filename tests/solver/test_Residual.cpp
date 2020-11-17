#include "../../src/solver/Residual.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include <iostream>

TEST(ResidualTEST_2elements, Reset) {
    Residual res = Residual(2,10);
    res.Reset();
    for(int iElement=0;iElement<2;iElement++){
        EXPECT_EQ(*res.GetRho(iElement),0);
        EXPECT_EQ(*res.GetU(iElement),0);
        EXPECT_EQ(*res.GetV(iElement),0);
        EXPECT_EQ(*res.GetE(iElement),0);
    }
}  

TEST(ResidualTEST_2elements, SolveRMS) {
    Residual res = Residual(2,10);
    res.Reset();
    for(int iElement=0;iElement<2;iElement++){
        *res.GetRho(iElement)=iElement*1;
        *res.GetU(iElement)=iElement*2;
        *res.GetV(iElement)=iElement*3;
        *res.GetE(iElement)=iElement*4;
    }
    res.solveRMS();
    double maxRMS = res.MaxRMS();
    EXPECT_NEAR(maxRMS,pow(8,0.5),1e-8);
}  