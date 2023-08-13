#include <gtest/gtest.h>
#include "superpositeur/CircuitInstruction.hpp"

namespace superpositeur {

class OperandPermutationTest : public ::testing::Test {
};

TEST_F(OperandPermutationTest, CNOT) {
    Matrix cnot{{1, 0, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 0, 1},
                {0, 0, 1, 0}};
    
    EXPECT_EQ(applyQubitOperandPermutation(cnot, {1, 0}), cnot);

    auto output = applyQubitOperandPermutation(cnot, {0, 1});

    Matrix reversedCnot{{1, 0, 0, 0},
                        {0, 0, 0, 1},
                        {0, 0, 1, 0},
                        {0, 1, 0, 0}};

    EXPECT_EQ(output, reversedCnot);
}

TEST_F(OperandPermutationTest, Toffoli) {
    Matrix toffoli{{1, 0, 0, 0, 0, 0, 0, 0},
                   {0, 1, 0, 0, 0, 0, 0, 0},
                   {0, 0, 1, 0, 0, 0, 0, 0},
                   {0, 0, 0, 1, 0, 0, 0, 0},
                   {0, 0, 0, 0, 1, 0, 0, 0},
                   {0, 0, 0, 0, 0, 1, 0, 0},
                   {0, 0, 0, 0, 0, 0, 0, 1},
                   {0, 0, 0, 0, 0, 0, 1, 0}};

    EXPECT_EQ(applyQubitOperandPermutation(toffoli, {2, 1, 0}), toffoli);

    auto output = applyQubitOperandPermutation(toffoli, {0, 1, 2});

    Matrix tccToffoli{{1, 0, 0, 0, 0, 0, 0, 0},
                      {0, 1, 0, 0, 0, 0, 0, 0},
                      {0, 0, 1, 0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 1},
                      {0, 0, 0, 0, 1, 0, 0, 0},
                      {0, 0, 0, 0, 0, 1, 0, 0},
                      {0, 0, 0, 0, 0, 0, 1, 0},
                      {0, 0, 0, 1, 0, 0, 0, 0}};

    EXPECT_EQ(output, tccToffoli);

    auto output1 = applyQubitOperandPermutation(toffoli, {0, 2, 1});
    EXPECT_EQ(output1, tccToffoli);

    Matrix ctcToffoli{{1, 0, 0, 0, 0, 0, 0, 0},
                      {0, 1, 0, 0, 0, 0, 0, 0},
                      {0, 0, 1, 0, 0, 0, 0, 0},
                      {0, 0, 0, 1, 0, 0, 0, 0},
                      {0, 0, 0, 0, 1, 0, 0, 0},
                      {0, 0, 0, 0, 0, 0, 0, 1},
                      {0, 0, 0, 0, 0, 0, 1, 0},
                      {0, 0, 0, 0, 0, 1, 0, 0}};

    auto output2 = applyQubitOperandPermutation(toffoli, {2, 0, 1});
    auto output3 = applyQubitOperandPermutation(toffoli, {1, 0, 2});
    EXPECT_EQ(output2, ctcToffoli);
    EXPECT_EQ(output3, ctcToffoli);

}

}