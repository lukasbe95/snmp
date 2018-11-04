//
// Created by lukas on 31.10.18.
//

#include "DataType.h"
#include <iostream>

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
}