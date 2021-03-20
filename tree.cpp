#include "tree.h"

Tree::Tree(string s)
{
    stack<binary> operandStack;
    stack<Expression*> variableStack;
    vector<binary> table;
    int strlen = s.length();
    string tmp;

    //split string
    for(int i = 0;i < strlen;){
        tmp = "";
        //constant
        if(s[i] <= '9' && s[i] >= '0'){
            while(s[i] <= '9' && s[i] >= '0' && i < strlen){
                tmp = tmp + s[i];
                ++i;
            }

            //debug
            table.push_back(binary(tmp, constant));
            qDebug() << i << "constant" << QString::fromStdString(tmp);
            continue;
        }
        //operand
        if(s[i]=='<'||s[i]=='>'||s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='/'||s[i]=='('||s[i]==')'||s[i]=='='){
            tmp = tmp + s[i];
            ++i;
            if(i<strlen && s[i]=='=' && (s[i-1]=='>' || s[i-1]=='<' || s[i-1]=='=')){
                tmp = tmp + s[i];
                ++i;
            }

            //debug
            table.push_back(binary(tmp, operand));
            qDebug() << i << "operand" << QString::fromStdString(tmp);
            continue;
        }
        //variable
        if((s[i]<='z'&&s[i]>='a')||(s[i]<='Z'&&s[i]>='A')){
            while((!(s[i]=='<'||s[i]=='>'||s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='/'||s[i]=='('||s[i]=='='||s[i]==')'))&&i<strlen){
                tmp = tmp + s[i];
                ++i;
            }

            //debug
            table.push_back(binary(tmp, variable));
            qDebug() << i << "variable" << QString::fromStdString(tmp);
            continue;
        }
    }
    //split done
    qDebug() << "spilt done";

    //construct an expression tree by using a stack
    int veclen = (int)table.size();
    for(int i = 0;i < veclen;++i){
        // variable or constant
        if(table[i].stp != operand){
            if(table[i].stp == variable){
                Expression* treeNode = new IdentifierExp(table[i].data);
                variableStack.push(treeNode);
            }
            else if(table[i].stp == constant){
                int constVal = std::stoi(table[i].data);
                qDebug() << "constant value" << constVal;//debug
                Expression* treeNode = new ConstantExp(constVal);
                variableStack.push(treeNode);
            }
        }
        else if(table[i].data != "(" && table[i].data != ")"){
            while(true){
                if(operandStack.empty() || operandStack.top().data == "("){
                    operandStack.push(table[i]);
                    break;
                }
                else if(check(operandStack, table[i].data)){
                    operandStack.push(table[i]);
                    break;
                }
                else{
                    binary tmpOp = operandStack.top();
                    operandStack.pop();

                    Expression *left, *right, *treeNode;
                    right = variableStack.top();
                    variableStack.pop();
                    left = variableStack.top();
                    variableStack.pop();
                    treeNode = new CompoundExp(tmpOp.data, left, right);
                    root = treeNode;
                    variableStack.push(treeNode);
                }
            }
        }
        //yu dao kuo hao
        else if(table[i].data == "("){
            operandStack.push(table[i]);
        }
        else if(table[i].data == ")"){
            while(!operandStack.empty() && operandStack.top().data != "("){
                binary tmpOp = operandStack.top();
                operandStack.pop();

                Expression *left, *right, *treeNode;
                right = variableStack.top();
                variableStack.pop();
                left = variableStack.top();
                variableStack.pop();
                treeNode = new CompoundExp(tmpOp.data, left, right);
                root = treeNode;
                variableStack.push(treeNode);
            }
            operandStack.pop();//pop "("
        }
    }
    while(!operandStack.empty()){
        binary tmpOp = operandStack.top();
        operandStack.pop();

        Expression *left, *right, *treeNode;
        right = variableStack.top();
        variableStack.pop();
        left = variableStack.top();
        variableStack.pop();
        treeNode = new CompoundExp(tmpOp.data, left, right);
        root = treeNode;
        variableStack.push(treeNode);
    }
    root = variableStack.top();

    qDebug() << "Tree: Tree constructed";
}

Tree::~Tree(){
    delete root;
}

bool Tree::check(const stack<binary> &opStack, const string &op){
    if(op=="*" || op=="/"){
        if(opStack.top().data == "+"|| opStack.top().data == "-"||
           opStack.top().data == "<"|| opStack.top().data == ">"||
           opStack.top().data == "=")
            return true;
    }
    else if(op=="+" || op=="-"){
        if(opStack.top().data == "<"|| opStack.top().data == ">"|| opStack.top().data == "=")
            return true;
    }
    return false;
}

int Tree::eval(map<string, int> &var){
    return root->eval(var);
}

ExpressionType Tree::type() const{
    return root->type();
}
