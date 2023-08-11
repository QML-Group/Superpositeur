version 1.0

qubits 25

# q[0] is the least significant bit.

# Set input
x q[0]

x q[10:24]


# QFT circuit
h q[9]
crk q[8], q[9], 2
crk q[7], q[9], 3
crk q[6], q[9], 4
crk q[5], q[9], 5
crk q[4], q[9], 6
crk q[3], q[9], 7
crk q[2], q[9], 8
crk q[1], q[9], 9
crk q[0], q[9], 10

h q[8]
crk q[7], q[8], 2
crk q[6], q[8], 3
crk q[5], q[8], 4
crk q[4], q[8], 5
crk q[3], q[8], 6
crk q[2], q[8], 7
crk q[1], q[8], 8
crk q[0], q[8], 9

h q[7]
crk q[6], q[7], 2
crk q[5], q[7], 3
crk q[4], q[7], 4
crk q[3], q[7], 5
crk q[2], q[7], 6
crk q[1], q[7], 7
crk q[0], q[7], 8

h q[6]
crk q[5], q[6], 2
crk q[4], q[6], 3
crk q[3], q[6], 4
crk q[2], q[6], 5
crk q[1], q[6], 6
crk q[0], q[6], 7

h q[5]
crk q[4], q[5], 2
crk q[3], q[5], 3
crk q[2], q[5], 4
crk q[1], q[5], 5
crk q[0], q[5], 6

h q[4]
crk q[3], q[4], 3
crk q[2], q[4], 4
crk q[1], q[4], 5
crk q[0], q[4], 6

h q[3]
crk q[2], q[3], 3
crk q[1], q[3], 4
crk q[0], q[3], 5

h q[2]
crk q[1], q[2], 3
crk q[0], q[2], 4

h q[1]
crk q[0], q[1], 3

h q[0]

# Swaps
swap q[0], q[9]
swap q[1], q[8]
swap q[2], q[7]
swap q[3], q[6]
swap q[4], q[5]

measure q[0:24]