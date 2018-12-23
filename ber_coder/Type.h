//
// Created by lukas on 23.12.18.
//
#include <vector>
#include <cstdint>
#ifndef SNMP_TYPE_H
#define SNMP_TYPE_H
class Type {
public:
    virtual std::vector<uint8_t> returnType() = 0;
    virtual void printType() = 0;
};

#endif //SNMP_TYPE_H
