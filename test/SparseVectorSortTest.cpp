#include <gtest/gtest.h>

#include "superpositeur/SparseVectorSort.hpp"

namespace superpositeur {

class SparseVectorSortTest : public ::testing::Test {
public:
    using V = SparseVector<64>;
    using B = BasisVector<64>;

    V create(std::initializer_list<std::string> l) {
        V v;
        for (auto& s: l) {
            v.emplace_back(B(s), 0.);
        }

        return v;
    }

    void check(V victim, std::vector<std::string> l) {
        ASSERT_EQ(victim.size(), l.size());

        for (std::uint64_t i = 0; i < victim.size(); ++i) {
            EXPECT_EQ(victim[i].amplitude, 0.) << "Index: " << i << std::endl;
            EXPECT_EQ(victim[i].ket, B(l[i])) << "Index: " << i << std::endl;
        }
    }
};

TEST_F(SparseVectorSortTest, FullRemoveRightmost) {
    auto victim = create({"000", "001", "010", "011", "100", "101", "110", "111"});
    B current("111"); 
    B desired("110");

    sortSparseVector(victim, current, desired);

    check(victim, {"000", "001", "010", "011", "100", "101", "110", "111"});
}

TEST_F(SparseVectorSortTest, FullRemoveTwoRightmost) {
    auto victim = create({"000", "001", "010", "011", "100", "101", "110", "111"});
    B current("111"); 
    B desired("100");

    sortSparseVector(victim, current, desired);

    check(victim, {"000", "001", "010", "011", "100", "101", "110", "111"});
}

TEST_F(SparseVectorSortTest, FullRemoveLeftmost) {
    auto victim = create({"000", "001", "010", "011", "100", "101", "110", "111"});
    B current("111"); 
    B desired("011");

    sortSparseVector(victim, current, desired);

    check(victim, {"000", "100", "001", "101", "010", "110", "011", "111"});
}

TEST_F(SparseVectorSortTest, FullRemoveTwoLeftmost) {
    auto victim = create({"000", "001", "010", "011", "100", "101", "110", "111"});
    B current("111"); 
    B desired("001");

    sortSparseVector(victim, current, desired);

    check(victim, {"000", "010", "100", "110", "001", "011", "101", "111"});
}

TEST_F(SparseVectorSortTest, FullAddRightmost) {
    auto victim = create({"000", "001", "010", "011", "100", "101", "110", "111"});
    B current("110"); 
    B desired("111");

    sortSparseVector(victim, current, desired);

    check(victim, {"000", "001", "010", "011", "100", "101", "110", "111"});
}

TEST_F(SparseVectorSortTest, FullAddMiddle) {
    auto victim = create({"000", "010", "001", "011", "100", "110", "101", "111"});
    B current("101"); 
    B desired("111");

    sortSparseVector(victim, current, desired);

    check(victim, {"000", "001", "010", "011", "100", "101", "110", "111"});
}

TEST_F(SparseVectorSortTest, FullAddLeftmost) {
    auto victim = create({"000", "100", "001", "101", "010", "110", "011", "111"});
    B current("011"); 
    B desired("111");

    sortSparseVector(victim, current, desired);

    check(victim, {"000", "001", "010", "011", "100", "101", "110", "111"});
}

TEST_F(SparseVectorSortTest, FullAddRemove) {
    auto victim = create({"000", "100", "001", "101", "010", "110", "011", "111"});
    B current("011"); 
    B desired("110");

    sortSparseVector(victim, current, desired);

    check(victim, {"000", "001", "010", "011", "100", "101", "110", "111"});
}

TEST_F(SparseVectorSortTest, NotFullAddRemove) {
    auto victim = create({"000", "101", "010", "011", "111"});
    B current("011"); 
    B desired("110");

    sortSparseVector(victim, current, desired);

    check(victim, {"000", "010", "011", "101", "111"});
}

TEST_F(SparseVectorSortTest, NotFullAddRemoveDisjoint) {
    auto victim = create({"000", "101", "010", "011", "111"});
    B current("011"); 
    B desired("100");

    sortSparseVector(victim, current, desired);

    check(victim, {"000", "010", "011", "101", "111"});
}

TEST_F(SparseVectorSortTest, LargerNotFullAddRemove) {
    auto victim = create({"00000", "01000", "01010", "11000", "10110"});
    B current("10111"); 
    B desired("11101");

    sortSparseVector(victim, current, desired);

    check(victim, {"00000", "01000", "01010", "10110", "11000"});
}

TEST_F(SparseVectorSortTest, EvenLargerNotFullAddRemove) {
    auto victim = create({"000000", "010000", "010100", "110000", "101100", "100111", "101111", "110111", "111111"});
    B current("100111"); 
    B desired("110011");

    sortSparseVector(victim, current, desired);

    check(victim, {"000000", "010000", "010100", "101100", "100111", "101111", "110000", "110111", "111111"});
}

TEST_F(SparseVectorSortTest, ThisThingCanAlsoCompletelySort) {
    // Radix sort.
    
    auto victim = create({"000000", "100111", "010100", "110000", "001100", "011000", "001111", "110111", "011111"});
    B current("000000"); 
    B desired("110011");

    sortSparseVector(victim, current, desired);

    check(victim, {"000000", "001100", "001111", "010100", "011000", "011111", "100111", "110000", "110111"});
}


}