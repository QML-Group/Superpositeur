from qiskit.circuit import QuantumCircuit
from ..python import QiskitRandomCircuitGeneration
import os 
import numpy as np
import glob

def circuit_to_cpp_string(name: str, circuit: QuantumCircuit):
    cppString = """#include <gtest/gtest.h>

#include "superpositeur/Circuit.hpp"
#include "superpositeur/DefaultOperations.hpp"

namespace superpositeur {
namespace {
Circuit randomCircuit() {
    using namespace std::complex_literals;

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

            def getKrausString(k: np.ndarray):
                return "Matrix{ " + ", ".join(map(lambda l: "{ " + ", ".join(map(lambda x: f"{x.real} + {x.imag}i", l)) + " }", k)) + " }"

            krausStringsList = [getKrausString(k) for k in operation.params]
            krausString = "{ " + ", ".join(krausStringsList) + " }"
            operandsString = "{ " + ", ".join(map(lambda op: "QubitIndex{ " + str(op) + " }", ops)) + " }"
            cppString += f"""    c.addInstruction(CircuitInstruction({krausString}, {operandsString}));\n\n"""
    
    cppString += """
    
    return c;
}

TEST(RandomizedIntegrationTests, """ + name + """) {
    auto c = randomCircuit();

    c.execute();
}
}

}
"""

    return cppString

def generate_googletest_file(name, num_qubits, num_gates, max_number_operands, weight_of_kraus, weight_of_unitary, weight_of_cnot):
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

    s = circuit_to_cpp_string(name, circuit)

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


if __name__ == "__main__":
    generate_googletests()
