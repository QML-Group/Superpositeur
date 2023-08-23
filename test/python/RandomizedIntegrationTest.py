from qiskit.circuit import QuantumCircuit
from qiskit.circuit.library import CXGate
from qiskit.quantum_info import random_quantum_channel, DensityMatrix, partial_trace, Kraus, random_unitary
from qiskit.extensions import UnitaryGate
import random
import numpy as np
import superpositeur
from timeit import default_timer as timer

def generate_random_kraus_circuit(qubits = 3, gates = 5):
    circuit = QuantumCircuit(qubits)

    for i in range(gates):
        numberOfOperands = random.randint(1, 2)
        operation = random_quantum_channel(2**numberOfOperands, 2**numberOfOperands).to_instruction()

        operands = random.sample(range(qubits), numberOfOperands)
        circuit.append(operation, operands)
    
    return circuit

def generate_random_unitary_and_kraus_circuit(qubits = 3, gates = 5):
    circuit = QuantumCircuit(qubits)

    for i in range(gates):
        nKraus = 0
        nUnitary = 40
        nCNOT = 40

        r = random.randint(0, nKraus + nUnitary + nCNOT)
        kraus = r < nKraus
        unitary = r >= nKraus and r < nKraus + nUnitary
        cnot = r >= nKraus + nUnitary

        numberOfOperands = random.randint(1, 2)

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

def execute_with_superpositeur(circuit: QuantumCircuit):
    state = superpositeur.QuantumState()
    for instruction in circuit.data:
        operation = instruction.operation
        ops = list(map(lambda x:  circuit.find_bit(x)[0], instruction.qubits))
        if operation.name == 'cx':
            state.apply(np.array([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 0, 1], [0, 0, 1, 0]]), operands = ops)
        else:
            ops.reverse()
            state.apply(*operation.params, operands = ops)
    return state

def test_random():
    numQubits = 12
    numGates = 20
    # circuit = generate_random_kraus_circuit(qubits = numQubits, gates = numGates)
    circuit = generate_random_unitary_and_kraus_circuit(qubits = numQubits, gates = numGates)

    print(circuit)

    keepIndices = [1, 5]
    reductionIndices = [0, 2, 3, 4, 6, 7, 8, 9, 10, 11]

    print("Qiskit output:")
    np.set_printoptions(suppress=True)
    start = timer()
    qiskitDensityMatrix = DensityMatrix(circuit)
    print(partial_trace(state=qiskitDensityMatrix, qargs=reductionIndices).data)
    
    print(timer() - start)

    print("\nSuperpositeur output:")
    np.set_printoptions(suppress=True)
    start = timer()
    s = execute_with_superpositeur(circuit)
    print(s.densityMatrix(*keepIndices))
    print(timer() - start)

test_random()