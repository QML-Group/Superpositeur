from qiskit.circuit import QuantumCircuit
from qiskit.circuit.library import CXGate
from qiskit.quantum_info import random_quantum_channel, random_unitary
import random
import numpy as np

def generate_random_unitary_and_kraus_circuit(*, qubits, gates, max_number_operands, weight_of_kraus, weight_of_unitary, weight_of_cnot):
    circuit = QuantumCircuit(qubits)

    for i in range(gates):
        r = random.randint(0, weight_of_kraus + weight_of_unitary + weight_of_cnot)
        kraus = r < weight_of_kraus
        unitary = r >= weight_of_kraus and r < weight_of_kraus + weight_of_unitary
        cnot = r >= weight_of_kraus + weight_of_unitary

        ########## FIXME: there is STILL a problem with the order of operands, when there are more than 3!!!
        numberOfOperands = random.randint(1, max_number_operands)

        if unitary:
            operation = random_unitary(2**numberOfOperands).to_instruction()
        elif cnot:
            operation = CXGate()
            numberOfOperands = 2
        else:
            assert(kraus)
            operation = random_quantum_channel(2**numberOfOperands, 2**numberOfOperands).to_instruction()

        operands = random.sample(range(qubits), numberOfOperands)
        
        circuit.append(operation, operands)
    
    return circuit

def circuit_to_cpp(circuit: QuantumCircuit):
    cppString = """
#include "superpositeur/Circuit.hpp"
#include "superpositeur/DefaultOperations.hpp"

namespace superpositeur {

Circuit randomCircuit() {
    using namespace std::complex_literals;

    Circuit c;
    """

    for instruction in circuit.data:
        operation = instruction.operation
        ops = list(map(lambda x:  circuit.find_bit(x)[0], instruction.qubits))
        if operation.name == 'cx':
            krausString = "{ default_operations::CNOT }"
            operandsString = "{ " + ", ".join(map(lambda op: "QubitIndex{ " + op + " }")) + " }"
            cppString += f"c.addInstruction(CircuitInstruction({krausString}, {operandsString}))\n"
        else:
            ops.reverse()

            def getKrausString(k: np.ndarray):
                return "{ " + ", ".join(map(lambda l: "{ " + ", ".join(map(lambda x: f"{x.real} + {y.imag}i", l)) + " }", k)) + " }"

            krausStringsList = [getKrausString(k) for k in operation.params]
            krausString = "{ " + ", ".join(krausStringsList) + " }"
            operandsString = "{ " + ", ".join(map(lambda op: "QubitIndex{ " + op + " }")) + " }"
            cppString += f"c.addInstruction(CircuitInstruction({krausString}, {operandsString}))\n"
    
    cppString += """
}

}
"""

    return cppString