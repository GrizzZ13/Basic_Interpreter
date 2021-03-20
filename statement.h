#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <vector>
#include "expression.h"
#include "tree.h"
using std::vector;
using std::string;

//class statement
//{
//public:
//    statement();
//};

enum stateType {Remark, Input, Print, End, Let, Goto, If};

class Statement{
public:
    Statement();
    virtual ~Statement(){};
    virtual void execute(map<string, int> &v)=0;
    virtual stateType type()=0;
};

class remarkStatement:public Statement{
public:
    remarkStatement(){};
    ~remarkStatement(){};
    void execute(map<string, int> &v){};
    stateType type(){return Remark;}
};

class inputStatement:public Statement{
public:
    inputStatement(vector<string> &lineVec);
    ~inputStatement(){};
    void execute(map<string, int> &v);
    stateType type(){return Input;}
    void getValue(string &input);
    string getName();
private:
    string varName;
    int value;
};

class printStatement: public Statement{
public:
    printStatement(vector<string> &lineVec);
    ~printStatement(){delete exp;};
    void execute(map<string, int> &v);
    int getValue(map<string, int> &v);
    stateType type(){return Print;}
private:
    Tree *exp;
    int value;
};

class endStatement: public Statement{
public:
    endStatement(){};
    ~endStatement(){};
    void execute(map<string, int> &v);
    stateType type(){return End;}
};

class letStatement: public Statement{
public:
    letStatement(vector<string> &lineVec);
    ~letStatement(){delete exp;}
    void execute(map<string, int> &v);
    bool getSuccess(map<string, int> &v);
    stateType type(){return Let;}
private:
    Tree *exp;
    bool success;
};

class gotoStatement:public Statement{
public:
    gotoStatement(vector<string> &lineVec);
    ~gotoStatement(){delete exp;}
    void execute(map<string, int> &v);
    stateType type(){return Goto;}
    int toLine(map<string, int> &v);
private:
    Tree *exp;
    int nextLine;
};

class ifStatement:public Statement{
public:
    ifStatement(vector<string> &lineVec);
    ~ifStatement(){delete exp;}
    void execute(map<string, int> &v);
    stateType type(){return If;}
    int toLine(map<string, int> &v);
private:
    Tree *exp;
    int nextLine;
    bool flag;
};
#endif // STATEMENT_H
