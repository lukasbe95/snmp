//
// Created by lukas on 23.12.18.
//

#include "ConstructedType.h"

#include <cstdint>
#include <vector>
#include <iostream>

ConstructedType::ConstructedType(std::vector<uint8_t> tag, std::vector<uint8_t> len, std::vector<uint8_t> content) {
    this->tag = tag;
    this->length = len;
    this->content_field = content;
}
void ConstructedType::add(Type *child) {
    this->childrens.push_back(child);
}
std::vector<uint8_t> ConstructedType::returnType() {
    std::vector<uint8_t> to_return;
    to_return.insert(std::end(to_return), std::begin(this->tag), std::end(this->tag));
    to_return.insert(std::end(to_return), std::begin(this->length), std::end(this->length));
    for(Type* x : this->childrens){
        to_return.insert(std::end(to_return),std::begin(x->returnType()),std::end(x->returnType()));
    }
    return to_return;
}
void ConstructedType::printType() {
    for(uint8_t x: this->tag){
        std::cout<<x<<std::endl;
    }
    for(uint8_t l: this->length){
        std::cout<<l<<std::endl;
    }
    for(Type* a:this->childrens){
        a->printType();
    }
}