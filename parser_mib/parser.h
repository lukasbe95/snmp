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
    std::vector <std::string> splittedFile;
    std::vector <DataType> d;
    std::vector <ObjectId> o;
    std::vector <ObjectType> ot;
    std::list <ObjectId*> outputTree;
public:
    Parser(std::string p, std::string fn);
    void readFile();
    void searchForOID();
    void searchForOT();
    void searchForDT();
    void searchForImports();
    void searchInformationsInImports(std::string path, std::string filename, std::vector<std::string> elementsToImport);
    void addObjectId(ObjectId obj);
    void addObjectType(ObjectType obj);
    void addDataType(DataType obj);
    std::string returnTypeAndSetLength(std::string t, int &n);
    void printDataTypeVector();
    void printObjectTypeVector();
    void printObjectIdVector();
    std::vector <DataType> getDataTypeVector();
    std::vector <ObjectId> getObjectIdVector();
    std::vector <ObjectType> getObjectTypeVector();
    void setDataTypeVector(std::vector <DataType> dt);
    void setObjectIdVector(std::vector <ObjectId> oid);
    void setObjectTypeVector(std::vector <ObjectType> o);
    void createOutputTree();
    void printOutputTree();
    ObjectId* returnNode(std::string address);
    std::vector<ObjectId> divideOidIntoObjects(ObjectId o);
    std::vector<ObjectId*> findChilds(std::vector<ObjectId> objects, std::string name);
    void createOIDFromObjectType();
    void getBerEncodedLeaf(std::string pathToLeaf, std::string value);
};


#endif //SNMP_PARSER_H
