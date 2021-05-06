#ifndef CONTEXT_H
#define CONTEXT_H

#include <map>
#include <string>
using std::string;
using std::map;
using std::pair;

enum variableType{_STRING, _INT};

extern map<string, int> varTable;
extern map<string, string> strTable;
extern map<string, variableType> typeTable;

#endif // CONTEXT_H
