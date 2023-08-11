from qiskit.qasm3 import load
from qiskit import transpile
import os
from qiskit import Aer


circuit = load(os.path.dirname(os.path.realpath(__file__)) + "/q17.qasm")
backend = Aer.get_backend('aer_simulator_statevector')
circuit = transpile(circuit, backend)
job = backend.run(circuit)
result = job.result()
# outputstate = result.get_statevector(circuit, decimals=3)
outputstate = result.get_counts(circuit)
print(outputstate)