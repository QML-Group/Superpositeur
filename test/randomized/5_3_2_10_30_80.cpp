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
    c.addInstruction(CircuitInstruction({ Matrix({ { 0.36871169934767756 + -0.38623860742459093i, -0.05243559730272361 + -0.14198631987161098i }, { 0.22349190495166904 + -0.3341321345145857i, 0.5129658079949744 + -0.13049852436031772i } }), Matrix({ { 0.064440303990646 + -0.36561775528193774i, 0.3601747234348836 + 0.064151285443376i }, { 0.048495626618525495 + -0.050216019579431226i, 0.11085952732031883 + 0.023342011551859894i } }), Matrix({ { 0.12375811145299338 + 0.3875682470484055i, 0.10453633251607881 + -0.11321273849013631i }, { -0.2557859382655414 + -0.08449150761902918i, 0.47571398628307304 + 0.36342383690740937i } }), Matrix({ { 0.11691824360151064 + -0.007592013406357439i, 0.11358584184528286 + 0.20990973335587296i }, { 0.3712723110276134 + 0.14462024474606364i, 0.05768799545831572 + -0.3283798251276172i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 1 } }));


    return c;
}

TEST(RandomizedIntegrationTests, 5_3_2_10_30_80) {
    auto c = randomCircuit();
    
    auto s = c.execute();

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, false, false, false}), Matrix({ { 0.6022102054819591 + 0.0i, 0.21085248918715932 + -0.08602314925814111i }, { 0.21085248918715932 + 0.08602314925814111i, 0.39778979451804036 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, false, false, false}), ElementsAreArray({ DoubleNear(0.6022102054819591, 1.0e-7), DoubleNear(0.39778979451804036, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, false, false, false}), Matrix({ { 0.9999999999999996 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, false, false, false}), ElementsAreArray({ DoubleNear(0.9999999999999996, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, true, false, false}), Matrix({ { 0.9999999999999996 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, true, false, false}), ElementsAreArray({ DoubleNear(0.9999999999999996, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, false, true, false}), Matrix({ { 0.9999999999999996 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, false, true, false}), ElementsAreArray({ DoubleNear(0.9999999999999996, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, false, false, true}), Matrix({ { 0.9999999999999996 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, false, false, true}), ElementsAreArray({ DoubleNear(0.9999999999999996, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, true, false, false, false}), Matrix({ { 0.6022102054819591 + 0.0i, 0.21085248918715932 + -0.08602314925814111i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.21085248918715932 + 0.08602314925814111i, 0.39778979451804036 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, true, false, false, false}), ElementsAreArray({ DoubleNear(0.6022102054819591, 1.0e-7), DoubleNear(0.39778979451804036, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, true, false, false}), Matrix({ { 0.6022102054819591 + 0.0i, 0.21085248918715932 + -0.08602314925814111i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.21085248918715932 + 0.08602314925814111i, 0.39778979451804036 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, true, false, false}), ElementsAreArray({ DoubleNear(0.6022102054819591, 1.0e-7), DoubleNear(0.39778979451804036, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, false, true, false}), Matrix({ { 0.6022102054819591 + 0.0i, 0.21085248918715932 + -0.08602314925814111i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.21085248918715932 + 0.08602314925814111i, 0.39778979451804036 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, false, true, false}), ElementsAreArray({ DoubleNear(0.6022102054819591, 1.0e-7), DoubleNear(0.39778979451804036, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, false, false, true}), Matrix({ { 0.6022102054819591 + 0.0i, 0.21085248918715932 + -0.08602314925814111i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.21085248918715932 + 0.08602314925814111i, 0.39778979451804036 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, false, false, true}), ElementsAreArray({ DoubleNear(0.6022102054819591, 1.0e-7), DoubleNear(0.39778979451804036, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, true, false, false}), Matrix({ { 0.9999999999999996 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, true, false, false}), ElementsAreArray({ DoubleNear(0.9999999999999996, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, false, true, false}), Matrix({ { 0.9999999999999996 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, false, true, false}), ElementsAreArray({ DoubleNear(0.9999999999999996, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, false, false, true}), Matrix({ { 0.9999999999999996 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, false, false, true}), ElementsAreArray({ DoubleNear(0.9999999999999996, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, true, true, false}), Matrix({ { 0.9999999999999996 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, true, true, false}), ElementsAreArray({ DoubleNear(0.9999999999999996, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, true, false, true}), Matrix({ { 0.9999999999999996 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, true, false, true}), ElementsAreArray({ DoubleNear(0.9999999999999996, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, false, true, true}), Matrix({ { 0.9999999999999996 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, false, true, true}), ElementsAreArray({ DoubleNear(0.9999999999999996, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, true, true, false, false}), Matrix({ { 0.6022102054819591 + 0.0i, 0.21085248918715932 + -0.08602314925814111i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.21085248918715932 + 0.08602314925814111i, 0.39778979451804036 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, true, true, false, false}), ElementsAreArray({ DoubleNear(0.6022102054819591, 1.0e-7), DoubleNear(0.39778979451804036, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, true, false, true, false}), Matrix({ { 0.6022102054819591 + 0.0i, 0.21085248918715932 + -0.08602314925814111i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.21085248918715932 + 0.08602314925814111i, 0.39778979451804036 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, true, false, true, false}), ElementsAreArray({ DoubleNear(0.6022102054819591, 1.0e-7), DoubleNear(0.39778979451804036, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, true, false, false, true}), Matrix({ { 0.6022102054819591 + 0.0i, 0.21085248918715932 + -0.08602314925814111i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.21085248918715932 + 0.08602314925814111i, 0.39778979451804036 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, true, false, false, true}), ElementsAreArray({ DoubleNear(0.6022102054819591, 1.0e-7), DoubleNear(0.39778979451804036, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, true, true, false}), Matrix({ { 0.6022102054819591 + 0.0i, 0.21085248918715932 + -0.08602314925814111i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.21085248918715932 + 0.08602314925814111i, 0.39778979451804036 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, true, true, false}), ElementsAreArray({ DoubleNear(0.6022102054819591, 1.0e-7), DoubleNear(0.39778979451804036, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, true, false, true}), Matrix({ { 0.6022102054819591 + 0.0i, 0.21085248918715932 + -0.08602314925814111i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.21085248918715932 + 0.08602314925814111i, 0.39778979451804036 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, true, false, true}), ElementsAreArray({ DoubleNear(0.6022102054819591, 1.0e-7), DoubleNear(0.39778979451804036, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, false, true, true}), Matrix({ { 0.6022102054819591 + 0.0i, 0.21085248918715932 + -0.08602314925814111i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.21085248918715932 + 0.08602314925814111i, 0.39778979451804036 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, false, true, true}), ElementsAreArray({ DoubleNear(0.6022102054819591, 1.0e-7), DoubleNear(0.39778979451804036, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, true, true, false}), Matrix({ { 0.9999999999999996 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, true, true, false}), ElementsAreArray({ DoubleNear(0.9999999999999996, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, true, false, true}), Matrix({ { 0.9999999999999996 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, true, false, true}), ElementsAreArray({ DoubleNear(0.9999999999999996, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, false, true, true}), Matrix({ { 0.9999999999999996 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, false, true, true}), ElementsAreArray({ DoubleNear(0.9999999999999996, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, true, true, true}), Matrix({ { 0.9999999999999996 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, true, true, true}), ElementsAreArray({ DoubleNear(0.9999999999999996, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));


}
}

}
