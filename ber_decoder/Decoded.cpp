//
// Created by lukas on 25.12.18.
//

#include "Decoded.h"

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
    this->type = std::move(d);
}
void Decoded::setValue(std::list<std::uint8_t> l) {
    this->value = std::move(l);
}
void Decoded::appendValue(std::uint8_t x) {
    this->value.push_back(x);
}
void Decoded::addChild(Decoded *child) {
    this->childs.push_back(child);
}
