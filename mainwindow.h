#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QTimer>
#include <QTime>
#include <QCloseEvent>
#include <QTextBlock>
#include <QTextEdit>
#include <QMessageBox>

#include <vector>
#include <queue>
#include "tree.h"
#include "variables.h"
#include "buffer.h"
#include "expression.h"
#include "statement.h"
#include "myexception.h"

#define COLOR_GREEN (QColor(100,255,100))
#define COLOR_RED (QColor(255,100,100))

using std::queue;
using std::vector;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct SyntaxNode{
    bool right;
    QString syntaxTree;

    SyntaxNode(bool _right, QString _syntaxTree){
        right = _right;
        syntaxTree = _syntaxTree;
    }
};

struct debugEnv{
    bool debug;

    // environment variable
    vector<SyntaxNode> SyntaxBlocks;
    vector<dataNode> NumData;
    QString displayBuffer;
    vector<string> lineVec;
    string thisLine;
    int index;
    int max;
    bool end;

    debugEnv(){
        debug = false;
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    Buffer *textBuffer;
    string handlingVar;
    int handlingVal;
    string handlingStr;
    bool gotInput;
    bool handlingInput;

    debugEnv env;

    // new added
    QList<QTextEdit::ExtraSelection> extras;


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // real function
    void displayBuffer();
    void doitNow(vector<string> &lineVec); //  LET, PRINT, INPUT, PRINTF, INPUTS
    void insertLine(int lineNumber, vector<string> &lineVec); // insert a line
    QString syntaxTree(Expression *node);
    void helpMessage();
    vector<SyntaxNode> buildSyntaxTree();
    void clearVariables();

    bool isString(string str);
    string trimString(string str);
    void highlight(int line, QColor color);
    void clearHighlight();
    void showVariables();
    void displaySyntaxTree(vector<SyntaxNode> &SyntaxBlocks);
    void displaySyntaxTreeLine(vector<SyntaxNode> &SyntaxBlocks, int index);
    void highlightWrong(vector<SyntaxNode> &SyntaxBlocks);

    // real function
    void Run();
    void HandldInput();
//    void Step();

public slots:
    void clearCode();
    void loadFile();
    // wrapper
    void handleInput();
    void runBegin();
    void Debug();
};
#endif // MAINWINDOW_H
