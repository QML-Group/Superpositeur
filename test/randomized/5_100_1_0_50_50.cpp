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
    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.2672937787456364 + -0.22693368661440697i, 0.27593532331132803 + -0.8949384532315866i }, { 0.9338903778848031 + 0.070027850300092i, -0.11361771360118082 + -0.33171656189067095i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.022559616629325596 + 0.3918069230048383i, 0.1848003139105137 + -0.901014563013048i }, { -0.9133998653370892 + -0.10806981440349503i, -0.36481104098638056 + -0.14468761381477935i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.16451038292627618 + 0.5368526542717605i, -0.17877091187226446 + -0.8079396775621153i }, { -0.2272099073315259 + -0.7956765797239573i, -0.1107601107123265 + -0.5504603858264259i } }) }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.8697191391032992 + 0.19866855818796908i, 0.2292234789844519 + 0.3893276508895741i }, { 0.10658253564852305 + -0.43904394559048504i, -0.7748459181809759 + 0.4421474641037208i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.9025481562805171 + 0.21978196680771603i, -0.18569483008157125 + -0.32034378836142174i }, { 0.3546159598530027 + 0.10653747546429193i, 0.42264265214221197 + 0.8272064288491876i } }) }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.11146271540428598 + -0.7972081858841593i, 0.35278916286424744 + -0.4770481925335579i }, { 0.5831281052879533 + -0.10952983272731037i, -0.7660559119686529 + -0.24723100190689i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.21851516476225985 + 0.3752980440511666i, -0.8890945703818106 + -0.14461447305810865i }, { -0.004678891144201253 + 0.9007666783790084i, 0.33391089402485946 + 0.27767069336036887i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.364018378089096 + -0.8900241581420535i, 0.1484379452281909 + 0.23089780153424092i }, { 0.05852177596190722 + 0.2681843024399031i, 0.8870382634381543 + 0.37123510186903186i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.019245775501493867 + -0.30661050207195933i, -0.2676121487538724 + 0.9132378321028223i }, { -0.8665729966323599 + -0.3932821400116452i, -0.21974156165509662 + -0.2146952396697519i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.27893099377612574 + 0.5555309732371624i, 0.764242207842956 + -0.17180420901874954i }, { -0.6412801996500226 + 0.44982501489141435i, -0.22333950877816156 + -0.5801177685035129i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.9401533062715273 + 0.302996120089871i, 0.13784182531294387 + -0.07283366743200956i }, { -0.011491960705611794 + 0.15547683671962642i, -0.6543544467918293 + -0.739942664026449i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.5921736392230323 + 0.3278577071580445i, -0.19096562527960612 + 0.7108950941088208i }, { 0.41439472341228045 + -0.6083721871309369i, -0.6347276044321263 + -0.23511946602830233i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.6663318656724357 + 0.5785606799179231i, 0.17226774150200927 + -0.43771361605529086i }, { -0.46326508480484346 + -0.08157723728567168i, -0.14560716086877143 + -0.8703615169924128i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.05913906406035796 + -0.4013764245248514i, 0.2090611621718846 + 0.8897713006211495i }, { -0.03234443943463189 + 0.913429457689739i, 0.019908852203742052 + 0.40522105160790717i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.10820963237237624 + 0.15933297882366762i, -0.43080065219590497 + -0.8816543967953631i }, { -0.936589580621598 + -0.29275183140686123i, -0.1411692642535206 + 0.13102504153446282i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.2887854480186516 + 0.3684145908955971i, -0.6600739204616083 + 0.5875168710380487i }, { 0.4910851335989869 + 0.7346489268936297i, -0.1868399781100566 + -0.42920527530767494i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.30373551239730756 + -0.07347025393979059i, 0.9135315129874664 + 0.26039783999411015i }, { 0.9464737703838554 + 0.08083478378851902i, 0.31003534732968985 + 0.0391308460346938i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.08943555715048834 + 0.16807912581858242i, -0.26894314379029227 + -0.9441505568443186i }, { -0.9047400055007062 + -0.3810462058961155i, -0.15604213178247894 + -0.10908787525447838i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.6574725127257304 + -0.013907437046114376i, 0.07326142302378712 + -0.7497794623097198i }, { -0.4410017601100202 + -0.6107814058931289i, 0.5749561027962709 + -0.3192005664976196i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.23849689734525725 + 0.45424927158758166i, -0.3700183393996055 + 0.7745083974540211i }, { 0.0031941557354845385 + -0.8583511091552075i, 0.017474747542406188 + 0.512755111119813i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.35512834273060156 + 0.8783075155178632i, -0.277532564851665 + 0.15948493289684512i }, { -0.11492584844405453 + 0.2987504271690151i, 0.9224595502797601 + 0.21589027241496625i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.7244808562482954 + 0.0827676541749934i, -0.613437783792238 + 0.30326735691164075i }, { 0.6839538032195338 + 0.02200453166146793i, -0.6937145907856022 + 0.22468436122493896i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.5327011375435462 + -0.4191206284672098i, -0.42333431668802757 + 0.601128483077739i }, { -0.7204638259359416 + 0.14662630177020805i, -0.17021707734698438 + -0.6560935525718782i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.1840663687551176 + 0.5656886177509657i, -0.7054218464099937 + 0.38535175910824904i }, { 0.2491923492632421 + 0.7642114450261609i, 0.5218856788774081 + -0.285515986491755i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.4422134768686812 + 0.3949285150159412i, 0.6182733881689705 + 0.5159619427681439i }, { -0.7533765049193663 + 0.2844337369907024i, 0.5473433726557555 + -0.22789586110064988i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.20936844928571863 + -0.4282208467773577i, -0.7111092633187283 + -0.5168320563309436i }, { 0.8788899534132382 + 0.018552860109755598i, -0.09228750125344387 + 0.4676443715938635i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.8435412051429036 + -0.36993898880941795i, 0.04325008609084232 + 0.38692739608078636i }, { -0.335657630677689 + 0.19727476968067437i, 0.20942090749328657 + 0.8969724096759975i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.4990656758223446 + 0.7075294078721239i, 0.4473327266713878 + -0.2241183166562913i }, { -0.363572365344817 + -0.3437305970810326i, -0.2735280998102373 + -0.8214905905742882i } }) }, { QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.9538918710238617 + -0.08446691084506806i, 0.2578112597177066 + -0.12840947679068476i }, { 0.22389263030256804 + 0.18118424176259315i, -0.912975592352472 + -0.28899814601786666i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.09755934066727878 + 0.5977381323082976i, 0.7429378339891081 + -0.2850169732853232i }, { 0.10285108893901479 + 0.78905827017858i, -0.47930096775666375 + 0.37024219380612444i } }) }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.839346509868421 + 0.22436399023275463i, 0.41893258623449153 + 0.2639199205240431i }, { 0.4006193084700522 + -0.29096804968847384i, 0.8523135150372733 + 0.16853318909422657i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.2577399191434295 + -0.1375016068071352i, 0.8956283971125589 + -0.33542989802786394i }, { -0.34512760727800423 + 0.8919363076475549i, 0.14028995829506677 + -0.25623287337141976i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.11182178229361783 + 0.2446844574045521i, 0.10999233432259864 + -0.9568317990637563i }, { -0.941259050773741 + -0.2040999868834759i, -0.26766295536945556 + 0.02703954536544717i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.3955614854675489 + -0.8137584294328364i, -0.40759759804111273 + 0.12325797261364016i }, { -0.11015195696946305 + 0.4113330476846069i, -0.8259994378674687 + -0.3693191017276464i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.6617278158582043 + -0.5851780952347966i, 0.17254920214542457 + 0.43578626345478494i }, { -0.025586389097748602 + -0.46800452056560776i, 0.3562301479751284 + -0.8083422462649792i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.16339006867222006 + -0.8802487102988815i, -0.4399186941442801 + -0.07026689134175637i }, { -0.4409347824088392 + -0.06357995862307902i, 0.4174879669819346 + -0.7919835250488789i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.4201458385631142 + -0.8068452180541595i, 0.4029185215453618 + -0.10067240653194448i }, { -0.16669962470823843 + 0.3803807350055542i, 0.7251285584066622 + 0.5492816266195716i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.09167011231526079 + 0.19096562753204005i, 0.4590393873222984 + 0.862792883892867i }, { -0.9417100292720383 + 0.2613636171655472i, 0.20159332358249293 + -0.06504930651100477i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.19595618499103362 + 0.464984205917223i, -0.2631862595021134 + -0.8222675079440851i }, { 0.1297866272418374 + 0.8535492330267057i, 0.38223640890644894 + 0.32940016073920275i } }) }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.9165916122461921 + 0.02039773329642014i, 0.3643782159540803 + -0.16331645530814098i }, { 0.3977447301635287 + 0.035254481465994754i, 0.8729194679175734 + -0.28029957847668346i } }) }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.3245246444849698 + -0.2074357607042042i, 0.5542767194857351 + 0.7378560012218536i }, { -0.9216098933081159 + -0.04784730775680989i, -0.3532884828659172 + -0.1534049789652429i } }) }, { QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.03943692289403189 + -0.2580302590311836i, 0.8008099746247852 + 0.5390440604240083i }, { -0.5525081724711103 + 0.7915805921632898i, 0.056276401033995116 + 0.2548879207606951i } }) }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.3484109358523557 + 0.5050719105334588i, 0.6656259279284442 + -0.4247991396389185i }, { 0.5725770900678362 + -0.5437533088616717i, 0.5741054270796379 + -0.2165427755182316i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.04712286123417897 + 0.5979935448041238i, -0.7955100633915902 + 0.08571403248249586i }, { 0.7918840075939239 + -0.11446778952477799i, -0.19410912609971806 + -0.5675724542673836i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.3030383482223864 + 0.8088847271621408i, 0.4525780325230213 + -0.221464178024501i }, { 0.497227264735966 + 0.0814757931760505i, 0.5235204355598383 + 0.6870612024272361i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.4117907205301228 + -0.5521396127222352i, -0.21934085921363755 + -0.6909846872598884i }, { 0.6454038290101443 + 0.33018804952218794i, -0.6124396278776145 + 0.31519430777998925i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.08376965228334488 + -0.6771577561702238i, -0.5721756462821213 + -0.455033019039797i }, { -0.43667028593395096 + -0.586309713374422i, 0.674162641694491 + 0.10518894394692868i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.38599486569413277 + -0.7281132550344258i, 0.38266047350446175 + -0.4176481934814067i }, { 0.45431549238991986 + -0.3383141807188196i, -0.6851958643029945 + -0.4578728819673978i } }) }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 1 }, QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.04644554085215935 + -0.35036872898357985i, -0.41329337350118356 + 0.8392098384226633i }, { -0.35736341191345933 + 0.8645090845739867i, -0.23246354908656153 + 0.2662257178799572i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.4461775274256729 + -0.6593407519629412i, 0.5120511477177772 + 0.3224887733583348i }, { -0.3776821014320009 + 0.472812454445908i, 0.638024666154175 + 0.4761608326580395i } }) }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.036144982579118246 + 0.41096176542543866i, -0.8648044224389034 + 0.2862119468556073i }, { 0.15843345430182904 + 0.8970522884150914i, 0.3445946451755003 + -0.2268271653122903i } }) }, { QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 0 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 3 }, QubitIndex{ 4 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.34569539678476 + 0.29476716484615056i, -0.7769038314244127 + 0.435921378104421i }, { -0.8854333310986768 + -0.09805522602029243i, 0.43333541212501303 + 0.13643096946095282i } }) }, { QubitIndex{ 2 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.8707297050405625 + -0.03387893172396902i, -0.4741207351456138 + -0.12606160101395095i }, { -0.33693343101368667 + 0.35659201030107657i, -0.7583738860563761 + 0.4291701879237801i } }) }, { QubitIndex{ 3 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 4 }, QubitIndex{ 1 } }));


    return c;
}

TEST(RandomizedIntegrationTests, 5_100_1_0_50_50) {
    auto c = randomCircuit();
    
    c.execute();


}
}

}
