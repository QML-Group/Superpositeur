#include <gtest/gtest.h>
#include "superpositeur/DefaultOperations.hpp"
#include "superpositeur/MixedState.hpp"
#include <iostream>

namespace superpositeur {

class IntegrationTest : public ::testing::Test {
public:
    template <typename T>
    void checkEquality(std::vector<std::pair<std::string, T>> const &left,
                       std::vector<std::pair<std::string, T>> const &right) {
        REQUIRE_EQ(left.size(), right.size());

        for (std::uint64_t i = 0; i < left.size(); ++i) {
            EXPECT_EQ(left[i].first, right[i].first);
            EXPECT_TRUE(utils::isNull(left[i].second - right[i].second));
        }
    }
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