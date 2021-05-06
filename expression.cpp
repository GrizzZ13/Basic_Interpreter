#include "expression.h"
#include "variables.h"
#include <QDebug>

Expression::Expression(){}
Expression::~Expression(){}

/*************************************************************/
ConstantExp::ConstantExp(int val){
    value = val;
}

Evaluation ConstantExp::eval(){
    return Evaluation(INT, value);
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

Evaluation IdentifierExp::eval(){
    //qDebug() << QString::fromStdString(this->name);
    auto itr_int=varTable.find(this->name);
    if(itr_int == varTable.end()) throw(myException("undefined variable"));
    if(varTable[this->name]==0x7fffffff){
        return Evaluation(STRING, strTable[this->name]);
    }
    else
        return Evaluation(INT, varTable[this->name]);
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

Evaluation CompoundExp::eval(){
    if(op == "+") return left->eval() + right->eval();
    if(op == "-") return left->eval() - right->eval();
    if(op == "*") return left->eval() * right->eval();
    if(op == "/") {
        if(right->eval().equalZero()) throw(myException("division by zero"));
        return left->eval() / right->eval();
    }
    if(op == "**"){
        int exp1 = left->eval().getValInt();
        int exp2 = right->eval().getValInt();
        if(exp2 < 0) throw(myException("exp2 should not be less than 0"));
        int tmp = 1;
        while(exp2 != 0){
            tmp = tmp * exp1;
            exp2--;
        }
        return Evaluation(INT, tmp);
    }
    // assignment
    if(op == "="){
        //qDebug() << "in compound node: assignment begins";
        if(this->left->type() == Iden){
            string var = this->left->toString();
            if(varTable.find(var) == varTable.end()){
                throw myException("undefined variable");
            }
           if(typeTable.find(var)!=typeTable.end()){
               if(typeTable[var]==_INT && this->right->eval().getTypeEval()!=INT)
                   throw myException("cannot cast non-int to int");
               if(typeTable[var]==_STRING && this->right->eval().getTypeEval()!=STRING)
                   throw myException("cannot cast npn-string to string");
           }
           if(this->right->eval().getTypeEval()==INT){
               if(typeTable.find(var)==typeTable.end())
                   typeTable.insert(pair<string, variableType>(var, _INT));
               varTable[var] = this->right->eval().getValInt();
               return Evaluation(BOOLEAN, true);
           }
           else if(this->right->eval().getTypeEval()==STRING){
               if(typeTable.find(var)==typeTable.end())
                   typeTable.insert(pair<string, variableType>(var, _STRING));
               strTable[var] = this->right->eval().toString();
               return Evaluation(BOOLEAN, true);
           }
           else
               throw myException("cannot assign successfully");
        }
        else throw(myException("invalid assignment"));
    }
    // compare
    if(op == "<")
        return left->eval() < right->eval();
    if(op == ">")
        return left->eval() > right->eval();
    if(op == "<=")
        return left->eval() <= right->eval();
    if(op == ">=")
        return left->eval() >= right->eval();
    if(op == "==")
        return left->eval() == right->eval();

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

/*************************************************************/

StringExp::StringExp(string _data) {
    data = _data;
}

Evaluation StringExp::eval() {
    return Evaluation(STRING, data);
}

string StringExp::toString() {
    return data;
}

ExpressionType StringExp::type() {
    return String;
}




