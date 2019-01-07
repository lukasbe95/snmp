#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <bitset>
#include <cmath>
#include "parser_mib/parser.h"
#include "ber_coder/BerCoder.h"
#include "ber_decoder/BerEncoder.h"
#include "pdu/SnmpMessage.h"
using  namespace std;


int main() {
//    Parser part start
//    Parser* p = new Parser("/usr/local/share/snmp/mibs/","RFC1213-MIB.txt");
//    p->readFile();
//    p->searchForImports();
//    p->searchForOID();
//    p->searchForOT();
//    p->searchForDT();
//    p->createOIDFromObjectType();
//    p->printObjectTypeVector();
//    p->printObjectIdVector();
//    p->printDataTypeVector();
//    p->createOutputTree();
//    p->printOutputTree();
//    ObjectId* t = p->returnNode("1.3.6.1.2.1.3.atEntry.1");
//    t->printOID();
//    p->getBerEncodedLeaf("1.3.6.1.2.1.4.ipAddrEntry.3","DUPA");
//    Parser part end
//    BerCoder* b = new BerCoder();
//    std::vector<uint8_t> x = b->enterData();
//    for (uint8_t element : x ){
//        std::cout<<(int)element<<" ,";
//    }
//    std::cout<<std::endl;
//    delete(p);
    BerEncoder bE;
    std::list<uint8_t> test_input = {
            48,35,2,1,0,4,7,112,114,105,118,97,116,101,144,22,2,1,1,2,1,0,2,1,0,48,12,6,8,1,3,6,1,2,1,1,1,5,0};
    bE.setInput(test_input);
    bE.decode();
    SnmpMessage snmp;
    snmp.setEncoded_message(bE.getOutput());
    snmp.processEncodedMessage();
//    snmp.printMessage();
    snmp.createTreeFromPDU();
    for(auto x : snmp.getMessage()){
        cout<<x->returnDecoded()<<endl;
    }
    return 0;
}