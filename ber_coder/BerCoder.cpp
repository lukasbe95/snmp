//
// Created by lukas on 24.11.18.
//

#include "BerCoder.h"
#include <regex>
#include <iostream>
#include <bitset>
#include <math.h>

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
std::vector<int> BerCoder::createTag() {
    // normal tag (one element on vector)
    std::vector<int> local_tag;
    if (this->tag_number > 31){
        int first = 31;
        first += this->setPC();
        first += this->setTagClass(this->dt.getAccess());
        local_tag.push_back(first);
        for(auto element: this->createExtendedTagNum()){
            local_tag.push_back(element);
        }

    } else {
        int first = (int)this->tag_number;
        first += this->setPC();
        first += this->setTagClass(this->dt.getAccess());
        local_tag.push_back(first);
    }
    return local_tag;

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
std::vector<int> BerCoder::createLength() {
    std::vector<int> local_length;
    if(value.length() > 128){
        if(obj.getIsSequenceType() || dt.getBaseType().find("OCTET") < 100){
            eoc = true;
            local_length.push_back(128);
        } else {
            local_length.push_back(128);
            //divide into 8bit ints
            int tempLength = value.length();
            for (int i = 0; i < sizeof(tempLength); ++i) {
                local_length.push_back(tempLength % 256);
                tempLength >>= 8;
            }

        }
    } else {
        local_length.push_back(value.length());
    }
    return local_length;
}
std::vector<int> BerCoder::createContentBasedOnLength(std::string value) {
    std::vector<int> local_content;
    for(char x: value){
        local_content.push_back((int)x);
    }
    return local_content;
}
void BerCoder::setContent(std::string c) {
    this->content = std::move(c);
}
void BerCoder::enterData() {
    std::string yes_no;
    std::cout<<"Do you want to enter adress (y/n)"<<std::endl;
    std::cin>>yes_no;
    //adress
    if(yes_no == "Y" || yes_no == "y"){
        this->enterAddress();
    }
    yes_no = "";
    std::cout<<"Do you want to define own datatype(y/n)"<<std::endl;
    std::cin>>yes_no;
    if(yes_no == "Y" || yes_no == "y"){
        this->enterDatatype();
    }
    std::cout<<"Please enter value:"<<std::endl;
    std::cin>>this->value;
}
void BerCoder::enterDatatype() {
    std::string temp_input;
    std::cout<<"Enter basetype. i -> INTEGER, n -> NULL, o -> OCTET STRING, oid -> OID, s -> SEQUENCE"<<std::endl;
    std::cin>>temp_input;
    if (temp_input == "i"){
        this->entered_datatype.setBaseType("INTEGER");
        this->enterInteger();
    } else if (temp_input == "n"){
        this->entered_datatype.setBaseType("NULL");
    } else if (temp_input == "o"){
        this->entered_datatype.setBaseType("OCTET STRING");
        this->enterOctet();
    } else if (temp_input == "oid"){
        this->entered_datatype.setBaseType("OBJECT IDENTIFIER");
        this->enterOID();
    } else if (temp_input == "s"){
        this->entered_datatype.setBaseType("");
        this->enterSequence();
    }
}
void BerCoder::enterInteger() {
    std::string temp_input;
    std::cout<<"Enter name:"<<std::endl;
    std::cin>>temp_input;
    this->entered_datatype.setName(temp_input);
    temp_input = "";
    std::cout<<"Enter access:"<<std::endl;
    std::cin>>temp_input;
    this->entered_datatype.setAccess(temp_input);
    temp_input = "";
    std::cout<<"Enter access number:"<<std::endl;
    std::cin>>temp_input;
    this->entered_datatype.setAccessNum(temp_input);
    temp_input = "";
    std::cout<<"Enter is it explicit/inplicit type(e/i):"<<std::endl;
    std::cin>>temp_input;
    if(temp_input=="e"){
        this->implicit_explicit = "EXPLICIT";
    } else {
        this->implicit_explicit = "IMPLICIT";
    }
    this->entered_datatype.setAccessNum(temp_input);
    temp_input = "";
    std::cout<<"Enter min/max value (s to skip to size):"<<std::endl;
    std::cin>>temp_input;
    if(temp_input == "s"){
        temp_input = "";
        std::cin>>temp_input;
        this->entered_datatype.setSize(temp_input);
    } else {
        std::string max_v;
        std::cin>>max_v;
        this->entered_datatype.setRange(temp_input,max_v);
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
    this->entered_objecttype.setName(temp_input);
    int oid_len;
    std::cout<<"Enter the OID length:"<<std::endl;
    std::cin>>oid_len;
    std::cout<<"Enter the OID:"<<std::endl;
    std::vector<std::string> temp_oid;
    for(int i = 0;i < oid_len;i++){
        std::string temp_elem;
        std::cin>>temp_elem;
        temp_oid.push_back(temp_elem);
    }
    this->entered_objecttype.setOID(temp_oid);
}
void BerCoder::enterSequence() {
    std::string temp_input;
    std::cout<<"Enter sequence name:"<<std::endl;
    std::cin>>temp_input;
    this->entered_objecttype.setName(temp_input);
    this->entered_objecttype.setIsSequenceType();
    int seq_len = 0;
    std::cout<<"Enter number of elements in sequence:"<<std::endl;
    std::cin>>seq_len;
    std::cout<<"Enter elements element type and then element value:"<<std::endl;
    for(int i = 0;i < seq_len;i++){
        std::cout<<"Enter type: ";
        std::cin>>temp_input;
        std::cout<<std::endl;
        this->sequence_types.push_back(temp_input);
        temp_input = "";
        std::cout<<"Enter value: ";
        std::cin>>temp_input;
        this->sequence_values.push_back(temp_input);
        std::cout<<std::endl;
        temp_input = "";
    }
}