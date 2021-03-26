#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H
#include <iostream>
#include <exception>
#include <string>

using std::exception;
using std::string;

class myException : public exception
{
public:
    myException(string errInfo){
        information = errInfo;
    }
    string info(){
        return information;
    }
private:
    string information;
};

#endif // MYEXCEPTION_H
