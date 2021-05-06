#ifndef BUFFER_H
#define BUFFER_H

#include <QString>
#include <vector>
#include <string>
#include <QTextStream>
#include <sstream>

using std::string;
using std::vector;
using std::stringstream;

/* with pointer */
struct node{
    int line;
    QString data;
    node *next;
    node(int l, QString s){
        line = l;
        data = s;
    }
};

/* without pointer */
struct dataNode{
    int line;
    QString data;

    dataNode(int l, QString s){
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
    bool isEmpty();
    void newLine(int l, QString s);
    void clearBuffer();
    QString lineAll();
    vector<dataNode> parseLine();
//    void displayAll();
//    void displayLine();
};

#endif // BUFFER_H
