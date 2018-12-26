//
// Created by lukas on 25.12.18.
//
#include <list>

#include "../parser_mib/DataType.h"
#include "../parser_mib/ObjectType.h"
#include "../parser_mib/parser.h"
#include "./Decoded.h"

#ifndef SNMP_BER_ENCODER_H
#define SNMP_BER_ENCODER_H


class BerEncoder {
private:
    Parser *mib_tree;
    std::list<std::uint8_t> input_bytes;
    std::list<Decoded*> output;

public:
    void buildOutputTree();
    DataType decodeTag();
    uint64_t decodeExtendedTag();
    void setInput(std::list<std::uint8_t> x);
    std::list<std::uint8_t> getInput();
    uint8_t decodeLength();
    std::list<uint8_t> decodeContent(int size);
    void decode();
    Decoded* decodeOne();
    uint8_t returnShifted(uint8_t value, int shift_size);
    uint8_t returnShiftedRef(uint8_t& value, int shift_size);
    std::string returnClass(uint8_t c);
};


#endif //SNMP_BER_ENCODER_H
