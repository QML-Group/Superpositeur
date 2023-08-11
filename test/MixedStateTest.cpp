#include <gtest/gtest.h>
#include "superpositeur/MixedState.hpp"
#include "superpositeur/DefaultOperations.hpp"

namespace superpositeur {

using namespace std::complex_literals;

class MixedStateTest : public ::testing::Test {
protected:
    using BV = BasisVector<64>;

    static void checkEqComplex(std::complex<double> left,
                               std::complex<double> right) {
        EXPECT_EQ(left.real(), right.real());
        EXPECT_EQ(left.imag(), right.imag());
    }

    static void checkMatrix(
        MixedState const &victim,
        std::initializer_list<std::initializer_list<std::complex<double>>>
            expected) {
        auto actual = victim.getMatrixOfVectors();
        auto expectedMatrix = Matrix(expected);

        EXPECT_EQ(actual, expectedMatrix);
    }
};

// TEST_F(MixedStateTest, "Identity") {
//     MixedState victim(1);

//     Matrix expected{{1, 0}, {0, 0}};

//     EXPECT_EQ(victim.getReducedDensityMatrix(), expected);

//     Matrix expectedMatrixOfVectors{{1, 0}};

//     EXPECT_EQ(victim.getMatrixOfVectors(), expectedMatrixOfVectors);

//     Matrix identity{{1, 0}, {0, 1}};

//     victim.applyCircuitInstruction(
//         CircuitInstruction({identity}, {QubitIndex{0}}));

//     EXPECT_EQ(victim.getReducedDensityMatrix(), expected);
//     EXPECT_EQ(victim.getMatrixOfVectors(), expectedMatrixOfVectors);
// }

// TEST_F(MixedStateTest, "Pure state") {
//     MixedState victim(1);

//     double angle = 1.23;
//     auto rx = default_operations::RX(angle);

//     victim(CircuitInstruction(rx, {QubitIndex{0}}));

//     Matrix expected{{std::cos(angle / 2) ** 2, 1i / 2 * std::sin(angle)}, {-1i / 2 * std::sin(angle), std::sin(angle / 2) ** 2}};
//     EXPECT_EQ(victim.getReducedDensityMatrix(), expected);

//     Matrix expectedMatrixOfVectors{{std::cos(angle / 2), - 1i * std::sin(angle / 2)}};

//     EXPECT_EQ(victim.getMatrixOfVectors(), expectedMatrixOfVectors);
// }

// TEST_F(MixedStateTest, "Measure in computational basis") {
//     MixedState victim(1);

//     victim.applyCircuitInstruction(
//         CircuitInstruction(default_operations::MEAS_Z,
//                            {QubitIndex{0}, MeasurementRegisterIndex{0}}));

//     Matrix expected{{1, 0}, {0, 0}};

//     EXPECT_EQ(victim.getReducedDensityMatrix(), expected);

//     checkMatrix(victim, {{1., 0., 0., 0.}});

//     auto measurementRegisterStatistics =
//         victim.getMeasurementRegisterStatistics();

//     EXPECT_EQ(measurementRegisterStatistics["0"], 1.);
// }

// TEST_F(MixedStateTest,
//                   "Measure in computational basis after Hadamard") {
//     MixedState victim(1);

//     victim.applyCircuitInstruction(
//         CircuitInstruction({default_operations::H}, {QubitIndex{0}}));
//     victim.applyCircuitInstruction(
//         CircuitInstruction(default_operations::MEAS_Z,
//                            {QubitIndex{0}, MeasurementRegisterIndex{0}}));

//     Matrix expected{{1. / 2, 0.}, {0., 1. / 2}};

//     EXPECT_EQ(victim.getReducedDensityMatrix(), expected);

//     checkMatrix(victim, {{1 / std::sqrt(2), 0., 0., 0.},
//                          {0., 0., 0., 1 / std::sqrt(2)}});

//     AssociationVectorStringMap<std::complex<double>>::T state;
//     AssociationVectorStringMap wrappedState(state);
//     victim.getPureState(wrappedState);

//     EXPECT_EQ(state.size(), 0);

//     auto measurementRegisterStatistics =
//         victim.getMeasurementRegisterStatistics();

//     EXPECT_EQ(measurementRegisterStatistics["0"], .5);
//     EXPECT_EQ(measurementRegisterStatistics["1"], .5);
// }

// TEST_F(MixedStateTest, "Probabilistic bit-flip") {
//     MixedState victim(1);

//     double p = 0.2;
//     Matrix k0 = std::sqrt(1 - p) * Matrix::identity(2);

//     Matrix k1 = std::sqrt(p) * default_operations::X;

//     victim.applyCircuitInstruction(
//         CircuitInstruction({k0, k1}, {QubitIndex{0}}));

//     Matrix expected{{1 - p, 0}, {0, p}};

//     EXPECT_EQ(victim.getReducedDensityMatrix(), expected);
// }

// TEST_F(MixedStateTest, "|+> state") {
//     MixedState victim(1);

//     victim.applyCircuitInstruction(
//         CircuitInstruction({default_operations::H}, {QubitIndex{0}}));

//     Matrix expected{{1 / 2., 1 / 2.}, {1 / 2., 1 / 2.}};

//     EXPECT_EQ(victim.getReducedDensityMatrix(), expected);
// }

// TEST_F(MixedStateTest, "|+> state with 2 qubits") {
//     MixedState victim(2);

//     victim.applyCircuitInstruction(
//         CircuitInstruction({default_operations::H}, {QubitIndex{1}}));

//     Matrix expected{{1 / 2., 0, 1 / 2., 0},
//                           {0, 0, 0, 0},
//                           {1 / 2., 0, 1 / 2., 0},
//                           {0, 0, 0, 0}};

//     EXPECT_EQ(victim.getReducedDensityMatrix(), expected);
// }

// TEST_F(MixedStateTest, "Measure Bell pair separately") {
//     MixedState victim(3);

//     victim.applyCircuitInstruction(
//         CircuitInstruction({default_operations::H}, {QubitIndex{1}}));

//     victim.applyCircuitInstruction(CircuitInstruction(
//         {default_operations::CNOT}, {QubitIndex{1}, QubitIndex{2}}));

//     victim.applyCircuitInstruction(
//         CircuitInstruction(default_operations::MEAS_Z,
//                            {QubitIndex{2}, MeasurementRegisterIndex{2}}));

//     auto measurementRegisterStatistics =
//         victim.getMeasurementRegisterStatistics();
//     EXPECT_EQ(measurementRegisterStatistics["000"], .5);
//     EXPECT_EQ(measurementRegisterStatistics["100"], .5);

//     victim.applyCircuitInstruction(
//         CircuitInstruction(default_operations::MEAS_Z,
//                            {QubitIndex{0}, MeasurementRegisterIndex{0}}));
//     victim.applyCircuitInstruction(
//         CircuitInstruction(default_operations::MEAS_Z,
//                            {QubitIndex{1}, MeasurementRegisterIndex{1}}));

//     measurementRegisterStatistics = victim.getMeasurementRegisterStatistics();
//     EXPECT_EQ(measurementRegisterStatistics["000"], .5);
//     EXPECT_EQ(measurementRegisterStatistics["110"], .5);
// }

// TEST_F(MixedStateTest, "Prep") {
//     MixedState victim(1);

//     victim.applyCircuitInstruction(
//         CircuitInstruction({default_operations::H}, {QubitIndex{0}}));

//     victim.applyCircuitInstruction(
//         CircuitInstruction(default_operations::MEAS_Z,
//                            {QubitIndex{0}, MeasurementRegisterIndex{0}}));

//     victim.applyCircuitInstruction(
//         CircuitInstruction(default_operations::PREP_Z, {QubitIndex{0}}));

//     auto measurementRegisterStatistics =
//         victim.getMeasurementRegisterStatistics();
//     EXPECT_EQ(measurementRegisterStatistics["0"], .5);
//     EXPECT_EQ(measurementRegisterStatistics["1"], .5);

//     checkMatrix(victim,
//                 {{1 / std::sqrt(2), 0, 0, 0}, {0, 0, 1 / std::sqrt(2), 0}});
// }

// TEST_F(MixedStateTest, "Measure mixed state") {
//     MixedState victim(3);

//     double p = 0.5;
//     Matrix k0 = std::sqrt(1 - p) * Matrix::identity(2);

//     Matrix k1 = std::sqrt(p) * default_operations::H;

//     victim.applyCircuitInstruction(
//         CircuitInstruction({k0, k1}, {QubitIndex{1}}));

//     victim.applyCircuitInstruction(
//         CircuitInstruction(default_operations::MEAS_Z,
//                            {QubitIndex{1}, MeasurementRegisterIndex{1}}));

//     auto measurementRegisterStatistics =
//         victim.getMeasurementRegisterStatistics();
//     EXPECT_EQ(measurementRegisterStatistics["000"], .75);
//     EXPECT_EQ(measurementRegisterStatistics["010"], .25);
// }

// TEST_F(MixedStateTest, "Cond gate not applied") {
//     MixedState victim(1);

//     victim.applyCircuitInstruction(
//         CircuitInstruction({default_operations::X}, {QubitIndex{0}},
//                            {MeasurementRegisterIndex{0}}));

//     Matrix expected{{1., 0.}, {0., 0.}};

//     EXPECT_EQ(victim.getReducedDensityMatrix(), expected);

//     checkMatrix(victim, {{1., 0., 0., 0.}});

//     auto measurementRegisterStatistics =
//         victim.getMeasurementRegisterStatistics();

//     EXPECT_EQ(measurementRegisterStatistics["0"], 1.);
// }

// TEST_F(MixedStateTest, "Prep with measure + cond X") {
//     MixedState victim(1);

//     victim.applyCircuitInstruction(
//         CircuitInstruction({default_operations::H}, {QubitIndex{0}}));

//     victim.applyCircuitInstruction(
//         CircuitInstruction(default_operations::MEAS_Z,
//                            {QubitIndex{0}, MeasurementRegisterIndex{0}}));

//     victim.applyCircuitInstruction(
//         CircuitInstruction({default_operations::X}, {QubitIndex{0}},
//                            {MeasurementRegisterIndex{0}}));

//     Matrix expected{{1., 0.}, {0., 0.}};

//     EXPECT_EQ(victim.getReducedDensityMatrix(), expected);

//     checkMatrix(victim, {{1 / std::sqrt(2), 0., 0., 0.},
//                          {0., 0., 1 / std::sqrt(2), 0.}});

//     auto measurementRegisterStatistics =
//         victim.getMeasurementRegisterStatistics();

//     EXPECT_EQ(measurementRegisterStatistics["0"], .5);
//     EXPECT_EQ(measurementRegisterStatistics["1"], .5);
// }

// TEST_F(MixedStateTest, "Conditional X") {
//     MixedState victim(2);

//     victim.applyCircuitInstruction(CircuitInstruction(
//         {default_operations::X},
//         {MeasurementRegisterIndex{
//             0}})); // Bit-flip of classical measurement bit...

//     victim.applyCircuitInstruction(
//         CircuitInstruction({default_operations::X}, {QubitIndex{1}},
//                            {MeasurementRegisterIndex{0}}));

//     EXPECT_EQ(victim.getSize(), 1);
//     checkMatrix(victim, {{0., 0., 0., 0., 0., 0., 1., 0., 0., 0., 0., 0., 0.,
//                           0., 0., 0.}});

//     auto measurementRegisterStatistics =
//         victim.getMeasurementRegisterStatistics();

//     EXPECT_EQ(measurementRegisterStatistics["01"], 1.);
// }

// TEST_F(MixedStateTest, "Simplify pure state" * doctest::skip()) {
//     MixedState victim(2);

//     victim.applyCircuitInstruction(
//         CircuitInstruction({default_operations::H}, {QubitIndex{0}}));
//     victim.applyCircuitInstruction(CircuitInstruction(
//         {default_operations::CNOT}, {QubitIndex{0}, QubitIndex{1}}));

//     checkMatrix(victim, {{1 / std::sqrt(2), 0., 0., 1 / std::sqrt(2), 0., 0.,
//                           0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}});

//     victim.simplify();

//     checkMatrix(victim, {{1 / std::sqrt(2), 0., 0., 1 / std::sqrt(2), 0., 0.,
//                           0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}});

//     auto measurementRegisterStatistics =
//         victim.getMeasurementRegisterStatistics();
//     EXPECT_EQ(measurementRegisterStatistics["00"], 1.);
// }

// TEST_F(MixedStateTest, "Simplify mixed state" * doctest::skip()) {
//     MixedState victim(1);

//     double p = 0.5;
//     Matrix k0 = std::sqrt(1 - p) * Matrix::identity(2);
//     Matrix k1 = std::sqrt(p) * default_operations::H;
//     victim.applyCircuitInstruction(
//         CircuitInstruction({k0, k1}, {QubitIndex{0}}));

//     checkMatrix(victim, {{1 / std::sqrt(2), 0, 0, 0}, {1 / 2., 1 / 2., 0, 0}});

//     victim.applyCircuitInstruction(
//         CircuitInstruction({k0, k1}, {QubitIndex{0}}));

//     Matrix expectedDensityMatrix{{3 / 4., 1 / 4.}, {1 / 4., 1 / 4.}};

//     EXPECT_EQ(victim.getReducedDensityMatrix(), expectedDensityMatrix);

//     checkMatrix(victim, {{1 / 2., 0, 0, 0},
//                          {0.5 / std::sqrt(2), 0.5 / std::sqrt(2), 0, 0},
//                          {0.5 / std::sqrt(2), 0.5 / std::sqrt(2), 0, 0},
//                          {1 / 2., 0, 0, 0}});

//     victim.simplify();

//     checkMatrix(victim, {{-0.86602540378443871, -0.28867513459481292, 0, 0},
//                          {0, -0.40824829046386313, 0, 0}});
//     EXPECT_EQ(victim.getReducedDensityMatrix(), expectedDensityMatrix);
// }

// TEST_F(MixedStateTest,
//                   "Simplify very mixed state" * doctest::skip()) {
//     MixedState victim(2);

//     double p = 0.5;
//     Matrix k0 = std::sqrt(1 - p) * Matrix::identity(2);
//     Matrix k1 = std::sqrt(p) * default_operations::H;
//     victim.applyCircuitInstruction(
//         CircuitInstruction({k0, k1}, {QubitIndex{0}}));
//     victim.applyCircuitInstruction(
//         CircuitInstruction({k0, k1}, {QubitIndex{1}}));
//     victim.applyCircuitInstruction(
//         CircuitInstruction({k0, k1}, {QubitIndex{1}}));
//     victim.applyCircuitInstruction(
//         CircuitInstruction({k0, k1}, {QubitIndex{0}}));
//     victim.applyCircuitInstruction(
//         CircuitInstruction({k0, k1}, {QubitIndex{0}}));
//     victim.applyCircuitInstruction(
//         CircuitInstruction({k0, k1}, {QubitIndex{1}}));
//     victim.applyCircuitInstruction(
//         CircuitInstruction({k0, k1}, {QubitIndex{0}}));
//     victim.applyCircuitInstruction(
//         CircuitInstruction({k0, k1}, {QubitIndex{1}}));
//     victim.applyCircuitInstruction(
//         CircuitInstruction({k0, k1}, {QubitIndex{0}}));

//     EXPECT_EQ(victim.getSize(), 1953);

//     victim.simplify();

//     EXPECT_EQ(victim.getSize(), 9);
//     checkMatrix(victim,
//                 {{-3 / 4., -1 / 4., -1 / 4., -1 / 12., 0, 0, 0, 0, 0, 0, 0, 0,
//                   0, 0, 0, 0},
//                  {0., -.5 / std::sqrt(2), 0, -1. / (6 * std::sqrt(2)), 0, 0, 0,
//                   0, 0, 0, 0, 0, 0, 0, 0, 0},
//                  {0., 0., -.5 / std::sqrt(2), -1. / (6 * std::sqrt(2)), 0, 0, 0,
//                   0, 0, 0, 0, 0, 0, 0, 0, 0},
//                  {0., 0., 0., -1 / 6., 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
// }

// TEST_F(MixedStateTest, "Simplify simplifies rho = .5 |+><+| + .5 "
//                                   "|-><-| to rho = .5 |0><0| + .5 |1><1|" *
//                                       doctest::skip()) {
//     MixedState victim(3);

//     Matrix k0 = std::sqrt(.5) * Matrix::identity(2);
//     Matrix k1 = std::sqrt(.5) * default_operations::X;

//     victim.applyCircuitInstruction(CircuitInstruction(
//         {default_operations::CNOT}, {QubitIndex{0}, QubitIndex{1}}));
//     victim.applyCircuitInstruction(
//         CircuitInstruction({k0, k1}, {QubitIndex{2}}));
//     victim.applyCircuitInstruction(
//         CircuitInstruction({default_operations::H}, {QubitIndex{2}}));

//     auto actualMatrix = victim.toMatrix();
//     EXPECT_EQ(actualMatrix.getNumberOfRows(), 2);
//     checkEqComplex(actualMatrix.get(0, 0), .5);
//     checkEqComplex(actualMatrix.get(0, 4), .5);
//     checkEqComplex(actualMatrix.get(1, 0), .5);
//     checkEqComplex(actualMatrix.get(1, 4), -.5);

//     victim.simplify();

//     auto newActualMatrix = victim.toMatrix();
//     EXPECT_EQ(newActualMatrix.getNumberOfRows(), 2);
//     checkEqComplex(newActualMatrix.get(0, 0), -1 / std::sqrt(2));
//     checkEqComplex(newActualMatrix.get(1, 4), -1 / std::sqrt(2));
// }

} // namespace  superpositeur