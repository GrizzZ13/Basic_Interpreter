#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <buffer.h>
#include <vector>

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

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void displayBuffer();
    void clearCode();
    void handleInput();
    void doitNow(vector<string> lineVec); //  LET, PRINT, INPUT
    void handleCommand(vector<string> lineVec); // Command
    void insertLine(int lineNumber, vector<string> lineVec); // insert a line
};
#endif // MAINWINDOW_H
