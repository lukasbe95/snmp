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
    std::regex matchFirst("\\s*.+\\s*OBJECT-TYPE");
    std::smatch m;
    std::string temp = std::move(wholeFile);
    while(regex_search(temp,m,matchFirst)){
        cout<<m[0]<<endl;
        temp = m.suffix();
    }
}