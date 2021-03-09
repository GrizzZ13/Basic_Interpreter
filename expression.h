#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "variables.h"
#include <string>

using std::string;

enum ExpressionType {Cons, Iden, Comp};

class Expression
{
public:
    Expression();
    virtual ~Expression();
    virtual int eval(map<string, int> &v) = 0;
    virtual string toString() = 0;
    virtual ExpressionType type() = 0;

};


class ConstantExp: public Expression
{
public:
    ConstantExp(int val);

    virtual int eval(map<string, int> &v);
    virtual string toString();
    virtual ExpressionType type();

    int value;
};


class IdentifierExp: public Expression
{
public:
    IdentifierExp(string name);

    virtual int eval(map<string, int> &v);
    virtual string toString();
    virtual ExpressionType type();

    string name;
};

class CompoundExp: public Expression
{
public:
    CompoundExp(string op, Expression *left, Expression *right);

    virtual ~CompoundExp();
    virtual int eval(map<string, int> &v);
    virtual string toString();
    virtual ExpressionType type();

    string op;
    Expression *left, *right;
};



#endif // EXPRESSION_H
