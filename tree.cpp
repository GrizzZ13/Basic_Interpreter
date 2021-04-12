#include "tree.h"

Tree::Tree(string s)
{
    stack<binary> operandStack;
    stack<Expression*> variableStack;
    vector<binary> table;
    vector<binary> table_tmp;
    int powerNumber = 0;// total power operation number
    int powerNow = 0;// already handled power operation number
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
            //qDebug() << i << "constant" << QString::fromStdString(tmp);
            continue;
        }
        //operand
        else if(s[i]=='<'||s[i]=='>'||s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='/'||s[i]=='('||s[i]==')'||s[i]=='='){

            if(s[i]=='-' && (i==0 || s[i-1]=='(' || s[i-1]=='=')) table.push_back(binary("0", constant));//negative

            tmp = tmp + s[i];
            ++i;
            if(i<strlen && s[i]=='=' && (s[i-1]=='>' || s[i-1]=='<' || s[i-1]=='=')){// <=, >=, ==
                tmp = tmp + s[i];
                ++i;
            }else if(i<strlen && s[i]=='*' && s[i-1]=='*'){// **
                tmp = tmp + s[i];
                ++i;
            }

            //debug
            table.push_back(binary(tmp, operand));
            //qDebug() << i << "operand" << QString::fromStdString(tmp);
            continue;
        }
        //variable
        else if((s[i]<='z'&&s[i]>='a')||(s[i]<='Z'&&s[i]>='A')){
            while((!(s[i]=='<'||s[i]=='>'||s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='/'||s[i]=='('||s[i]=='='||s[i]==')'))&&i<strlen){
                tmp = tmp + s[i];
                ++i;
            }

            //debug
            table.push_back(binary(tmp, variable));
            //qDebug() << i << "variable" << QString::fromStdString(tmp);
            continue;
        }
        else throw myException("unknown symbol");
    }
    // add '(' and ')' for the power operation (exp1 ** exp2)
    for(auto itr=table.begin();itr!=table.end();++itr){
        if((*itr).stp==operand && (*itr).data=="**") powerNumber++;
    }
    while(powerNumber != powerNow){
        int tableSize = table.size();
        int count = 0;
        int tableIndex; // the current handling power operation's index
        int bracketIndex1, bracketIndex2; // left bracket's index and right bracket's index
        int countBracket1, countBracket2; // left and right bracket's counts on the same side of "**"
        for(tableIndex=tableSize-1;tableIndex >= 0;--tableIndex){
            if(table[tableIndex].stp==operand && table[tableIndex].data=="**") count++;
            if(count>powerNow) break; // break here and tableIndex is the last "power" operation's index
        }
        powerNow++; // the numeber of already handled power operation
        // add '('  left bracket before table[bracketIndex1]
        countBracket1=0;
        countBracket2=0;
        for(bracketIndex1=tableIndex-1;bracketIndex1>=0;--bracketIndex1){
            if(table[bracketIndex1].data=="(") countBracket1++;
            if(table[bracketIndex1].data==")") countBracket2++;
            if(countBracket1==countBracket2) break;
        }

        // add ')' right bracket after table[bracketIndex2]
        countBracket1=0;
        countBracket2=0;
        for(bracketIndex2=tableIndex+1;bracketIndex1<=tableSize-1;++bracketIndex2){
            if(table[bracketIndex2].data=="(") countBracket1++;
            if(table[bracketIndex2].data==")") countBracket2++;
            if(countBracket1==countBracket2) break;
        }
        int i=-1;
        while(i<tableSize-1){
            ++i;
            if(i==bracketIndex1){
                table_tmp.push_back(binary("(", operand));
                table_tmp.push_back(table[i]);
            }
            else if(i==bracketIndex2){
                table_tmp.push_back(table[i]);
                table_tmp.push_back(binary(")", operand));
            }
            else{
                table_tmp.push_back(table[i]);
            }
        }
        table.clear();
        table = table_tmp;
        table_tmp.clear();
    }

    //split done
    //qDebug() << "spilt done";

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
                //qDebug() << "constant value" << constVal;//debug
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
                    if(variableStack.empty()) throw myException("illegal expression");
                    right = variableStack.top();
                    variableStack.pop();
                    if(variableStack.empty()) throw myException("illegal expression");
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
                if(variableStack.empty()) throw myException("illegal expression");
                right = variableStack.top();
                variableStack.pop();
                if(variableStack.empty()) throw myException("illegal expression");
                left = variableStack.top();
                variableStack.pop();
                treeNode = new CompoundExp(tmpOp.data, left, right);
                root = treeNode;
                variableStack.push(treeNode);
            }
            if(operandStack.empty()) throw myException("unexpected ')'");
            operandStack.pop();//pop "("
        }
    }
    while(!operandStack.empty()){
        binary tmpOp = operandStack.top();
        operandStack.pop();
        if(tmpOp.data == "(") throw myException("missing ')'");

        Expression *left, *right, *treeNode;
        if(variableStack.empty()) throw myException("illegal expression");
        right = variableStack.top();
        variableStack.pop();
        if(variableStack.empty()) throw myException("illegal expression");
        left = variableStack.top();
        variableStack.pop();
        treeNode = new CompoundExp(tmpOp.data, left, right);
        root = treeNode;
        variableStack.push(treeNode);
    }
    if(variableStack.size() != 1) throw myException("cannot match variables and operands");
    root = variableStack.top();

    //qDebug() << "Tree: Tree constructed";
}

Tree::~Tree(){
    delete root;
}

bool Tree::check(const stack<binary> &opStack, const string &op){
    if(op=="**"){
        if(opStack.top().data == "*"|| opStack.top().data == "/"||
           opStack.top().data == "+"|| opStack.top().data == "-"||
           opStack.top().data == "<"|| opStack.top().data == ">"||
           opStack.top().data == "=")
            return true;
    }
    else if(op=="*" || op=="/"){
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
