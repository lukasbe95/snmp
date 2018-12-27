//
// Created by lukas on 25.12.18.
//
#include <list>

#include "../parser_mib/DataType.h"

#ifndef SNMP_DECODED_H
#define SNMP_DECODED_H

class Decoded {
private:
    DataType type;
    std::list<std::uint8_t> value;
    std::list<Decoded*> childs;
public:
    Decoded();
    DataType getType();
    std::list<std::uint8_t> getValue();
    std::list<Decoded*> getChilds();
    void setType(DataType d);
    void setValue(std::list<std::uint8_t> l);
    void appendValue(std::uint8_t x);
    void addChild(Decoded* child);
    void setChilds(std::list<Decoded*> l);
    void print();
};


#endif //SNMP_DECODED_H
