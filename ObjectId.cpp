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
    //TODO:implement
}
bool ObjectId::createObjectIdFromString(std::string s) {
    std::regex multispace("\\s+");
    std::vector<std::string> v = splitOI(regex_replace(s,multispace," ")," ");
    name = v[0];
    value = v[v.size()-1];
    std::cout<<v[0]<<std::endl;
    std::cout<<v[v.size()-1]<<std::endl;
    bool pushPath = false;
    v.pop_back();
    for(auto i: v){
        if (i.find("{")<1000){
            pushPath = true;
        } else if(pushPath){
            path.push_back(i);
        }
    }
    return true;
}