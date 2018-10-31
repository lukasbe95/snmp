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
void Parser::searchForImports() {
    istringstream stream{wholeFile};
    string line;
    string imports;
    string importfile;
    bool process = false; // here starts
    while(getline(stream,line)){
        if (line.find("IMPORTS")<100000){
            process = true;
            continue;
        }else if((line.find(";")<10000)&&process){
            imports+=line;
            process = false;
        }
        if(process){
            imports+=line;
        }
    }
    regex multispace("\\s+|,\\s+");
    std::vector<std::string> v = split(regex_replace(imports,multispace," ")," ");
    for(auto i=0;i<=v.size();i++){
        if (v[i].find("FROM") < 1000 || v[i].find("OBJECT-TYPE") < 1000){
            v.erase(v.begin()+i);
        }
        if(v[i].find("RFC")<1000){
            importfile = v[i] + ".txt";
            v.erase(v.begin()+i);
        }
    }
    v.pop_back(); //workaround to fix
//    this->searchInformationsInImports("/usr/local/share/snmp/mibs/",filename);
}
void Parser::searchForOID() {
    std::regex declaration  (".+\\s+OBJECT\\sIDENTIFIER\\s+::=\\s*\\{.*?}");
    std::smatch m;
    std::string temp = std::move(wholeFile);
//    cout<<regex_search(wholeFile,m,declaration)<<endl;
    while(regex_search(temp,m,declaration)){
//        cout<<m[0]<<endl;
//        string d = m[0].str();
        ObjectId* o = new ObjectId();
        bool sasa = o->createObjectIdFromString(m[0].str());
        temp = m.suffix();
    }
}
void Parser::searchForOT() {
    std::regex matchOT(".+\\s*OBJECT-TYPE");
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
        v.push_back(o);
    }
}
void Parser::parseOTAccess(std::string &file, objectType &o) {
    std::regex matchAccess("ACCESS\\s*.+");
    std::smatch mAccess;
    if(regex_search(file,mAccess,matchAccess)){
        o.access = mAccess[0].str().substr(8,mAccess[0].str().length());
        file = mAccess.suffix();
    }
}
void Parser::parseOTSyntax(std::string &file, objectType &o) {
    std::smatch mSyntax;
    std::regex matchSyntaxNormal("SYNTAX\\s.+");
    std::regex matchSyntaxInt("SYNTAX\\s.+\\{.*?\\n}");
//    std::regex matchSyntaxIntPost("SYNTAX\\s.+\\{.*?\\n}");
    std::regex matchSyntaxIntMultiline("SYNTAX\\s.+\\{");
    if(regex_search(file,mSyntax,matchSyntaxInt)) {
        o.syntax = mSyntax[0].str().substr(7,mSyntax.length());
        file = mSyntax.suffix();
    }else if(regex_search(file,mSyntax,matchSyntaxInt)){
        o.syntax = mSyntax[0].str().substr(7,mSyntax.length());
        file = mSyntax.suffix();
    }else if (regex_search(file,mSyntax,matchSyntaxNormal)){
        o.syntax = mSyntax[0].str().substr(7,mSyntax.length());
        file = mSyntax.suffix();
    }
//    std::cout<<file.substr(file.find("SYNTAX"),file.find("}"))<<endl;
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
    std::regex matchEndOfOT("::=\\s*\\{.*?}");
    if(regex_search(file,mDescription,matchEndOfOT)){
        std::string collectionToParse = mDescription[0].str();
        collectionToParse.erase(0,collectionToParse.find("{")+1);
        std::vector<std::string> v = split(collectionToParse," ");
        o.oid.insert(o.oid.end(),v.begin(),v.end());
        file = mDescription.suffix();
    }
}

void Parser::parseOTStatus(std::string &file, objectType &o) {
    std::smatch mStatus;
    std::regex matchStatus("STATUS\\s*.+");
    if(regex_search(file,mStatus,matchStatus)){
        o.status = mStatus[0].str().substr(8,mStatus[0].str().length());
        file = mStatus.suffix();
    }
}