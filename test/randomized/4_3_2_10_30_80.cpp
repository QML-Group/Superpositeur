#include <gtest/gtest.h>

#include "superpositeur/Circuit.hpp"
#include "superpositeur/DefaultOperations.hpp"

namespace superpositeur {
namespace {
Circuit randomCircuit() {
    using namespace std::complex_literals;

    Circuit c;
    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix{ { 0.6025092919221446 + -0.4791868794025821i, 0.49791594572677894 + 0.3993021396723268i }, { -0.209992847945275 + -0.6027150998338465i, -0.6529572801534028 + 0.407779723059864i } } }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 1 } }));


    
    return c;
}

TEST(RandomizedIntegrationTests, 4_3_2_10_30_80) {
    auto c = randomCircuit();

    c.execute();
}
}

}
