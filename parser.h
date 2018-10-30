//
// Created by lukas on 22.10.18.
//

#ifndef SNMP_PARSER_H
#define SNMP_PARSER_H

#include <string>

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
};


#endif //SNMP_PARSER_H
