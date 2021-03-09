#include "expression.h"

Expression::Expression(){}
Expression::~Expression(){}

/*************************************************************/
ConstantExp::ConstantExp(int val){
    value = val;
}

int ConstantExp::eval(map<string, int> &v){
    return value;
}

string ConstantExp::toString(){
    string retVal = std::to_string(value);
    return retVal;
}

ExpressionType ConstantExp::type(){
    return Cons;
}

/*************************************************************/
IdentifierExp::IdentifierExp(string n){
    name = n;
}

string IdentifierExp::toString(){
    return name;
}

int IdentifierExp::eval(map<string, int> &v){
    auto itr = v.find(name);
    if(itr != v.end()) return itr->second;

    return 0x3f3f3f3f;
}

ExpressionType IdentifierExp::type(){
    return Iden;
}

/*************************************************************/
CompoundExp::CompoundExp(string opera, Expression *l, Expression *r){
    op = opera;
    left = l;
    right = r;
}

CompoundExp::~CompoundExp(){
    delete left;
    delete right;
}

int CompoundExp::eval(map<string, int> &v){
    if(op == "+") return left->eval(v) + right->eval(v);
    if(op == "-") return left->eval(v) - right->eval(v);
    if(op == "*") return left->eval(v) * right->eval(v);
    if(op == "/") return left->eval(v) / right->eval(v);

    return 0x3f3f3f3f;
}

string CompoundExp::toString(){
    if(op == "=") return left->toString() + op + right->toString();
    else return "(" + left->toString() + op + right->toString() + ")";
}

ExpressionType CompoundExp::type(){
    return Comp;
}





