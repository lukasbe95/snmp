#include <iostream>
#include <string>
#include <fstream>
#include "parser.h"
using  namespace std;


int main() {
    Parser* p = new Parser("/usr/local/share/snmp/mibs/","RFC1213-MIB.txt");
    p->readFile();
    p->searchForImports();
    p->searchForOID();
    p->searchForOT();
    p->searchForDT();
//    p->createOIDFromObjectType();
//    p->printObjectIdVector();
//    p->printDataTypeVector();
    p->createOutputTree();
//    p->printOutputTree();
    ObjectId* t = p->returnNode("1.3.6.1.2.1");
    t->printOID();
    delete(p);

    return 0;
}