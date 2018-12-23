//
// Created by lukas on 23.12.18.
//

#include "SimpleType.h"

#include <vector>
#include <cstdint>
#include <iostream>

SimpleType::SimpleType(std::vector<uint8_t> tag, std::vector<uint8_t> len, std::vector<uint8_t> content) {
    this->tag = tag;
    this->length = len;
    this->content_field = content;
}
std::vector<uint8_t> SimpleType::returnType() {
    std::vector<uint8_t> vector_to_return;
    vector_to_return.insert(std::end(vector_to_return),std::begin(this->tag),std::end(this->tag));
    vector_to_return.insert(std::end(vector_to_return),std::begin(this->length),std::end(this->length));
    vector_to_return.insert(std::end(vector_to_return),std::begin(this->content_field),std::end(this->content_field));
    return vector_to_return;
}
void SimpleType::printType() {
    for(uint8_t t : this->tag){
        std::cout<<t<<std::endl;
    }
    for(uint8_t l : this->length){
        std::cout<<l<<std::endl;
    }
    for(uint8_t c : this->content_field){
        std::cout<<c<<std::endl;
    }
}