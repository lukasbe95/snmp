//
// Created by lukas on 30.10.18.
//
#include <string>
#include <vector>
#ifndef SNMP_STRUCTURES_H
#define SNMP_STRUCTURES_H

struct objectType {
    std::string name;
    std::string syntax;
    std::string access;
    std::string status;
    std::string description;
    std::vector<std::string> oid;
};
#endif //SNMP_STRUCTURES_H
