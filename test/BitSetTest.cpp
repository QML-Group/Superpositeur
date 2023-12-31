#include <gtest/gtest.h>

#include "superpositeur/utils/BitSet.hpp"

namespace superpositeur {
namespace utils {

class BitSetTest : public ::testing::Test {
public:
};

TEST_F(BitSetTest, SetTest) {
    BitSet<256> victim{};
    EXPECT_FALSE(victim.test(1));
    EXPECT_FALSE(victim.test(13));

    EXPECT_EQ(victim.toUInt64(), 0);
    victim.set(0);
    EXPECT_FALSE(victim.test(1));
    EXPECT_EQ(victim.toUInt64(), 1);
    victim.set(1);
    EXPECT_EQ(victim.toUInt64(), 3);
    EXPECT_TRUE(victim.test(1));
    EXPECT_TRUE(victim.test(0));
    EXPECT_FALSE(victim.test(10));

    victim.set(3);
    EXPECT_TRUE(victim.test(3));
    EXPECT_EQ(victim.toUInt64(), 11);
}

TEST_F(BitSetTest, SetTest64) {
    BitSet<64> victim{};

    victim.set(31);

    EXPECT_TRUE(victim.test(31));
    for (auto i = 0; i < 64; ++i) {
        if (i != 31) {
            EXPECT_FALSE(victim.test(i));
        }
    }
}

TEST_F(BitSetTest, SetTestLarge) {
    BitSet<256> victim{};
    EXPECT_FALSE(victim.test(120));
    EXPECT_FALSE(victim.test(130));

    victim.set(0);
    EXPECT_TRUE(victim.test(0));
    EXPECT_FALSE(victim.test(1));
    victim.set(132);
    EXPECT_TRUE(victim.test(0));
    EXPECT_TRUE(victim.test(132));
    EXPECT_FALSE(victim.test(149));

    EXPECT_EQ(victim.toString(),
             "000000000000000000000000000000000000000000000000000000000000000"
             "000000000000000000000000000000000000000000000000000000000000100"
             "000000000000000000000000000000000000000000000000000000000000000"
             "0000000000000000000000000000000000000000000000000000000000000000001");
}

TEST_F(BitSetTest, FromString) {
    BitSet<64> victim{"1010"};
    EXPECT_FALSE(victim.test(0));
    EXPECT_TRUE(victim.test(1));
    EXPECT_FALSE(victim.test(2));
    EXPECT_TRUE(victim.test(3));
    EXPECT_FALSE(victim.test(4));
    EXPECT_EQ(victim.toUInt64(), 10);
}

TEST_F(BitSetTest, ToString) {
    BitSet<64> victim{};
    victim.set(0);
    EXPECT_EQ(victim.toString(), "0000000000000000000000000000000000000000000000000000000000000001");

    victim.set(13);
    EXPECT_EQ(victim.toString(), "0000000000000000000000000000000000000000000000000010000000000001");
}

TEST_F(BitSetTest, AndSmall) {
    BitSet<64> victim{"000010000010001"};
    BitSet<64> mask{"000011001000001"};

    victim ^= mask;
    EXPECT_EQ(victim.toString(), "0000000000000000000000000000000000000000000000000000001001010000");
}

TEST_F(BitSetTest, AndLarge) {
    BitSet<1024> victim{};
    victim.set(457);

    BitSet<1024> mask{};

    mask.set(457);
    mask.set(654);

    victim ^= mask;

    EXPECT_FALSE(victim.test(457));
    EXPECT_TRUE(victim.test(654));
    EXPECT_FALSE(victim.test(875));
}

TEST_F(BitSetTest, Or) {
    BitSet<704> victim{};
    victim.set(457);

    EXPECT_EQ(BitSet<704>() | victim, victim);
    EXPECT_EQ(victim | victim, victim);

    BitSet<704> expected{};
    expected.set(1);
    expected.set(457);
    EXPECT_EQ(BitSet<704>("10") | victim, expected);
}

TEST_F(BitSetTest, Not) {
    EXPECT_EQ(~BitSet<192>(0b111),
              BitSet<192>("111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"
                          "111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111000"));
    EXPECT_EQ(~(~BitSet<192>()), BitSet<192>());
}

TEST_F(BitSetTest, Hash) {
    BitSet<192> victim;
    victim.set(1);
    victim.set(150);
    EXPECT_EQ(victim.hash(), 9652543998053464956ULL);
}

TEST_F(BitSetTest, CountlZero) {
    BitSet<8576> victim;
    victim.set(1);

    EXPECT_EQ(victim.countlZero(), 8576 - 2);
    
    victim.set(1, false);
    EXPECT_EQ(victim.countlZero(), 8576);
    
    victim.set(14);
    victim.set(1451);
    victim.set(4452);
    EXPECT_EQ(victim.countlZero(), 8576 - 4452 - 1);

    EXPECT_EQ(BitSet<64>("11111").countlZero(), 59);
}

TEST_F(BitSetTest, CountrZero) {
    BitSet<8576> victim;
    victim.set(1);

    EXPECT_EQ(victim.countrZero(), 1);
    
    victim.set(1, false);
    EXPECT_EQ(victim.countrZero(), 8576);
    
    victim.set(14);
    victim.set(1451);
    victim.set(4452);
    EXPECT_EQ(victim.countrZero(), 14);
    
    victim.set(14, false);

    EXPECT_EQ(victim.countrZero(), 1451);

    EXPECT_EQ(BitSet<64>("10100").countrZero(), 2);
}

TEST_F(BitSetTest, LeftShiftSmall) {
    BitSet<64> victim{"000010000010001"};
    EXPECT_EQ(victim << 0, victim);
    EXPECT_EQ(victim << 1, BitSet<64>{"000100000100010"});
    EXPECT_EQ(victim << 2, BitSet<64>{"001000001000100"});
    EXPECT_EQ(victim << 14, BitSet<64>{"00001000001000100000000000000"});
    EXPECT_EQ(victim << 15, BitSet<64>{"000010000010001000000000000000"});
    EXPECT_EQ(victim << 63, BitSet<64>{"1000000000000000000000000000000000000000000000000000000000000000"});
    EXPECT_EQ(victim << 64, BitSet<64>{"0000000000000000000000000000000000000000000000000000000000000000"});
}

TEST_F(BitSetTest, LeftShiftLarge) {
    BitSet<512> victim;
    victim.set(3);
    victim.set(30);
    victim.set(300);
    EXPECT_EQ(victim << 0, victim);

    BitSet<512> expected1;
    expected1.set(4);
    expected1.set(31);
    expected1.set(301);

    EXPECT_EQ(victim << 1, expected1);

    BitSet<512> expected200;
    expected200.set(203);
    expected200.set(230);
    expected200.set(500);

    EXPECT_EQ(victim << 200, expected200);

    BitSet<512> expected400;
    expected400.set(403);
    expected400.set(430);

    EXPECT_EQ(victim << 400, expected400);
}

TEST_F(BitSetTest, Pext) {
    BitSet<384> victim;
    victim.set(345);
    victim.set(346);
    
    BitSet<384> mask;
    mask.set(1);
    mask.set(345);
    EXPECT_EQ(victim.pext(mask), 2);
    mask.set(346);
    EXPECT_EQ(victim.pext(mask), 6);
    mask.set(345, false);
    mask.set(346, false);
    EXPECT_EQ(victim.pext(mask), 0);
    victim.set(1);
    EXPECT_EQ(victim.pext(mask), 1);
    mask.set(1, false);
    EXPECT_EQ(mask, BitSet<384>());
    EXPECT_EQ(victim.pext(mask), 0);
}

TEST_F(BitSetTest, Pdep) {
    BitSet<384> victim;
    
    BitSet<384> mask;
    mask.set(1);
    mask.set(50);
    EXPECT_EQ(victim.pdep(0, mask), BitSet<384>());
    EXPECT_EQ(victim.pdep(1, mask), BitSet<384>("10"));
    EXPECT_EQ(victim.pdep(2, mask), BitSet<384>(1UL << 50));
    EXPECT_EQ(victim.pdep(3, mask), BitSet<384>((1UL << 50) + 2UL));
}

} // namespace utils
} // namespace  superpositeur