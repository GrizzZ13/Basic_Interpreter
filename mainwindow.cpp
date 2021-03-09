#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textBuffer = new Buffer;
    connect(ui->textInput, SIGNAL(returnPressed()), this, SLOT(handleInput()));
    connect(ui->buttonClear, SIGNAL(clicked()), this, SLOT(clearCode()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete textBuffer;
}

void MainWindow::displayBuffer(){
    ui->browserCode->setText(textBuffer->lineAll());
}

void MainWindow::clearCode(){
    textBuffer->clearBuffer();
    ui->browserCode->setText(textBuffer->lineAll());
}

void MainWindow::handleInput(){
    // handle input
    QString tmpQS = ui->textInput->text();
    ui->textInput->clear();
    string tmpS = tmpQS.toStdString();

    //
    int lineNumber = -1;
    vector<string> lineVec;
    stringstream ss;
    string buf;
    ss << tmpS;
    while(ss >> buf) lineVec.push_back(buf);

    stringstream sstmp(lineVec[0]);
    if(!(sstmp >> lineNumber)){
        if(lineVec[0] == "LET" || lineVec[0] == "PRINT" || lineVec[0] == "INPUT")
            doitNow(lineVec);
        else
            handleCommand(lineVec);
    }
    else insertLine(lineNumber, lineVec);

}

void MainWindow::doitNow(vector<string> lineVec){
    return;
}

void MainWindow::handleCommand(vector<string> lineVec){
    return;
}

void MainWindow::insertLine(int lineNumber, vector<string> lineVec){
    string str = "";
    for(auto itr = lineVec.begin() + 1;itr != lineVec.end();++itr){
        str = str + " " + *itr;
    }

    QString dataQS = QString::fromStdString(str);
    textBuffer->newLine(lineNumber, dataQS);
    displayBuffer();
    return;
}
