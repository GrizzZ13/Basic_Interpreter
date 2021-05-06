#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "variables.h"
#include "myexception.h"
#include "Evaluation.h"
#include <string>

using std::string;

enum ExpressionType {Cons, Iden, Comp, String};

class Expression
{
public:
    Expression();
    virtual ~Expression();
    virtual Evaluation eval() = 0;
    virtual string toString() = 0;
    virtual ExpressionType type() = 0;
};

class ConstantExp: public Expression
{
public:
    ConstantExp(int val);

    virtual Evaluation eval();
    virtual string toString();
    virtual ExpressionType type();

    int value;
};

class IdentifierExp: public Expression
{
public:
    IdentifierExp(string name);

    virtual Evaluation eval();
    virtual string toString();
    virtual ExpressionType type();

    string name;
};

class CompoundExp: public Expression
{
public:
    CompoundExp(string op, Expression *left, Expression *right);

    virtual ~CompoundExp();
    virtual Evaluation eval();
    virtual string toString();
    virtual ExpressionType type();

    string op;
    Expression *left, *right;
};

class StringExp: public Expression
{
public:
    StringExp(string _data);

    virtual ~StringExp(){};
    virtual Evaluation eval();
    virtual string toString();
    virtual ExpressionType type();

    string data;
};

#endif // EXPRESSION_H
