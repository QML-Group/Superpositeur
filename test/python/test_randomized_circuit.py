from qiskit.circuit import QuantumCircuit
from qiskit.quantum_info import DensityMatrix, partial_trace
import numpy as np
import superpositeur
from timeit import default_timer as timer
import pytest
import random
from . import QiskitRandomCircuitGeneration

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

@pytest.mark.parametrize(
    "num_qubits,num_gates,max_number_operands,weight_of_kraus,weight_of_unitary,weight_of_cnot",
    [
        (3,3,2,10,30,80),
        (4,3,2,10,30,80),
        (5,3,2,10,30,80),
        (8,10,2,10,30,80),
        (8,15,2,10,30,80),
        (8,20,2,10,30,80),
        (3,3,2,100,30,20),
        (3,4,2,100,30,20),
        (3,5,2,100,30,20),
        (6,5,2,100,0,0),
        (8,8,2,100,0,0),
        (9,12,2,100,0,0),
    ]
)
def test_random_compare_with_qiskit(num_qubits, num_gates, max_number_operands, weight_of_kraus, weight_of_unitary, weight_of_cnot):
    circuit = QiskitRandomCircuitGeneration.generate_random_unitary_and_kraus_circuit(
        qubits = num_qubits,
        gates = num_gates,
        max_number_operands = max_number_operands,
        weight_of_kraus = weight_of_kraus,
        weight_of_unitary = weight_of_unitary,
        weight_of_cnot = weight_of_cnot,
    )

    print("Circuit")
    print(circuit)

    np.set_printoptions(suppress=True)
    
    qiskitDensityMatrix = DensityMatrix(circuit)
    superpositeurState = execute_with_superpositeur(circuit)

    for numberOfReductionQubits in range(min(3, num_qubits)):
        keepIndices = random.sample(range(num_qubits), numberOfReductionQubits)
        reductionIndices = [q for q in range(num_qubits) if q not in keepIndices]

        qiskitReducedDM = partial_trace(state=qiskitDensityMatrix, qargs=reductionIndices).data
        print("Qiskit reduced density matrix:")
        print(qiskitReducedDM)

        superpositeurReducedDM = superpositeurState.densityMatrix(*keepIndices)
        print("Superpositeur reduced density matrix:")
        print(superpositeurReducedDM)
        
        assert(np.allclose(qiskitReducedDM, superpositeurReducedDM))