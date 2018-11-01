//
// Created by lukas on 22.10.18.
//

#ifndef SNMP_PARSER_H
#define SNMP_PARSER_H

#include <string>
#include <list>
#include "structures.h"
#include "DataType.h"
#include "ObjectId.h"
#include "ObjectType.h"

class Parser {
private:
    std::string wholeFile;
    std::string pathToFile;
    std::string filename;
    std::vector<std::string> splittedFile;
    std::vector <DataType> d;
    std::vector <ObjectId> o;
    std::vector <ObjectType> ot;
public:
    Parser(std::string p, std::string fn);
    void readFile();
    void searchForOID();
    void searchForOT();
    void searchForDT();
    void searchForDT(std::string name);
    void searchForImports();
    void parseOTSyntax(std::string & file,ObjectType & o);
    void parseOTAccess(std::string & file,ObjectType & o);
    void parseOTStatus(std::string & file,ObjectType & o);
    void parseOTDescription(std::string & file,ObjectType & o);
    void parseOToid(std::string & file,ObjectType & o);
    void searchInformationsInImports(std::string path, std::string filename, std::vector<std::string> elementsToImport);
    void addObjectId(ObjectId obj);
    void addObjectType(ObjectType obj);
    void addDataType(DataType obj);
};


#endif //SNMP_PARSER_H
