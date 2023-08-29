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
    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.09214344620813697 + 0.03429238785526139i, 0.0552682634699607 + 0.05582599589074772i, -0.18234232247888643 + -0.23776699858848196i, -0.07143994839352227 + 0.035971975437367056i }, { 0.06434153456218619 + -0.027577764046451028i, -0.11742493124932775 + 0.036355041095735355i, -0.022591458313247367 + -0.1021893380136644i, -0.01546535406517127 + 0.016903449273602624i }, { -0.14000049869015022 + 0.0088515649989482i, 0.013169115953020892 + 0.11037121750862461i, 0.037511904369659524 + -0.22568470608842528i, 0.11710417645187594 + 0.10147895455775928i }, { 0.09314889292457394 + -0.008890883798977284i, 0.010207574384446704 + -0.036782497707730934i, 0.021813379654491528 + 0.03701692246936263i, -0.009571165920667486 + 0.015052016199245693i } }), Matrix({ { -0.1361982719359829 + -0.007966427054722013i, -0.021529519020202628 + -0.133518196510849i, -0.05889388419418765 + 0.03836772460937351i, 0.05901424479101121 + -0.01172732152763002i }, { 0.10057624173094766 + -0.03099054324914009i, 0.07734177163924172 + 0.1263708300978721i, 0.14613422380695598 + -0.0004184917671206654i, -0.01771486518746363 + 0.051979025974255605i }, { -0.19867957511749776 + -0.007526357696069376i, 0.10158244979644772 + 0.047856753888225835i, 0.010712033152573014 + -0.06060328705769719i, -0.16020926356733522 + 0.11483343657733135i }, { -0.12567117041548317 + 0.07131211893465642i, -0.11468956977651015 + 0.01721340254988003i, 0.013976222443900723 + 0.1841041932066995i, 0.01911173017581089 + 0.028987780750801658i } }), Matrix({ { 0.04015987746600675 + 0.005339589137846026i, 0.06766370951434268 + -0.11435818349123768i, -0.002215615316687534 + 0.16017348598888842i, 0.13251944871215032 + 0.1389061014671625i }, { 0.027444470230815143 + -0.01590216413341597i, 0.17854816244058086 + -0.10434944752400645i, 0.014516238245693733 + 0.0060465137680071526i, 0.04646676570007869 + 0.032587532671691635i }, { 0.043094569902490956 + 0.15425349688053613i, -0.09232918399699114 + 0.10791136415806106i, -0.09672289237875184 + -0.07675979746446075i, 0.021221307363717162 + 0.019146158499858533i }, { -0.017769327078608697 + 0.004498881576632478i, -0.15862853172265373 + -0.03989080677266825i, 0.03365498464943584 + 0.12101649956048316i, 0.15944070337035826 + 0.05920670943968642i } }), Matrix({ { -0.026599674050416553 + 0.014744665308319494i, -0.036925951799101624 + -0.02618823628961125i, 0.02005419189456438 + -0.12550942903272727i, 0.1347580768566878 + 0.03591352194578108i }, { 0.045857076924100085 + 0.0740371298980245i, 0.06241841032005151 + -0.014219396882432204i, -0.12548271635518232 + -0.05235593906579013i, 0.03502409863831043 + -0.05038577240349188i }, { -0.02918868608910061 + -0.11824711612784479i, 0.0007442063262755789 + -0.08645199672027727i, 0.015689828661211783 + -0.04860944483389646i, 0.07481375515533581 + 0.06617217694397916i }, { -0.06648230293094905 + -0.10331168860574884i, 0.024445573062128963 + 0.14992381445525027i, 0.16317819048813348 + -0.06888216277230935i, -0.056609667977098085 + 0.00027609902838671954i } }), Matrix({ { 0.05090568350267205 + 0.03110276204257551i, 0.07056559337513095 + 0.0474831007976356i, -0.08001026867334685 + 0.05799622716128267i, -0.11461065203797 + 0.08600012798889164i }, { -0.05479446011523621 + -0.024191057742987535i, 0.01285414348057934 + 0.12025494020319838i, 0.01070877875943999 + 0.05154427581829478i, 0.12993560886191066 + 0.04145052172495954i }, { -0.10093630128821662 + 0.007689131650783782i, 0.09504817052953546 + 0.06114161531096241i, 0.028309966937896683 + 0.011389412897362925i, -0.06191749360283395 + 0.10206201521136521i }, { -0.07974162814149323 + -0.11948160279991463i, -0.09921628373094617 + 0.023240978643851815i, -0.028439224679483517 + -0.07280293399913072i, -0.14172189960819828 + -0.0590422480987743i } }), Matrix({ { 0.06552455579220977 + 0.0463198612964734i, -0.08276980004361724 + -0.021163672147971813i, -0.11155812617481223 + 0.029681047003140892i, -0.053063658147298264 + -0.08070004603943157i }, { -0.0002203799228137019 + 0.25264748060692716i, -0.06014816448525449 + 0.12206432867433646i, -0.053947660072260276 + 0.0043028012255724i, 0.21431299124402983 + -0.08062632470692752i }, { -0.18843300101018165 + 0.055740069468708166i, 0.004758481514047677 + -0.08814230114671147i, 0.009117646829076467 + 0.1023176631715558i, -0.05854824344187338 + -0.044541253168484236i }, { -0.007470988527149802 + 0.03276610165455261i, -0.009649134595370635 + -0.1415493260832984i, 0.0324574311337607 + -0.048382105779216575i, 0.15039088421888883 + -0.008992049998491418i } }), Matrix({ { 0.13228795841185031 + -0.008857945514397465i, -0.053189108530725386 + -0.03425024931952936i, 0.10098020264868221 + -0.028235779737975055i, 0.03371861358539892 + -0.0670569064574452i }, { -0.036783517419527754 + -0.1753200766240194i, 0.11192933766936973 + -0.17723126663002878i, -0.11986928352754612 + 0.039471346935732995i, 0.01869712094908103 + -0.16491321479601936i }, { 0.1425470145640187 + -0.001674115689442532i, 0.031024072115953597 + 0.06881984669601085i, 0.027876438283547276 + -0.046928362956332265i, 0.08654983346682445 + 0.010558082009891428i }, { 0.060119142172932044 + 0.07447228840083492i, 0.04110528966685312 + 0.07677541710647856i, 0.21928992156971888 + -0.06394476124240477i, -0.03967969599060992 + 0.037348357162100485i } }), Matrix({ { -0.04180041897329324 + 0.08422681600867493i, -0.11156189934735862 + 0.041217228199782484i, -0.11037606468013576 + -0.06238555110661711i, 0.013051623264089823 + -0.0756926052697527i }, { 0.0447036375173119 + 0.044687582155360575i, -0.027972147918404973 + 0.21219834652985994i, -0.06055779496868699 + -0.007827263870915164i, 0.08022157946969578 + -0.06885890040060183i }, { -0.07509974335570116 + 0.095230101686679i, 0.03967912543621501 + 0.026334042218061564i, 0.0048841988917341745 + -0.1203423261572519i, -0.06019051986565331 + 0.028372939894490797i }, { 0.01130992024723618 + 0.026291199539940188i, 0.14426928709403689 + -0.11382196179925215i, 0.034517417266550915 + 0.22656475661746991i, 0.12983746889734707 + 0.11390534119543476i } }), Matrix({ { -0.12120002775435113 + -0.04031246051394848i, 0.034263159590411575 + -0.049669415746494354i, 0.007240777242224018 + -0.05531452194771278i, -0.07660491053286134 + -0.07110035890440142i }, { 0.11361444034121167 + -0.15883101810170575i, 0.05541435731309765 + -0.12868363561241933i, -0.05491635515370083 + -0.016841065239759992i, -0.05868753851023051 + 0.06689893879082684i }, { 0.07502621327937453 + 0.03892794563257865i, 0.0009447778355477393 + -0.03854206098529299i, 0.1215918210478492 + 0.09438839626901402i, 0.07945490291041642 + 0.08479051148644588i }, { -0.07228105447111269 + 0.07288624941264087i, 0.09461207728138842 + 0.042878736645431466i, 0.003806508619634981 + 0.010218011815788167i, -0.048633751413432486 + 0.14209881271150593i } }), Matrix({ { 0.0838301711081018 + 0.019386485237031775i, 0.06382259233445081 + -0.1494037590794174i, -0.1309448580382491 + -0.10432635398386841i, 0.05995811871085426 + -0.13048630081007687i }, { -0.08451966853915126 + 0.17538416166453344i, -0.07369169824364337 + 0.030651166001714553i, -0.0011625474366853113 + 0.11142839724916771i, -0.061554939824104975 + -0.07418687110535474i }, { 0.017890295580817818 + -0.1336368106151552i, -0.05862094500309309 + 0.022447452295199306i, -0.05521903719877618 + -0.06686802395589886i, -0.03118240830599207 + 0.045724273529029025i }, { -0.042140169238482295 + -0.013210052952758154i, 0.08918517466788413 + -0.07837432664474347i, 0.0904991645717522 + -0.08998649524333634i, -0.07044224864944543 + -0.03565188484022687i } }), Matrix({ { 0.06858071547046922 + 0.00957450387260343i, 0.05536638884942658 + 0.08682954814712275i, 0.00378455846520501 + -0.0357814744767812i, -0.1437454143809595 + 0.10556709011075036i }, { 0.10760438986449905 + -0.026500462297428144i, -0.054485698333543095 + -0.08598271791719707i, -0.06489124983854849 + -0.17554144632205096i, -0.17721556229204027 + 0.17855682314236687i }, { 0.028754507389375947 + -0.1739579895507113i, 0.035773231000927906 + 0.08201179689234149i, 0.06337758395105819 + 0.07504504039170641i, -0.026017766976391844 + 0.04560525500590265i }, { -0.0008960451908436575 + 0.061489721619222434i, 0.17993163655143893 + -0.0525838370340857i, -0.009794781395475951 + 0.07402530121795722i, -0.04706300641105031 + -0.1096451864379448i } }), Matrix({ { -0.06720750219627067 + -0.0507414362693286i, -0.1007577437734891 + -0.17108818827494718i, -0.06366037946134617 + -0.00963172604785614i, -0.12306383268973727 + -0.017579660081842766i }, { 0.07098900258598145 + -0.004295747581453907i, -0.17139358407940275 + -0.0009748344659343127i, 0.08463894545854607 + 0.11434456997059043i, -0.020686305572706214 + 0.06409279622161933i }, { 0.1475966424844475 + -0.06624255092337489i, -0.041241978763809015 + -0.05897669899147721i, 0.059624593620680046 + -0.059356097183266274i, -0.01698690088075183 + -0.009708378816879278i }, { 0.14189130114005274 + -0.18342164084553808i, 0.10392229682657733 + 0.04329509152343364i, 0.1590028859507411 + 0.08116519421276701i, 0.07218861201080432 + -0.11995227064800372i } }), Matrix({ { 0.08324759029949601 + 0.08197429435681883i, -0.032424145060574505 + -0.1309946863800901i, -0.00864866004497943 + 0.03301177730035026i, 0.11381077346846911 + -0.017348175534800083i }, { 0.045712257776584025 + 0.01712616066592971i, -0.03198817688808289 + 0.02116739651514435i, -0.06407669528698459 + -0.03404015970890644i, -0.018171592221959125 + -0.07226328905038792i }, { 0.027491684178141712 + 0.07540895294024423i, 0.0667691503849241 + -0.061909913707719935i, -0.21168582601447777 + 0.09719178968691951i, -0.0670419001630382 + -0.19266858251537772i }, { -0.08860388180665901 + 0.02054002379984051i, -0.033630197034097586 + 0.013966066172382285i, 0.044216197594132874 + -0.11547096504005568i, 0.05390116434854536 + -0.07475167462617689i } }), Matrix({ { 0.1422676075181468 + -0.12972768064871468i, -0.12232020151249477 + -0.04426765456291397i, -0.03487942549177259 + -0.015589527031278208i, 0.2085029830347274 + -0.046708755033373135i }, { -0.0655366029001904 + -0.02432082778970956i, 0.003305297057928883 + -0.1180474119703374i, 0.023609643532068413 + -0.06270018101536728i, 0.016258066041910754 + 0.04372111450762614i }, { -0.05037474466521934 + -0.1840346124154725i, -0.09911166002476055 + 0.07145715028504646i, -0.04639988605805171 + -0.06952916252804194i, 0.09852673267586971 + 0.18603406528825803i }, { 0.11717365232118596 + -0.080825804002087i, -0.16164877264559516 + 0.039625769487453846i, -0.062318192047051045 + -0.004824157713813241i, -0.07223678985196354 + 0.12405843727756254i } }), Matrix({ { -0.13264108660918883 + -0.15308350613375138i, 0.05289146593851686 + 0.022117758227800852i, 0.005709373948726007 + 0.0823163623601161i, -0.03848971836566079 + -0.18399071903101885i }, { 0.13742606743016736 + 0.07633028511240965i, -0.09885521380170868 + -0.025550406728147054i, 0.08496035462234486 + -0.08901657452640842i, 0.001320013188985411 + 0.06060437236047776i }, { 0.06515722225871699 + 0.014032158461395775i, 0.10384776455710591 + 0.05115217123035414i, 0.08125316940482147 + 0.04194313942740957i, 0.06840933521450483 + 0.12122158011472449i }, { 0.07692392946490709 + 0.0626827574448351i, 0.05875603147847285 + 0.03612552557497557i, -0.006263805311881158 + 0.11284460809555814i, -0.01113152904672673 + -0.026358373239255394i } }), Matrix({ { 0.04139134612221386 + 0.01699502530258434i, -0.03428713249331761 + 0.0505169660396497i, -0.02884904766186121 + -0.05006662928185531i, -0.010050535823847106 + 0.017351838135003374i }, { 0.10319793625023281 + -0.034655879313779864i, -0.1804603208056867 + -0.06942176408806645i, -0.2213554257839794 + -0.08795227754652027i, 0.08648390445487744 + 0.037020820213655284i }, { -0.13560365249902342 + 0.11299547306372468i, 0.001980836088716925 + -0.14402226791157863i, 0.16779823421196807 + 0.03184127933292213i, 0.15429306812584934 + -0.13463870609867773i }, { 0.08091370818992166 + 0.04124982858508863i, 0.1085461861286282 + -0.23057551272098215i, -0.020363634013873203 + 0.041768797888663514i, -0.1232332475941225 + 0.02246479665637648i } }) }, { QubitIndex{ 2 }, QubitIndex{ 0 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { 0.5571278930730994 + 0.12882927665488167i, 0.5588605144089851 + -0.1862059311622116i, -0.17840291607419967 + 0.019787456707971537i, -0.4483399563161113 + 0.3046078536984343i }, { -0.0986753371858764 + 0.11490444790082073i, 0.0638882999185104 + 0.3055754705876842i, -0.25599298599448717 + 0.8802129420813947i, 0.11401096925094654 + 0.16216142943684356i }, { 0.5016607725948842 + 0.29949837275730357i, -0.623315779686135 + -0.2614186362376126i, 0.01806895597824383 + 0.24284759214712942i, -0.1806079716713107 + -0.33144267945473765i }, { -0.2660108194829064 + -0.48779822856754407i, 0.18471059324179492 + -0.254094324433609i, -0.050971983790604386 + 0.25607428706127855i, -0.4748474660476721 + -0.5467695159040082i } }) }, { QubitIndex{ 0 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ default_operations::CNOT }, { QubitIndex{ 2 }, QubitIndex{ 1 } }));

    c.addInstruction(CircuitInstruction({ Matrix({ { -0.09978692993403526 + 0.1306092079945748i, -0.14937380537222186 + 0.11892472448712654i }, { 0.06442807203109377 + 0.12394392650392749i, 0.27307864595662823 + -0.14398682237381405i } }), Matrix({ { -0.06742488924889194 + -0.45932708432520797i, 0.30973451602513524 + 0.11714443368620714i }, { -0.031601401802604304 + 0.2174430987329341i, -0.34303719196947147 + 0.09651267641226363i } }), Matrix({ { -0.4345257591404085 + 0.6248856803670065i, 0.135940630539385 + -0.062016149668917636i }, { 0.06828712005527426 + -0.15517939721353785i, 0.06282518180576654 + -0.42584774162323424i } }), Matrix({ { -0.28354235442786424 + 0.02028841235635912i, -0.09881200509442035 + 0.28163455243839874i }, { -0.025231876284722378 + 0.013417718530046701i, 0.052899214316718644 + 0.5762754575868219i } }) }, { QubitIndex{ 2 } }));


    return c;
}

TEST(RandomizedIntegrationTests, 3_5_2_100_30_20) {
    auto c = randomCircuit();
    
    auto s = c.execute();

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, false}), Matrix({ { 0.5930773310708685 + -1.9949319973733282e-17i, 0.2567177886685251 + -0.09681065492847357i }, { 0.2567177886685251 + 0.09681065492847357i, 0.4069226689291315 + 1.431146867680866e-17i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, false}), ElementsAreArray({ DoubleNear(0.5930773310708685, 1.0e-7), DoubleNear(0.4069226689291315, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, false}), Matrix({ { 0.44490789668012365 + 5.204170427930422e-18i, -0.13473364452911313 + 0.07294879211102161i }, { -0.13473364452911313 + -0.07294879211102162i, 0.5550921033198762 + -1.0842021724855044e-17i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, false}), ElementsAreArray({ DoubleNear(0.44490789668012365, 1.0e-7), DoubleNear(0.5550921033198762, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, false, true}), Matrix({ { 0.5505575578938601 + -1.035941122002391e-17i, -0.07292623715996467 + 0.06697382286946896i }, { -0.07292623715996467 + -0.06697382286946896i, 0.4494424421061398 + 4.7215599230992866e-18i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, false, true}), ElementsAreArray({ DoubleNear(0.5505575578938601, 1.0e-7), DoubleNear(0.4494424421061398, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, true, false}), Matrix({ { 0.2685028605292963 + -8.239936510889834e-18i, 0.07434350094171252 + -0.058919229683369274i, -0.006628171591973208 + 0.07890599676709034i, 0.004166238503606472 + 0.15421674415230716i }, { 0.0743435009417125 + 0.05891922968336928i, 0.1764050361508274 + 1.3444106938820257e-17i, -0.030071888310627112 + -0.0928135561530094i, -0.12810547293713992 + -0.005957204656068738i }, { -0.006628171591973205 + -0.07890599676709034i, -0.03007188831062709 + 0.0928135561530094i, 0.3245744705415722 + -1.1709383462843448e-17i, 0.18237428772681258 + -0.03789142524510429i }, { 0.004166238503606465 + -0.15421674415230716i, -0.12810547293713992 + 0.005957204656068724i, 0.18237428772681258 + 0.0378914252451043i, 0.2305176327783041 + 8.673617379884037e-19i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, true, false}), ElementsAreArray({ DoubleNear(0.2685028605292963, 1.0e-7), DoubleNear(0.1764050361508274, 1.0e-7), DoubleNear(0.3245744705415722, 1.0e-7), DoubleNear(0.2305176327783041, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({true, false, true}), Matrix({ { 0.33845466797926405 + -1.4041661612344866e-17i, 0.15216034762848152 + -0.03456882949587259i, -0.051578518755107786 + 0.04847697736605938i, -0.021090373315410683 + 0.03171152581026293i }, { 0.15216034762848152 + 0.03456882949587259i, 0.21210288991459608 + 3.6822503923209575e-18i, -0.015655704249667952 + 0.02478878750000154i, -0.021347718404856893 + 0.01849684550340957i }, { -0.05157851875510778 + -0.04847697736605938i, -0.015655704249667952 + -0.024788787500001543i, 0.2546226630916044 + -5.907658361388416e-18i, 0.1045574410400436 + -0.062241825432600976i }, { -0.02109037331541068 + -0.03171152581026293i, -0.02134771840485689 + -0.01849684550340957i, 0.10455744104004358 + 0.06224182543260099i, 0.1948197790145354 + 1.0629218284487703e-17i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({true, false, true}), ElementsAreArray({ DoubleNear(0.33845466797926405, 1.0e-7), DoubleNear(0.21210288991459608, 1.0e-7), DoubleNear(0.2546226630916044, 1.0e-7), DoubleNear(0.1948197790145354, 1.0e-7) }));

    EXPECT_EQ(s.getReducedDensityMatrix({false, true, true}), Matrix({ { 0.21604379393512083 + 4.728245776637489e-19i, -0.06126694583770273 + 0.06619335226870254i, -0.009639985054857124 + 0.02716574190934162i, -0.002872773803913961 + -0.01083982256277265i }, { -0.061266945837702744 + -0.06619335226870254i, 0.3345137639587393 + -1.0832235797687658e-17i, 0.0196706392238378 + 0.016752185521114096i, -0.06328625210510755 + 0.039808080960127334i }, { -0.009639985054857123 + -0.027165741909341623i, 0.019670639223837796 + -0.0167521855211141i, 0.22886410274500285 + 4.731345850266673e-18i, -0.07346669869141038 + 0.006755439842319064i }, { -0.0028727738039139574 + 0.010839822562772647i, -0.06328625210510755 + -0.039808080960127334i, -0.0734666986914104 + -0.006755439842319074i, 0.22057833936113697 + -9.785927167386587e-21i } }));

    EXPECT_THAT(s.getReducedDensityMatrixDiagonal({false, true, true}), ElementsAreArray({ DoubleNear(0.21604379393512083, 1.0e-7), DoubleNear(0.3345137639587393, 1.0e-7), DoubleNear(0.22886410274500285, 1.0e-7), DoubleNear(0.22057833936113697, 1.0e-7) }));


}
}

}
