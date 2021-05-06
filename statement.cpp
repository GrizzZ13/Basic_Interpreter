#include "statement.h"

Statement::Statement(){}

/* ----------------------remark-------------------- */
remarkStatement::remarkStatement(vector<string> &lineVec){
    string tmp = "";
    for(auto itr = lineVec.begin()+1;itr != lineVec.end();++itr){
        tmp = tmp + " " + *itr;
    }
    remark = tmp;
}


/* ----------------------input--------------------- */
inputStatement::inputStatement(vector<string> &lineVec){
    varName = lineVec[1];
    value = 0x7fffffff;
}

void inputStatement::execute(){
    varTable[varName] = value;
}

/* ----------------------print--------------------- */
printStatement::printStatement(vector<string> &lineVec){
    string tmp = "";
    for(auto itr = lineVec.begin()+1;itr != lineVec.end();++itr){
        tmp = tmp + *itr;
    }
    exp = new Tree(tmp);
}

void printStatement::execute(){
    value = exp->eval().toString();
}

string printStatement::getValue(){
    execute();
    return value;
}

/* ----------------------printf-------------------- */
printfStatement::printfStatement(vector<string> &lineVec) {
    // including "PRINTF" and ".?*{}"
    trimSpace(lineVec);

    /**********************************************/
    for(auto itr=lineVec.begin();itr!=lineVec.end();++itr){
        std::cout << " * " << *itr << " * ";
    }

    /**********************************************/
    int size = lineVec.size();
    int index = 1;
    while(index < size){
        string fragment="";
        string str_now = lineVec[index]; // handling string
        int strlen = str_now.length();
        int strindex = 0;
        if(index==1){ // 格式化输出部分，含有大括号
            if(!isString(str_now))
                throw myException("printf format error");
            else{
                while(strindex < strlen){
                    if(str_now[strindex]=='{'){
                        if(!fragment.empty()){
                            output.push_back(fragment);
                            fragment.clear();
                        }
                        if(strindex+1 < strlen && str_now[strindex+1]=='}'){
                            output.push_back(string("{}"));
                            strindex += 2;
                        }
                        else
                            throw myException("printf missing } ");

                    }
                    else if(str_now[strindex]=='}')
                        throw myException("printf unexpected } ");
                    else{
                        fragment += str_now[strindex];
                        strindex++;
                    }
                }
                if(!fragment.empty()){
                    output.push_back(fragment);
                    fragment.clear();
                }
            }
        }
        else{
            // 填入大括号的部分
            if(str_now.empty()){
                index++;
                continue;
            }
            if(isString(str_now)){ // 是字符串
                string tmp = trimString(str_now);
                int outputLen = output.size();
                int i;
                for (i = 0; i < outputLen; ++i) {
                    if(output[i]=="{}"){
                        output[i]=tmp;
                        break;
                    }
                }
                if(i==outputLen)
                    throw myException("printf : cannot match {}");
            }
            else if(typeTable.find(str_now)!=typeTable.end()){ // 变量
                string tmp;
                if(typeTable[str_now]==_STRING){
                    tmp = strTable[str_now];
                }
                else{
                    tmp = std::to_string(varTable[str_now]);
                }
                int outputLen = output.size();
                int i;
                for (i = 0; i < outputLen; ++i) {
                    if(output[i]=="{}"){
                        output[i]=tmp;
                        break;
                    }
                }
                if(i==outputLen)
                    throw myException("printf : cannot match {}");
            }
            else { // 其他的东西
                int j;
                for (j = 0; j < strlen; ++j) {
                    if(!((str_now[j]<='9' && str_now[j]>='0')||str_now[j]=='-' || str_now[j]=='+'))
                        break;
                }
                if(j != strlen)
                    throw myException("printf unknown symbol");

                else{
                    int outputLen = output.size();
                    int i;
                    for (i = 0; i < outputLen; ++i) {
                        if(output[i]=="{}"){
                            output[i]=str_now;
                            break;
                        }
                    }
                    if(i==outputLen)
                        throw myException("printf : cannot match {}");
                }
            }
        }
        index++;
    }
}

bool printfStatement::isString(string &str) {
    int strlen = str.length();
    if(strlen >= 2){
        if(str[0]=='\'' && str[strlen-1]=='\'')
            return true;
        if(str[0]=='\"' && str[strlen-1]=='\"')
            return true;
    }
    return false;
}

string printfStatement::trimString(string str) {
    return str.substr(1, str.length()-2);
}

string printfStatement::getValue() {
    string ret = "";
    int size = output.size();
    for (int i = 0; i < size; ++i) {
        if(output[i]=="{}")
            throw myException("cannot match {} and value");
        ret += output[i];
    }
    ret = trimString(ret);
    return ret;
}

void printfStatement::trimSpace(vector<string> &lineVec) {
    for(int i=0;i < lineVec.size();++i){
        if(isString(lineVec[i]))
            continue;
        else{
            string trimmed = "";
            int strlen = lineVec[i].length();
            for (int j = 0; j < strlen; ++j) {
                if(lineVec[i][j]!=' ')
                    trimmed += lineVec[i][j];
            }
            lineVec[i] = trimmed;
        }
    }
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

void letStatement::execute(){
    success = exp->eval().toBoolean();
}

bool letStatement::getSuccess(){
    execute();
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

void gotoStatement::execute(){
    nextLine = exp->eval().getValInt();
}

int gotoStatement::toLine(){
    execute();
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
    if(itr == lineVec.end()) throw myException("invalid statement IF-THEN");
    exp = new Tree(tmp);
    if(*itr == "THEN") itr++;
    nextLine = std::stoi(*itr);
    flag = false;
}

void ifStatement::execute(){
    flag = (bool)exp->eval().toBoolean();
}

intAndbool ifStatement::toLine(){
    execute();
    return intAndbool(nextLine, flag);
    // -1 means do not jump
}
