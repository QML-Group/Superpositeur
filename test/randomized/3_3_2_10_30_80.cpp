#include <gtest/gtest.h>

#include "superpositeur/Circuit.hpp"
#include "superpositeur/DefaultOperations.hpp"

namespace superpositeur {
namespace {
Circuit randomCircuit() {
    using namespace std::complex_literals;

    Circuit c;
    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix{ { 0.2770190176524787 + 0.07350614051313845i, -0.2860230333560726 + 0.4421949394377147i, -0.2653812009935471 + -0.4296752165910143i, 0.1838325234001772 + 0.593017201131802i }, { -0.5694584562335072 + 0.265798317382564i, 0.12417935533510129 + -0.3254178236888368i, -0.05985863322116605 + -0.5283409157119249i, 0.4481707772276636 + -0.012916039937177298i }, { -0.3486887931903173 + -0.45695954864057203i, -0.21448256981054115 + 0.33224555535597416i, -0.2928113541896987 + -0.3624298953550703i, -0.34975524427596116 + -0.4168831557107121i }, { -0.4082867152060086 + 0.16072239066925156i, -0.5274237397853638 + 0.40837666995573296i, 0.29449566483989265 + 0.3979990357039616i, 0.33502001396618414 + -0.07178876431489278i } } }, { QubitIndex{ 1 }, QubitIndex{ 2 } }));


    
    return c;
}

TEST(RandomizedIntegrationTests, 3_3_2_10_30_80) {
    auto c = randomCircuit();

    c.execute();
}
}

}
