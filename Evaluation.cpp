//
// Created by 19856 on 2021/4/29.
//

#include "Evaluation.h"

Evaluation::Evaluation(const TypeEval &_typeEval, const string &_val_str) {
    typeEval = _typeEval;
    val_str = _val_str;
}

Evaluation::Evaluation(const TypeEval &_typeEval, const int &_val_int) {
    typeEval = _typeEval;
    val_int = _val_int;
}

Evaluation::Evaluation(const TypeEval &_typeEval, const bool &_val_bool) {
    typeEval = _typeEval;
    val_bool = _val_bool;
}

Evaluation::Evaluation(const string &_val_str){
    typeEval = STRING;
    val_str = _val_str;
}

Evaluation::Evaluation(const int &_val_int){
    typeEval = INT;
    val_int = _val_int;
}

Evaluation::Evaluation(const bool &_val_bool){
    typeEval = BOOLEAN;
    val_bool = _val_bool;
}

Evaluation operator+(const Evaluation &e1, const Evaluation &e2) {
    if(e1.typeEval==INT && e2.typeEval==INT)
        return Evaluation(INT, e1.val_int+e2.val_int);
    if(e1.typeEval==STRING && e2.typeEval==STRING)
        return Evaluation(STRING, e1.val_str+e2.val_str);
    throw myException("type error (+)");
}

Evaluation operator-(const Evaluation &e1, const Evaluation &e2) {
    if(e1.typeEval==INT && e2.typeEval==INT)
        return Evaluation(INT, e1.val_int-e2.val_int);
    throw myException("type error (-)");
}

Evaluation operator*(const Evaluation &e1, const Evaluation &e2) {
    if(e1.typeEval==INT && e2.typeEval==INT)
        return Evaluation(INT, e1.val_int*e2.val_int);
    throw myException("type error (*)");
}

Evaluation operator/(const Evaluation &e1, const Evaluation &e2) {
    if(e1.typeEval==INT && e2.typeEval==INT)
        return Evaluation(INT, e1.val_int/e2.val_int);
    throw myException("type error (/)");
}

Evaluation operator>(const Evaluation &e1, const Evaluation &e2) {
    if(e1.typeEval==INT && e2.typeEval==INT)
        return Evaluation(BOOLEAN, e1.val_int>e2.val_int);
    throw myException("type error (>)");
}

Evaluation operator<(const Evaluation &e1, const Evaluation &e2) {
    if(e1.typeEval==INT && e2.typeEval==INT)
        return Evaluation(BOOLEAN, e1.val_int<e2.val_int);
    throw myException("type error (<)");
}

Evaluation operator>=(const Evaluation &e1, const Evaluation &e2) {
    if(e1.typeEval==INT && e2.typeEval==INT)
        return Evaluation(BOOLEAN, e1.val_int>=e2.val_int);
    throw myException("type error (>=)");
}

Evaluation operator<=(const Evaluation &e1, const Evaluation &e2) {
    if(e1.typeEval==INT && e2.typeEval==INT)
        return Evaluation(BOOLEAN, e1.val_int<=e2.val_int);
    throw myException("type error (<=)");
}

Evaluation operator==(const Evaluation &e1, const Evaluation &e2) {
    if(e1.typeEval==INT && e2.typeEval==INT)
        return Evaluation(BOOLEAN, e1.val_int==e2.val_int);
    else if(e1.typeEval==STRING && e2.typeEval==STRING)
        return Evaluation(BOOLEAN, e1.val_str==e2.val_str);
    else if(e1.typeEval==BOOLEAN && e2.typeEval==BOOLEAN)
        return Evaluation(BOOLEAN, e1.val_bool==e2.val_bool);
    throw myException("type error (==)");
}

string Evaluation::toString() const {
    if(typeEval==INT)
       return std::to_string(val_int);
    else if(typeEval==STRING)
        return val_str;
    else
        return val_bool ? "1" : "0";
}

bool Evaluation::toBoolean() const {
    if(typeEval==BOOLEAN) return val_bool;
    throw myException("not boolean variable");
}

bool Evaluation::equalZero() const {
    return (typeEval == INT && val_int == 0);
}

int Evaluation::getValInt() const {
    if(typeEval==INT) return val_int;
    throw myException("not calculable value");
}

TypeEval Evaluation::getTypeEval() const {
    return typeEval;
}
