#include <gtest/gtest.h>
#include "superpositeur/MixedState.hpp"

namespace superpositeur {

using namespace std::complex_literals;

class MatrixTest : public ::testing::Test {};

TEST_F(MatrixTest, ProductAssignment) {
    Matrix a{{1, 2}};

    Matrix b{{1, 2}, {1, 2}};

    a *= b;

    Matrix aTimesB{{3, 6}};

    EXPECT_EQ(a, aTimesB);
}

TEST_F(MatrixTest, ProductAssignment2) {
    Matrix a{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

    Matrix b{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

    a *= b;

    Matrix aTimesB{{3, 3, 3}, {3, 3, 3}, {3, 3, 3}, {3, 3, 3}};

    EXPECT_EQ(a, aTimesB);
}

TEST_F(MatrixTest, MultiplyLeft) {
    Matrix a{{1}, {2}};

    Matrix b{{1, 2}, {1, 2}};

    a.multiplyLeft(b);

    Matrix aTimesB{{5}, {5}};

    EXPECT_EQ(a, aTimesB);
}

TEST_F(MatrixTest, Product) {
    Matrix a{{1, 2}, {3, 4}, {5.5 + 1i, 6}};

    Matrix b{{1}, {3}};

    Matrix aTimesB{{7}, {15}, {23.5 + 1i}};

    EXPECT_EQ(a * b, aTimesB);
}

TEST_F(MatrixTest, SumAssignment) {
    Matrix a{{1, 2}};

    Matrix b{{2, 3.5}};

    Matrix aPlusB{{3, 5.5}};

    a += b;
    EXPECT_EQ(a, aPlusB);
}


TEST_F(MatrixTest, Dagger) {
    Matrix m{{1 / std::sqrt(2), 1 / std::sqrt(2)},
             {1i / std::sqrt(2), -1i / std::sqrt(2)}};

    Matrix mDag{{1 / std::sqrt(2), -1i / std::sqrt(2)},
                {1 / std::sqrt(2), 1i / std::sqrt(2)}};

    EXPECT_EQ(m.dagger(), mDag);
}

TEST_F(MatrixTest, SpectralRadius) {
    Matrix m0{{1 / std::sqrt(2), 0.}, {0., -1i / std::sqrt(2)}};

    EXPECT_NEAR(computeSpectralRadius(m0), 1. / std::sqrt(2), config::ATOL);

    Matrix m1{{1, 0.}, {0., -2}};

    EXPECT_NEAR(computeSpectralRadius(m1), 2., config::ATOL);

    Matrix m2{{1, -10}, {0., -2i}};

    EXPECT_NEAR(computeSpectralRadius(m2), 2., config::ATOL);

    Matrix m3{{1, -10, 1, 2}, {0., -2, 4, 2i}, {0, 0, 4i, 23}, {0, 0, 0, 1i}};

    EXPECT_NEAR(computeSpectralRadius(m3), 4., config::ATOL);

    Matrix m4{{0, 1, -2}, {0, 1, 0}, {1, -1, 3}};

    EXPECT_NEAR(computeSpectralRadius(m4), 2., config::ATOL);
}

} // namespace  superpositeur