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
    BerEncoder x;
    std::list<std::uint8_t> l = {129,200,3,4,5,6,7,8,9,44};
    x.setInput(l);
    x.decodeExtendedTag();
    uint8_t a = 129;
    uint8_t b = 200;
    long sum;
    sum = ((a<<8)|(b));
    cout<<sum<<endl;
//    int x1 = 1;
//    x1<<=8;
//    x1>>=8;
//    int base = 0;
//    cout<<"sdfsdfds"<<endl;
    return 0;
}