//
// Created by lukas on 23.12.18.
//
#include "Type.h"
#include "SimpleType.h"

#ifndef SNMP_CONSTRUCTEDTYPE_H
#define SNMP_CONSTRUCTEDTYPE_H


class ConstructedType : public Type{
private:
    std::vector<uint8_t> tag;
    std::vector<uint8_t> length;
    std::vector<uint8_t> content_field;
    std::vector<Type*> childrens;
public:
    ConstructedType(std::vector<uint8_t> tag, std::vector<uint8_t> len, std::vector<uint8_t> content);
    void add(Type* child);
    std::vector<uint8_t> returnType();
    void printType();

};


#endif //SNMP_CONSTRUCTEDTYPE_H
