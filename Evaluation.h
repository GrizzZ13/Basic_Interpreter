//
// Created by 19856 on 2021/4/29.
//

#ifndef BASICINTREPRETER_EVALUATION_H
#define BASICINTREPRETER_EVALUATION_H

#include <string>
#include "myexception.h"
using std::string;

enum TypeEval {BOOLEAN, INT, STRING};

class Evaluation {
private:
    TypeEval typeEval;
    string val_str;
    int val_int;
    bool val_bool;
public:
    Evaluation(const TypeEval &_typeEval, const string &_val_str);
    Evaluation(const TypeEval &_typeEval, const int &_val_int);
    Evaluation(const TypeEval &_typeEval, const bool &_val_bool);
    Evaluation(const string &_val_str);
    Evaluation(const int &_val_int);
    Evaluation(const bool &_val_bool);

    string toString() const;
    bool toBoolean() const;
    bool equalZero() const;
    int getValInt() const;
    TypeEval getTypeEval() const;

    friend Evaluation operator+  (const Evaluation &e1, const Evaluation &e2);
    friend Evaluation operator-  (const Evaluation &e1, const Evaluation &e2);
    friend Evaluation operator*  (const Evaluation &e1, const Evaluation &e2);
    friend Evaluation operator/  (const Evaluation &e1, const Evaluation &e2);
    friend Evaluation operator>  (const Evaluation &e1, const Evaluation &e2);
    friend Evaluation operator<  (const Evaluation &e1, const Evaluation &e2);
    friend Evaluation operator>= (const Evaluation &e1, const Evaluation &e2);
    friend Evaluation operator<= (const Evaluation &e1, const Evaluation &e2);
    friend Evaluation operator== (const Evaluation &e1, const Evaluation &e2);
};

Evaluation operator+  (const Evaluation &e1, const Evaluation &e2);
Evaluation operator-  (const Evaluation &e1, const Evaluation &e2);
Evaluation operator*  (const Evaluation &e1, const Evaluation &e2);
Evaluation operator/  (const Evaluation &e1, const Evaluation &e2);;
Evaluation operator>  (const Evaluation &e1, const Evaluation &e2);
Evaluation operator<  (const Evaluation &e1, const Evaluation &e2);
Evaluation operator>= (const Evaluation &e1, const Evaluation &e2);
Evaluation operator<= (const Evaluation &e1, const Evaluation &e2);
Evaluation operator== (const Evaluation &e1, const Evaluation &e2);

#endif //BASICINTREPRETER_EVALUATION_H
