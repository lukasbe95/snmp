//
// Created by lukas on 24.11.18.
//

#ifndef SNMP_BERCODER_H
#define SNMP_BERCODER_H
#include <bitset>
#include <string>
#include <vector>

#include "../parser_mib/parser.h"
#include "../parser_mib/ObjectType.h"
#include "../parser_mib/DataType.h"

class BerCoder {
private:
    std::vector<int> tag;
    ObjectType obj;
    DataType dt;
    long tag_number;
    long value;

public:
    void createTag();
    u_int8_t setTagClass(std::string c);
    u_int8_t setPC();
    void setTagNumber(std::string access, std::string basetype);
    void setDataType(DataType d);
    void setObjectType(ObjectType o);
    std::list<int> createExtendedTagNum();
};


#endif //SNMP_BERCODER_H
