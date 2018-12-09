//
// Created by lukas on 31.10.18.
//

#include "ObjectType.h"

#include <iostream>
#include <regex>

ObjectType::ObjectType() {
    this->name ="";
    this->syntax = "";
    this->access = "";
    this->status = "";
    this->isSequenceType = false;
    this->syntax_min = 0;
    this->syntax_max = 0;
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
void ObjectType::setDescription(std::string s) {
    this->description = s;
}
void ObjectType::setOID(std::vector<std::string> v) {
    this->oid = v;
}
std::string ObjectType::getName() {
    return this->name;
}
std::string ObjectType::getAccess() {
    return this->access;
}
std::vector<std::string> ObjectType::getOID() {
    return this->oid;
}
std::string ObjectType::getStatus() {
    return this->status;
}
std::string ObjectType::getSyntax() {
    return this->syntax;
}
void ObjectType::appendOID(std::string element) {
    this->oid.push_back(element);
}
std::string ObjectType::getDescription() {
    return this->description;
}
void ObjectType::setIsSequenceType() {
    this->isSequenceType = true;
}
void ObjectType::printOT() {
    std::cout<<"Name: "<<name<<std::endl;
    std::cout<<"Syntax: "<<syntax<<std::endl;
    std::cout<<"Access: "<<access<<std::endl;
    std::cout<<"Status: "<<status<<std::endl;
    std::cout<<"Description: "<<description<<std::endl;
    std::cout<<"OID: ";
    for(auto x:oid){
        std::cout<<x<<" ";
    }
    std::cout<<std::endl;

}
void ObjectType::setMinMaxValue(std::string syntax) {
    std::vector<int> v;
    std::regex re("\\d+");
    std::sregex_iterator next(syntax.begin(), syntax.end(), re);
    std::sregex_iterator end;
    while (next != end) {
        std::smatch match = *next;
        v.push_back(std::stoi(match.str()));
        next++;
    }
    switch (v.size()){
        case 0:
            this->syntax_min = 0;
            this->syntax_max = 0;
            break;
        case 1:
            this->syntax_min = 0;
            this->syntax_max = v[0];
            break;
        case 2:
            this->syntax_min = v[0];
            this->syntax_max = v[1];
            break;
    }
}
void ObjectType::setType(std::string syntax) {
    std::vector<std::string> v;
    std::regex re("\\w+\\s+");
    std::sregex_iterator next(syntax.begin(), syntax.end(), re);
    std::sregex_iterator end;
    while (next != end) {
        std::smatch match = *next;
        v.push_back(match.str());
        next++;
    }
    if (!v.empty()){
        this->type = v[0];
    } else {
        this->type = "";
    }
}
void ObjectType::preprocessSyntax(std::vector <DataType> &externalTypes) {
    bool externalType = false;
    DataType tempDt;
    for(DataType dt: externalTypes){
        if(this->syntax.find(dt.getName())<10000){
            externalType = true;
            tempDt = dt;
            break;
        }
    }
    if(externalType){
        if(this->syntax.find("..")<10000){
            this->setType(this->syntax);
            this->type = tempDt.getBaseType();
        }else if(tempDt.getBaseType() == ""){
            this->isSequenceType = true;
        }else {
            if(tempDt.getBaseType()=="NULL") {
                this->setDefaultNull();
            }else if(tempDt.getBaseType()=="INTEGER"){
                this->setDefaultInt();
            }else if(tempDt.getBaseType()=="OCTET STRING"){
                this->setDefaultOctet();
                if(tempDt.returnSize() > 0){
                    this->syntax_max = tempDt.returnSize();
                }
            }
        }
    } else {
        this->setType(this->syntax);
        this->setMinMaxValue(this->syntax);
        if(this->syntax_min == 0 && this->syntax_max == 0){
            if(this->type.find("NULL")<10000) {
                this->setDefaultNull();
            }else if(this->type.find("INTEGER")<10000){
                this->setDefaultInt();
            }else if(this->type.find("OCTET")<10000){
                this->setDefaultOctet();
            }
        }
    }
}
bool ObjectType::checkIfValueIsPermitted(std::vector <DataType> &externalTypes, std::string value) {
    this->preprocessSyntax(externalTypes);
    if(!isSequenceType&&type.find("INTEGER")<1000){
        if(!isItInteger(value)){
            return false;
        }
        int v = std::stoi(value);
        if(v >= syntax_min && v <= syntax_max){
            return true;
        }else{
            return false;
        }
    }else if(!isSequenceType&&type.find("OCTET")<100){
        long size = value.size();
        if(size>=syntax_max && size <= syntax_max){
            return true;
        } else {
            return false;
        }
    }

}
void ObjectType::setDefaultInt() {
    this->type = "INTEGER";
    this->syntax_min = 0;
    this->syntax_max = 4294967295;
}
void ObjectType::setDefaultNull() {
    this->type = "NULL";
    this->syntax_min = 0;
    this->syntax_max = 0;
}
void ObjectType::setDefaultOctet() {
    this->type = "OCTET STRING";
    this->syntax_min = 0;
    this->syntax_max = 8;
}
bool ObjectType::getIsSequenceType() {
    return this->isSequenceType;
}
bool ObjectType::isItInteger(std::string val) {
    std::string::const_iterator it = val.begin();
    while (it != val.end() && std::isdigit(*it)) ++it;
    return !val.empty() && it == val.end();
}
std::string ObjectType::getType() {
    return this->type;
}
void ObjectType::clear() {
    this->name ="";
    this->syntax = "";
    this->access = "";
    this->status = "";
    this->isSequenceType = false;
    this->syntax_min = 0;
    this->syntax_max = 0;
}