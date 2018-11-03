#include <iostream>
#include <string>
#include <fstream>
#include "parser.h"
using  namespace std;


int main() {
    Parser* p = new Parser("/usr/local/share/snmp/mibs/","RFC1213-MIB.txt");
    p->readFile();
    p->searchForDT();
    p->printDataTypeVector();
    delete(p);

    return 0;
}