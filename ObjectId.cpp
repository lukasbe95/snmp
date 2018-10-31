//
// Created by lukas on 31.10.18.
//

#include "ObjectId.h"

ObjectId::ObjectId(std::string n, int v) {
    this->name = n;
    this->value = v;
}

std::string ObjectId::getName() {
    return this->name;
}
int ObjectId::getValue() {
    return this->value;
}
void ObjectId::setName(std::string n) {
    this->name = n;
}
void ObjectId::setValue(int v) {
    this->value = v;
}