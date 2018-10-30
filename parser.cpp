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
#include "structures.h"
using namespace std;

struct oid{
    string name;
    vector<string> v;
};


Parser::Parser(std::string p, std::string fn) {
    this->pathToFile = std::move(p);
    this->filename = std::move(fn);
}
// reading file and preprocessing comments
void Parser::readFile() {
    std::ifstream file;
    file.open(pathToFile+filename);
    for(std::string line; getline(file,line);){
        if (line.find("--")<1000000){
            wholeFile += line.substr(0,line.find("--"))+'\n';
        } else{
            wholeFile += line+'\n';
        }
    }
}
void Parser::searchForOID() {
//    cout<<wholeFile<<endl;
    std::regex declaration  (".+\\s+OBJECT\\sIDENTIFIER\\s+::=\\s*\\{.*?}");
    std::smatch m;
    std::string temp = std::move(wholeFile);
    cout<<regex_search(wholeFile,m,declaration)<<endl;
    while(regex_search(temp,m,declaration)){
        cout<<m[0]<<endl;
        temp = m.suffix();
    }
}
void Parser::searchForOT() {
    std::regex matchOT(".+\\s*OBJECT-TYPE");
//    std::regex matchDescriptionEnd("\\s*[^::=]");
    std::vector<objectType> v;
    std::smatch mOT;
    std::string temp = std::move(wholeFile);
    while(regex_search(temp,mOT,matchOT)){
        objectType o;
        o.name = mOT[0].str().substr(0,mOT[0].str().find("OBJECT-TYPE"));
        temp = mOT.suffix();
        this->parseOTSyntax(temp,o);
        this->parseOTAccess(temp,o);
        this->parseOTStatus(temp,o);
        this->parseOTDescription(temp,o);
        this->parseOToid(temp,o);
//        cout<<o.name<<endl;
        cout<<o.syntax<<endl;
//        cout<<o.access;
//        cout<<o.status;
//        cout<<o.description;
//        cout<<o.oid;
        v.push_back(o);
    }
}
void Parser::parseOTAccess(std::string &file, objectType &o) {
    std::regex matchAccess("\\s*?ACCESS\\s*.+");
    std::smatch mAccess;
    if(regex_search(file,mAccess,matchAccess)){
        o.access = mAccess[0];
        file = mAccess.suffix();
    }
}
void Parser::parseOTSyntax(std::string &file, objectType &o) {
    std::smatch mSyntax;
    std::regex matchSyntaxNormal("SYNTAX\\s.+");
    std::regex matchSyntaxInt("SYNTAX\\s.+\\{.*?\\n}");
    std::regex matchSyntaxIntMultiline("SYNTAX\\s.+\\{");
    if(regex_search(file,mSyntax,matchSyntaxInt)) {

    }else if(regex_search(file,mSyntax,matchSyntaxNormal)){
        o.syntax = mSyntax[0].str().substr(7,mSyntax.length());
        file = mSyntax.suffix();
    }else if (regex_search(file,mSyntax,matchSyntaxNormal)){

    }
}
void Parser::parseOTDescription(std::string &file, objectType &o) {
    std::smatch mDescription;
    std::regex matchDescription("\\s*?DESCRIPTION\\s*");
    if(regex_search(file,mDescription,matchDescription)){
//        cout<<mDescription[0];
        o.description = "";
        file = mDescription.suffix();
    }
}
void Parser::parseOToid(std::string &file, objectType &o) {
    std::smatch mDescription;
    std::regex matchEndOfOT("\\s+::=\\s*\\{.*?}");
    if(regex_search(file,mDescription,matchEndOfOT)){
        o.oid.push_back(mDescription[0]);
        file = mDescription.suffix();
    }
}
void Parser::parseOTStatus(std::string &file, objectType &o) {
    std::smatch mStatus;
    std::regex matchStatus("\\s*?STATUS\\s*.+");
    if(regex_search(file,mStatus,matchStatus)){
        o.status = mStatus[0];
        file = mStatus.suffix();
    }
}