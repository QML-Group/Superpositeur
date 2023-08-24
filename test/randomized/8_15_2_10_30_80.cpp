#include <gtest/gtest.h>

#include "superpositeur/Circuit.hpp"
#include "superpositeur/DefaultOperations.hpp"

namespace superpositeur {
namespace {
Circuit randomCircuit() {
    using namespace std::complex_literals;

    Circuit c;
    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 5 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 5 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix{ { 0.03392058014612287 + -0.3820784360782897i, -0.5801421606011423 + 0.718540560037885i }, { 0.2899730239161825 + 0.8768016356778904i, -0.1027898961892435 + 0.36955212665452986i } } }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 7 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix{ { -0.4555294697451464 + 0.11878386862175074i, -0.7664454969809018 + 0.43697207566005314i }, { -0.7664460185891848 + -0.43697116076206177i, 0.4555296115358378 + 0.11878332486107684i } } }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 5 } }));


    
    return c;
}

TEST(RandomizedIntegrationTests, 8_15_2_10_30_80) {
    auto c = randomCircuit();

    c.execute();
}
}

}
