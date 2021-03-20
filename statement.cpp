#include "statement.h"

Statement::Statement(){}

/* ----------------------remark-------------------- */

// do nothing

/* ----------------------input--------------------- */
inputStatement::inputStatement(vector<string> &lineVec){
    varName = lineVec[1];
    value = 0x3f3f3f3f;
}

void inputStatement::execute(map<string, int> &v){
    v[varName] = value;
}

/* ----------------------print--------------------- */
printStatement::printStatement(vector<string> &lineVec){
    string tmp = "";
    for(auto itr = lineVec.begin()+1;itr != lineVec.end();++itr){
        tmp = tmp + *itr;
    }
    exp = new Tree(tmp);
}

void printStatement::execute(map<string, int> &v){
    value = exp->eval(v);
}

int printStatement::getValue(map<string, int> &v){
    execute(v);
    return value;
}

/* -----------------------end---------------------- */

// do nothing

/* -----------------------let---------------------- */
letStatement::letStatement(vector<string> &lineVec){
    string tmp = "";
    for(auto itr = lineVec.begin()+1;itr != lineVec.end();++itr){
        tmp = tmp + *itr;
    }
    exp = new Tree(tmp);
}

void letStatement::execute(map<string, int> &v){
    int result = exp->eval(v);
    success = (result == 1) ? true : false;
}

bool letStatement::getSuccess(map<string, int> &v){
    execute(v);
    return success;
}

/* -----------------------goto--------------------- */
gotoStatement::gotoStatement(vector<string> &lineVec){
    string tmp = "";
    for(auto itr = lineVec.begin()+1;itr != lineVec.end();++itr){
        tmp = tmp + *itr;
    }
    exp = new Tree(tmp);
}

void gotoStatement::execute(map<string, int> &v){
    nextLine = exp->eval(v);
}

int gotoStatement::toLine(map<string, int> &v){
    execute(v);
    return nextLine;
}

/* -----------------------if----------------------- */
ifStatement::ifStatement(vector<string> &lineVec){
    string tmp = "";
    auto itr = lineVec.begin()+1;
    while(itr != lineVec.end() && *itr != "THEN"){
        tmp = tmp + *itr;
        itr++;
    }
    exp = new Tree(tmp);
    if(*itr == "THEN") itr++;
    nextLine = std::stoi(*itr);
    flag = false;
}

void ifStatement::execute(map<string, int> &v){
    flag = (bool)exp->eval(v);
}

int ifStatement::toLine(map<string, int> &v){
    execute(v);
    return flag ? nextLine : -1;
    // -1 means do not jump
}
