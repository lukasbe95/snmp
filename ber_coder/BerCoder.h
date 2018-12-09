//
// Created by lukas on 24.11.18.
//

#ifndef SNMP_BERCODER_H
#define SNMP_BERCODER_H
#include <bitset>
#include <string>
#include <vector>
#include <cstdint>

#include "../parser_mib/parser.h"
#include "../parser_mib/ObjectType.h"
#include "../parser_mib/DataType.h"

class BerCoder {
private:
    std::vector<uint8_t> tag;
    std::vector<uint8_t> length;
    std::vector<uint8_t> content_field;
    ObjectType obj;
    DataType dt;
    long tag_number;
    std::string value;
    bool eoc = false;
    std::string entered_adress;
    std::string implicit_explicit;
//    std::vector<std::string> sequence_types;
//    std::vector<std::string> sequence_values;
//    std::vector<DataType> datatypes_vector;
//    std::vector<ObjectId> objecttypes_vector;
    std::vector<uint8_t> sequence_content;


public:
    void enterData();
    std::vector<uint8_t> createTag();
    uint8_t setTagClass(std::string c);
    uint8_t setPC();
    void setTagNumber(std::string access, std::string basetype);
    void setDataType(DataType d);
    void setObjectType(ObjectType o);
    std::list<uint8_t> createExtendedTagNum();
    std::vector<uint8_t> createLength();
    std::vector<uint8_t> createContentBasedOnLength(std::string value);
    void enterAddress();
    void enterDatatype();
    void enterSequence();
    void enterInteger();
    void enterOctet();
    void enterOID();
    void encodeOID();
    void encodeInteger();
    void encodeOctet();
    void encodeNull();
    void encodeSequence();
    void printEncoded();
};


#endif //SNMP_BERCODER_H
