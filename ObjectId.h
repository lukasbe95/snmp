//
// Created by lukas on 31.10.18.
//

#ifndef SNMP_OBJECT_ID_H
#define SNMP_OBJECT_ID_H

#include <string>
#include <vector>
class ObjectId {
private:
    std::string value;
    std::string name;
    std::vector<std::string> path;
public:
    ObjectId(std::string n,std::string v);
    ObjectId();
    std::string getName();
    std::string getValue();
    void setName(std::string n);
    void setValue(std::string v);
    std::vector<std::string> getPath();
    bool createObjectIdFromString(std::string s);
};


#endif //SNMP_OBJECT_ID_H
