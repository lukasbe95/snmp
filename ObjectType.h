//
// Created by lukas on 31.10.18.
//

#ifndef SNMP_OBJECTTYPE_H
#define SNMP_OBJECTTYPE_H

#include <string>
#include <vector>

class ObjectType {
private:
    std::string name;
    std::string syntax;
    std::string access;
    std::string status;
    std::string description;
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
    std::vector<std::string> getOID();
    void printOT();
};


#endif //SNMP_OBJECTTYPE_H
