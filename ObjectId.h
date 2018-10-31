//
// Created by lukas on 31.10.18.
//

#ifndef SNMP_OBJECT_ID_H
#define SNMP_OBJECT_ID_H

#include <string>
class ObjectId {
private:
    int value;
    std::string name;
public:
    ObjectId(std::string n,int v);
    std::string getName();
    int getValue();
    void setName(std::string n);
    void setValue(int v);
};


#endif //SNMP_OBJECT_ID_H
