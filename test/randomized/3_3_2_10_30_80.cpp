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
    c.addInstruction(CircuitInstruction({ Matrix({ { -0.42919548035887267 + -0.5193692662325289i, 0.5363368717409196 + -0.5083203369370202i }, { 0.5594858795588588 + -0.48272389158488055i, -0.47376712416252664 + -0.47905939833194777i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.013807130506925834 + 0.7932363552993742i, 0.45956258033030306 + -0.3992338694783841i }, { 0.49923545662528657 + 0.34835241728265803i, 0.07022484972464149 + 0.7902423822490301i } }) }, { QubitIndex{ 0 } }));


    return c;
}

TEST(RandomizedIntegrationTests, 3_3_2_10_30_80) {
    auto c = randomCircuit();
    
    auto s = c.execute();

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, false}), Matrix({ { 0.6294145522214704 + 0.0i, 0.28321881094833246 + 0.39120196676183644i }, { 0.28321881094833246 + -0.3912019667618365i, 0.37058544777852953 + -1.3877787807814457e-17i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, false}), ElementsAreArray({ DoubleNear(0.6294145522214704, 1.0e-7), DoubleNear(0.37058544777852953, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, false}), Matrix({ { 0.4539531950673988 + 0.0i, 0.010583142534079468 + -0.49776268326342255i }, { 0.010583142534079468 + 0.49776268326342255i, 0.5460468049326012 + -1.3877787807814457e-17i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, false}), ElementsAreArray({ DoubleNear(0.4539531950673988, 1.0e-7), DoubleNear(0.5460468049326012, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, true}), Matrix({ { 1.0 + -1.3877787807814457e-17i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, true}), ElementsAreArray({ DoubleNear(1.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, true, false}), Matrix({ { 0.2857247470028526 + 0.0i, 0.12856808413318513 + 0.177587382728186i, 0.006661183919183623 + -0.3132990763988047i, 0.19772308571789865 + -0.13683560911446524i }, { 0.12856808413318513 + -0.17758738272818603i, 0.16822844806454618 + 0.0i, -0.19172839562870125 + -0.14511590146217074i, 0.003921958614895846 + -0.18446360686461782i }, { 0.0066611839191836235 + 0.3132990763988047i, -0.19172839562870125 + 0.1451159014621707i, 0.34368980521861775 + 0.0i, 0.15465072681514735 + 0.21361458403365044i }, { 0.19772308571789865 + 0.13683560911446524i, 0.003921958614895846 + 0.18446360686461782i, 0.15465072681514733 + -0.21361458403365047i, 0.20235699971398338 + -1.3877787807814457e-17i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, true, false}), ElementsAreArray({ DoubleNear(0.2857247470028526, 1.0e-7), DoubleNear(0.16822844806454618, 1.0e-7), DoubleNear(0.34368980521861775, 1.0e-7), DoubleNear(0.20235699971398338, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, true}), Matrix({ { 0.6294145522214704 + 0.0i, 0.28321881094833246 + 0.39120196676183644i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.28321881094833246 + -0.3912019667618365i, 0.37058544777852953 + -1.3877787807814457e-17i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, true}), ElementsAreArray({ DoubleNear(0.6294145522214704, 1.0e-7), DoubleNear(0.37058544777852953, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, true}), Matrix({ { 0.4539531950673988 + 0.0i, 0.010583142534079468 + -0.49776268326342255i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.010583142534079468 + 0.49776268326342255i, 0.5460468049326012 + -1.3877787807814457e-17i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i }, { 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i, 0.0 + 0.0i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, true}), ElementsAreArray({ DoubleNear(0.4539531950673988, 1.0e-7), DoubleNear(0.5460468049326012, 1.0e-7), DoubleNear(0.0, 1.0e-7), DoubleNear(0.0, 1.0e-7) }));


}
}

}
