#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <chrono>
#include <iostream>

#include "superpositeur/Circuit.hpp"
#include "superpositeur/DefaultOperations.hpp"

namespace superpositeur {

using namespace std::complex_literals;

using testing::ElementsAreArray;
using testing::DoubleNear;

namespace {
Circuit randomCircuit() {

    Circuit c;
    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.6601169889188971 + 0.4494971127202702i, 0.40645153483138385 + -0.4438412513838719i }, { -0.5993111491731257 + -0.054991390904185394i, 0.1860622567264195 + -0.7766485241247049i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 1 } }));


    return c;
}

TEST(RandomizedIntegrationTests, 3_3_2_10_30_80) {
    auto c = randomCircuit();
    
    auto s = c.execute();

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, false}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, false}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, false}), Matrix({ { 0.6378020934032107 + 0.0i, -0.42033394265359075 + -0.23308787979425427i }, { -0.42033394265359075 + 0.23308787979425427i, 0.3621979065967894 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, false}), ElementsAreArray({ DoubleNear(0.6378020934032107, 1.0e-7), DoubleNear(0.3621979065967894, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, true}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, true}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, true, false}), Matrix({ { 0.6378020934032107 + 0.0i, 0.0 + 0.0i, -0.42033394265359075 + -0.23308787979425427i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { -0.42033394265359075 + 0.23308787979425427i, 0.0 + 0.0i, 0.3621979065967894 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, true, false}), ElementsAreArray({ DoubleNear(0.6378020934032107, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.3621979065967894, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, true}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, true}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, true}), Matrix({ { 0.6378020934032107 + 0.0i, -0.42033394265359075 + -0.23308787979425427i, 0.0 + 0.0i, 0.0 + 0.0i }, { -0.42033394265359075 + 0.23308787979425427i, 0.3621979065967894 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, true}), ElementsAreArray({ DoubleNear(0.6378020934032107, 1.0e-7), DoubleNear(0.3621979065967894, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));


}
}

}
