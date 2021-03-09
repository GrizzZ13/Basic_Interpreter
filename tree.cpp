#include "tree.h"

Tree::Tree(string s)
{
    stack<string> operand;
    stack<string> variable;
    vector<string> table;
    int length = s.length();
    string tmp;

    for(int i = 0;i < length;++i){
        tmp = "";
        //constant
        if(s[i] <= '9' && s[i] >= '0'){
            while(s[i] <= '9' && s[i] >= '0' && i < length){
                tmp = tmp + s[i];
                i++;
            }
            table.push_back(tmp);
            break;
        }
        //operand
        if(s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='/'||s[i]=='('||s[i]==')'){
            tmp = tmp + s[i];
            table.push_back(tmp);
            break;
        }
        //variable
        if((s[i]<='z'&&s[i]>='a')||(s[i]<='Z'&&s[i]>='A')){
            while(!(s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='/'||s[i]=='('||s[i]==')')){
                tmp = tmp + s[i];
                i++;
            }
            break;
        }
    }
}

Tree::~Tree(){
    delete root;
}

int Tree::eval(map<string, int> var){
    return root->eval(var);
}
