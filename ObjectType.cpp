//
// Created by lukas on 31.10.18.
//

#include "ObjectType.h"

#include <iostream>
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