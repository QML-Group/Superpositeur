#include <gtest/gtest.h>

#include "superpositeur/Circuit.hpp"
#include "superpositeur/DefaultOperations.hpp"

namespace superpositeur {
namespace {
Circuit randomCircuit() {
    using namespace std::complex_literals;

    Circuit c;
    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix{ { 0.22442117740136736 + -0.06343655861061132i, -0.04618569926469588 + 0.14577568665025714i }, { -0.33344361435938313 + -0.012318428180617107i, 0.1454493490442345 + 0.15804740642882598i } }, Matrix{ { 0.03685186186327827 + -0.3264600559512102i, -0.2773188213763743 + -0.23588976469100115i }, { -0.38438239007455455 + -0.12225035167910543i, 0.09482567903868781 + -0.2762278281439026i } }, Matrix{ { 0.5702358644973209 + -0.01028476308261602i, -0.2660275962049982 + -0.19583846581307623i }, { 0.041733626153081106 + -0.06916090056147967i, 0.6640369911943568 + 0.2182239552212064i } }, Matrix{ { -0.12240011334858142 + -0.03564205657811203i, -0.02926042957489624 + 0.03536685084845791i }, { -0.19214732139547339 + -0.42269725821518667i, 0.0066141323049229775 + -0.3358533337061615i } } }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix{ { 0.12268542976789765 + -0.17403652709668968i, 0.25144001208962713 + 0.29421362075433755i }, { -0.38419238110102716 + -0.34576711848150854i, 0.30815812342159804 + 0.09157135289428087i } }, Matrix{ { 0.35290453442285624 + -0.092267319439806i, -0.26392120755878967 + -0.49967723860278546i }, { -0.2742740184635443 + 0.21334183299408602i, -0.19257493964295053 + 0.0336241702086981i } }, Matrix{ { 0.183162528103811 + -0.12872340347941877i, 0.3169994747608794 + 0.3579911668639998i }, { -0.2346435799646816 + 0.0749668548589095i, -0.15203226155021887 + -0.01852595060014374i } }, Matrix{ { -0.2506546583520652 + 0.14919325818060739i, -0.1181440825617792 + 0.3362877657148059i }, { -0.4862166612158833 + -0.0376317703892545i, -0.0701918692276584 + 0.07241780951636881i } } }, { QubitIndex{ 2 } }));


    
    return c;
}

TEST(RandomizedIntegrationTests, 3_3_2_100_30_20) {
    auto c = randomCircuit();

    c.execute();
}
}

}
