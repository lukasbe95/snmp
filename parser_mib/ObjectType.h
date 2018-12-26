//
// Created by lukas on 31.10.18.
//

#ifndef SNMP_OBJECTTYPE_H
#define SNMP_OBJECTTYPE_H

#include <string>
#include <vector>
#include "DataType.h"

class ObjectType {
private:
    std::string name;
    std::string syntax;
    std::string access;
    std::string status;
    std::string description;
    long syntax_min;
    long syntax_max;
    std::string type;
    bool isSequenceType;
    std::vector<std::string> oid;

public:
    ObjectType();
    void setName(std::string s);
    void setSyntax(std::string s);
    void setAccess(std::string s);
    void setStatus(std::string s);
    void setDescription(std::string s);
    void setOID(std::vector<std::string> v);
    void appendOID(std::string element);
    std::string getName();
    std::string getSyntax();
    std::string getAccess();
    std::string getStatus();
    std::string getDescription();
    void setIsSequenceType();
    bool getIsSequenceType();
    std::vector<std::string> getOID();
    void printOT();
    void setMinMaxValue(std::string syntax);
    void setType(std::string syntax);
    std::string getType();
    void setDefaultInt();
    void setDefaultNull();
    void setDefaultOctet();
    void preprocessSyntax(std::vector <DataType> &externalTypes);
    bool checkIfValueIsPermitted(std::vector <DataType> &externalTypes, std::string value);
    bool isItInteger(std::string val);
    void clear();
};


#endif //SNMP_OBJECTTYPE_H
