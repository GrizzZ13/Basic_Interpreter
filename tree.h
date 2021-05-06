#ifndef TREE_H
#define TREE_H
#include <QDebug>
#include <QString>
#include "expression.h"
#include <stack>
#include <vector>

using std::stack;
using std::vector;

enum strType {constant, operand, variable, str};

struct binary{
    string data;// store data
    strType stp;// store string type

    binary(string s, strType t){
        data = s;
        stp = t;
    }
};

class Tree
{
public:
    Tree(string s);
    ~Tree();
    Evaluation eval();
    bool check(const stack<binary> &opStack, const string &op);
    ExpressionType type() const;

    Expression *root;
};

#endif // TREE_H
