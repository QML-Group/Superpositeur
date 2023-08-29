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
    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.0354715013412521 + -0.39942950113971276i, 0.10654589065497518 + -0.013316503268332598i }, { -0.18947622125026412 + -0.2750083783136793i, 0.3511940037982345 + 0.13029839869723864i } }), Matrix({ { -0.04262142167847174 + 0.07690362579143571i, 0.18719427441810327 + 0.5783101637608401i }, { -0.08927400794521836 + -0.01523009065086069i, 0.007822876151170423 + -0.0038456795614230216i } }), Matrix({ { -0.09387820397539003 + -0.09080138682185335i, 0.36228690659655444 + 0.383019245078982i }, { -0.40233295319988593 + 0.6481686947917367i, -0.042411937385148805 + 0.005000205202580681i } }), Matrix({ { 0.3210276335415151 + -0.03853045487206731i, 0.2406661017980672 + -0.331478656729914i }, { 0.08991736465019586 + -0.007314448970852944i, 0.17477695071874066 + -0.021698852275578304i } }) }, { QubitIndex{ 1 } }));


    return c;
}

TEST(RandomizedIntegrationTests, 3_4_2_100_30_20) {
    auto c = randomCircuit();
    
    auto s = c.execute();

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, false}), Matrix({ { 0.9999999999999998 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, false}), ElementsAreArray({ DoubleNear(0.9999999999999998, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, false}), Matrix({ { 0.290134253514216 + 0.0i, 0.11382265576708417 + 0.17418761591399362i }, { 0.11382265576708417 + -0.17418761591399362i, 0.7098657464857837 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, false}), ElementsAreArray({ DoubleNear(0.290134253514216, 1.0e-7), DoubleNear(0.7098657464857837, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, true}), Matrix({ { 0.9999999999999998 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, true}), ElementsAreArray({ DoubleNear(0.9999999999999998, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, true, false}), Matrix({ { 0.290134253514216 + 0.0i, 0.0 + 0.0i, 0.11382265576708417 + 0.17418761591399362i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.11382265576708417 + -0.17418761591399362i, 0.0 + 0.0i, 0.7098657464857837 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, true, false}), ElementsAreArray({ DoubleNear(0.290134253514216, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.7098657464857837, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, true}), Matrix({ { 0.9999999999999998 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, true}), ElementsAreArray({ DoubleNear(0.9999999999999998, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, true}), Matrix({ { 0.290134253514216 + 0.0i, 0.11382265576708417 + 0.17418761591399362i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.11382265576708417 + -0.17418761591399362i, 0.7098657464857837 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, true}), ElementsAreArray({ DoubleNear(0.290134253514216, 1.0e-7), DoubleNear(0.7098657464857837, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));


}
}

}
