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
    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.22209745738728603 + 0.49571930736840863i, -0.24829503390699367 + 0.02333397855340033i }, { 0.25569337590396735 + 0.14269908966050276i, 0.16795916760361507 + -0.5868464603001216i } }), Matrix({ { -0.26997334336832945 + -0.07460029309601646i, 0.3828229140807292 + 0.07493591564083611i }, { 0.1686920400605853 + -0.3934901858356905i, 0.27996764987661266 + -0.003029219069703361i } }), Matrix({ { 0.02384071476090216 + -0.2108762054307635i, 0.16225487780811593 + 0.14100004275149153i }, { 0.2534304076579048 + 0.2230962973189718i, 0.016335316749279336 + 0.2483890439359415i } }), Matrix({ { -0.06242352997443017 + 0.004566457689184202i, 0.015433842479309906 + 0.36197912241199104i }, { -0.3121921683798025 + -0.31150125479143204i, -0.21830469669745206 + 0.2180609299809534i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 3 } }));


    return c;
}

TEST(RandomizedIntegrationTests, 4_3_2_10_30_80) {
    auto c = randomCircuit();
    
    auto s = c.execute();

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, false, false}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, false, false}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, false, false}), Matrix({ { 0.42247042546268154 + 0.0i, -0.025176084314528554 + -0.040002926636900506i }, { -0.025176084314528554 + 0.040002926636900506i, 0.5775295745373186 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, false, false}), ElementsAreArray({ DoubleNear(0.42247042546268154, 1.0e-7), DoubleNear(0.5775295745373186, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, true, false}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, true, false}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, false, true}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, false, true}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, true, false, false}), Matrix({ { 0.42247042546268154 + 0.0i, 0.0 + 0.0i, -0.025176084314528554 + -0.040002926636900506i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { -0.025176084314528554 + 0.040002926636900506i, 0.0 + 0.0i, 0.5775295745373186 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, true, false, false}), ElementsAreArray({ DoubleNear(0.42247042546268154, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.5775295745373186, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, true, false}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, true, false}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, false, true}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, false, true}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, true, false}), Matrix({ { 0.42247042546268154 + 0.0i, -0.025176084314528554 + -0.040002926636900506i, 0.0 + 0.0i, 0.0 + 0.0i }, { -0.025176084314528554 + 0.040002926636900506i, 0.5775295745373186 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, true, false}), ElementsAreArray({ DoubleNear(0.42247042546268154, 1.0e-7), DoubleNear(0.5775295745373186, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, false, true}), Matrix({ { 0.42247042546268154 + 0.0i, -0.025176084314528554 + -0.040002926636900506i, 0.0 + 0.0i, 0.0 + 0.0i }, { -0.025176084314528554 + 0.040002926636900506i, 0.5775295745373186 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, false, true}), ElementsAreArray({ DoubleNear(0.42247042546268154, 1.0e-7), DoubleNear(0.5775295745373186, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, true, true}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, true, true}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, true, true, false}), Matrix({ { 0.42247042546268154 + 0.0i, 0.0 + 0.0i, -0.025176084314528554 + -0.040002926636900506i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { -0.025176084314528554 + 0.040002926636900506i, 0.0 + 0.0i, 0.5775295745373186 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, true, true, false}), ElementsAreArray({ DoubleNear(0.42247042546268154, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.5775295745373186, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, true, false, true}), Matrix({ { 0.42247042546268154 + 0.0i, 0.0 + 0.0i, -0.025176084314528554 + -0.040002926636900506i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { -0.025176084314528554 + 0.040002926636900506i, 0.0 + 0.0i, 0.5775295745373186 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, true, false, true}), ElementsAreArray({ DoubleNear(0.42247042546268154, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.5775295745373186, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, true, true}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, true, true}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, true, true}), Matrix({ { 0.42247042546268154 + 0.0i, -0.025176084314528554 + -0.040002926636900506i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { -0.025176084314528554 + 0.040002926636900506i, 0.5775295745373186 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, true, true}), ElementsAreArray({ DoubleNear(0.42247042546268154, 1.0e-7), DoubleNear(0.5775295745373186, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));


}
}

}
