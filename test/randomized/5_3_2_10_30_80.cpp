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
    c.addInstruction(CircuitInstruction({ Matrix({ { -0.18937288658218643 + -0.07016615064387116i, -0.13414526639133165 + 0.1469002678194625i }, { 0.33090532119014454 + -0.11930046506017242i, -0.26384251276691983 + -0.22674828033756805i } }), Matrix({ { -0.3582571910821785 + 0.23825979747518802i, 0.04669990666715867 + 0.20482892796455845i }, { 0.21352136405283662 + 0.13035418506396546i, -0.24475901417818136 + -0.045113445222227586i } }), Matrix({ { 0.11198435803931243 + 0.19131432891886296i, -0.043158776204651784 + 0.18590860243301538i }, { -0.47636087912188474 + 0.18479593754423504i, 0.11467766546210627 + 0.28517994741926705i } }), Matrix({ { 0.23995827298823202 + 0.20228652270894257i, 0.08459207540836046 + 0.5206679367332646i }, { -0.3990476411361459 + -0.14083476674056405i, 0.36323085722256276 + -0.438439166446674i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 3 } }));


    return c;
}

TEST(RandomizedIntegrationTests, 5_3_2_10_30_80) {
    auto c = randomCircuit();
    
    auto s = c.execute();

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, false, false, false}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, false, false, false}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, false, false, false}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.6264571961360197 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, false, false, false}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, true, false, false}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, true, false, false}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, false, true, false}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.6264571961360197 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, false, true, false}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, false, false, true}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, false, false, true}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, true, false, false, false}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.6264571961360197 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, true, false, false, false}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, true, false, false}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, true, false, false}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, false, true, false}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.6264571961360197 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, false, true, false}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, false, false, true}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, false, false, true}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, true, false, false}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.6264571961360197 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, true, false, false}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, false, true, false}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, -0.24196565835693856 + -0.10699315357528859i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { -0.24196565835693856 + 0.10699315357528859i, 0.0 + 0.0i, 0.0 + 0.0i, 0.6264571961360197 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, false, true, false}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, false, false, true}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.6264571961360197 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, false, false, true}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, true, true, false}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.6264571961360197 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, true, true, false}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, true, false, true}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, true, false, true}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, false, true, true}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.6264571961360197 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, false, true, true}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, true, true, false, false}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.6264571961360197 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, true, true, false, false}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, true, false, true, false}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, -0.24196565835693856 + -0.10699315357528859i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { -0.24196565835693856 + 0.10699315357528859i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.6264571961360197 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, true, false, true, false}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, true, false, false, true}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.6264571961360197 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, true, false, false, true}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, true, true, false}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.6264571961360197 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, true, true, false}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, true, false, true}), Matrix({ { 1.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, true, false, true}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, false, true, true}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.6264571961360197 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, false, true, true}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, true, true, false}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, -0.24196565835693856 + -0.10699315357528859i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { -0.24196565835693856 + 0.10699315357528859i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.6264571961360197 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, true, true, false}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, true, false, true}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.6264571961360197 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, true, false, true}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, false, true, true}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, -0.24196565835693856 + -0.10699315357528859i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { -0.24196565835693856 + 0.10699315357528859i, 0.0 + 0.0i, 0.0 + 0.0i, 0.6264571961360197 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, false, true, true}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, true, true, true}), Matrix({ { 0.3735428038639804 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.6264571961360197 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, true, true, true}), ElementsAreArray({ DoubleNear(0.3735428038639804, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.6264571961360197, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));


}
}

}
