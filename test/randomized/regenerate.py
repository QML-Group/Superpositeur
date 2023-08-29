from qiskit.circuit import QuantumCircuit
from ..python import QiskitRandomCircuitGeneration
from qiskit.quantum_info import DensityMatrix, partial_trace, Statevector
import os, qiskit, random
import numpy as np
import glob
from itertools import combinations
from qiskit_aer import AerSimulator

def arrayToCppStr(k: np.ndarray):
                return "Matrix({ " + ", ".join(map(lambda l: "{ " + ", ".join(map(lambda x: f"{x.real} + {x.imag}i", l)) + " }", k)) + " })"

def circuit_to_cpp_string(name: str, circuit: QuantumCircuit, is_hard = False):
    cppString = """#include "gtest/gtest.h"
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
"""

    for instruction in circuit.data:
        operation = instruction.operation
        ops = list(map(lambda x:  circuit.find_bit(x)[0], instruction.qubits))
        if operation.name == 'cx':
            krausString = "{ default_operations::CNOT }"
            operandsString = "{ " + ", ".join(map(lambda op: "QubitIndex{ " + str(op) + " }", ops)) + " }"
            cppString += f"    c.addInstruction(CircuitInstruction({krausString}, {operandsString}));\n\n"
        else:
            ops.reverse()

            krausStringsList = [arrayToCppStr(k) for k in operation.params]
            krausString = "{ " + ", ".join(krausStringsList) + " }"
            operandsString = "{ " + ", ".join(map(lambda op: "QubitIndex{ " + str(op) + " }", ops)) + " }"
            cppString += f"""    c.addInstruction(CircuitInstruction({krausString}, {operandsString}));\n\n"""
    
    cppString += """
    return c;
}

TEST(RandomizedIntegrationTests, """ + name + """) {
    auto c = randomCircuit();
    
    auto s = c.execute();

"""


    if is_hard:
        i = 0
        numberOfReductionQubits = 8
    
        circuit.save_statevector('finalstatevector')
        backend = AerSimulator(method='statevector')
        result = qiskit.execute(circuit, backend).result()
        qiskitProbabilities = result.data()["finalstatevector"].probabilities()

        maskStr = "{" + ", ".join("true" for q in range(circuit.num_qubits)) + "}"
        cppString += f"    auto probas = s.getReducedDensityMatrixDiagonal({maskStr});\n"

        for _ in range(10):
            index = random.randint(0, 1 << circuit.num_qubits)
            cppString += f"    EXPECT_NEAR(probas[{index}], {qiskitProbabilities[index]}, 1.0e-7);\n\n"

    else:
        qiskitDensityMatrix = DensityMatrix(circuit)
        for numberOfReductionQubits in range(1, min(4, circuit.num_qubits)):
            for keepIndices in combinations(range(circuit.num_qubits), numberOfReductionQubits):
                maskStr = "{" + ", ".join("true" if q in keepIndices else "false" for q in range(circuit.num_qubits)) + "}"

                qiskitReductionIndices = [q for q in range(circuit.num_qubits) if q not in keepIndices]
                expected = partial_trace(state=qiskitDensityMatrix, qargs=qiskitReductionIndices).data
                expectedStr = arrayToCppStr(expected)
                npDiag = np.real(np.diagonal(expected))
                expectedDiagonalStr = "{ " + ", ".join(map(lambda x: f"DoubleNear({x}, 1.0e-7)", npDiag)) + " }"
                cppString += f"    EXPECT_EQ(s.getReducedDensityMatrix({maskStr}), {expectedStr});\n\n"
                cppString += f"    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({maskStr}), ElementsAreArray({expectedDiagonalStr}));\n\n"

    cppString += """
}
}

}
"""

    return cppString

def generate_googletest_file(name, num_qubits, num_gates, max_number_operands, weight_of_kraus, weight_of_unitary, weight_of_cnot, is_hard=False):
    tests_dir = os.path.dirname(os.path.realpath(__file__))
    file_to_write = os.path.join(tests_dir, name + ".cpp")

    circuit = QiskitRandomCircuitGeneration.generate_random_unitary_and_kraus_circuit(
        qubits = num_qubits,
        gates = num_gates,
        max_number_operands = max_number_operands,
        weight_of_kraus = weight_of_kraus,
        weight_of_unitary = weight_of_unitary,
        weight_of_cnot = weight_of_cnot,
    )

    s = circuit_to_cpp_string(name, circuit, is_hard=is_hard)

    with open(file_to_write, 'w') as f:
        f.write(s)
        print(f"Generated {file_to_write}...")

settings = [
    { "num_qubits": 3, "num_gates": 3, "max_number_operands": 2, "weight_of_kraus": 10, "weight_of_unitary": 30, "weight_of_cnot": 80 },
    { "num_qubits": 4, "num_gates": 3, "max_number_operands": 2, "weight_of_kraus": 10, "weight_of_unitary": 30, "weight_of_cnot": 80 },
    { "num_qubits": 5, "num_gates": 3, "max_number_operands": 2, "weight_of_kraus": 10, "weight_of_unitary": 30, "weight_of_cnot": 80 },
    { "num_qubits": 8, "num_gates": 10, "max_number_operands": 2, "weight_of_kraus": 10, "weight_of_unitary": 30, "weight_of_cnot": 80 },
    { "num_qubits": 8, "num_gates": 15, "max_number_operands": 2, "weight_of_kraus": 10, "weight_of_unitary": 30, "weight_of_cnot": 80 },
    { "num_qubits": 8, "num_gates": 20, "max_number_operands": 2, "weight_of_kraus": 10, "weight_of_unitary": 30, "weight_of_cnot": 80 },
    { "num_qubits": 3, "num_gates": 3, "max_number_operands": 2, "weight_of_kraus": 100, "weight_of_unitary": 30, "weight_of_cnot": 20 },
    { "num_qubits": 3, "num_gates": 4, "max_number_operands": 2, "weight_of_kraus": 100, "weight_of_unitary": 30, "weight_of_cnot": 20 },
    { "num_qubits": 3, "num_gates": 5, "max_number_operands": 2, "weight_of_kraus": 100, "weight_of_unitary": 30, "weight_of_cnot": 20 },
    { "num_qubits": 6, "num_gates": 5, "max_number_operands": 2, "weight_of_kraus": 100, "weight_of_unitary": 0, "weight_of_cnot": 0 },
    { "num_qubits": 8, "num_gates": 8, "max_number_operands": 2, "weight_of_kraus": 100, "weight_of_unitary": 0, "weight_of_cnot": 0 },
    { "num_qubits": 9, "num_gates": 12, "max_number_operands": 2, "weight_of_kraus": 100, "weight_of_unitary": 0, "weight_of_cnot": 0 },
    { "num_qubits": 9, "num_gates": 15, "max_number_operands": 2, "weight_of_kraus": 100, "weight_of_unitary": 0, "weight_of_cnot": 50 },
    { "num_qubits": 10, "num_gates": 10, "max_number_operands": 2, "weight_of_kraus": 100, "weight_of_unitary": 0, "weight_of_cnot": 50 },
    { "num_qubits": 10, "num_gates": 15, "max_number_operands": 2, "weight_of_kraus": 70, "weight_of_unitary": 0, "weight_of_cnot": 50 },
    { "num_qubits": 10, "num_gates": 20, "max_number_operands": 2, "weight_of_kraus": 70, "weight_of_unitary": 0, "weight_of_cnot": 50 },
]

hard_settings = [
    { "num_qubits": 15, "num_gates": 1000, "max_number_operands": 1, "weight_of_kraus": 0, "weight_of_unitary": 50, "weight_of_cnot": 50 },
    { "num_qubits": 15, "num_gates": 2000, "max_number_operands": 1, "weight_of_kraus": 0, "weight_of_unitary": 50, "weight_of_cnot": 50 },
    { "num_qubits": 16, "num_gates": 1000, "max_number_operands": 1, "weight_of_kraus": 0, "weight_of_unitary": 50, "weight_of_cnot": 50 },
    { "num_qubits": 16, "num_gates": 2000, "max_number_operands": 1, "weight_of_kraus": 0, "weight_of_unitary": 50, "weight_of_cnot": 50 },
    { "num_qubits": 17, "num_gates": 1000, "max_number_operands": 1, "weight_of_kraus": 0, "weight_of_unitary": 50, "weight_of_cnot": 50 },
    { "num_qubits": 17, "num_gates": 2000, "max_number_operands": 1, "weight_of_kraus": 0, "weight_of_unitary": 50, "weight_of_cnot": 50 },
    { "num_qubits": 18, "num_gates": 1000, "max_number_operands": 1, "weight_of_kraus": 0, "weight_of_unitary": 50, "weight_of_cnot": 50 },
    { "num_qubits": 20, "num_gates": 1000, "max_number_operands": 1, "weight_of_kraus": 0, "weight_of_unitary": 50, "weight_of_cnot": 50 },
]

def generate_googletests():
    tests_dir = os.path.dirname(os.path.realpath(__file__))
    print(f"Writing randomized googletests to directory {tests_dir}...")

    for f in glob.glob(os.path.join(tests_dir, "*.cpp")):
        os.remove(f)
        print(f"Removed {f}...")
    
    for setting in settings:
        name = "_".join(map(str, setting.values()))
        generate_googletest_file(name, **setting)
    
    for setting in hard_settings:
        name = "_".join(map(str, setting.values()))
        generate_googletest_file(name, **setting, is_hard=True)


if __name__ == "__main__":
    generate_googletests()
