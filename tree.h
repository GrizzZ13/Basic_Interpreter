#ifndef TREE_H
#define TREE_H

#include "expression.h"
#include "variables.h"
#include <stack>
#include <vector>

using std::stack;
using std::vector;

class Tree
{
public:
    Tree(string s);
    ~Tree();
    int eval(map<string, int> var);
private:
    Expression *root;
};

#endif // TREE_H
