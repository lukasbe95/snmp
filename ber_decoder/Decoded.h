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
    std::list<std::uint8_t> coded;
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
    std::string returnDecoded();
    std::list<std::uint8_t> getCoded();
    void setCoded(std::list<std::uint8_t> l);
    void appendChildsCoded(std::list<std::uint8_t> l);
    void addChildWithDecoded(Decoded* d);
    void appendChildValue(std::list<uint8_t> l);
    void codeBER();
    void codeIntOctet();
    void codeNull();
    void codeOID();
    void codeOctet();
    void codeSequence();
    std::list<uint8_t> createTagToBER();
    uint8_t setTagClass(std::string c);
};


#endif //SNMP_DECODED_H
