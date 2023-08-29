from qiskit.circuit import QuantumCircuit
import qiskit
from qiskit.quantum_info import DensityMatrix, partial_trace, Statevector
from qiskit_aer import AerSimulator
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
        elif operation.name != 'save_statevector':
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
        (10,1000,1,0,50,50),
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

    # print("Circuit")
    # print(circuit)

    np.set_printoptions(suppress=True)
    
    start = timer()
    qiskitDensityMatrix = DensityMatrix(circuit)
    end = timer()
    print(f"Qiskit took {end - start} seconds to compute the full density matrix")

    start = timer()
    superpositeurState = execute_with_superpositeur(circuit)
    end = timer()
    print(f"Superpositeur took {end - start} seconds to compute the full state")

    for numberOfReductionQubits in range(1, min(3, num_qubits)):
        keepIndices = random.sample(range(num_qubits), numberOfReductionQubits)
        reductionIndices = [q for q in range(num_qubits) if q not in keepIndices]

        start = timer()
        qiskitReducedDM = partial_trace(state=qiskitDensityMatrix, qargs=reductionIndices).data
        end = timer()
        print(f"Qiskit reduced density matrix (took {end - start} seconds):")
        # print(qiskitReducedDM)

        start = timer()
        superpositeurReducedDM = superpositeurState.densityMatrix(*keepIndices)
        end = timer()
        print(f"Superpositeur reduced density matrix (took {end - start} seconds):")
        # print(superpositeurReducedDM)
        
        assert(np.allclose(qiskitReducedDM, superpositeurReducedDM))

@pytest.mark.parametrize(
    "num_qubits,num_gates,max_number_operands,weight_of_kraus,weight_of_unitary,weight_of_cnot",
    [
        (10,5000,1,0,50,50),
        (15,5000,1,0,50,50),
        (16,5000,1,0,50,50),
        (17,5000,1,0,50,50),
        (18,5000,1,0,50,50),
        (19,5000,1,0,50,50),
        (23,5000,1,0,50,50),
        (27,5000,1,0,50,50),
    ]
)
def test_random_probabilities_compare_with_qiskit(num_qubits, num_gates, max_number_operands, weight_of_kraus, weight_of_unitary, weight_of_cnot):
    circuit = QiskitRandomCircuitGeneration.generate_random_unitary_and_kraus_circuit(
        qubits = num_qubits,
        gates = num_gates,
        max_number_operands = max_number_operands,
        weight_of_kraus = weight_of_kraus,
        weight_of_unitary = weight_of_unitary,
        weight_of_cnot = weight_of_cnot,
    )

    np.set_printoptions(suppress=True)
    
    start = timer()
    superpositeurState = execute_with_superpositeur(circuit)
    superpositeurProbabilities = superpositeurState.densityMatrixDiagonal(*list(range(num_qubits)))
    end = timer()
    print(f"Superpositeur took {end - start} seconds to compute the probabilities")

    circuit.save_statevector('finalstatevector')
    backend = AerSimulator(method='statevector')
    start = timer()
    result = qiskit.execute(circuit, backend).result()
    qiskitProbabilities = result.data()["finalstatevector"].probabilities()
    end = timer()
    print(f"Qiskit took {end - start} seconds to compute the probabilities")
    
    assert(np.allclose(qiskitProbabilities, superpositeurProbabilities))

if __name__ == '__main__':
    pytest.main()