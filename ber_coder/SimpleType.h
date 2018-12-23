//
// Created by lukas on 23.12.18.
//
#include <vector>
#include <cstdint>

#include "Type.h"

#ifndef SNMP_SIMPLETYPE_H
#define SNMP_SIMPLETYPE_H


class SimpleType : public Type{
private:
    std::vector<uint8_t> tag;
    std::vector<uint8_t> length;
    std::vector<uint8_t> content_field;
public:
    SimpleType(std::vector<uint8_t> tag, std::vector<uint8_t> len, std::vector<uint8_t> content);
    std::vector<uint8_t> returnType();
    void printType();
};


#endif //SNMP_SIMPLETYPE_H
