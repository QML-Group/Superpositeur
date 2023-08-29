#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <chrono>
#include <iostream>

#include "superpositeur/Circuit.hpp"
#include "superpositeur/DefaultOperations.hpp"

namespace superpositeur {

using namespace std::complex_literals;

using testing::ElementsAreArray;
using testing::DoubleNear;

namespace {
Circuit randomCircuit() {

    Circuit c;
    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.8267367049880994 + -0.3564910420716469i, -0.13216843570253728 + -0.4146710288313326i }, { 0.18118599815609288 + 0.39571731276287614i, -0.8515043418386506 + 0.292437682732295i } }) }, { QubitIndex{ 11 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.025712727441901118 + 0.5902621416124303i, -0.8064628525413999 + 0.023390752390494984i }, { 0.6743009289274123 + 0.44299855200015537i, 0.28777207642347225 + -0.516001717249388i } }) }, { QubitIndex{ 9 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 5 }, QubitIndex{ 11 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.3283549556566454 + -0.19704324412455457i, -0.917016720567267 + 0.11152272073729413i }, { -0.25693799307894916 + 0.887321728998651i, -0.14191610306320795 + -0.3556723726278992i } }) }, { QubitIndex{ 11 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.048979383388024544 + 0.5663518767220108i, -0.4353295854413447 + -0.69809363539274i }, { 0.7930130054203521 + -0.21903640101719613i, 0.4194829847785563 + -0.3836501710476461i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 8 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 5 }, QubitIndex{ 9 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.22122667238440738 + -0.09071716741825377i, -0.8339719487142918 + -0.4973127222573505i }, { -0.9607859600691715 + -0.14042611543188996i, -0.11438208914541581 + -0.2099704329695009i } }) }, { QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 10 }, QubitIndex{ 8 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.044979855458407325 + -0.024725045108294246i, 0.8409598400992406 + -0.5386576204673921i }, { -0.6194852620692702 + 0.7833284718598585i, 0.020137390311491976 + 0.047212294628719564i } }) }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 7 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.5554855151011646 + 0.10169876667554019i, -0.565559475736079 + 0.6010288535291791i }, { -0.5434799084437274 + -0.6210658519734163i, 0.5514494135886627 + 0.12169774395300914i } }) }, { QubitIndex{ 11 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.9433677866697769 + -0.2120196369181576i, 0.25448585438840715 + -0.018488984587079232i }, { 0.04932745423267314 + -0.25034315427784787i, -0.32526962053340036 + 0.9105464190930624i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.5159000966630586 + 0.5423840383225815i, -0.6551676617230696 + 0.10208810052234304i }, { 0.5741739606887023 + 0.3316487722018329i, 0.6768721169774473 + -0.3196521422148357i } }) }, { QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 6 }, QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 12 }, QubitIndex{ 11 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 6 }, QubitIndex{ 9 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.6373833328519469 + -0.4777653200472565i, 0.6005076912930789 + -0.06980901561075867i }, { -0.40763068808719416 + 0.4464526941258698i, 0.015120653796630051 + 0.796422362736052i } }) }, { QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 11 }, QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.18246979911313055 + 0.978263911785216i, -0.09411749297605491 + -0.029092762436782053i }, { 0.014182631549467292 + 0.09748509769514947i, 0.9947151285733365 + -0.02893651114127975i } }) }, { QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.42053766353905603 + -0.5613877589461354i, -0.6301856389191451 + -0.33295332728569665i }, { 0.05166918593954783 + -0.7108601500118654i, 0.25315725171209164 + 0.654155599421906i } }) }, { QubitIndex{ 9 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.27099086704776654 + 0.42016061606064864i, -0.07650618159447387 + 0.8626562530151102i }, { -0.22422161382892816 + 0.8365128059869659i, 0.39873690275755924 + -0.30162870501839306i } }) }, { QubitIndex{ 8 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.815504485470369 + 0.4808462286297692i, 0.15000886968284194 + -0.2850204862930256i }, { -0.05445150271789463 + -0.31744979515192595i, 0.9242704753375806 + -0.2049018053336927i } }) }, { QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.4431767548918444 + -0.09952864730407059i, -0.815333682877211 + 0.35905347492433487i }, { 0.6589573897287867 + -0.5995528090261779i, -0.08882653682235411 + 0.4454452088268439i } }) }, { QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 9 }, QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 6 }, QubitIndex{ 11 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.625754604134989 + -0.5533946426058518i, 0.12938020303853487 + -0.5342717548221237i }, { -0.021191926326989654 + 0.5493054225087856i, 0.7529306225730087 + -0.36181477671982604i } }) }, { QubitIndex{ 11 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.20078769865760893 + -0.1712171479778659i, -0.4523394716979068 + 0.8519143094526379i }, { -0.9633779278166735 + 0.04766504486265166i, -0.24028839544267316 + 0.10905273361763501i } }) }, { QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 9 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.09037415530961557 + -0.9239084630384692i, 0.011969277918587207 + -0.3715944030311474i }, { -0.37147634879437863 + 0.015198232277616658i, 0.924658984431457 + 0.0823413537186446i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.23606324821582866 + 0.13612285147907424i, 0.8539731014043425 + 0.4432546155708328i }, { 0.3612167512039213 + 0.8917775343641946i, -0.15766066770878323 + 0.222257512157908i } }) }, { QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 10 }, QubitIndex{ 6 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 5 }, QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 9 }, QubitIndex{ 6 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.5319810714166124 + -0.09932466228278636i, -0.8344919427979102 + -0.10370124648340984i }, { 0.2035045480964375 + 0.8159146095155475i, -0.1627063174707697 + -0.5161355472531004i } }) }, { QubitIndex{ 11 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.67538787057695 + 0.09181589924251478i, 0.7001033232306734 + 0.2127825221327839i }, { -0.5065010166154013 + -0.5280888041715962i, 0.3874405982557498 + 0.5607751045645708i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 5 }, QubitIndex{ 9 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.4489777184467487 + -0.33806154276484446i, -0.7871759984998288 + 0.2539435941850128i }, { 0.7524258485239141 + 0.34349489678141154i, 0.4067557478014732 + 0.38783548056960865i } }) }, { QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 7 }, QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.464093134010626 + 0.10709645702935972i, -0.7802854615595771 + 0.4053424605623142i }, { 0.6088243727559923 + -0.6344137411770226i, 0.028043790644878525 + -0.47546359897505575i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.7593298919246152 + 0.0005898978218811833i, 0.6314973692651183 + -0.15693578260405283i }, { -0.11694968812653776 + 0.6401097856597863i, -0.0482963023102867 + -0.7577926496957076i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.7940269503381977 + 0.6002496891593311i, 0.05178874125388744 + -0.0808667983790947i }, { 0.05411616648501751 + -0.07932813703669793i, 0.24122673189088847 + -0.9657060375810508i } }) }, { QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 12 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.14293029653560696 + -0.19055942686191774i, 0.9706110779712529 + 0.0342369754181599i }, { 0.5362187935267397 + -0.8097699924268795i, -0.07200981445413657 + -0.22706067791667472i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.4429321672465043 + 0.7000354200790725i, 0.14429623065708155 + 0.5412394143733341i }, { -0.44502873466765974 + -0.3401630949605394i, 0.4776603549145147 + 0.6768153954291442i } }) }, { QubitIndex{ 6 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 5 }, QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.48669316467473966 + -0.2460539675504856i, -0.8074575155421754 + 0.22494347980327908i }, { 0.8300913000427655 + -0.11634277848178821i, 0.5444654639363204 + 0.031147232125122257i } }) }, { QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.4959770893768045 + -0.19910245596037246i, 0.7855085202904486 + -0.31199567848765364i }, { -0.3721693208684765 + 0.7588510627574812i, 0.23709024717401078 + -0.478981498443421i } }) }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 11 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 8 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 5 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.9018358263350721 + -0.007891209933329613i, -0.3148085581541109 + 0.2958469923072668i }, { -0.4127193293969902 + -0.12764257239034696i, -0.8137792251763641 + 0.388758924691164i } }) }, { QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.09931853261055923 + 0.810515304095431i, 0.18494373510276216 + 0.546805802596695i }, { -0.5004285609285392 + 0.28770127965333714i, 0.46171657642132946 + -0.6735109740387236i } }) }, { QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 6 }, QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 11 }, QubitIndex{ 6 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 10 }, QubitIndex{ 6 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.7601116519403097 + 0.33873121045932686i, -0.07466812678140987 + 0.5494689386018903i }, { 0.060692833239857705 + 0.5511876483001515i, 0.7684671133327778 + -0.3193224891557982i } }) }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 6 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.2024208820138016 + -0.1382103656746286i, -0.7476627867623477 + -0.6171904395205746i }, { 0.7610653413773668 + 0.60058573700929i, -0.17781328764452517 + -0.16869722402044826i } }) }, { QubitIndex{ 9 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.6096909261329854 + 0.08919442173207344i, -0.7092826881588133 + -0.34240239193263633i }, { -0.05724291005716698 + 0.7855218513645373i, -0.22696701407559092 + 0.5728565656415463i } }) }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.6214781096811552 + -0.07664001399123177i, 0.7761005841258741 + -0.07455971272771049i }, { -0.5338910796631591 + -0.5682003013891036i, -0.5175041031314658 + -0.3525595492956267i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 8 }, QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.9601407383165742 + 0.1654154896325599i, -0.1897037058956204 + 0.12157295087309561i }, { 0.21572060794827422 + 0.06505457494251364i, -0.9687740731746354 + 0.10348582864383907i } }) }, { QubitIndex{ 6 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.4724690747686946 + -0.845843531082456i, 0.24727858455437743 + 0.013228602872537338i }, { -0.11722576751637272 + 0.21812797560921204i, 0.9681848112890058 + 0.036006622669961796i } }) }, { QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 6 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 9 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.3814213850365815 + 0.7405971229910947i, -0.5323786897883707 + 0.15035477748746987i }, { -0.0025985166638063384 + -0.5531969596493067i, -0.8171546323591659 + -0.16194035432796686i } }) }, { QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.031074357605196168 + 0.07092959358277627i, 0.5784222477603181 + 0.812053619134542i }, { 0.7646473965192329 + 0.6397794432849226i, -0.07312994632684872 + 0.025468291984851232i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.19277412341562705 + 0.8378807930674298i, 0.20386377881744217 + -0.46822374313601883i }, { 0.19410784102999562 + 0.47235162749873694i, -0.21011229489101504 + 0.8337019308995269i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 11 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 9 }, QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 6 }, QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 9 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.05626940556694948 + -0.6720371558739935i, 0.31851195102160323 + 0.6661455938294182i }, { 0.06880079141728751 + 0.7351641083610454i, 0.3934101553537579 + 0.547748696522955i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.5959051676854734 + -0.23024973067075483i, 0.33391791069968246 + 0.6930951749693194i }, { -0.660821135900096 + -0.39395116321633017i, -0.1766044235125929 + -0.6139452621724448i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.19184291533481376 + -0.4644770345469831i, 0.7340161514445673 + -0.45681251037245707i }, { -0.19476005287725293 + -0.8423336049437478i, -0.19347142348598545 + 0.4638010651993226i } }) }, { QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 11 }, QubitIndex{ 9 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 8 }, QubitIndex{ 6 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.18161404313826846 + -0.834674391369437i, -0.4655182570865738 + 0.23157666559070508i }, { -0.0461607635400548 + -0.517884334225732i, 0.8063635560516091 + -0.28185601952907274i } }) }, { QubitIndex{ 6 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 10 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.8132234543200141 + -0.024958434956663902i, -0.33119180057395836 + 0.477866802677187i }, { -0.3241421710912548 + 0.48267643695190454i, 0.27338435741702427 + 0.7663004001377937i } }) }, { QubitIndex{ 9 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.1583408173891223 + 0.521648964462131i, 0.7111453207902463 + -0.44394017180501755i }, { 0.776306103412827 + -0.31647966321412174i, 0.3301224065419798 + 0.4338302124965491i } }) }, { QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.905687680757852 + -0.0332523586327258i, 0.021426865031902546 + 0.4220959547587819i }, { 0.42250749926631576 + -0.010560238280795855i, 0.10165986318086687 + 0.9005782401597456i } }) }, { QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 12 }, QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.4058503225000585 + 0.22587977960263206i, 0.8148446488714812 + 0.3468314274958044i }, { -0.8817838935449946 + -0.08198174173494138i, -0.24978867023057238 + -0.3915887885667532i } }) }, { QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.1732132057445659 + -0.14825019909347686i, -0.9534291804334115 + -0.19746357062128728i }, { -0.14429469136870798 + -0.9629112658325502i, 0.13841792291331062 + 0.1811668148197394i } }) }, { QubitIndex{ 8 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 11 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.9126282850684224 + 0.264297040895786i, 0.3026877928362303 + 0.07507854243878959i }, { 0.307420178725012 + -0.05243587683526871i, -0.9297082349857413 + 0.19592322560864484i } }) }, { QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.6908152425563738 + 0.6542613380629112i, 0.26895808147277306 + -0.1495926220720541i }, { -0.113196663040732 + -0.286186857916904i, -0.11957439075899667 + -0.9439203159730738i } }) }, { QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.7401370913773632 + 0.49800349753057616i, -0.05044295060290018 + 0.4490491188603349i }, { 0.14033014465710733 + -0.4295312013291761i, -0.8800194171129504 + 0.14620609798983253i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.09009807798913894 + 0.09849979478123382i, -0.9018971537880328 + 0.41080609873730367i }, { -0.8595389089151647 + 0.4933284816748838i, 0.13255437940705872 + -0.015786377966197862i } }) }, { QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.23216108765644106 + 0.0939146863858885i, 0.9346646959456301 + -0.2523552401121901i }, { -0.6665578540458952 + -0.7021266896144523i, -0.149303435942848 + -0.2010652206535018i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 6 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.15922850578266012 + 0.8890699037736092i, -0.025860275550381423 + -0.42840662378010064i }, { -0.29417872440225423 + 0.31250578756123687i, -0.4469792458607687 + 0.7848621309628412i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.8579565095892028 + -0.4260398333778529i, 0.2843706835643422 + -0.03916634215793097i }, { -0.0434098499882848 + -0.28375389504441684i, -0.16416521344668064 + 0.9437420699879405i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.08028992921569911 + -0.6483224893799875i, 0.7561272972846668 + 0.0387683805653417i }, { -0.6793927562712659 + 0.33415110318059177i, 0.3297361996021189 + 0.5639526235786809i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 11 }, QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.5313189795557385 + 0.2566419321430157i, 0.43775574393072764 + -0.678384086845648i }, { -0.47045491092929637 + -0.6561305033392798i, -0.5432506524357213 + -0.23031210996908946i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 11 }, QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 5 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.332331480527446 + -0.45443752368533896i, -0.2569221208457006 + -0.7855147025596146i }, { 0.11951831591337828 + 0.8177760673181887i, -0.4998522340466753 + -0.25904713857013567i } }) }, { QubitIndex{ 6 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.28953851372832196 + -0.8729034642505441i, -0.009802364956379843 + -0.3925696177860116i }, { 0.21239972582752994 + -0.3302928210479051i, -0.7010268154462935 + 0.5952767531643033i } }) }, { QubitIndex{ 6 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.31470568293926093 + -0.6271218099802567i, -0.4187395356692068 + -0.5764857065360401i }, { 0.19750725864312313 + 0.6845943699418647i, -0.6881382786216828 + -0.13706619175700202i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.331388714731627 + 0.7077801999684618i, 0.22393090023229645 + -0.5823089044504451i }, { 0.5064824006674252 + -0.3642859949205929i, 0.768683028182559 + 0.1410591858156242i } }) }, { QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.1317245105906697 + 0.46566055491652664i, 0.011143678382267319 + -0.8750341246697719i }, { 0.05034245766415807 + -0.8736558463495873i, 0.1641051923328244 + -0.4552588109470542i } }) }, { QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 7 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 9 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.6214021621466796 + 0.1016747890901297i, -0.6930849510465956 + 0.35093423995906625i }, { 0.6158463646107858 + 0.4735555356434857i, 0.6294743162518747 + 0.015507902383151022i } }) }, { QubitIndex{ 6 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.25585264028542865 + 0.21924840135780121i, -0.08649777248430789 + -0.9375434391623018i }, { 0.10898335040897113 + 0.9351963399707354i, 0.29561537558301443 + 0.1616848315640323i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 11 }, QubitIndex{ 8 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.8032335043136034 + -0.24856297427952628i, 0.27875477802603116 + -0.46403465290111845i }, { 0.3358828716146071 + 0.4245174695125384i, 0.7647898961113279 + -0.34937662978599543i } }) }, { QubitIndex{ 6 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.24518081222996058 + -0.29088244815106923i, -0.8673790455214072 + 0.32082294503708764i }, { -0.06174900302344505 + -0.922746352633002i, 0.3656911241224533 + 0.10486291559058869i } }) }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.878561974676228 + 0.2156996021511647i, 0.40527977878165283 + 0.1317225842273479i }, { -0.42115868617677543 + -0.06502229881116618i, -0.6871272673982827 + -0.5884331568775949i } }) }, { QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.6297766094146398 + -0.3041831340891446i, 0.5556335689880529 + 0.4495835630708759i }, { -0.0417158590317549 + 0.7135221301929943i, -0.19914002365391656 + 0.6704395631294531i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.5831643995050397 + 0.34780956788067396i, -0.367430583804238 + 0.6355647517962761i }, { 0.7233198727160361 + -0.12552350128419149i, -0.5276927899939096 + 0.42730847376088676i } }) }, { QubitIndex{ 11 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.21002293571496633 + 0.001281641106875646i, 0.6778678595666011 + 0.7045451645113566i }, { 0.9651610466132104 + -0.1560540866822532i, -0.16867571575287274 + -0.12513903885485064i } }) }, { QubitIndex{ 6 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 12 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.19319416517198237 + 0.6851570084993086i, 0.12622415081396907 + 0.6908714439018824i }, { -0.3207891648360635 + -0.6247641154640364i, 0.5706093728013424 + 0.4256395839480625i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 11 }, QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.49056805735528664 + 0.3070776762314352i, 0.6077222960383883 + -0.5438013357475242i }, { 0.5844093933813799 + -0.5687811027017068i, 0.27252238911655635 + 0.510573467355328i } }) }, { QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.2709702569576806 + -0.72573788584822i, 0.1814999835107169 + 0.605753577681927i }, { -0.4509731065368562 + 0.4432864740428497i, -0.11234578310022503 + 0.766484692691741i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 5 }, QubitIndex{ 9 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.5639007480952216 + 0.5349326595688756i, 0.07279776656886316 + 0.6249507830275949i }, { 0.621956518097158 + -0.09504254637097119i, -0.3667867398614992 + 0.6852769450649525i } }) }, { QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.9261434551352944 + -0.25899261808153107i, -0.25495674792981204 + 0.10088697128166191i }, { 0.2741800876580184 + 0.002530577241245779i, 0.9573491417423811 + -0.0911125486186946i } }) }, { QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.5180498561901015 + 0.7417693885513994i, -0.39897694862657307 + -0.14906346022458714i }, { 0.38195312096242734 + 0.18845247171783044i, 0.18528954223308444 + -0.8855875252224115i } }) }, { QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.14938669971466045 + 0.3033653899591985i, -0.9373328485630431 + -0.08402490778308924i }, { 0.18172451085745478 + -0.9233792591762522i, -0.333054947824681 + 0.058492286724264875i } }) }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.9547502568656417 + 0.12699844650034559i, -0.19861712883964563 + -0.18131347918173468i }, { 0.19285014374340434 + -0.18743575875386742i, -0.9582546956904143 + -0.0970803614835756i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.21261815474230206 + 0.33847596563346993i, -0.8370838857994616 + 0.3735212297813111i }, { -0.9047848933192205 + -0.1469416135197775i, -0.1189510018787876 + 0.3816059724238654i } }) }, { QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.5471287045579823 + -0.05365617263682558i, 0.4163799881952392 + -0.7241539209430573i }, { 0.575974598649204 + -0.6049995516507093i, 0.10403573342246275 + 0.5398197573121782i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.16057702368141832 + 0.7217423438944987i, 0.2180026267564577 + -0.6370069569656023i }, { 0.6644595865542308 + -0.10861153866635291i, 0.7384203290555884 + -0.03784718142932547i } }) }, { QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.7510988147648126 + 0.31392195027802666i, 0.5262435029770739 + 0.24570583055139844i }, { -0.45716341626923274 + 0.35819713904158734i, 0.6608054704556616 + -0.475428807102954i } }) }, { QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.9078367018978684 + 0.34907391010738437i, -0.19077960284940867 + 0.1326011730949785i }, { 0.13358092929841628 + 0.19009487972408434i, 0.9725076450553988 + -0.015778223577436083i } }) }, { QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.08239654366825433 + -0.5105292983450671i, -0.010564453421471387 + -0.8558382075174746i }, { -0.6511064307484118 + 0.5555457325557298i, 0.4385034242855702 + -0.2741242451280262i } }) }, { QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 10 }, QubitIndex{ 8 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.18202911945839007 + 0.13435683929085446i, -0.7543181521782305 + -0.6162935702239598i }, { -0.8822105934790379 + -0.41293838300446084i, -0.1999443667907389 + -0.10587072675520726i } }) }, { QubitIndex{ 6 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.08924247511766081 + -0.6763083480857709i, -0.36919959161737115 + -0.6311374339190601i }, { 0.1505966923841231 + 0.7155162019041434i, -0.3882992885778115 + -0.5608750872927021i } }) }, { QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 5 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.01772494966098503 + -0.555051250365079i, 0.6811636174685167 + -0.4770954431399254i }, { -0.773713427542009 + -0.3049122294511551i, 0.11232947208491567 + 0.5438549016727223i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.36579158015665003 + -0.3420067036727956i, 0.6511457574574907 + -0.5702956575974256i }, { -0.7718808393475165 + -0.3916987418636202i, 0.438962580998467 + 0.2410060537700138i } }) }, { QubitIndex{ 8 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 11 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 8 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.015907514187246674 + -0.8758279742654147i, -0.1951410298380358 + 0.441126159913775i }, { -0.46792432372943427 + 0.11712872298831789i, -0.8657323291191936 + -0.13354858228874655i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.567087164920168 + -0.2521490058022102i, -0.7404225001109269 + 0.2580843807463891i }, { -0.7280185309080051 + -0.2912422443779626i, -0.555127771552959 + -0.2774889745465262i } }) }, { QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.23415012361679 + -0.8123661995460343i, -0.20740430385945577 + -0.49215681666106137i }, { -0.4829186411411241 + 0.22808871844917059i, 0.7165433174298657 + -0.4486990046804523i } }) }, { QubitIndex{ 9 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 6 }, QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.3370060676727469 + -0.2074899390751357i, -0.6600185963028775 + 0.6385532773925191i }, { -0.9161973046866507 + 0.06290734789565505i, 0.071436232200429 + -0.3892583065197941i } }) }, { QubitIndex{ 8 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.6991819082100819 + 0.25137632843717406i, 0.4918715753426195 + -0.4538909055082778i }, { -0.6261524307830306 + -0.23640586743491904i, 0.5539636924056607 + -0.49514606608871703i } }) }, { QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 7 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.6611576679923159 + 0.6451465068475309i, -0.23822944312884384 + 0.29983871528525474i }, { 0.37953134397609495 + -0.051112441705422965i, -0.8926877939191541 + 0.23759625382224037i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 9 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 10 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.21914413073631311 + 0.0549459458952326i, -0.432969452850274 + -0.8726363766724591i }, { -0.8989097745827777 + -0.37539047690777044i, -0.06906023774322922 + -0.2151136689504991i } }) }, { QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.1827328653582163 + 0.7199201734682219i, -0.25180738667208546 + -0.6204165405345317i }, { -0.562160388050043 + 0.3637297648789365i, -0.2633111118004963 + 0.6945096217122735i } }) }, { QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.5914805683722788 + -0.6187089735810515i, 0.3267875049535159 + -0.40069922617163084i }, { 0.3975365634661863 + -0.3306276212837983i, -0.6129895324584926 + 0.5974059673689498i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 9 }, QubitIndex{ 12 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.9416940781000123 + 0.15414129161788975i, -0.20120923593638154 + -0.22128616961670777i }, { -0.27722787852986197 + -0.11223826823133413i, -0.4703814465176118 + -0.8302340448826839i } }) }, { QubitIndex{ 9 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.022174193400898542 + -0.28246547594060206i, 0.8060916855510906 + -0.5195553430906762i }, { -0.9590191515350756 + -0.0019562815779365357i, 0.13388003048304753 + 0.2497089853993837i } }) }, { QubitIndex{ 11 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 7 }, QubitIndex{ 11 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.631485739973072 + -0.6721041608611376i, -0.3853939186559586 + -0.03119750994570479i }, { -0.35277391858464346 + -0.15827924541847957i, 0.8771257908273714 + -0.28486591565445446i } }) }, { QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.9220080669854607 + 0.08999766632283532i, -0.2541438999018505 + 0.2778712338707553i }, { -0.3608746025898813 + 0.10756888804748835i, 0.4829781221984061 + -0.790525514457012i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 5 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.7812065500654899 + -0.2759539407326053i, 0.5314292604054054 + -0.17648991448206008i }, { -0.22467035050424972 + -0.5129220041421683i, -0.7279821908783383 + -0.39557070295340857i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 11 }, QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.6690527789337148 + 0.4401005814195617i, 0.3337179004821367 + 0.49730495687558957i }, { -0.34577656502728243 + 0.4889973663662932i, -0.6580929418504177 + -0.4563264430771307i } }) }, { QubitIndex{ 7 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.06853379325872222 + -0.6612056275217723i, 0.3191902112078297 + 0.675446405264112i }, { -0.5921650333888099 + -0.45546768331764187i, -0.6315286752135227 + -0.20751215643412996i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 10 }, QubitIndex{ 11 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 10 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.06220705435387708 + -0.25565532951035186i, 0.8481092906695071 + -0.4598709231530439i }, { -0.880669702693962 + -0.3939435361044725i, 0.08149701644382079 + 0.2501751415092802i } }) }, { QubitIndex{ 1 } }));


    return c;
}

TEST(RandomizedIntegrationTests, 13_200_1_0_50_50) {
    auto c = randomCircuit();
    
    c.execute();


}
}

}
