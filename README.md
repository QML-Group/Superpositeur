# Superpositeur
Superpositeur is a project to explore efficient algorithms behind density matrix simulation of quantum circuits.

## Context
Developed at the TU in Delft, The Netherlands, as a collaboration between the [Quantum Machine Learning group](https://www.tudelft.nl/en/eemcs/the-faculty/departments/quantum-computer-engineering/sections/quantum-circuits-architectures-and-technology/groups/quantum-machine-learning) of Sebastian Feld and [QuTech](https://qutech.nl/).

## Goals
- Reads cQasm (and possibly also OpenQasm) to input a list of gates
- Allows access to the full and reduced density matrices of the final quantum state, in an efficient fashion
- Allow access to the diagonal of a reduced density matrix (e.g. computational basis measurement probabilities), in an efficient fashion
- Has a user-friendly Python and C++ API, but also a "low-level" API which allows efficient access to the underlying state
- Allows definition of gates with arbitrary number of qubits and arbitrary (static) integer or floating point operands (simple example: arbitrary Bloch sphere rotations)
- Written in C++ and portable on Linux, MacOS, Windows, for architectures 64 bits x86 and ARM
- NO usage of randomness: the output of the simulator is perfectly deterministic, whatever the input circuit
- Fast and performant on a large class of quantum circuits
- Efficient use of memory

## Future improvements
- Multithreading
- Documentation
- Research paper
- When input circuit is obviously a tensor product of smaller circuits, simulate those separately
- Do some obvious optimization of the input circuit (e.g. collapse consecutive 1-qubit gates operating on the same qubit)
- Explore how scheduling impacts simulation complexity
- Benchmark, profiling and performance tuning
- Usage as backend of [QX-simulator](https://github.com/QuTech-Delft/qx-simulator) once stable, robust and tested enough

## Current author
- Pablo Le Hénaff (p.lehenaff@tudelft.nl)
