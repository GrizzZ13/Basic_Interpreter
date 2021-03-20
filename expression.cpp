#include "expression.h"
#include <QDebug>

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
    qDebug() << QString::fromStdString(this->name);
    return v[this->name];
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
    // assignemt
    if(op == "="){
        qDebug() << "in compound node: assignment begins";
        if(this->left->type() == Iden){
           string var = this->left->toString();

           auto itr = v.find(var);
           if(itr == varTable.end()){
               v.insert(pair<string, int>(var, 0x3f3f3f3f));
           }

           v[var] = this->right->eval(v);
           return 1;
        }
        else return 0;
    }
    // compare
    if(op == "<")
        return (left->eval(v) < right->eval(v)) ? 1 : 0;
    if(op == ">")
        return (left->eval(v) > right->eval(v)) ? 1 : 0;
    if(op == "<=")
        return (left->eval(v) <= right->eval(v)) ? 1 : 0;
    if(op == ">=")
        return (left->eval(v) >= right->eval(v)) ? 1 : 0;
    if(op == "==")
        return (left->eval(v) == right->eval(v)) ? 1 : 0;

    return 0x3f3f3f3f;
}

string CompoundExp::toString(){
    if(op == "=") return left->toString() + op + right->toString();
    else return "(" + left->toString() + op + right->toString() + ")";
}

ExpressionType CompoundExp::type(){
    return Comp;
}





