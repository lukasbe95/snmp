//
// Created by lukas on 25.12.18.
//
#include <iostream>
#include "Decoded.h"

Decoded::Decoded() {
    std::cout<<""<<std::endl;
}
DataType Decoded::getType() {
    return this->type;
}
std::list<std::uint8_t> Decoded::getValue() {
    return this->value;
}
std::list<Decoded*> Decoded::getChilds() {
    return this->childs;
}
void Decoded::setType(DataType d) {
    this->type.setName(d.getName());
    this->type.setAccess(d.getAccess());
    this->type.setAccessNum(d.getAccessNum());
}
void Decoded::setValue(std::list<std::uint8_t> l) {
    for (auto x : l){
        this->value.push_back(x);
    }
}
void Decoded::appendValue(std::uint8_t x) {
    this->value.push_back(x);
}
void Decoded::addChild(Decoded *child) {
    this->childs.push_back(child);
}
void Decoded::setChilds(std::list<Decoded *> l) {
    this->childs = std::move(l);
}
void Decoded::print() {
    this->type.printDataType();
    std::cout<<"Childs:"<<std::endl;
    for (auto x : this->childs){
        x->getType().printDataType();
    }
    std::cout<<"----------------------"<<std::endl;
}