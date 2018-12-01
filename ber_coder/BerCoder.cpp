//
// Created by lukas on 24.11.18.
//

#include "BerCoder.h"
#include <regex>
#include <iostream>

u_int8_t BerCoder::setTagClass(std::string c) {
    if (c.find("APPLICATION")<10000){
        return 64;
    } else if (c.find("PRIVATE")<10000){
        return 192;
    } else if (c.find("CONTEXT-SPECIFIC")<10000){
        return 128;
    } else {
        return 0;
    }
}
u_int8_t BerCoder::setPC() {
    if(this->obj.getIsSequenceType() || this->obj.getType().find("OCTET") < 100){
        return 32;
    } else {
        return 0;
    }
}

void BerCoder::setTagNumber(std::string access, std::string basetype) {
    if (!access.empty()){
        std::regex re("\\d+");
        std::sregex_iterator next(access.begin(), access.end(), re);
        std::smatch match = *next;
        this->tag_number = std::stoi(match.str());
    } else {
        if (basetype.find("INTEGER") < 100){
            this->tag_number = 2;
        }else if(basetype.find("NULL") < 100){
            this->tag_number = 5;
        }else if(basetype.find("OCTET") < 100){
            this->tag_number = 4;
        }else if(basetype.find("SEQUENCE") < 100){
            this->tag_number = 16;
        }else if(basetype.find("OBJECT") < 100){
            this->tag_number = 6;
        }
    }
}
void BerCoder::createTag() {
    // normal tag (one element on vector)
    if (this->tag_number > 31){
        int first = 31;
        first += this->setPC();
        first += this->setTagClass(this->dt.getAccess());
        tag.push_back(first);
        for(auto element: this->createExtendedTagNum()){
            tag.push_back(element);
        }

    } else {
        int first = (int)this->tag_number;
        first += this->setPC();
        first += this->setTagClass(this->dt.getAccess());
        tag.push_back(first);
    }

}
std::list<int> BerCoder::createExtendedTagNum() {
    int base = (int)this->tag_number;
    std::list<int> v;
    std::list<int> list_to_return;
    for (int i = 0; i < 5; ++i) {
        v.push_front(base % 128);
        base >>= 7;
    }
    for (auto it=v.begin(); it!=v.end(); ++it){
        if (*it == v.back()){
            list_to_return.push_back(*it);
        }else {
            if(*it > 0){
                list_to_return.push_back(128+*it);
            }
        }
    }
    return list_to_return;
}
void BerCoder::setObjectType(ObjectType o) {
    this->obj = std::move(o);
}
void BerCoder::setDataType(DataType d) {
    this->dt = std::move(d);
}