//
// Created by lukas on 31.10.18.
//

#include "ObjectType.h"


ObjectType::ObjectType() {
    this->name ="";
    this->syntax = "";
    this->access = "";
    this->status = "";
}
void ObjectType::setName(std::string s) {
    this->name = s;
}
void ObjectType::setStatus(std::string s) {
    this->status = s;
}
void ObjectType::setSyntax(std::string s) {
    this->syntax = s;
}
void ObjectType::setAccess(std::string s) {
    this->access = s;
}
void ObjectType::setOID(std::vector<std::string> v) {
    this->oid = v;
}