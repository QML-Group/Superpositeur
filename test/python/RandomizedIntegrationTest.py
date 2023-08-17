from qiskit.circuit import QuantumCircuit
from qiskit.quantum_info import random_quantum_channel, DensityMatrix, partial_trace, Kraus
from qiskit.extensions import UnitaryGate
import random
import superpositeur

def generate_random_circuit(qubits = 3, gates = 5):
    circuit = QuantumCircuit(qubits)

    for i in range(gates):
        numberOfOperands = random.randint(1, qubits)
        operation = random_quantum_channel(2**numberOfOperands, 2**numberOfOperands).to_instruction()

        operands = random.sample(range(qubits), numberOfOperands)
        circuit.append(operation, operands)
    
    return circuit

def execute_with_superpositeur(circuit: QuantumCircuit):
    state = superpositeur.QuantumState()
    for instruction in circuit.data:
        operation = instruction.operation
         # FIXME: WHY do I have to reverse the operands? Should Qiskit and Superpositeur be aligned, since this can be confusing?
        state.apply(*operation.params, operands = list(reversed(list(map(lambda x: circuit.find_bit(x)[0], instruction.qubits)))))
    return state

def test_random():
    numQubits = 2
    numGates = 5
    circuit = generate_random_circuit(qubits = numQubits, gates = numGates)

    # print(circuit[0])

    keepIndices = [0]
    reductionIndices = [1]

    qiskitDensityMatrix = DensityMatrix(circuit)
    qiskitReducedDensityMatrix = partial_trace(state=qiskitDensityMatrix, qargs=reductionIndices)
    print("Qiskit output:")
    print(qiskitReducedDensityMatrix.data)

    print("\nSuperpositeur output:")
    print(execute_with_superpositeur(circuit).densityMatrix(*keepIndices))

test_random()