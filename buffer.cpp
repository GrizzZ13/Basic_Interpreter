#include "buffer.h"
#include <QtDebug>

Buffer::Buffer(){
    head = new node(0, "head");
    end = head;
}

Buffer::~Buffer(){
    node* tmp;
    while(head != nullptr){
        tmp = head->next;
        delete head;
        head = tmp;
    }
}

void Buffer::newLine(int l, QString s){
    end->next = new node(l, s);
    end = end->next;
}

void Buffer::clearBuffer(){
    if(head == end) return;

    node* tmp;
    while(head != end){
        tmp = head->next;
        delete head;
        head = tmp;
    }
}

QString Buffer::lineAll(){
    node* tmp = head;
    QString retVal = "";
    QString lineNumber;
    while(tmp != end){
        tmp = tmp->next;
        lineNumber = QString::number(tmp->line);
        retVal.append(lineNumber + "\t" + tmp->data + "\n");
    }
    return retVal;
}
