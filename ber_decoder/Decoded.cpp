//
// Created by lukas on 25.12.18.
//
#include <iostream>
#include <regex>
#include "../ber_coder/BerCoder.h"
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
std::string Decoded::returnDecoded() {
    std::string to_decoded;
    to_decoded += this->type.getAccess() + "(" + this->type.getAccessNum() + ") v: ";
    for(auto v : this-> value){
        to_decoded += std::to_string((int)v) + " ";
    }
    to_decoded += "(";
    for(auto child : this->childs){
        to_decoded += child->returnDecoded();
    }
    to_decoded +=")";
    return to_decoded;
}
std::list<std::uint8_t> Decoded::getCoded() {
    return this->coded;
}
void Decoded::setCoded(std::list<std::uint8_t> l) {
    this->coded = std::move(l);
}
void Decoded::appendChildsCoded(std::list<std::uint8_t> l) {
    for(auto x : l){
        this->coded.push_back(x);
    }
}
void Decoded::addChildWithDecoded(Decoded *d) {
    this->childs.push_back(d);
    d->codeBER();
}
void Decoded::codeBER() {
    std::string acc_num = this->type.getAccessNum();
    if (acc_num == "2"){
        this->codeIntOctet();
    } else if (acc_num == "4"){
        this->codeIntOctet();
    } else if (acc_num == "5"){
        this->codeNull();
    } else if (acc_num == "6"){
        this->codeIntOctet(); //??? to fix
    } else if (acc_num == "16"){
        this->codeSequence();
    }
}
void Decoded::codeIntOctet() {
    std::list<uint8_t> tag = this->createTagToBER();
    for(auto x: tag){
        this->coded.push_back(x);
    }
    this->coded.push_back((uint8_t)this->value.size());
    for(auto i: this->value){
        this->coded.push_back(i);
    }
}
void Decoded::codeNull() {
    this->coded.push_back(5);
    this->coded.push_back(0);
}
void Decoded::codeSequence() {
    for(auto child: this->childs){
        for(auto element: child->getCoded()){
            this->value.push_back(element);
        }
    }
    std::list<uint8_t> tag = this->createTagToBER();
    for(auto x: tag){
        this->coded.push_back(x);
    }
    this->coded.push_back((uint8_t)this->value.size());
    for(auto i: this->value){
        this->coded.push_back(i);
    }
}
void Decoded::appendChildValue(std::list<uint8_t> l) {
    for(auto x : l){
        this->value.push_back(x);
    }
}
std::list<uint8_t> Decoded::createTagToBER() {
    uint8_t tag = this->setTagClass(this->type.getAccess());
    tag += (uint8_t) std::stoi(this->type.getAccessNum());
    if(this->type.getAccessNum() == "16") {
        tag += 32;
    }
    std::list<uint8_t> to_return;
    to_return.push_back(tag);
    return to_return;
}
uint8_t Decoded::setTagClass(std::string c) {
    if (c.find("APPLICATION")<10000){
        return 64;
    } else if (c.find("PRIVATE")<10000){
        return 192;
    } else if (c.find("UNIVERSAL")<10000){
        return 0;
    } else {
        return 128;
    }
}