#include <gtest/gtest.h>
#include "superpositeur/DefaultOperations.hpp"
#include "superpositeur/MixedState.hpp"
#include <iostream>

namespace superpositeur {

class IntegrationTest : public ::testing::Test {
};

TEST_F(IntegrationTest, Nothing) {
    MixedState s;

    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({0}), Matrix({{1., 0.}, {0., 0.}}));
    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({1}), Matrix({{1., 0.}, {0., 0.}}));
    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({2}), Matrix({{1., 0.}, {0., 0.}}));
    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({30}), Matrix({{1., 0.}, {0., 0.}}));
    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({0, 1}), Matrix({{1., 0., 0., 0.}, {0., 0., 0., 0.}, {0., 0., 0., 0.}, {0., 0., 0., 0.}}));
    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({0, 32}), Matrix({{1., 0., 0., 0.}, {0., 0., 0., 0.}, {0., 0., 0., 0.}, {0., 0., 0., 0.}}));
    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({35, 32}), Matrix({{1., 0., 0., 0.}, {0., 0., 0., 0.}, {0., 0., 0., 0.}, {0., 0., 0., 0.}}));
}

TEST_F(IntegrationTest, X) {
    MixedState s;

    s(CircuitInstruction({ default_operations::X }, {QubitIndex(0)}));

    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({0}), Matrix({{0., 0.}, {0., 1.}}));
    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({0, 1}), Matrix({{0., 0., 0., 0.}, {0., 1., 0., 0.}, {0., 0., 0., 0.}, {0., 0., 0., 0.}}));
    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({0, 32}), Matrix({{0., 0., 0., 0.}, {0., 1., 0., 0.}, {0., 0., 0., 0.}, {0., 0., 0., 0.}}));

    auto const pureState = std::get<std::span<KeyValue<64> const>>(s.getPureState());
    EXPECT_EQ(pureState.size(), 1);
    EXPECT_EQ(pureState.begin()->ket, BasisVector<64>("1"));
    EXPECT_NEAR(pureState.begin()->amplitude.real(), 1., 1e-7);
}

TEST_F(IntegrationTest, XOn37) {
    MixedState s;

    s(CircuitInstruction({ default_operations::X }, {QubitIndex(37)}));

    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({0}), Matrix({{1., 0.}, {0., 0.}}));
    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({0, 1}), Matrix({{1., 0., 0., 0.}, {0., 0., 0., 0.}, {0., 0., 0., 0.}, {0., 0., 0., 0.}}));
    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({0, 37}), Matrix({{0., 0., 0., 0.}, {0., 0., 0., 0.}, {0., 0., 1., 0.}, {0., 0., 0., 0.}}));
}

TEST_F(IntegrationTest, H) {
    MixedState s;

    s(CircuitInstruction({ default_operations::H }, {QubitIndex(0)}));

    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({0}), Matrix({{.5, .5}, {.5, .5}}));
    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({1}), Matrix({{1., 0.}, {0., 0.}}));
    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({0, 1}), Matrix({{.5, .5, 0., 0.}, {.5, .5, 0., 0.}, {0., 0., 0., 0.}, {0., 0., 0., 0.}}));
    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({1, 0}), Matrix({{.5, .5, 0., 0.}, {.5, .5, 0., 0.}, {0., 0., 0., 0.}, {0., 0., 0., 0.}}));
}

TEST_F(IntegrationTest, MeasuredH) {
    MixedState s;

    s(CircuitInstruction({ default_operations::H }, {QubitIndex(0)}));
    s(CircuitInstruction(default_operations::MEAS_Z, {QubitIndex(0)}));

    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({0}), Matrix({{.5, 0.}, {0., .5}}));
    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({0, 1}), Matrix({{.5, 0., 0., 0.}, {0., .5, 0., 0.}, {0., 0., 0., 0.}, {0., 0., 0., 0.}}));
    EXPECT_EQ(s.getReducedDensityMatrixFromIndices({1, 0}), Matrix({{.5, 0., 0., 0.}, {0., .5, 0., 0.}, {0., 0., 0., 0.}, {0., 0., 0., 0.}}));
}

TEST_F(IntegrationTest, SomeDepolarizingChannels) {
    MixedState s;

    s(CircuitInstruction({ default_operations::H }, {QubitIndex(0)}));
    s(CircuitInstruction(default_operations::DEPOLARIZING_CHANNEL(0.3), {QubitIndex(0)}));
    s(CircuitInstruction({ default_operations::CNOT }, {QubitIndex(0), QubitIndex(1)}));
    s(CircuitInstruction(default_operations::DEPOLARIZING_CHANNEL(0.3), {QubitIndex(0)}));
    s(CircuitInstruction({ default_operations::H }, {QubitIndex(0)}));
    s(CircuitInstruction(default_operations::DEPOLARIZING_CHANNEL(0.3), {QubitIndex(1)}));
    s(CircuitInstruction(default_operations::DEPOLARIZING_CHANNEL(0.3), {QubitIndex(0)}));
    s(CircuitInstruction({ default_operations::H }, {QubitIndex(0)}));
    s(CircuitInstruction(default_operations::DEPOLARIZING_CHANNEL(0.3), {QubitIndex(0)}));
    s(CircuitInstruction(default_operations::DEPOLARIZING_CHANNEL(0.3), {QubitIndex(1)}));
    s(CircuitInstruction({ default_operations::CNOT }, {QubitIndex(0), QubitIndex(1)}));
}

} // namespace  superpositeur