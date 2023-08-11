OPENQASM 3.0;
include "stdgates.inc";

qubit[25] q;
bit[25] b;


// Set input
x q[0];

x q[10:24];

// QFT circuit
h q[9];
cphase(pi / 2) q[8], q[9];
cphase(pi / 4) q[7], q[9];
cphase(pi / 8) q[6], q[9];
cphase(pi / 16) q[5], q[9];
cphase(pi / 32) q[4], q[9];
cphase(pi / 64) q[3], q[9];
cphase(pi / 128) q[2], q[9];
cphase(pi / 256) q[1], q[9];
cphase(pi / 512) q[0], q[9];

h q[8];
cphase(pi / 2) q[7], q[8];
cphase(pi / 4) q[6], q[8];
cphase(pi / 8) q[5], q[8];
cphase(pi / 16) q[4], q[8];
cphase(pi / 32) q[3], q[8];
cphase(pi / 64) q[2], q[8];
cphase(pi / 128) q[1], q[8];
cphase(pi / 256) q[0], q[8];

h q[7];
cphase(pi / 2) q[6], q[7];
cphase(pi / 4) q[5], q[7];
cphase(pi / 8) q[4], q[7];
cphase(pi / 16) q[3], q[7];
cphase(pi / 32) q[2], q[7];
cphase(pi / 64) q[1], q[7];
cphase(pi / 128) q[0], q[7];

h q[6];
cphase(pi / 2) q[5], q[6];
cphase(pi / 4) q[4], q[6];
cphase(pi / 8) q[3], q[6];
cphase(pi / 16) q[2], q[6];
cphase(pi / 32) q[1], q[6];
cphase(pi / 64) q[0], q[6];

h q[5];
cphase(pi / 2) q[4], q[5];
cphase(pi / 4) q[3], q[5];
cphase(pi / 8) q[2], q[5];
cphase(pi / 16) q[1], q[5];
cphase(pi / 32) q[0], q[5];

h q[4];
cphase(pi / 4) q[3], q[4];
cphase(pi / 8) q[2], q[4];
cphase(pi / 16) q[1], q[4];
cphase(pi / 32) q[0], q[4];

h q[3];
cphase(pi / 4) q[2], q[3];
cphase(pi / 8) q[1], q[3];
cphase(pi / 16) q[0], q[3];

h q[2];
cphase(pi / 4) q[1], q[2];
cphase(pi / 8) q[0], q[2];

h q[1];
cphase(pi / 4) q[0], q[1];

h q[0];

// Swaps
swap q[0], q[9];
swap q[1], q[8];
swap q[2], q[7];
swap q[3], q[6];
swap q[4], q[5];

measure q -> b;