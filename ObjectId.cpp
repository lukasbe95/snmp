//
// Created by lukas on 31.10.18.
//

#include "ObjectId.h"
#include <regex>
#include <vector>
#include <iostream>

std::vector<std::string> splitOI(std::string s, std::string delimiter){
    size_t pos = 0;
    std::string token;
    std::vector<std::string> v;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        if(token !=""&&token!=" "){
            v.push_back(token);
        }
        s.erase(0, pos + 1);
    }
    return v;
}
ObjectId::ObjectId(std::string n, std::string v) {
    this->name = n;
    this->value = v;
}
ObjectId::ObjectId() {
    this->name = "";
    this->value = "";
}
std::string ObjectId::getName() {
    return this->name;
}
std::string ObjectId::getValue() {
    return this->value;
}
void ObjectId::setName(std::string n) {
    this->name = n;
}
void ObjectId::setValue(std::string v) {
    this->value = v;
}
std::vector<std::string> ObjectId::getPath() {
    return this->path;
}
void ObjectId::printOID() {
    std::cout<<"Name: "<<this->name<<std::endl;
    std::cout<<"Value: "<<this->value<<std::endl;
    for(auto x: this->path){
        std::cout<<x<<", ";
    }
    std::cout<<std::endl;
}
void ObjectId::appendPath(std::string element) {
    this->path.push_back(element);
}
void ObjectId::addChild(ObjectId* o) {
    this->childs.push_back(o);
}
std::vector<ObjectId*> ObjectId::getChilds() {
    return this->childs;
}
void ObjectId::setVisited(bool x) {
    this->visited = x;
}
bool ObjectId::getVisited() {
    return this->visited;
}

