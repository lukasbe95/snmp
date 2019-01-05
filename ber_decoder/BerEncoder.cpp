//
// Created by lukas on 25.12.18.
//

#include "BerEncoder.h"
#include <iostream>

void BerEncoder::setInput(std::list<std::uint8_t> x) {
    this->input_bytes = std::move(x);
}
std::list<std::uint8_t> BerEncoder::getInput() {
    return this->input_bytes;
}
void BerEncoder::buildOutputTree() {
    this->mib_tree = new Parser("/usr/local/share/snmp/mibs/","RFC1213-MIB.txt");
    this->mib_tree->readFile();
    this->mib_tree->searchForImports();
    this->mib_tree->searchForOID();
    this->mib_tree->searchForOT();
    this->mib_tree->searchForDT();
    this->mib_tree->createOIDFromObjectType();
    this->mib_tree->createOutputTree();
}
uint8_t BerEncoder::returnShifted(uint8_t value, int shift_size) {
    uint8_t value_before_shift = value;
    value>>=shift_size;
    value<<=shift_size;
    return value_before_shift - value;
}
uint8_t BerEncoder::returnShiftedRef(uint8_t &value, int shift_size) {
    uint8_t value_before_shift = value;
    value>>=shift_size;
    value<<=shift_size;
    return value_before_shift - value;
}
std::string BerEncoder::returnClass(uint8_t c) {
    if(c == 64){
        return "APPLICATION";
    } else if (c == 192){
        return "PRIVATE";
    } else if (c == 0){
        return "UNIVERSAL";
    } else {
        return "CONTEXT-SPECIFIC";
    }
}
DataType BerEncoder::decodeTag() {
    DataType datatype_to_return;
    auto it = this->input_bytes.begin();
    uint8_t tag = *it;
    this->input_bytes.pop_front();
    uint64_t tag_number = this->returnShiftedRef(tag,5);
    u_int8_t pc = this->returnShiftedRef(tag,6);
    u_int8_t c = this->returnShiftedRef(tag,8);
    if(tag_number == 31){
        tag_number = this->decodeExtendedTag();
    }
    datatype_to_return.setName("decoded_type");
    datatype_to_return.setAccess(this->returnClass(c));
    datatype_to_return.setAccessNum(std::to_string((uint64_t)tag_number));
    return datatype_to_return;
}
uint64_t BerEncoder::decodeExtendedTag() {
    uint64_t value_to_ret = 0;
    auto it = this->input_bytes.begin();
    std::list<uint8_t> temp;
    while(*it>127){
        temp.push_back(*it);
        it++;
        this->input_bytes.pop_front();
    }
    temp.push_back(*it);
    it++;
    this->input_bytes.pop_front();
    if (temp.size() > 8){
        return 0;
    }
    for (auto x : temp) {
        value_to_ret = ((value_to_ret<<8)|x);
    }
    return value_to_ret;
}
uint8_t BerEncoder::decodeLength() {
    auto it = this->input_bytes.begin();
    uint8_t length = *it;
    input_bytes.pop_front();
    return length;
}
std::list<uint8_t> BerEncoder::decodeContent(int size) {
    auto it = this->input_bytes.begin();
    std::list<uint8_t> to_return;
    for(int i = 0; i< size; i++){
        to_return.push_back(*it);
        it++;
        input_bytes.pop_front();
    }
    return to_return;
}
Decoded* BerEncoder::decodeOne() {
    DataType decoded_dt;
    uint8_t len_bef_seq;
    uint8_t len;
    std::list<std::uint8_t> val;
    std::list<Decoded*> childs;
    decoded_dt = this->decodeTag();
    len = this->decodeLength();
    len_bef_seq = (uint8_t)input_bytes.size();
    if (decoded_dt.getAccessNum() == "16") {
        while(len_bef_seq - (uint8_t)input_bytes.size() < (len - 1)) {
            Decoded * temp_decode = this->decodeOne();
            childs.push_back(temp_decode);
        }
    }else{
        val = this->decodeContent(len);
    }
    Decoded* to_return = new Decoded();
    to_return->setValue(val);
    to_return->setChilds(childs);
    to_return->setType(decoded_dt);
    return to_return;

}
void BerEncoder::decode() {
    while(!input_bytes.empty()){
        Decoded * temp_decode = this->decodeOne();
        output.push_back(temp_decode);
    }
}
std::list<Decoded*> BerEncoder::getOutput() {
    return this->output;
}