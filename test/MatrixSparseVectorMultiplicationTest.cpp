#include <gtest/gtest.h>
#include "superpositeur/MatrixSparseVectorMultiplication.hpp"

namespace superpositeur {
    
class MatrixSparseVectorMultiplicationTest : public ::testing::Test {
public:
    using BV = BasisVector<64>;

    void check1Q(SparseVector<64> const& inputState, Matrix const& matrix, SparseVector<64> const& expectedState, QubitIndex operand = QubitIndex{0}) {
        // Input should be sorted.

        assert(matrix.getNumberOfRows() == 2);
        assert(matrix.getNumberOfCols() == 2);

        SparseVector<64> result;

        CircuitInstruction::Mask operands;
        operands.set(operand.value);

        multiplyMatrix(matrix, {inputState.begin(), inputState.end()}, operands, std::back_inserter(result));

        std::stringstream resultString;
        for (auto const& kv: result) {
            resultString << "\n" << kv.key.toString() << "  ->   " << kv.amplitude;
        }

        EXPECT_TRUE(std::ranges::equal(expectedState, result, [](auto left, auto right) { return left.key == right.key && utils::isNull(left.amplitude - right.amplitude); }));
    }
};

TEST_F(MatrixSparseVectorMultiplicationTest, Identity) {
    Matrix m{{1, 0}, {0, 1}};

    auto input = SparseVector<64>{{BV("0"), 1.}};
    
    check1Q(input, m, input);
}

TEST_F(MatrixSparseVectorMultiplicationTest, X) {
    Matrix m{{0, 1}, {1, 0}};

    SparseVector<64> input = SparseVector<64>{{BV("0"), 1.}};

    SparseVector<64> expected = SparseVector<64>{{BV("1"), 1.}};

    check1Q(input, m, expected);
}

TEST_F(MatrixSparseVectorMultiplicationTest, RandomMatrix) {
    Matrix m{{1, 2},
             {3, 4}};

    SparseVector<64> input = SparseVector<64>{{BV("1"), 10}};

    SparseVector<64> expected = SparseVector<64>{{BV("0"), 20}, {BV("1"), 40}};

    check1Q(input, m, expected);

    SparseVector<64> expected2 = SparseVector<64>{{BV("0001"), 10}, {BV("1001"), 30}};

    check1Q(input, m, expected2, QubitIndex{3});

    SparseVector<64> expected3 = SparseVector<64>{{BV("0001"), 10}, {BV("0101"), 30}, {BV("1001"), 30}, {BV("1101"), 90}};

    check1Q(expected2, m, expected3, QubitIndex{2});

    SparseVector<64> expected4 = SparseVector<64>{{BV("0001"), 70}, {BV("1001"), 150}};

    check1Q(expected2, m, expected4, QubitIndex{3});
}

TEST_F(MatrixSparseVectorMultiplicationTest, HOnPlus) {
    Matrix m{{1, 1}, {1, -1}};
    m *= 1. / std::sqrt(2);

    SparseVector<64> input = SparseVector<64>{{BV("0"), 1. / std::sqrt(2)}, {BV("1"), 1. / std::sqrt(2)}};

    SparseVector<64> expected = SparseVector<64>{{BV("0"), 1.}};

    check1Q(input, m, expected);
}

TEST_F(MatrixSparseVectorMultiplicationTest,XOnPlus) {
    Matrix m{{0, 1}, {1, 0}};

    SparseVector<64> input = SparseVector<64>{{BV("0"), 1. / std::sqrt(2)}, {BV("1"), 1. / std::sqrt(2)}};

    check1Q(input, m, input);
}

TEST_F(MatrixSparseVectorMultiplicationTest, XOn2Qubits) {
    Matrix m{{0, 1}, {1, 0}};

    SparseVector<64> input = SparseVector<64>{{BV("00"), 1. / std::sqrt(2)}, {BV("01"), 1. / std::sqrt(2)}};

    SparseVector<64> expected = SparseVector<64>{{BV("10"), 1. / std::sqrt(2)}, {BV("11"), 1. / std::sqrt(2)}};

    check1Q(input, m, expected, QubitIndex{1});
}

TEST_F(MatrixSparseVectorMultiplicationTest, AnotherH) {
    Matrix m{{1, 1}, {1, -1}};
    m *= 1. / std::sqrt(2);

    SparseVector<64> input = SparseVector<64>{{BV("000"), 1. / std::sqrt(2)}, {BV("001"), 1. / std::sqrt(2)}};

    SparseVector<64> expected = SparseVector<64>{{BV("000"), .5}, {BV("001"), .5}, {BV("100"), .5}, {BV("101"), .5}};

    check1Q(input, m, expected, QubitIndex{2});
}

TEST_F(MatrixSparseVectorMultiplicationTest, NullMatrix) {
    Matrix m{{0, 0}, {0, 0}};

    SparseVector<64> input = SparseVector<64>{{BV("10111001010010"), 1.}, {BV("11111111111111"), 2.6512}};

    check1Q(input, m, SparseVector<64>{}, QubitIndex{7});
}

TEST_F(MatrixSparseVectorMultiplicationTest, Random) {
    Matrix m{{0, 0}, {1, 0}};

    SparseVector<64> input = SparseVector<64>{{BV("10111001010010"), 1.}, {BV("11111111111111"), 2.6512}};

    check1Q(input, m, SparseVector<64>{{BV("10111011010010"), 1.}}, QubitIndex{7});
}

TEST_F(MatrixSparseVectorMultiplicationTest, Hadamards) {
    Matrix m{{1, 1}, {1, -1}};
    m *= 1. / std::sqrt(2);

    SparseVector<64> input = SparseVector<64>{{BV("000"), 1.}};
    SparseVector<64> expected1 = SparseVector<64>{{BV("000"), 1. / std::sqrt(2)}, {BV("001"), 1. / std::sqrt(2)}};
    SparseVector<64> expected2 = SparseVector<64>{{BV("000"), .5}, {BV("001"), .5}, {BV("010"), .5}, {BV("011"), .5}};
    SparseVector<64> expected3 = SparseVector<64>{{BV("000"), .5 / std::sqrt(2)}, {BV("001"), .5 / std::sqrt(2)}, {BV("010"), .5 / std::sqrt(2)}, {BV("011"), .5 / std::sqrt(2)},
                                          {BV("100"), .5 / std::sqrt(2)}, {BV("101"), .5 / std::sqrt(2)}, {BV("110"), .5 / std::sqrt(2)}, {BV("111"), .5 / std::sqrt(2)}};

    check1Q(input, m, expected1, QubitIndex{0});
    check1Q(expected1, m, expected2, QubitIndex{1});
    check1Q(expected2, m, expected3, QubitIndex{2});
}

TEST_F(MatrixSparseVectorMultiplicationTest, FourByFour) {
    Matrix m{{1, 2, 3, 4},
             {5, 6, 7, 8},
             {9, 10, 11, 12},
             {13, 14, 15, 16}};

    SparseVector<64> input = SparseVector<64>{{BV("00"), 0}, {BV("01"), 1}, {BV("10"), 2}, {BV("11"), 3}};

    SparseVector<64> result;

    CircuitInstruction::Mask operands;
    operands.set(0);
    operands.set(1);

    multiplyMatrix(m, {input.begin(), input.end()}, operands, std::back_inserter(result));

    
    SparseVector<64> expected = SparseVector<64>{{BV("00"), 20}, {BV("01"), 44}, {BV("10"), 68}, {BV("11"), 92}};

    EXPECT_TRUE(std::ranges::equal(expected, result, [](auto left, auto right) { return left.key == right.key && utils::isNull(left.amplitude - right.amplitude); }));
}

TEST_F(MatrixSparseVectorMultiplicationTest, BellPair) {
    Matrix h{{1, 1}, {1, -1}};
    h *= 1. / std::sqrt(2);

    Matrix cnot{{1, 0, 0, 0}, // Reversed operands.
                {0, 0, 0, 1},
                {0, 0, 1, 0},
                {0, 1, 0, 0}};

    SparseVector<64> input = SparseVector<64>{{BV("00"), 1}};

    CircuitInstruction::Mask hOperands;
    hOperands.set(0);
    
    CircuitInstruction::Mask cnotOperands;
    cnotOperands.set(0);
    cnotOperands.set(1);

    SparseVector<64> intermediateResult;

    multiplyMatrix(h, {input.begin(), input.end()}, hOperands, std::back_inserter(intermediateResult));
    
    SparseVector<64> result;

    multiplyMatrix(cnot, {intermediateResult.begin(), intermediateResult.end()}, cnotOperands, std::back_inserter(result));

    SparseVector<64> expected = SparseVector<64>{{BV("00"), 1 / std::sqrt(2)}, {BV("11"), 1 / std::sqrt(2)}};

    EXPECT_TRUE(std::ranges::equal(expected, result, [](auto left, auto right) { return left.key == right.key && utils::isNull(left.amplitude - right.amplitude); }));
}

}