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
             "00000000000000000100000000000000000000000000000000000000000000000"
             "00000000000000000000000000000000000000000000000000000000000000000"
             "00000000000000000001");
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
    EXPECT_EQ(victim.toString(), "000000000000001");

    victim.set(13);
    EXPECT_EQ(victim.toString(), "010000000000001");
}

TEST_F(BitSetTest, AndSmall) {
    BitSet<64> victim{"000010000010001"};
    BitSet<64> mask{"000011001000001"};

    victim ^= mask;
    EXPECT_EQ(victim.toString(), "000001001010000");
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

TEST_F(BitSetTest, LeftShiftSmall) {
    BitSet<64> victim{"000010000010001"};
    EXPECT_EQ(victim << 0, victim);
    EXPECT_EQ(victim << 1, BitSet<64>{"000100000100010"});
    EXPECT_EQ(victim << 2, BitSet<64>{"001000001000100"});
    EXPECT_EQ(victim << 14, BitSet<64>{"100000000000000"});
    EXPECT_EQ(victim << 15, BitSet<64>{"000000000000000"});
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

TEST_F(BitSetTest, Add) {
    EXPECT_EQ(BitSet<704>(123) + BitSet<704>(456), BitSet<704>(579));
    EXPECT_EQ(BitSet<448>(123) + BitSet<448>(456), BitSet<448>(579));

    auto x = BitSet<448>("10000000000000000000000000000000000000000000000000000000000000000");
    EXPECT_EQ(BitSet<448>(~(0UL)) + BitSet<448>(1), x);
    EXPECT_EQ(x + x, BitSet<448>());
    EXPECT_EQ(x + BitSet<448>(1), BitSet<448>("10000000000000000000000000000000000000000000000000000000000000001"));
}

TEST_F(BitSetTest, Not) {
    EXPECT_EQ(~BitSet<192>(0b111), BitSet<192>());
    EXPECT_EQ(~BitSet<192>(0b101), BitSet<192>(0b010));
}

TEST_F(BitSetTest, Hash) {
    BitSet<192> victim;
    victim.set(1);
    victim.set(150);
    // 3 * (3 * (1UL << 22) + 0) + 2 = 37748738

    EXPECT_EQ(victim.hash(), 37748738);
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

TEST_F(BitSetTest, NextWithBits) {
    // FIXME: also test the extra bits in "bits" of "mask" that are not taken into account.
    BitSet<64> victim("001");
    EXPECT_EQ(victim.nextWithBits({"1111111111111111111111111111111111111111111111111111111111111110"}, {"000"}), std::nullopt);
    EXPECT_EQ(victim.nextWithBits({"001"}, {"000"}), std::optional<BitSet<64>>("010"));
    EXPECT_EQ(victim.nextWithBits({"010"}, {"010"}), std::optional<BitSet<64>>("010"));
    EXPECT_EQ(victim.nextWithBits({"010"}, {"000"}), std::optional<BitSet<64>>("100"));
    EXPECT_EQ(victim.nextWithBits({"110"}, {"110"}), std::optional<BitSet<64>>("110"));
    EXPECT_EQ(victim.nextWithBits({"1111111111111111111111111111111111111111111111111111111111111111"}, {"000"}), std::nullopt);
    EXPECT_EQ(victim.nextWithBits({"1111111111111111111111111111111111111111111111111111111111111111"}, {"001"}), std::nullopt);
    EXPECT_EQ(victim.nextWithBits({"111"}, {"010"}), std::optional<BitSet<64>>("010"));
    EXPECT_EQ(victim.nextWithBits({"111"}, {"111"}), std::optional<BitSet<64>>("111"));

    BitSet<128> victim2("101010101");
    EXPECT_EQ(victim2.nextWithBits({"1111111111111111111111111111111111111111111111111111111111111111"}, {"001"}),
        std::optional<BitSet<128>>("10000000000000000000000000000000000000000000000000000000000000001"));
    EXPECT_EQ(victim2.nextWithBits({"1111111111111111111111111111111111111111111111111111111111111111"}, {"101010101"}),
        std::optional<BitSet<128>>("10000000000000000000000000000000000000000000000000000000101010101"));
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