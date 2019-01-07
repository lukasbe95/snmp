//
// Created by lukas on 26.12.18.
//
#include <list>

#include "gtest/gtest.h"

#include "../../ber_decoder/BerEncoder.h"
#include "../../ber_decoder/Decoded.h"
#include "../../pdu/SnmpMessage.h"


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
TEST(BerEncoded_testcase, decodeSeqSeq) {
    BerEncoder bE;
    //seq(seq(int,int),octet)
    std::list<std::uint8_t> l = {80,12,144,6,2,1,13,66,1,11,132,2,88,88};
    bE.setInput(l);
    bE.decode();
    std::string answer = "APPLICATION(16) v: (CONTEXT-SPECIFIC(16) v: (UNIVERSAL(2) v: 13 ()APPLICATION(2) v: 11 ())CONTEXT-SPECIFIC(4) v: 88 88 ())";
    for (auto x : bE.getOutput()){
        ASSERT_EQ(answer, x->returnDecoded());
    }
}
TEST(BerEncoded_testcase, decodeNormal) {
    std::string answer = "CONTEXT-SPECIFIC(8505347) v: 9 44 ()";
    BerEncoder bE;
    std::list<std::uint8_t> l = {159,129,200,3,2,9,44};
    bE.setInput(l);
    bE.decode();
    for (auto x : bE.getOutput()){
        ASSERT_EQ(answer, x->returnDecoded());
    }
}
TEST(pdu_testcase, first) {
    std::list<uint8_t> test_input = {
            48,35,2,1,0,4,7,112,114,105,118,97,116,101,144,22,2,1,1,2,1,0,2,1,0,48,12,6,8,1,3,6,1,2,1,1,1,5,0};
    ASSERT_EQ(1,1);
}
TEST(pdu_testcase, intToUint8){
    SnmpMessage sm;
    std::list<uint8_t> to_compare;
    to_compare.push_back(8);
    ASSERT_EQ(sm.intToUint8(8), to_compare);
    to_compare.push_front(0);
    to_compare.push_front(1);
    ASSERT_EQ(sm.intToUint8(65544),to_compare);
}
TEST(pdu_testcase, strToUint8){
    SnmpMessage sm;
    std::list<uint8_t> to_compare;
    to_compare.push_back('p');
    to_compare.push_back('r');
    to_compare.push_back('i');
    to_compare.push_back('v');
    to_compare.push_back('a');
    ASSERT_EQ(sm.strToUint8("priva"),to_compare);
}
TEST(pdu_testcase, createTreeFromPDU) {
    std::string ans = "UNIVERSAL(16) v: 2 1 0 4 7 112 114 105 118 97 116 101 176 23 2 1 1 2 1 0 2 1 0 48 12 6 8 1 3 6 1 2 1 1 1 5 0 (UNIVERSAL(2) v: 0 ()UNIVERSAL(4) v: 112 114 105 118 97 116 101 ()CONTEXT-SPECIFIC(16) v: 2 1 1 2 1 0 2 1 0 48 12 6 8 1 3 6 1 2 1 1 1 5 0 (UNIVERSAL(2) v: 1 ()UNIVERSAL(2) v: 0 ()UNIVERSAL(2) v: 0 ()UNIVERSAL(16) v: 6 8 1 3 6 1 2 1 1 1 5 0 (UNIVERSAL(6) v: 1 3 6 1 2 1 1 1 ()UNIVERSAL(5) v: ())))";
    BerEncoder bE;
    std::list<uint8_t> test_input = {
            48,37,2,1,0,4,7,112,114,105,118,97,116,101,176,23,2,1,1,2,1,0,2,1,0,48,12,6,8,1,3,6,1,2,1,1,1,5,0};
    bE.setInput(test_input);
    bE.decode();
    SnmpMessage snmp;
    snmp.setEncoded_message(bE.getOutput());
    snmp.processEncodedMessage();
    snmp.createTreeFromPDU();
    for(auto x : snmp.getMessage()){
        ASSERT_EQ(x->getCoded(),test_input);
    }
}
