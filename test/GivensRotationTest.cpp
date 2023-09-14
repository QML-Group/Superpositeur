#include "superpositeur/GivensRotation.hpp"
#include <gtest/gtest.h>

namespace superpositeur {

using namespace std::complex_literals;

class MixedStateSimplifierTest : public ::testing::Test {};

using BV = BasisVector<64>;

TEST_F(MixedStateSimplifierTest, ApplyGivensRotation) {
    SparseVector<64> a = {{BV(0), 1.}, {BV(1), 1.}};
    SparseVector<64> b = {{BV(0), 1.}, {BV(1), 1.}};

    std::uint64_t hashA = BV(0).hash() + BV(1).hash();
    std::uint64_t hashB = BV(0).hash() + BV(1).hash();

    applyGivensRotation(a, hashA, b, hashB);

    EXPECT_TRUE(a.empty());
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b[0].ket, BV(0));
    EXPECT_DOUBLE_EQ(b[0].amplitude.real(), std::sqrt(2));
    EXPECT_DOUBLE_EQ(b[0].amplitude.imag(), 0.);
    EXPECT_EQ(b[1].ket, BV(1));
    EXPECT_DOUBLE_EQ(b[1].amplitude.real(), std::sqrt(2));
    EXPECT_DOUBLE_EQ(b[1].amplitude.imag(), 0.);

    // EXPECT_EQ(hashA, 1);
    // EXPECT_EQ(hashB, 1); // FIXME
}

} // namespace  superpositeur