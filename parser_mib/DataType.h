//
// Created by lukas on 31.10.18.
//

#ifndef SNMP_DATATYPE_H
#define SNMP_DATATYPE_H

#include <string>
#include <list>

class DataType {
private:
    std::string name;
    std::string access;
    std::string access_number;
    std::string base_type;
    std::string min_value;
    std::string max_value;
    std::string size;
    std::list<std::string> sequence;

public:
    DataType();
    void setName(std::string n);
    std::string getBaseType();
    int returnSize();
    void setAccess(std::string a);
    void setAccessNum(std::string n);
    std::string getAccessNum();
    void setBaseType(std::string t);
    void setRange(std::string min_v,std::string max_v);
    void setSize(std::string s);
    void setSequence(std::string s);
    std::list<std::string> getSequence();
    void appendSequence(std::string s);
    void printDataType();
    std::string getName();
    std::string getAccess();
    bool isItInteger(std::string val);
    bool checkIfValueIsGood(std::string val);
    void clear();
};


#endif //SNMP_DATATYPE_H
