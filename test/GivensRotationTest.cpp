#include "superpositeur/GivensRotation.hpp"
#include <gtest/gtest.h>

namespace superpositeur {

using namespace std::complex_literals;

class MixedStateSimplifierTest : public ::testing::Test {};

using BV = BasisVector<64>;

TEST_F(MixedStateSimplifierTest, ApplyGivensRotation) {
    SparseVector<64> a = {{BV(0), 1.}, {BV(1), 1.}};
    SparseVector<64> b = {{BV(0), 1.}, {BV(1), 1.}};

    std::uint64_t hashA = 0xDEADBEEF;
    std::uint64_t hashB = 0xDEADBEEF;

    std::span<KeyValue<64>> firstLine{a.begin(), a.end()};
    std::span<KeyValue<64>> secondLine{b.begin(), b.end()};
    applyGivensRotation(firstLine, hashA, secondLine, hashB);

    EXPECT_EQ(a[0].first, BV(0));
    EXPECT_EQ(a[0].second, 0.);
    EXPECT_EQ(a[1].first, BV(1));
    EXPECT_EQ(a[1].second, 0.);
    EXPECT_EQ(b[0].first, BV(0));
    EXPECT_DOUBLE_EQ(b[0].second.real(), std::sqrt(2));
    EXPECT_DOUBLE_EQ(b[0].second.imag(), 0.);
    EXPECT_EQ(b[1].first, BV(1));
    EXPECT_DOUBLE_EQ(b[1].second.real(), std::sqrt(2));
    EXPECT_DOUBLE_EQ(b[1].second.imag(), 0.);

    // EXPECT_EQ(hashA, 1);
    // EXPECT_EQ(hashB, 1); // FIXME
}

// FIXME: should fail / revert if keys are not identical in both rows.

} // namespace  superpositeur