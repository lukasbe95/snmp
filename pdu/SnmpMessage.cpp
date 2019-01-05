//
// Created by lukas on 05.01.19.
//
#include <iostream>
#include "../ber_decoder/BerEncoder.h"
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
const Decoded &SnmpMessage::getOid() const {
    return oid;
}
Decoded SnmpMessage::getValue(){
    return value;
}
Decoded *SnmpMessage::getMessage() const {
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
void SnmpMessage::setOid(const Decoded &oid) {
    SnmpMessage::oid = oid;
}
void SnmpMessage::setMessage(Decoded *message) {
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
    this->setOID((*it)->getValue());
    it++;
    this->setValue((*it)->getValue());
}
void SnmpMessage::printMessage() {
    std::cout<<"SNMP VER: "<<this->snmp_version<<std::endl;
    std::cout<<"SNMP COM STR: "<<this->community_string<<std::endl;
    std::cout<<"REQ ID: "<<this->request_id<<std::endl;
    std::cout<<"ERROR: "<<this->error<<std::endl;
    std::cout<<"ERROR ID: "<<this->error_index<<std::endl;
    std::cout<<"OID: "<<std::endl;
    for(auto x: this->oid.getValue()){
        std::cout<<(int)x<<std::endl;
    }
    std::cout<<"VALUE: "<<std::endl;
    for(auto i: this->value.getValue()){
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
void SnmpMessage::setOID(std::list<uint8_t> l) {
    this->oid.setValue(std::move(l));
}
void SnmpMessage::setValue(std::list<uint8_t> l) {
    this->value.setValue(l);
}