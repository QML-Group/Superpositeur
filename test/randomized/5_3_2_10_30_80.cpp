#include <gtest/gtest.h>

#include "superpositeur/Circuit.hpp"
#include "superpositeur/DefaultOperations.hpp"

namespace superpositeur {
namespace {
Circuit randomCircuit() {
    using namespace std::complex_literals;

    Circuit c;
    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 4 } }));


    
    return c;
}

TEST(RandomizedIntegrationTests, 5_3_2_10_30_80) {
    auto c = randomCircuit();

    c.execute();
}
}

}
