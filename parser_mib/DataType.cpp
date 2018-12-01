//
// Created by lukas on 31.10.18.
//

#include "DataType.h"
#include <iostream>

std::list<std::string> splitToList(std::string s, std::string delimiter){
    size_t pos = 0;
    std::string token;
    std::list<std::string> v;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        if(token !=""&&token!=" "){
            v.push_back(token);
        }
        s.erase(0, pos + 1);
    }
    return v;
}
DataType::DataType() {
    this->name = "";
    this->access = "";
    this->access_number = "";
    this->base_type = "";
    this->min_value = "";
    this->max_value = "";
    this->size = "";
}
void DataType::setName(std::string n) {
    this->name = n;
}
void DataType::setAccess(std::string a) {
    this->access = a;
}
void DataType::setAccessNum(std::string n) {
    this->access_number = n;
}
void DataType::setBaseType(std::string t) {
    this->base_type = t;
}
void DataType::setRange(std::string min_v, std::string max_v) {
    this->min_value = min_v;
    this->max_value = max_v;
}
void DataType::setSize(std::string s) {
    this->size = s;
}
std::string DataType::getName() {
    return this->name;
}
void DataType::printDataType() {
    std::cout<<"Name: "<<name<<std::endl;
    std::cout<<"Access(num):"<<access<<"("<<access_number<<")"<<std::endl;
    std::cout<<"Base type: "<<base_type<<std::endl;
    std::cout<<"Range: "<<min_value<<","<<max_value<<std::endl;
    std::cout<<"Size: "<<size<<std::endl;
    std::cout<<"Sequence: "<<std::endl;
    for (auto x : this->sequence){
        std::cout<<x<<" ";
    }
    std::cout<<std::endl;
}
void DataType::setSequence(std::string s) {
    this->sequence = splitToList(s," ");
}
void DataType::appendSequence(std::string s) {
    this->sequence.push_back(s);
}
std::list<std::string> DataType::getSequence() {
    return this->sequence;
}
std::string DataType::getBaseType() {
    return this->base_type;
}
int DataType::returnSize() {
    if(this->size == ""){
        return 0;
    } else {
        return std::stoi(this->size);
    }
}
std::string DataType::getAccess() {
    return this->access;
}