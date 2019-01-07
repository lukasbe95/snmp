//
// Created by lukas on 05.01.19.
//
#include <iostream>
#include "../ber_decoder/BerEncoder.h"
#include "../ber_coder/BerCoder.h"
#include "SnmpMessage.h"

int SnmpMessage::getSnmp_version() const {
    return snmp_version;
}
const std::string &SnmpMessage::getCommunity_string() const {
    return community_string;
}
int SnmpMessage::getRequest_id() const {
    return request_id;
}
int SnmpMessage::getError() const {
    return error;
}
int SnmpMessage::getError_index() const {
    return error_index;
}
std::list<Decoded *> SnmpMessage::getMessage() const {
    return message;
}
std::list<Decoded *> SnmpMessage::getEncoded_message() const {
    return encoded_message;
}
const std::list<uint8_t> &SnmpMessage::getInput() const {
    return input;
}
const std::list<uint8_t> &SnmpMessage::getOutput() const {
    return output;
}
void SnmpMessage::setSnmp_version(int snmp_version) {
    SnmpMessage::snmp_version = snmp_version;
}
void SnmpMessage::setCommunity_string(const std::string &community_string) {
    SnmpMessage::community_string = community_string;
}
void SnmpMessage::setRequest_id(int request_id) {
    SnmpMessage::request_id = request_id;
}
void SnmpMessage::setError(int error) {
    SnmpMessage::error = error;
}
void SnmpMessage::setError_index(int error_index) {
    SnmpMessage::error_index = error_index;
}
void SnmpMessage::setMessage(std::list<Decoded *> message) {
    SnmpMessage::message = message;
}
void SnmpMessage::setEncoded_message(std::list<Decoded*> encoded_message) {
    SnmpMessage::encoded_message = encoded_message;
}
void SnmpMessage::decodeBER() {
    BerEncoder bE;
    bE.setInput(this->input);
    bE.decode();
    this->encoded_message = bE.getOutput();
}
void SnmpMessage::processEncodedMessage() {
    std::list<Decoded*>::iterator it;
    it = this->encoded_message.begin();
    std::list<Decoded*> pdu_ch;
    pdu_ch = (*it)->getChilds();
    it = pdu_ch.begin();
    this->setSNMPVer((*it)->getValue());
    it++;
    this->setCommunityString((*it)->getValue());
    it++;
    pdu_ch = (*it)->getChilds();
    it = pdu_ch.begin();
    this->setRequestID((*it)->getValue());
    it++;
    this->setError((*it)->getValue());
    it++;
    this->setErrorID((*it)->getValue());
    it++;
    pdu_ch = (*it)->getChilds();
    it = pdu_ch.begin();
    this->setOID((*it));
    it++;
    this->setValue((*it));
}
void SnmpMessage::printMessage() {
    std::cout<<"SNMP VER: "<<this->snmp_version<<std::endl;
    std::cout<<"SNMP COM STR: "<<this->community_string<<std::endl;
    std::cout<<"REQ ID: "<<this->request_id<<std::endl;
    std::cout<<"ERROR: "<<this->error<<std::endl;
    std::cout<<"ERROR ID: "<<this->error_index<<std::endl;
    std::cout<<"OID: "<<std::endl;
    for(auto x: this->oid->getValue()){
        std::cout<<(int)x<<std::endl;
    }
    std::cout<<"VALUE: "<<std::endl;
    for(auto i: this->value->getValue()){
        std::cout<<(int)i<<std::endl;
    }

}
void SnmpMessage::setSNMPVer(std::list<uint8_t> l) {
    this->snmp_version = l.front();
}
void SnmpMessage::setCommunityString(std::list<uint8_t> l) {
    std::string to_set;
    for(auto x : l){
        to_set += x;
    }
    this->community_string = to_set;
}
void SnmpMessage::setRequestID(std::list<uint8_t> l) {
    this->request_id = l.front();
}
void SnmpMessage::setError(std::list<uint8_t> l) {
    this->error = l.front();
}
void SnmpMessage::setErrorID(std::list<uint8_t> l) {
    this->error_index = l.front();
}
void SnmpMessage::setOID(Decoded *d) {
    this->oid = d;
}
void SnmpMessage::setValue(Decoded *d) {
    this->value = d;
}
void SnmpMessage::createTreeFromPDU() {
    auto root = new Decoded();
    auto oid_seq = new Decoded();
    oid_seq->addChildWithDecoded(this->oid);
    oid_seq->addChildWithDecoded(this->value);
    DataType d_oid;
    d_oid.setAccessNum("16");
    d_oid.setAccess("UNIVERSAL");
    oid_seq->setType(d_oid);
    auto pdu = new Decoded();
    pdu->addChildWithDecoded(this->createRequestID());
    pdu->addChildWithDecoded(this->createError());
    pdu->addChildWithDecoded(this->createErrorID());
    pdu->addChildWithDecoded(oid_seq);
    DataType d_pdu;
    d_pdu.setAccess("CONTEXT-SPECIFIC");
    d_pdu.setAccessNum("16");
    pdu->setType(d_pdu);
    root->addChildWithDecoded(this->createSnmpVersion());
    root->addChildWithDecoded(this->createCommunityString());
    root->addChildWithDecoded(pdu);
    DataType d_root;
    d_root.setAccessNum("16");
    d_root.setAccess("UNIVERSAL");
    root->setType(d_root);
    root->codeBER();
    this->message.push_back(root);
}
Decoded* SnmpMessage::createSnmpVersion() {
    auto to_return = new Decoded();
    DataType d;
    d.setAccessNum("2");
    d.setAccess("UNIVERSAL");
    to_return->setType(d);
    to_return->setValue(this->intToUint8(this->snmp_version));
    return to_return;
}
std::list<uint8_t> SnmpMessage::strToUint8(std::string x) {
    std::list<uint8_t> to_return;
    for(auto i : x){
        to_return.push_back((uint8_t)i);

    }
    return to_return;
}
std::list<uint8_t> SnmpMessage::intToUint8(int x) {
    int& ref = x;
    std::list<uint8_t> to_return;
    for (int i = 0; i < 5; ++i) {
        to_return.push_front((uint8_t)ref % 256);
        ref >>= 8;
    }
    auto it = to_return.begin();
    while(*it == 0 && !to_return.empty()){
        it++;
        to_return.pop_front();
    }
    if(to_return.empty()){
        to_return.push_back(0);
    }
    return to_return;
}
Decoded* SnmpMessage::createCommunityString() {
    auto to_return = new Decoded();
    DataType d;
    d.setAccessNum("4");
    d.setAccess("UNIVERSAL");
    to_return->setType(d);
    to_return->setValue(this->strToUint8(this->community_string));
    return to_return;
}
Decoded* SnmpMessage::createRequestID() {
    auto to_return = new Decoded();
    DataType d;
    d.setAccessNum("2");
    d.setAccess("UNIVERSAL");
    to_return->setType(d);
    to_return->setValue(this->intToUint8(this->request_id));
    return to_return;
}
Decoded* SnmpMessage::createError() {
    auto to_return = new Decoded();
    DataType d;
    d.setAccessNum("2");
    d.setAccess("UNIVERSAL");
    to_return->setType(d);
    to_return->setValue(this->intToUint8(this->error));
    return to_return;
}
Decoded* SnmpMessage::createErrorID() {
    auto to_return = new Decoded();
    DataType d;
    d.setAccessNum("2");
    d.setAccess("UNIVERSAL");
    to_return->setType(d);
    to_return->setValue(this->intToUint8(this->error_index));
    return to_return;
}
