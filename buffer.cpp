#include "buffer.h"
#include <QtDebug>

Buffer::Buffer(){
    head = new node(0, "head");
    end = head;
    end->next = nullptr;
}

Buffer::~Buffer(){
    node* tmp;
    while(head != nullptr){
        tmp = head->next;
        delete head;
        head = tmp;
    }
}

bool Buffer::isEmpty(){
    if(head == end) return true;
    return false;
}

void Buffer::newLine(int l, QString s){
    node *tmp = head;
    node *insert;
    while(tmp != end){
        if(l > tmp->next->line){
            tmp = tmp->next;
        }
        else if(l < tmp->next->line){
            insert = new node(l, s);
            insert->next = tmp->next;
            tmp->next = insert;
            return;
        }
        else if(l == tmp->next->line){
            tmp = tmp->next;
            tmp->data = s;
            return;
        }
    }
    // tmp == end
    tmp->next = new node(l, s);
    end = tmp->next;
    end->next = nullptr;
}

void Buffer::clearBuffer(){
    if(head == end) return;

    node* tmp;
    while(head != end){
        tmp = head->next;
        delete head;
        head = tmp;
    }
    end->next = nullptr;
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

vector<dataNode> Buffer::parseLine(){
    vector<dataNode> NumData;//store line number and corresponding data
    node *tmp = head->next;
    dataNode tmp_node(0, "");
    while(tmp != nullptr){
        tmp_node.line = tmp->line;
        tmp_node.data = tmp->data;
        NumData.push_back(tmp_node);
        tmp = tmp->next;
    }
    return NumData;
}
