from qiskit.circuit import QuantumCircuit
from qiskit.quantum_info import random_quantum_channel, DensityMatrix, partial_trace, Kraus
from qiskit.extensions import UnitaryGate
import random
import numpy as np
import superpositeur
from timeit import default_timer as timer

def generate_random_circuit(qubits = 3, gates = 5):
    circuit = QuantumCircuit(qubits)

    for i in range(gates):
        # numberOfOperands = random.randint(1, qubits)
        numberOfOperands = random.randint(1, 2)
        # numberOfOperands = 2
        operation = random_quantum_channel(2**numberOfOperands, 2**numberOfOperands).to_instruction()

        operands = random.sample(range(qubits), numberOfOperands)
        circuit.append(operation, operands)
    
    return circuit

def execute_with_superpositeur(circuit: QuantumCircuit):
    state = superpositeur.QuantumState()
    for instruction in circuit.data:
        operation = instruction.operation
        ops = list(map(lambda x:  circuit.find_bit(x)[0], instruction.qubits))
        ops.reverse()
        state.apply(*operation.params, operands = ops)
    return state

def test_random():
    numQubits = 4 # Need to diagonalize?
    numGates = 4
    circuit = generate_random_circuit(qubits = numQubits, gates = numGates)

    print(circuit)

    keepIndices = [1, 2]
    reductionIndices = [0, 3]

    print("Qiskit output:")
    np.set_printoptions(suppress=True)
    start = timer()
    qiskitDensityMatrix = DensityMatrix(circuit)
    print(partial_trace(state=qiskitDensityMatrix, qargs=reductionIndices).data)
    
    # print(partial_trace(state=qiskitDensityMatrix, qargs=[1, 2]).data)
    # print(partial_trace(state=qiskitDensityMatrix, qargs=[0, 2]).data)
    # print(partial_trace(state=qiskitDensityMatrix, qargs=[0, 1]).data)
    print(timer() - start)

    print("\nSuperpositeur output:")
    np.set_printoptions(suppress=True)
    start = timer()
    s = execute_with_superpositeur(circuit)
    print(s.densityMatrix(*keepIndices))
    print(timer() - start)
    # print(s.densityMatrix(0))
    # print(s.densityMatrix(1))
    # print(s.densityMatrix(2))

test_random()