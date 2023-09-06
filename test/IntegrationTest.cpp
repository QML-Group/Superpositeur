#include <gtest/gtest.h>
#include "superpositeur/DefaultOperations.hpp"
#include "superpositeur/MixedState.hpp"
#include <iostream>

namespace superpositeur {

class IntegrationTest : public ::testing::Test {
};

TEST_F(IntegrationTest, Simple) {
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

    std::cout << s.getReducedDensityMatrix(std::vector<bool>(2, true)) << std::endl;
}

} // namespace  superpositeur