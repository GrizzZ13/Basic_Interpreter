#ifndef BUFFER_H
#define BUFFER_H

#include <QString>
#include <string>
#include <QTextStream>
#include <sstream>

using std::string;
using std::stringstream;

struct node{
    int line;
    QString data;
    node *next;
    node(int l, QString s){
        line = l;
        data = s;
    }
};


class Buffer
{
private:
    node* head;
    node* end;
public:
    Buffer();
    ~Buffer();
    void newLine(int l, QString s);
    void clearBuffer();
    QString lineAll();
//    void displayAll();
//    void displayLine();
};

#endif // BUFFER_H
