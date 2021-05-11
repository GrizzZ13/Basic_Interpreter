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

struct intAndbool{
    int i;
    bool b;

    intAndbool(int x, bool y):i(x),b(y){}
};

enum stateType {Remark, Input, Print, End, Let, Goto, If, Inputs, Printf};

class Statement{
public:
    Statement();
    virtual ~Statement(){};
    virtual void execute()=0;
    virtual stateType type()=0;
};

class remarkStatement:public Statement{
public:
    remarkStatement(vector<string> &lineVec);
    ~remarkStatement(){};
    void execute(){};
    stateType type(){return Remark;}

    string remark;
};

class inputStatement:public Statement{
public:
    inputStatement(vector<string> &lineVec);
    ~inputStatement(){};
    void execute();
    stateType type(){return Input;}
    void getValue(string &input);
    string getName();

    string varName;
    int value;
};

class inputsStatement:public Statement{
public:
    inputsStatement(vector<string> &lineVec);
    ~inputsStatement(){};
    void execute();
    stateType type(){return Inputs;}
    void getValue(string &input);
    string getName();

    string varName;
    string value;
};

class printStatement: public Statement{
public:
    printStatement(vector<string> &lineVec);
    ~printStatement(){delete exp;};
    void execute();
    string getValue();
    stateType type(){return Print;}

    Tree *exp;
    string value;
};

class printfStatement{
public:
    printfStatement(vector<string> &lineVec);

    string getValue();
    stateType type(){return Printf;}
    bool isString(string &str);
    bool validString(string &str);
    string trimString(string str);// 删除开头和结尾的引号
    void trimSpace(vector<string> &lineVec); // 去除空格

    vector<string> output;
};

class endStatement: public Statement{
public:
    endStatement(){};
    ~endStatement(){};
    void execute();
    stateType type(){return End;}
};

class letStatement: public Statement{
public:
    letStatement(vector<string> &lineVec);
    ~letStatement(){delete exp;}
    void execute();
    bool getSuccess();
    stateType type(){return Let;}

    Tree *exp;
    bool success;
};

class gotoStatement:public Statement{
public:
    gotoStatement(vector<string> &lineVec);
    ~gotoStatement(){delete exp;}
    void execute();
    stateType type(){return Goto;}
    int toLine();

    Tree *exp;
    int nextLine;
};

class ifStatement:public Statement{
public:
    ifStatement(vector<string> &lineVec);
    ~ifStatement(){delete exp;}
    void execute();
    stateType type(){return If;}
    intAndbool toLine();

    Tree *exp;
    int nextLine;
    bool flag;
};
#endif // STATEMENT_H
