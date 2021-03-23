#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QTimer>
#include <vector>
#include <queue>
#include "tree.h"
#include "variables.h"
#include "buffer.h"
#include "expression.h"
#include "statement.h"

using std::queue;

using std::vector;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    Buffer *textBuffer;
    string handlingVar;
    int handlingVal;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void displayBuffer();
    void doitNow(vector<string> &lineVec); //  LET, PRINT, INPUT
    void handleCommand(vector<string> &lineVec); // Command
    void insertLine(int lineNumber, vector<string> &lineVec); // insert a line
    QString syntaxTree(Expression *node);

public slots:
    void clearCode();
    void handleInput();
    void loadFile();
    void runBegin();
};
#endif // MAINWINDOW_H
