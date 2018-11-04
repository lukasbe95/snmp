//
// Created by lukas on 31.10.18.
//

#ifndef SNMP_DATATYPE_H
#define SNMP_DATATYPE_H

#include <string>

class DataType {
private:
    std::string name;
    std::string access;
    std::string access_number;
    std::string base_type;
    std::string min_value;
    std::string max_value;
    std::string size;
public:
    DataType();
    void setName(std::string n);
    void setAccess(std::string a);
    void setAccessNum(std::string n);
    void setBaseType(std::string t);
    void setRange(std::string min_v,std::string max_v);
    void setSize(std::string s);
    void printDataType();
    std::string getName();

};


#endif //SNMP_DATATYPE_H
