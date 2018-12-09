//
// Created by lukas on 24.11.18.
//

#include "BerCoder.h"
#include <regex>
#include <iostream>
#include <vector>
#include <bitset>
#include <math.h>
#include "../parser_mib/ObjectType.h"

uint8_t BerCoder::setTagClass(std::string c) {
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
uint8_t BerCoder::setPC() {
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
std::vector<uint8_t> BerCoder::createTag() {
    // normal tag (one element on vector)
    std::vector<uint8_t > local_tag;
    if (this->tag_number > 31){
        uint8_t first = 31;
        first += this->setPC();
        first += this->setTagClass(this->dt.getAccess());
        local_tag.push_back(first);
        for(auto element: this->createExtendedTagNum()){
            local_tag.push_back(element);
        }

    } else {
        uint8_t first = (uint8_t)this->tag_number;
        first += this->setPC();
        first += this->setTagClass(this->dt.getAccess());
        local_tag.push_back(first);
    }
    return local_tag;

}
std::list<uint8_t> BerCoder::createExtendedTagNum() {
    int base = (int)this->tag_number;
    std::list<uint8_t> v;
    std::list<uint8_t> list_to_return;
    for (int i = 0; i < 5; ++i) {
        v.push_front((uint8_t)base % 128);
        base >>= 7;
    }
    for (auto it=v.begin(); it!=v.end(); ++it){
        if (*it == v.back()){
            list_to_return.push_back(*it);
        }else {
            if(*it > 0){
                list_to_return.push_back((uint8_t)128+*it);
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
std::vector<uint8_t> BerCoder::createLength() {
    std::vector<uint8_t> local_length;
    if(value.length() > 128){
        if(obj.getIsSequenceType() || dt.getBaseType().find("OCTET") < 100){
            eoc = true;
            local_length.push_back(128);
        } else {
            local_length.push_back(128);
            //divide into 8bit ints
            int tempLength = value.length();
            for (int i = 0; i < sizeof(tempLength); ++i) {
                local_length.push_back((uint8_t)tempLength % 256);
                tempLength >>= 8;
            }

        }
    } else {
        local_length.push_back((uint8_t)value.length());
    }
    return local_length;
}
std::vector<uint8_t> BerCoder::createContentBasedOnLength(std::string value) {
    std::vector<uint8_t> local_content;
    for(char x: value){
        local_content.push_back((uint8_t)x);
    }
    return local_content;
}
void BerCoder::enterData() {
    std::string yes_no;
//    std::cout<<"Do you want to enter adress (y/n)"<<std::endl;
//    std::cin>>yes_no;
//    //adress
//    if(yes_no == "Y" || yes_no == "y"){
//        this->enterAddress();
//    }
    std::cout<<"Please enter value:"<<std::endl;
    yes_no = "";
    std::cin>>yes_no;
    this->value = yes_no;
    yes_no = "";
    std::cout<<"Do you want to define own datatype(y/n)"<<std::endl;
    std::cin>>yes_no;
    if(yes_no == "Y" || yes_no == "y"){
        this->enterDatatype();
    }
}
void BerCoder::enterDatatype() {
    this->obj.clear();
    this->dt.clear();
    std::string temp_input;
    std::cout<<"Enter basetype. i -> INTEGER, n -> NULL, o -> OCTET STRING, oid -> OID, s -> SEQUENCE"<<std::endl;
    std::cin>>temp_input;
    if (temp_input == "i"){
        this->dt.setBaseType("INTEGER");
        this->enterInteger();
        this->encodeInteger();
    } else if (temp_input == "n"){
        this->dt.setBaseType("NULL");
        this->encodeNull();
    } else if (temp_input == "o"){
        this->dt.setBaseType("OCTET STRING");
        this->enterOctet();
        this->encodeOctet();
    } else if (temp_input == "oid"){
        this->dt.setBaseType("OBJECT IDENTIFIER");
        this->enterOID();
        this->encodeOID();
    } else if (temp_input == "s"){
        this->dt.setBaseType("");
        this->enterSequence();
        this->encodeSequence();
    }
}
void BerCoder::enterInteger() {
    std::string temp_input;
    std::cout<<"Enter name:"<<std::endl;
    std::cin>>temp_input;
    this->dt.setName(temp_input);
    temp_input = "";
    std::cout<<"Enter access:"<<std::endl;
    std::cin>>temp_input;
    this->dt.setAccess(temp_input);
    temp_input = "";
    std::cout<<"Enter access number:"<<std::endl;
    std::cin>>temp_input;
    this->dt.setAccessNum(temp_input);
    temp_input = "";
    std::cout<<"Enter is it explicit/inplicit type(e/i):"<<std::endl;
    std::cin>>temp_input;
    if(temp_input=="e"){
        this->implicit_explicit = "EXPLICIT";
    } else {
        this->implicit_explicit = "IMPLICIT";
    }
    this->implicit_explicit = temp_input;
    temp_input = "";
    std::cout<<"Enter min/max value (s to skip to size):"<<std::endl;
    std::cin>>temp_input;
    if(temp_input.find("s") < 1000){
        temp_input = "";
        std::cin>>temp_input;
        this->dt.setSize(temp_input);
    } else {
        std::string max_v;
        std::cin>>max_v;
        this->dt.setRange(temp_input,max_v);
    }
}
void BerCoder::enterAddress() {
    int adr_len;
    std::cout<<"Enter the address length:"<<std::endl;
    std::cin>>adr_len;
    std::cout<<"Enter the address:"<<std::endl;
    for(int i = 0;i < adr_len;i++){
        std::string temp_elem;
        std::cin>>temp_elem;
        this->entered_adress += temp_elem + ".";
    }
    this->entered_adress = this->entered_adress.substr(0,this->entered_adress.length()-1);
    std::cout<<std::endl;
}
void BerCoder::enterOctet() {
    this->enterInteger();
}
void BerCoder::enterOID() {
    std::string temp_input;
    std::cout<<"Enter name:"<<std::endl;
    std::cin>>temp_input;
    this->obj.setName(temp_input);
    int oid_len;
    std::cout<<"Enter the OID length:"<<std::endl;
    std::cin>>oid_len;
    std::cout<<"Enter the OID(for now only values (int)):"<<std::endl;
    std::vector<std::string> temp_oid;
    for(int i = 0;i < oid_len;i++){
        std::string temp_elem;
        std::cin>>temp_elem;
        temp_oid.push_back(temp_elem);
    }
    this->obj.setOID(temp_oid);
}
void BerCoder::enterSequence() {
    std::string temp_input;
    std::cout<<"Enter sequence name:"<<std::endl;
    std::cin>>temp_input;
    this->obj.setName(temp_input);
    this->obj.setIsSequenceType();
    int seq_len = 0;
    std::cout<<"Enter number of elements in sequence:"<<std::endl;
    std::cin>>seq_len;
    std::cout<<"Enter elements element type and then element value:"<<std::endl;
    for(int i = 0;i < seq_len;i++){
        this->enterData();
        for(uint8_t x : this->tag){
            this->sequence_content.push_back(x);
        }
        for(uint8_t l : this->length){
            this->sequence_content.push_back(l);
        }
        for(uint8_t c : this->content_field){
            this->sequence_content.push_back(c);
        }
        this->tag.clear();
        this->length.clear();
        this->content_field.clear();
    }
}
void BerCoder::encodeOID() {
    // create tag
    this->obj.setAccess("");
    this->setTagNumber("","OBJECT IDENTIFIER");
    this->tag = this->createTag();
    // create length
    this->length.push_back((uint8_t)this->obj.getOID().size() - 1);
    //create content
    std::regex re("\\d+");
    std::vector<std::string> oid = this->obj.getOID();
    for(int i = 0; i < oid.size(); i++){
        int oid_value = 0;
        std::sregex_iterator next(oid[i].begin(), oid[i].end(), re);
        std::smatch match = *next;
        if(!match.empty()){
            oid_value = std::stoi(match.str());
        }
        if(i==0){
            uint8_t first = 40*oid_value;
            i++;
            std::sregex_iterator next(oid[i].begin(), oid[i].end(), re);
            std::smatch match = *next;
            if(!match.empty()){
                oid_value = std::stoi(match.str());
            }
            first += oid_value;
            this->content_field.push_back(first);
        } else {
            this->content_field.push_back(oid_value);
        }
    }
}
void BerCoder::encodeInteger() {
    if(dt.isItInteger(this->value)){
        if(dt.checkIfValueIsGood(this->value)){
            //create tag
            this->setTagNumber(dt.getAccessNum(),dt.getBaseType());
            this->tag = this->createTag();
            //create length
            int v = std::stoi(this->value);
            this->length.push_back((uint8_t)ceil(log2(v)/8));
            //create content
            std::list<uint8_t> temp_content;
            int content_length = ceil(log2(v)/8);
            for(int i = 0; i < content_length; i++){
                temp_content.push_front((uint8_t)(v % 256));
                v >>= 8;
            }
            for(uint8_t x : temp_content){
                this->content_field.push_back(x);
            }
        }else{
            this->encodeNull();
            std::cout<<"Wrong value!"<<std::endl;
        }
    }else{
        this->encodeNull();
        std::cout<<"It's not an integer!"<<std::endl;
    }
}
void BerCoder::encodeOctet() {
    if(dt.checkIfValueIsGood(this->value)){
        //create tag
        this->setTagNumber(dt.getAccessNum(),dt.getBaseType());
        this->tag = this->createTag();
        //create length
        this->length = this->createLength();
        //create content
        for(char x : this->value){
            this->content_field.push_back((uint8_t)x);
        }
    }else{
        this->encodeNull();
        std::cout<<"Wrong value!"<<std::endl;
    }
}
void BerCoder::encodeNull() {
    this->tag.push_back(5);
    this->content_field.push_back(0);
}
void BerCoder::printEncoded() {
    for(auto d: this->tag){
        std::cout<<(int)d<<std::endl;
    }
    for(auto e: this->length){
        std::cout<<(int)e<<std::endl;
    }
    for(auto f: this->content_field){
        std::cout<<(int)f<<std::endl;
    }
}
void BerCoder::encodeSequence() {
    this->obj.setIsSequenceType();
    this->setTagNumber("","SEQUENCE");
    this->createTag();
    this->length.push_back((uint8_t)this->sequence_content.size());
    for(uint8_t x : this->sequence_content){
        this->content_field.push_back(x);
    }
}
