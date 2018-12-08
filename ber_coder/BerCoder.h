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
    std::vector<int> tag;
    std::vector<int> length;
    ObjectType obj;
    DataType dt;
    long tag_number;
    std::string value;
    bool eoc = false;
    std::string content;
    std::string entered_adress;
    std::string implicit_explicit;
    DataType entered_datatype;
    ObjectType entered_objecttype;
    std::vector<std::string> sequence_types;
    std::vector<std::string> sequence_values;
public:
    void enterData();
    std::vector<int> createTag();
    u_int8_t setTagClass(std::string c);
    u_int8_t setPC();
    void setTagNumber(std::string access, std::string basetype);
    void setDataType(DataType d);
    void setObjectType(ObjectType o);
    std::list<int> createExtendedTagNum();
    std::vector<int> createLength();
    std::vector<int> createContentBasedOnLength(std::string value);
    void setContent(std::string c);
    uint8_t convertIntToInt8(int number, int bit_vector_size);
    void enterAddress();
    void enterDatatype();
    void enterValue();
    void enterSequence();
    void enterInteger();
    void enterOctet();
    void enterOID();
};


#endif //SNMP_BERCODER_H
