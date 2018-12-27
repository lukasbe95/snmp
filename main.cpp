#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <bitset>
#include <cmath>
#include "parser_mib/parser.h"
#include "ber_coder/BerCoder.h"
#include "ber_decoder/BerEncoder.h"
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
    std::list<std::uint8_t> l = {80,12,144,6,2,1,13,66,1,11,132,2,88,88};
    bE.setInput(l);
    bE.decode();
    for (auto x : bE.getOutput()){
        x->print();
    }

    return 0;
}