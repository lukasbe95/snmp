//
// Created by lukas on 26.12.18.
//
#include <list>

#include "gtest/gtest.h"

#include "../../ber_decoder/BerEncoder.h"
#include "../../ber_decoder/Decoded.h"


TEST(Decoded_testcase, value) {
    Decoded d;
    std::list<std::uint8_t> x;
    x.push_back(12);
    x.push_back(121);
    x.push_back(122);
    d.setValue(x);
    ASSERT_EQ(d.getValue(),x);
}
TEST(Decoded_testcase, value_append) {
    Decoded d;
    d.appendValue(12);
    d.appendValue(121);
    d.appendValue(122);
    std::list<std::uint8_t> x;
    x.push_back(12);
    x.push_back(121);
    x.push_back(122);
    ASSERT_EQ(d.getValue(),x);
}
// BER encoder test
//long decodeExtendedTag();
//void decode();
//Decoded* decodeOne();

TEST(BerEncoded_testcase, returnClass) {
    BerEncoder bE;
    EXPECT_EQ("UNIVERSAL",bE.returnClass(0));
    EXPECT_EQ("APPLICATION",bE.returnClass(64));
    EXPECT_EQ("CONTEXT-SPECIFIC",bE.returnClass(128));
    EXPECT_EQ("PRIVATE",bE.returnClass(192));
}
TEST(BerEncoded_testcase, returnShifted) {
    BerEncoder bE;
    ASSERT_EQ(bE.returnShifted(137,4),9);
    ASSERT_EQ(bE.returnShifted(137,3),1);
    ASSERT_EQ(bE.returnShifted(137,8),137);
}
TEST(BerEncoded_testcase, returnShiftedRef) {
    BerEncoder bE;
    std::uint8_t y = 137;
    std::uint8_t& x = y;
    ASSERT_EQ(bE.returnShiftedRef(x,4),9);
    ASSERT_EQ(y,128);
}
TEST(BerEncoded_testcase, decodeExtendedTag) {
    BerEncoder bE;
    std::list<std::uint8_t> l = {129,200,3,4,5,6,7,8,9,44};
    std::list<std::uint8_t> l2 = {3,4,5,6,7,8,9,44};
    bE.setInput(l);
    ASSERT_EQ(bE.decodeExtendedTag(),8505347);
    bE.setInput(l2);
    ASSERT_EQ(bE.decodeExtendedTag(),3);
}
TEST(BerEncoded_testcase, decodeTag) {
    BerEncoder bE;
    std::list<std::uint8_t> l = {159,129,200,3,8,9,44};
    std::list<std::uint8_t> l2 = {80,4,5,6,7,8,9,44};
    bE.setInput(l);
    // Extended tag
    DataType dl = bE.decodeTag();
    EXPECT_EQ(dl.getAccess(),"CONTEXT-SPECIFIC");
    EXPECT_EQ(dl.getAccessNum(),"8505347");
    // Normal tag
    bE.setInput(l2);
    DataType dl2 = bE.decodeTag();
    EXPECT_EQ(dl2.getAccess(),"APPLICATION");
    EXPECT_EQ(dl2.getAccessNum(),"16");
}
TEST(BerEncoded_testcase, decodeLength) {
    BerEncoder bE;
    std::list<std::uint8_t> l = {200,3,8,9,44};
    bE.setInput(l);
    ASSERT_EQ(bE.decodeLength(),200);
    ASSERT_EQ(bE.decodeLength(),3);
    ASSERT_EQ(bE.decodeLength(),8);
}
TEST(BerEncoded_testcase, decodeContent) {
    BerEncoder bE;
    std::list<std::uint8_t> l = {200,3,8,9,44};
    bE.setInput(l);
    ASSERT_EQ(bE.decodeContent(5),l);
}
TEST(BerEncoded_testcase, decodeOne) {
    BerEncoder bE;
    //seq(seq(int,int),octet)
    std::list<std::uint8_t> l = {80,12,144,6,2,1,13,66,1,11,132,2,88,88};
    bE.setInput(l);
    bE.decode();
    for (auto x : bE.getOutput()){
        x->print();
    }
}