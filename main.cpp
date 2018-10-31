#include <iostream>
#include <string>
#include <fstream>
#include "parser.h"
using  namespace std;

//void readFile(string path, string filename){
//    string toOpen = path+filename;
//    ifstream file;
//    file.open(toOpen);
//    for(string line; getline(file,line);){
//        cout<<line<<" N"<<endl;
//    }
//}

int main() {
    Parser* p = new Parser("/usr/local/share/snmp/mibs/","RFC1213-MIB.txt");
    p->readFile();
    p->searchForOID();
    delete(p);

    return 0;
}