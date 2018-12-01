#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include "parser_mib/parser.h"
using  namespace std;


int main() {
//    Parser part start
    Parser* p = new Parser("/usr/local/share/snmp/mibs/","RFC1213-MIB.txt");
    p->readFile();
    p->searchForImports();
    p->searchForOID();
    p->searchForOT();
    p->searchForDT();
    p->createOIDFromObjectType();
//    p->printObjectTypeVector();
//    p->printObjectIdVector();
//    p->printDataTypeVector();
    p->createOutputTree();
//    p->printOutputTree();
//    ObjectId* t = p->returnNode("1.3.6.1.2.1.3.atEntry.1");
//    t->printOID();
    p->getBerEncodedLeaf("1.3.6.1.2.1.4.ipAddrEntry.3","DUPA");
    delete(p);
//    Parser part end


    return 0;
}