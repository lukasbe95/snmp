//
// Created by lukas on 22.10.18.
//

#include "parser.h"
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <regex>
#include <sstream>
#include "structures.h"
using namespace std;

struct oid{
    string name;
    vector<string> v;
};

std::vector<std::string> split(std::string s, std::string delimiter){
    size_t pos = 0;
    std::string token;
    std::vector<std::string> v;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        if(token !=""&&token!=" "){
            v.push_back(token);
        }
        s.erase(0, pos + 1);
    }
    return v;
}
Parser::Parser(std::string p, std::string fn) {
    this->pathToFile = std::move(p);
    this->filename = std::move(fn);
}
void Parser::readFile() {
    std::ifstream file;
    file.open(pathToFile+filename);
    for(std::string line; getline(file,line);){
        if (line.find("--")<1000000){
            wholeFile += line.substr(0,line.find("--"))+" ";
        } else{
            wholeFile += line+" ";
        }
    }
    std::regex whitespaces("\\s+|,\\s*");
    splittedFile = split(regex_replace(wholeFile,whitespaces," ")," ");
//    for (auto x:splittedFile){
//        cout<<x<<endl;
//    }
}
void Parser::searchForImports() {
    vector<string> imports;
    string importfile;
    bool process = false; // here starts
    for(std::vector<std::string>::size_type i = 0; i != splittedFile.size(); i++) {
        if (splittedFile[i].find("IMPORTS")<100000){
            process = true;
            continue;
        } else if((splittedFile[i].find("FROM")<10000)&&process){
            importfile = splittedFile[i+1];
            break;
        }
        if(process){
            imports.push_back(splittedFile[i]);
        }
    }
    if(process){
        this->searchInformationsInImports("/usr/local/share/snmp/mibs/",importfile,imports);
    }
    }

void Parser::searchInformationsInImports(std::string path, std::string filename,
                                         std::vector<std::string> elementsToImport) {
    Parser* p = new Parser(path,filename);
    p->readFile();
    p->searchForImports();
//    p->searchForOID();
//    for(auto item: elementsToImport){
//        p->searchForDT(item);
//    }
}
void Parser::searchForDT(std::string name) {
    std::string regexWithName = "\\s*" + name + "\\s+::=\\s+";
    std::smatch mName;
    std::regex matchDTName(regexWithName);
    std::string temp = std::move(wholeFile);
    while(regex_search(temp,mName,matchDTName)){
        cout<<mName[0]<<endl;
    }
}
void Parser::addObjectId(ObjectId obj) {
    this->o.push_back(obj);
}
void Parser::addObjectType(ObjectType obj) {
    this->ot.push_back(obj);
}
void Parser::searchForOID() {
    for(std::vector<std::string>::size_type i = 0; i != splittedFile.size(); i++) {
        if (splittedFile[i].find("OBJECT")<10000&&splittedFile[i+1].find("IDENTIFIER")<10000
            &&splittedFile[i+2].find("::=")<10000){
            ObjectId o;
            o.setName(splittedFile[i-1]);
            cout<<o.getName()<<endl;
            int j = 4;
            while((i+j<splittedFile.size())&&splittedFile[i+j].find("}")>1000){
                o.appendPath(splittedFile[i+j]);
                j++;
            }
            i += j;
            o.setValue(o.getPath().back());
            this->addObjectId(o);
        }
    }
}
void Parser::searchForOT() {
    for(std::vector<std::string>::size_type i = 0; i != splittedFile.size(); i++) {
        if (splittedFile[i].find("OBJECT-TYPE")<10000&&splittedFile[i+1].find("SYNTAX")<10000){
            ObjectType o;
            o.setName(splittedFile[i-1]);
            int j = 2;
            string temporarySyntaxValue = "";
            //set syntax
            while((i+j<splittedFile.size())&&splittedFile[i+j].find("ACCESS")>1000){
                temporarySyntaxValue += splittedFile[i+j] + " ";
                j++;
            }
            o.setSyntax(temporarySyntaxValue);
            //set access
            j++;
            string temporaryAccessValue = "";
            while((i+j<splittedFile.size())&&splittedFile[i+j].find("STATUS")>1000){
                temporaryAccessValue += splittedFile[i+j] + " ";
                j++;
            }
            o.setAccess(temporaryAccessValue);
            //set status
            j++;
            string temporaryStatusValue = "";
            while((i+j<splittedFile.size())&&splittedFile[i+j].find("DESCRIPTION")>1000){
                temporaryStatusValue += splittedFile[i+j] + " ";
                j++;
            }
            o.setStatus(temporaryStatusValue);
            //set oid
            while((i+j<splittedFile.size())&&splittedFile[i+j].find("::=")>1000) j++;
            j+= 2; //must ignore ::= and {
            while((i+j<splittedFile.size())&&splittedFile[i+j].find("}")>1000) {
                o.appendOID(splittedFile[i+j]);
                j++;
            }
            i += j;
            o.printOT();
            this->addObjectType(o);
        }
    }
//    std::regex matchOT(".+\\s*OBJECT-TYPE");
//    std::vector<ObjectType> v;
//    std::smatch mOT;
//    std::string temp = std::move(wholeFile);
//    while(regex_search(temp,mOT,matchOT)){
//        ObjectType temporaryObjectTypeInstance;
//        temporaryObjectTypeInstance.setName(mOT[0].str().substr(0,mOT[0].str().find("OBJECT-TYPE")));
//        temp = mOT.suffix();
//        this->parseOTSyntax(temp,temporaryObjectTypeInstance);
//        this->parseOTAccess(temp,temporaryObjectTypeInstance);
//        this->parseOTStatus(temp,temporaryObjectTypeInstance);
//        this->parseOTDescription(temp,temporaryObjectTypeInstance);
//        this->parseOToid(temp,temporaryObjectTypeInstance);
//        this->addObjectType(temporaryObjectTypeInstance);
//    }
}
void Parser::parseOTAccess(std::string &file, ObjectType &o) {
    std::regex matchAccess("ACCESS\\s*.+");
    std::smatch mAccess;
    if(regex_search(file,mAccess,matchAccess)){
        o.setAccess(mAccess[0].str().substr(8,mAccess[0].str().length()));
        file = mAccess.suffix();
    }
}
void Parser::parseOTSyntax(std::string &file, ObjectType &o) {
    std::smatch mSyntax;
    std::regex matchSyntaxNormal("SYNTAX\\s.+");
    std::regex matchSyntaxInt("SYNTAX\\s.+\\{.*?\\n}");
    std::regex matchSyntaxIntMultiline("SYNTAX\\s.+\\{");
    if(regex_search(file,mSyntax,matchSyntaxInt)) {
        o.setSyntax(mSyntax[0].str().substr(7,mSyntax.length()));
        file = mSyntax.suffix();
    }else if(regex_search(file,mSyntax,matchSyntaxInt)){
        o.setSyntax(mSyntax[0].str().substr(7,mSyntax.length()));
        file = mSyntax.suffix();
    }else if (regex_search(file,mSyntax,matchSyntaxNormal)){
        o.setSyntax(mSyntax[0].str().substr(7,mSyntax.length()));
        file = mSyntax.suffix();
    }
}
void Parser::parseOTDescription(std::string &file, ObjectType &o) {
    std::smatch mDescription;
    std::regex matchDescription("\\s*?DESCRIPTION\\s*");
    if(regex_search(file,mDescription,matchDescription)){
        o.setDescription("");
        file = mDescription.suffix();
    }
}
void Parser::parseOToid(std::string &file, ObjectType &o) {
    std::smatch mDescription;
    std::regex matchEndOfOT("::=\\s*\\{.*?}");
    if(regex_search(file,mDescription,matchEndOfOT)){
        std::string collectionToParse = mDescription[0].str();
        collectionToParse.erase(0,collectionToParse.find("{")+1);
        std::vector<std::string> v = split(collectionToParse," ");
        o.setOID(v);
        file = mDescription.suffix();
    }
}
void Parser::parseOTStatus(std::string &file, ObjectType &o) {
    std::smatch mStatus;
    std::regex matchStatus("STATUS\\s*.+");
    if(regex_search(file,mStatus,matchStatus)){
        o.setStatus(mStatus[0].str().substr(8,mStatus[0].str().length()));
        file = mStatus.suffix();
    }
}