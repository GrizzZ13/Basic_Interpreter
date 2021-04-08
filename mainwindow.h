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

#include <vector>
#include <queue>
#include "tree.h"
#include "variables.h"
#include "buffer.h"
#include "expression.h"
#include "statement.h"
#include "myexception.h"

using std::queue;
using std::vector;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    bool gotInput;
    bool handlingInput;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // real function
    void displayBuffer();
    void doitNow(vector<string> &lineVec); //  LET, PRINT, INPUT
    void handleCommand(vector<string> &lineVec); // Command
    void insertLine(int lineNumber, vector<string> &lineVec); // insert a line
    QString syntaxTree(Expression *node);
    void helpMessage();

    // real function
    void Run();
    void HandldInput();

public slots:
    void clearCode();
    void loadFile();
    // wrapper
    void handleInput();
    void runBegin();
};
#endif // MAINWINDOW_H
