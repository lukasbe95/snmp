//
// Created by lukas on 22.10.18.
//

#ifndef SNMP_PARSER_H
#define SNMP_PARSER_H

#include <string>
#include "structures.h"
class Parser {
private:
    std::string wholeFile;
    std::string pathToFile;
    std::string filename;
public:
    Parser(std::string p, std::string fn);
    void readFile();
    void searchForOID();
    void searchForOT();
    void parseOTSyntax(std::string & file,objectType & o);
    void parseOTAccess(std::string & file,objectType & o);
    void parseOTStatus(std::string & file,objectType & o);
    void parseOTDescription(std::string & file,objectType & o);
    void parseOToid(std::string & file,objectType & o);
};


#endif //SNMP_PARSER_H
