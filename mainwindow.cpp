#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "variables.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    handlingVar = "";
    handlingVal = 0x3f3f3f3f;

    textBuffer = new Buffer;
    connect(ui->textInput, SIGNAL(returnPressed()), this, SLOT(handleInput()), Qt::DirectConnection);
    connect(ui->buttonClear, SIGNAL(clicked()), this, SLOT(clearCode()));
    connect(ui->buttonLoad, SIGNAL(clicked()), this, SLOT(loadFile()));
    connect(ui->buttonRun, SIGNAL(clicked()), this, SLOT(runBegin()));
}

MainWindow::~MainWindow()
{
    delete textBuffer;
    delete ui;
}

void MainWindow::displayBuffer(){
    ui->browserCode->setText(textBuffer->lineAll());
}

void MainWindow::clearCode(){
    handlingVar = "";
    handlingVal = 0x3f3f3f3f;
    varTable.clear();
    textBuffer->clearBuffer();
    ui->browserResult->clear();
    ui->browserCode->clear();
    ui->browserStructure->clear();
}

void MainWindow::loadFile(){
    QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), "/", tr("Basic Files(*.txt)"));
    if(path.length() == 0) return;

    clearCode();
    QFile fileName(path);
    fileName.open(QIODevice::ReadWrite);
    QTextStream in(&fileName);
    while(!in.atEnd()){
        QString thisQLine = in.readLine();
        string thisLine = thisQLine.toStdString();
        int lineNumber = -1;

        vector<string> lineVec;
        stringstream ss;
        string buf;
        ss << thisLine;
        while(ss >> buf) lineVec.push_back(buf);

        stringstream sstmp(lineVec[0]);
        sstmp >> lineNumber;
        insertLine(lineNumber, lineVec);
    }
}

void MainWindow::runBegin(){
    vector<dataNode> NumData = textBuffer->parseLine();
    QString displayBuffer="";
    stringstream ss;
    vector<string> lineVec;
    string thisLine;
    string fragment;
    int length = (int)NumData.size();
    int index = 0;
    bool end = false;
    while(!end){
        lineVec.clear();
        ss.clear();
        thisLine = NumData[index].data.toStdString();
        ss << thisLine;
        while(ss >> fragment) lineVec.push_back(fragment);
        // remark
        if(lineVec[0]=="REM"){
            continue;
            index++;
            end = false;
        }
        else if(lineVec[0]=="INPUT"){
        // input
            // when enter is pressed, lineEdit emits a signal and calls a slot function
            // note that the connection type here is direction connection
            // and the running process will be interrupted, wait until the slot return
            handlingVar = lineVec[1];

            // whether this variable exists or not
            auto itr = varTable.find(handlingVar);
            if(itr != varTable.end()){
                // exists
                handlingVar = lineVec[1];
                ui->textInput->setText("? ");
                while(handlingVal==0x3f3f3f3f){
                    QEventLoop eventloop;
                    QTimer::singleShot(100, &eventloop, SLOT(quit()));
                    eventloop.exec();
                    // after the slot function returns, the handling Value will be changed
                }

                varTable[handlingVar] = handlingVal;

                handlingVar = "";
                handlingVal = 0x3f3f3f3f;
            }
            else handlingVar = "";// doesn't exist
            index++;
            end = false;
        }
        else if(lineVec[0]=="PRINT"){
            printStatement state(lineVec);
            displayBuffer = displayBuffer + QString::number(state.getValue(varTable)) + '\n';
            ui->browserResult->setText(displayBuffer);
            index++;
            end = false;
        }
        else if(lineVec[0]=="END"){
            end = true;
        }
        else if(lineVec[0]=="LET"){
            letStatement state(lineVec);
            if(state.getSuccess(varTable)){
                index++;
                end = false;
            }
            else{
                ui->browserResult->setText("assignment error occured");
                end = true;
            }
        }
        else if(lineVec[0]=="GOTO"){
            gotoStatement state(lineVec);
            int nextLine = state.toLine(varTable);
            bool flag = false;
            for(int i = 0;i < length;++i){
                if(NumData[i].line == nextLine){
                    index = i;
                    end = false;
                    flag = true;
                    break;
                }
            }
            if(flag == false){
                ui->browserResult->setText("goto error occured");
                end = true;
            }
        }
        else if(lineVec[0]=="IF"){
            ifStatement state(lineVec);
            int nextLine = state.toLine(varTable);
            bool flag = false;

            if(nextLine == -1){// do not jump
                index++;
                end = false;
            }
            else{// jump
                for(int i = 0;i < length;++i){
                    if(NumData[i].line == nextLine){
                        index = i;
                        end = false;
                        flag = true;
                        break;
                    }
                }
                if(flag == false){
                    ui->browserResult->setText("if-then error occured");
                    end = true;
                }
            }
        }
        else{
            ui->browserResult->setText("invalid statement");
            end = true;
        }
    }
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
        else if(lineVec[0] == "?"){
            handlingVal = std::stoi(lineVec[1]);
        }
        else
            handleCommand(lineVec);
    }
    else insertLine(lineNumber, lineVec);

}

void MainWindow::doitNow(vector<string> &lineVec){
    string tmp = "";
    qDebug() << "do it now";
    if(lineVec[0] == "PRINT"){
        for(auto itr = lineVec.begin()+1;itr != lineVec.end();++itr){
            tmp = tmp + *itr;
        }
        Tree *expressionTree = new Tree(tmp);
        int result = expressionTree->eval(varTable);
        ui->browserResult->setText(QString::number(result));
        qDebug() << "delete expression tree";
        delete expressionTree;
        qDebug() << "successfully delete expression tree";
        return;
    }
    else if(lineVec[0] == "LET"){// assignment
        qDebug() << "assigment";
        tmp = tmp + lineVec[1];// ???
        Tree *expressionTree = new Tree(tmp);

        //???????
        if(expressionTree->type() == Comp) qDebug() << "Compound";
        if(expressionTree->type() == Iden) qDebug() << "Identifier";
        if(expressionTree->type() == Cons) qDebug() << "Constant";

        int result = expressionTree->eval(varTable);

        if(!result)
            ui->browserResult->setText("Syntax error");

        qDebug() << "delete expression tree";
        delete expressionTree;
        qDebug() << "successfully delete expression tree";
        return;
    }
}

void MainWindow::handleCommand(vector<string> &lineVec){
    return;
}

void MainWindow::insertLine(int lineNumber, vector<string> &lineVec){
    string str = "";
    for(auto itr = lineVec.begin() + 1;itr != lineVec.end();++itr){
        str = str + " " + *itr;
    }

    QString dataQS = QString::fromStdString(str);
    textBuffer->newLine(lineNumber, dataQS);
    displayBuffer();
    return;
}
