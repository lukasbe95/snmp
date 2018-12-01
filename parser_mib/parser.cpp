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
#include <list>
#include "structures.h"
using namespace std;


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
    v.push_back(s);
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
    file.close();
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
    Parser* p = new Parser(path,filename+".txt");
    p->readFile();
    p->searchForImports();
    p->searchForOID();
    p->searchForOT();
    p->searchForDT();
    for (auto oid: p->getObjectIdVector()){
        this->addObjectId(oid);
    }
    for (auto ot:p->getObjectTypeVector()){
        this->addObjectType(ot);
    }
    for (DataType dt:p->getDataTypeVector()){
        if(std::find(elementsToImport.begin(),elementsToImport.end(),dt.getName())!=elementsToImport.end()){
            this->addDataType(dt);
        }
    }


}
void Parser::searchForDT() {
    for(std::vector<std::string>::size_type i = 0; i != splittedFile.size(); i++) {
        if (splittedFile[i].find("::=")<10000){
            if (splittedFile[i+1].find("SEQUENCE")<10000){
                DataType d;
                int j = 3;
                while ((i+j<splittedFile.size())&&(splittedFile[i+j].find("}")>10000)) {
                    d.appendSequence(splittedFile[i+j]);
                    j++;
                }
                d.setName(splittedFile[i-1]);
                i +=j;
                this->addDataType(d);
            } else if (splittedFile[i+1].find("CHOICE")<10000){
                int j = 3;
                while ((i+j<splittedFile.size())&&(splittedFile[i+j].find("}")>10000)) {
                    j++;
                }
                i +=j;
            } else if (splittedFile[i+1].find("[")<10000){
                DataType d;
                d.setName(splittedFile[i-1]);
                d.setAccess(splittedFile[i+1].substr(1,splittedFile[i+1].size()));
                d.setAccessNum(splittedFile[i+2].substr(0,splittedFile[i+2].size()-1));
                int j = 4;
                if (splittedFile[i+j+1].find("(")>10000 && splittedFile[i+j+2].find("(")>10000){
                    d.setBaseType(splittedFile[i+j]+" "+splittedFile[i+j+1]);
                    this->addDataType(d);
                    continue;
                }
                int x;
                d.setBaseType(this->returnTypeAndSetLength(splittedFile[i+j],x));
                j += x;
                if (splittedFile[i+j].find("SIZE")<10000){
                    d.setSize(splittedFile[i+j+1].substr(1,splittedFile[i+j+1].find(")")-1));
                } else {
                    std::string min_v = splittedFile[i+j].substr(1,splittedFile[i+j].find(".")-1);
                    std::string max_v = splittedFile[i+j].substr(splittedFile[i+j].find(".")+2,splittedFile[i+j].size()-1);
                    d.setRange(min_v,max_v);
                }
                i += j;
                this->addDataType(d);
            } else {
                int x = 0;
                if (this->returnTypeAndSetLength(splittedFile[i+1],x) != ""){
                    DataType d;
                    d.setName(splittedFile[i-1]);
                    d.setBaseType(this->returnTypeAndSetLength(splittedFile[i+1],x));
                    this->addDataType(d);
                    i += x;
                }
            }
        }
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
//            std::regex base_regex("[[:digit:]]+..[[:digit:]]+");
//            std::regex base_regex("(?<start>\\d+)..(?<end>\\d+)");
//            std::smatch regex_match;
//            cout<<temporarySyntaxValue<<endl;
//            cout<<std::regex_match(temporarySyntaxValue,regex_match,base_regex)<<endl;
//            if(std::regex_match(temporarySyntaxValue,regex_match,base_regex)){
//                for(auto x: regex_match){
//                    cout<<x<<endl;
//                }
//                cout<<"cos"<<endl;
//            }
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
            //set description
            string temporaryDescriptionValue = "";
            j++;
            while((i+j<splittedFile.size())&&splittedFile[i+j].find("::=")>1000){
                temporaryDescriptionValue += splittedFile[i+j] + " ";
                j++;
            }
            o.setDescription(temporaryDescriptionValue);
            //set oid
            j+= 2; //must ignore ::= and {
            while((i+j<splittedFile.size())&&splittedFile[i+j].find("}")>1000) {
                o.appendOID(splittedFile[i+j]);
                j++;
            }
            i += j;
            this->addObjectType(o);
        }
    }
}
void Parser::addDataType(DataType obj) {
    this->d.push_back(obj);
}
std::string Parser::returnTypeAndSetLength(std::string t, int &n) {
    std::string toReturn;
    if (t =="INTEGER"){
        n = 1;
        toReturn = "INTEGER";
    } else if (t =="OCTET") {
        n = 2;
        toReturn = "OCTET STRING";
    } else if (t =="NULL") {
        n = 1;
        toReturn = "NULL";
    } else if (t =="OBJECT") {
        n = 2;
        toReturn = "OBJECT IDENTIFIER";
    } else {
        n = 0;
        toReturn = "";
    }
    return toReturn;


}
void Parser::printDataTypeVector() {
    for(std::vector<std::string>::size_type i = 0; i != d.size(); i++) {
        d[i].printDataType();
    }
}
void Parser::printObjectIdVector() {
    for(std::vector<std::string>::size_type i = 0; i != o.size(); i++) {
        o[i].printOID();
    }
}
void Parser::printObjectTypeVector() {
    for(std::vector<std::string>::size_type i = 0; i != ot.size(); i++) {
        ot[i].printOT();
    }
}
std::vector <DataType> Parser::getDataTypeVector() {
    return d;
}
std::vector <ObjectId> Parser::getObjectIdVector() {
    return o;
}
std::vector <ObjectType> Parser::getObjectTypeVector() {
    return ot;
}
void Parser::setDataTypeVector(std::vector<DataType> dt) {
    d = dt;
}
void Parser::setObjectIdVector(std::vector<ObjectId> oid) {
    o = oid;
}
void Parser::setObjectTypeVector(std::vector<ObjectType> o) {
    ot = o;
}
void Parser::createOutputTree() {
    ObjectId* start;
    for(std::vector<std::string>::size_type i = 0; i != o.size(); i++) {
        if (o[i].getPath().size() > 2){
            for (ObjectId o:this->divideOidIntoObjects(o[i])){
                this->addObjectId(o);
            }
            o.erase(o.begin()+i,o.begin()+i+1);
        }
    }

    for(std::vector<std::string>::size_type i = 0; i != o.size(); i++) {
        if (o[i].getName() == "iso"){
            start = new ObjectId(o[i].getName(),o[i].getValue());
            break;
        }
    }

    outputTree.push_back(start);
    std::list<ObjectId*>::iterator it = outputTree.begin();
    for (ObjectId* x:this->findChilds(o,(*it)->getName())){
        (*it)->addChild(x);
        outputTree.push_back(x);
    }
    do {
        *it++;
        for (ObjectId* y:this->findChilds(o,(*it)->getName())){
            (*it)->addChild(y);
            outputTree.push_back(y);
        }
    }while (it != outputTree.end()&&(outputTree.size() < o.size()));
}
void Parser::printOutputTree() {
    std::list<ObjectId*>::iterator it = outputTree.begin();
//    for (std::list<ObjectId*>::iterator it = outputTree.begin() ; it != outputTree.end(); ++it){
//        (*it)->printOID();
//    }
    cout<<"---------------------"<<endl;
    (*it)->printOID();
    cout<<"Childs: "<<endl;
    for (ObjectId* x: (*it)->getChilds()){
        x->printOID();
    }
    do {
        *it++;
        if (it == outputTree.end()){
            break;
        }
        cout<<"---------------------"<<endl;
        (*it)->printOID();
        cout<<"Childs: "<<endl;
        for (ObjectId* y: (*it)->getChilds()){
            y->printOID();
        }
    }while (it != outputTree.end());
}
std::vector<ObjectId*> Parser::findChilds(std::vector<ObjectId> objects, std::string name) {
    std::vector<ObjectId*> listToReturn;
    for(std::vector<std::string>::size_type i = 0; i != objects.size(); i++) {
        if (objects[i].getPath().empty()){
            continue;
        }
        if (objects[i].getPath()[0].find(name)<10000){
            listToReturn.push_back(new ObjectId(objects[i].getName(),objects[i].getValue()));
        }
    }
    return listToReturn;
}
std::vector<ObjectId> Parser::divideOidIntoObjects(ObjectId o) {
    std::vector<ObjectId> temp;
    for(auto i = 0; i != o.getPath().size()-1;i++) {
        ObjectId element;
        if (i == 0){
            element.setName(o.getPath()[i]);
            element.setValue("1");
            temp.push_back(element);
            continue;
        }
        element.setName(o.getPath()[i].substr(0,o.getPath()[i].find("(")));
        element.setValue(o.getPath()[i].substr(o.getPath()[i].find("(")+1,o.getPath()[i].find(")")-4));
        element.appendPath(o.getPath()[i-1]);
        element.appendPath(element.getValue());
        temp.push_back(element);
    }
    ObjectId last;
    last.setName(o.getName());
    last.setValue(o.getValue());
    last.appendPath(temp.back().getName());
    last.appendPath(last.getValue());
    temp.push_back(last);
    return temp;
}
ObjectId* Parser::returnNode(std::string address) {
    std::vector<std::string> adr = split(address,".");
    ObjectId* it = outputTree.front();
    ObjectId* tempPointer;
    ObjectId* empty;
    if(!(it->getName()==adr[0]||it->getValue()==adr[0])){
        cout<<"Error address"<<endl;
        return empty;
    }
    for(int i=1;i<adr.size();i++){
        for(ObjectId* child:it->getChilds()){
            if(child->getName()==adr[i]||child->getValue()==adr[i]){
                tempPointer = child;
                break;
            }
        }
        if(it == tempPointer){
            cout<<"Error address"<<endl;
            return empty;
        }
        it = tempPointer;
    }
    return it;
}
void Parser::createOIDFromObjectType() {
    for(ObjectType i: this->ot){
        ObjectId o;
        o.createOID(i.getName(),i.getOID().back(),i.getOID());
        this->addObjectId(o);
    }
}
void Parser::getBerEncodedLeaf(std::string pathToLeaf, std::string value) {
    ObjectId* leaf = this->returnNode(pathToLeaf);
    if (leaf->getChilds().empty()){
        ObjectType leaf_type;
        for(ObjectType o : ot){
            if(o.getName()==leaf->getName()){
                leaf_type = o;
                break;
            }
        }
        leaf_type.printOT();
        for (DataType x : d){
            if(leaf_type.getSyntax().find(x.getName())<10000){
                cout<<"Finded"<<endl;
                x.printDataType();
            }
        }
        if(leaf_type.checkIfValueIsPermitted(this->d,value)){
            //process value
            cout<<"Good value!"<<endl;
        } else {
            if(leaf_type.getIsSequenceType()){
                //process sequence or choice ??
                cout<<"Sequence"<<endl;
            }else{
                //send NULL
                cout<<"Null"<<endl;
            }
        }
    } else {
        std::cout << "It's not a leaf!!!"<<std::endl;
    }
}