//
// Created by lukas on 05.01.19.
//
#include "../ber_decoder/BerEncoder.h"

#ifndef SNMP_SNMP_MESSAGE_H
#define SNMP_SNMP_MESSAGE_H


class SnmpMessage {
private:
    int snmp_version;
    std::string community_string;
    int request_id;
    int error;
    int error_index;
    Decoded oid;
    Decoded value;
    Decoded * message;
    std::list<Decoded *> encoded_message;
    std::list<uint8_t> input;
    std::list<uint8_t> output;
public:
    int getSnmp_version() const;
    const std::string &getCommunity_string() const;
    int getRequest_id() const;
    int getError() const;
    int getError_index() const;
    const Decoded &getOid() const;
    Decoded getValue();
    Decoded *getMessage() const;
    std::list<Decoded *>getEncoded_message() const;
    const std::list<uint8_t> &getInput() const;
    const std::list<uint8_t> &getOutput() const;
    void setSnmp_version(int snmp_version);
    void setCommunity_string(const std::string &community_string);
    void setRequest_id(int request_id);
    void setError(int error);
    void setError_index(int error_index);
    void setOid(const Decoded &oid);
    void setMessage(Decoded *message);
    void setEncoded_message(std::list<Decoded*> encoded_message);
    void decodeBER();
    void processEncodedMessage();
    void setSNMPVer(std::list<uint8_t> l);
    void setCommunityString(std::list<uint8_t> l);
    void setRequestID(std::list<uint8_t> l);
    void setError(std::list<uint8_t> l);
    void setErrorID(std::list<uint8_t> l);
    void setOID(std::list<uint8_t> l);
    void setValue(std::list<uint8_t> l);
    void printMessage();
};


#endif //SNMP_SNMP_MESSAGE_H
