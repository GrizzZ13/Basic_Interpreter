#include "expression.h"
#include <qDebug>

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
    //qDebug() << QString::fromStdString(this->name);
    auto itr=v.find(this->name);
    if(itr == v.end()) throw(myException("undefined variable"));
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
    if(op == "/") {
        if(right->eval(v) == 0) throw(myException("division by zero"));
        return left->eval(v) / right->eval(v);
    }
    if(op == "**"){
        int exp1 = left->eval(v);
        int exp2 = right->eval(v);
        if(exp2 < 0) throw(myException("exp2 should not be less than 0"));
        int tmp = 1;
        while(exp2 != 0){
            tmp = tmp * exp1;
            exp2--;
        }
        return tmp;
    }
    // assignemt
    if(op == "="){
        //qDebug() << "in compound node: assignment begins";
        if(this->left->type() == Iden){
           string var = this->left->toString();

           auto itr = v.find(var);
           if(itr == v.end()){
               throw(myException("undifined variable"));
           }

           v[var] = this->right->eval(v);
           return 1;
        }
        else throw(myException("invalid assignment"));
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

    throw(myException("invalid operand"));
}

string CompoundExp::toString(){
//    if(op == "=") return left->toString() + op + right->toString();
//    else return "(" + left->toString() + op + right->toString() + ")";
    return op;
}

ExpressionType CompoundExp::type(){
    return Comp;
}
