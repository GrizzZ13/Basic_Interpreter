#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "variables.h"

void MainWindow::closeEvent(QCloseEvent *event){
    // when in input mode, there is a child process running in loop
    // set handlingInput false and gotInput true to jump out of the loop
    // or the application cannot end normally
    handlingInput = false;
    gotInput = true;
    event->accept();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    handlingVar = "";
    handlingVal = 0x7fffffff;
    gotInput = true;
    handlingInput = false;

    textBuffer = new Buffer;
    connect(ui->textInput, SIGNAL(returnPressed()), this, SLOT(handleInput()), Qt::DirectConnection);
    connect(ui->buttonClear, SIGNAL(clicked()), this, SLOT(clearCode()));
    connect(ui->buttonLoad, SIGNAL(clicked()), this, SLOT(loadFile()));
    connect(ui->buttonRun, SIGNAL(clicked()), this, SLOT(runBegin()));
}

MainWindow::~MainWindow()
{
    qDebug() << "destruct";
    gotInput = true;
    handlingInput = false;
    delete textBuffer;
    delete ui;
}

void MainWindow::displayBuffer(){
    ui->browserCode->setText(textBuffer->lineAll());
}

void MainWindow::clearCode(){
    handlingVar = "";
    handlingVal = 0x7fffffff;
    handlingInput = false;
    gotInput = true;
    varTable.clear();
    textBuffer->clearBuffer();
    ui->browserResult->clear();
    ui->browserCode->clear();
    ui->browserStructure->clear();
}

void MainWindow::loadFile(){
    clearCode();
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), "/", tr("Basic Files(*.txt)"));
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
    try {
        Run();
    }
    catch(myException &err) {
        string errInfo = "Error : ";
        errInfo = errInfo + err.info();
        ui->browserResult->setText(QString::fromStdString(errInfo));
    }
    catch(std::invalid_argument&){
        ui->browserResult->setText("Error : invalid argument");
        gotInput = false;
    }
    catch(std::out_of_range&){
        ui->browserResult->setText("Error : out of range");
        gotInput = false;
    }
}

void MainWindow::handleInput(){
    try {
        HandldInput();
    }
    catch(myException &err) {
        string errInfo = "Error : ";
        errInfo = errInfo + err.info();
        ui->browserResult->setText(QString::fromStdString(errInfo));
    }
    catch(std::invalid_argument&){
        ui->browserResult->setText("Error : invalid argument");
        gotInput = false;
    }
    catch(std::out_of_range&){
        ui->browserResult->setText("Error : out of range");
        gotInput = false;
    }
}

void MainWindow::doitNow(vector<string> &lineVec){
    string tmp = "";
    //qDebug() << "do it now";
    if(lineVec[0] == "PRINT"){
        for(auto itr = lineVec.begin()+1;itr != lineVec.end();++itr){
            tmp = tmp + *itr;
        }
        Tree *expressionTree = new Tree(tmp);
        int result = expressionTree->eval(varTable);
        ui->browserResult->setText(QString::number(result));
        //qDebug() << "delete expression tree";
        delete expressionTree;
        //qDebug() << "successfully delete expression tree";
        return;
    }
    else if(lineVec[0] == "LET"){// assignment
        //qDebug() << "assigment";
        tmp = tmp + lineVec[1];// ???
        string dfvar="";
        string exp = lineVec[1];
        int len = exp.length();
        for(int i=0;i < len;++i){
            qDebug() << exp[i];
            if(!((exp[i]<='Z'&&exp[i]>='A')||(exp[i]<='z'&&exp[i]>='a')||(exp[i]<='9'&&exp[i]>='0')))
                break;
            dfvar+=exp[i];
        }
        qDebug() << QString::fromStdString(dfvar);
        auto itr=varTable.find(dfvar);
        if(itr==varTable.end()){
            varTable.insert(pair<string,int>(dfvar, 0x7fffffff));
        }

        Tree *expressionTree = new Tree(tmp);

        //???????
        // if(expressionTree->type() == Comp) qDebug() << "Compound";
        // if(expressionTree->type() == Iden) qDebug() << "Identifier";
        // if(expressionTree->type() == Cons) qDebug() << "Constant";

        int result = expressionTree->eval(varTable);

        if(!result)
            ui->browserResult->setText("Syntax error");

        //qDebug() << "delete expression tree";
        delete expressionTree;
        //qDebug() << "successfully delete expression tree";
        return;
    }
    else if(lineVec[0] == "INPUT" && lineVec.size()==2){
        handlingVar = lineVec[1];
        gotInput = false;
        handlingInput = true;

        // whether this variable exists or not
        auto itr = varTable.find(handlingVar);
        if(itr == varTable.end()){
            varTable.insert(pair<string, int>(handlingVar, 0x7fffffff));
        }
        handlingVar = lineVec[1];
        qDebug() << "begin to input";
        ui->textInput->setText("? ");
        ui->textInput->setFocus();
        while(!gotInput){
            QEventLoop loop;//定义一个新的事件循环
            QTimer::singleShot(250, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
            loop.exec();
            // after the slot function returns, the handling Value will be changed
        }

        varTable[handlingVar] = handlingVal;
        handlingVar = "";
        handlingVal = 0x7fffffff;
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

QString MainWindow::syntaxTree(Expression *node){
    Expression *tmp;
    queue<Expression*> queueA;
    queue<Expression*> queueB;
    char popNow = 'A';
    string output = "";
    int layer = 1;

    queueA.push(node);
    while(true){
        if(popNow == 'A'){
            if(queueA.empty()) break;
            tmp = queueA.front();
            queueA.pop();
            for(int i = 0;i < layer;i++){
                output = output + "   ";
            }
            output = output + tmp->toString() + '\n';
            if(tmp->type() == Comp){
                queueB.push(((CompoundExp*)tmp)->left);
                queueB.push(((CompoundExp*)tmp)->right);
            }
            if(queueA.empty()){
                popNow = 'B';
                layer++;
            }
        }
        else{
            if(queueB.empty()) break;
            tmp = queueB.front();
            queueB.pop();
            for(int i = 0;i < layer;i++){
                output = output + "   ";
            }
            output = output + tmp->toString() + '\n';
            if(tmp->type() == Comp){
                queueA.push(((CompoundExp*)tmp)->left);
                queueA.push(((CompoundExp*)tmp)->right);
            }
            if(queueB.empty()){
                popNow = 'A';
                layer++;
            }
        }
    }
    return QString::fromStdString(output);
}

void MainWindow::helpMessage(){
    QString tmp;
    tmp = "RUN   : run this program if there exists code in the buffer\n"
          "LOAD  : load program from existing .txt file\n"
          "LIST  : this command is no longer supported\n"
          "CLEAR : clear buffer and all the browser\n"
          "QUIT  : quit the basic interpreter";

    ui->browserResult->setText(tmp);
}

void MainWindow::Run()
{
    // syntax tree
    buildSyntaxTree();
    // run
    handlingVar = "";
    handlingVal = 0x7fffffff;
    varTable.clear();
    ui->browserResult->clear();
//    ui->browserStructure->clear();

    if(textBuffer->isEmpty()) {
        ui->browserResult->setText("Empty buffer");
        return;
    }

    vector<dataNode> NumData = textBuffer->parseLine();
    QString displayBuffer="";
    QString thisLineNumber;
    stringstream ss;
    vector<string> lineVec;
    string thisLine;
    string fragment;
    int length = (int)NumData.size();
    int index = 0;
    int max = NumData.size();
    bool end = false;

    QString syntax="";
    while(!end){
        if(max == index) break;

        lineVec.clear();
        ss.clear();
        thisLine = NumData[index].data.toStdString();
        thisLineNumber = QString::number(NumData[index].line);
        ss << thisLine;
        while(ss >> fragment) lineVec.push_back(fragment);
        // remark
        if(lineVec.size()==0){
            index++;
            continue;
        }
        if(lineVec[0]=="REM"){
            remarkStatement remark(lineVec);
            syntax = syntax + thisLineNumber + " REM " + QString::fromStdString(remark.remark) + "\n";
            index++;
            end = false;
        }
        else if(lineVec[0]=="INPUT" && lineVec.size()==2){
        // input
            // when enter is pressed, lineEdit emits a signal and calls a slot function
            // note that the connection type here is direction connection
            // and the running process will be interrupted, wait until the slot return
            handlingVar = lineVec[1];
            gotInput = false;
            handlingInput = true;

            // whether this variable exists or not
            auto itr = varTable.find(handlingVar);
            if(itr != varTable.end()){
                varTable.insert(pair<string, int>(handlingVar, 0x7fffffff));
            }
            // exists
            handlingVar = lineVec[1];
            syntax = syntax + thisLineNumber + " INPUT " + QString::fromStdString(lineVec[1]) + "\n";
            ui->textInput->setText("? ");
            ui->textInput->setFocus();
            while(!gotInput){
                QEventLoop eventloop;
                QTimer::singleShot(500, &eventloop, SLOT(quit()));
                eventloop.exec();
                // after the slot function returns, the handling Value will be changed
            }

            varTable[handlingVar] = handlingVal;
            index++;
            handlingVar = "";
            handlingVal = 0x7fffffff;
        }
        else if(lineVec[0]=="PRINT"){
            syntax = syntax + thisLineNumber+ " PRINT\n";
            printStatement state(lineVec);
            Expression *node = state.exp->root;
            syntax = syntax + syntaxTree(node);
            displayBuffer = displayBuffer + QString::number(state.getValue(varTable)) + '\n';
            ui->browserResult->setText(displayBuffer);
            index++;
            end = false;
        }
        else if(lineVec[0]=="END" && lineVec.size()==1){
            syntax = syntax + thisLineNumber+ " END\n";
            end = true;
        }
        else if(lineVec[0]=="LET"){
            syntax = syntax + thisLineNumber + " LET";
            // declare variable
            string dfvar="";
            string exp = lineVec[1];
            int len = exp.length();
            for(int i=0;i < len;++i){
                if(!((exp[i]<='Z'&&exp[i]>='A')||(exp[i]<='z'&&exp[i]>='a')||(exp[i]<='9'&&exp[i]>='0')))
                    break;
                dfvar+=exp[i];
            }
            auto itr=varTable.find(dfvar);
            if(itr==varTable.end()){
                varTable.insert(pair<string,int>(dfvar, 0x7fffffff));
            }

            letStatement state(lineVec);
            if(state.getSuccess(varTable)){
                Expression *node = state.exp->root;
                syntax = syntax + syntaxTree(node);
                index++;
                end = false;
            }
            else
                throw myException("invalid assignment");
        }
        else if(lineVec[0]=="GOTO" && lineVec.size()==2){
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
            if(flag)
                syntax = syntax + thisLineNumber + " GOTO\n" + "     " + QString::number(nextLine) + "\n";
            else
                throw myException("invalid line number after GOTO");
        }
        else if(lineVec[0]=="IF"){
            ifStatement state(lineVec);
            int nextLine = state.toLine(varTable).i;
            bool jump = state.toLine(varTable).b;
            bool flag = false;

            if(!jump){// do not jump
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
                    throw myException("invalid line number after THEN");
                }
            }

            syntax = syntax + thisLineNumber + " IF THEN\n";
            Expression *node = state.exp->root;
            syntax = syntax + syntaxTree(node) + "   " + QString::number(nextLine) + "\n";
        }
        else throw myException("invalid instruction");
    }
    // ui->browserStructure->setText(syntax);
}

void MainWindow::buildSyntaxTree()
{
    ui->browserStructure->clear();

    if(textBuffer->isEmpty()) {
        return;
    }

    vector<dataNode> NumData = textBuffer->parseLine();
    QString thisLineNumber;
    stringstream ss;
    vector<string> lineVec;
    string thisLine;
    string fragment;
    int index = 0;
    int max = NumData.size();

    QString syntax="";
    while(max != index){

        lineVec.clear();
        ss.clear();
        thisLine = NumData[index].data.toStdString();
        thisLineNumber = QString::number(NumData[index].line);
        index++;
        ss << thisLine;
        while(ss >> fragment) lineVec.push_back(fragment);
        if(lineVec.size()==0){
            continue;
        }
        if(lineVec[0]=="REM"){
            remarkStatement remark(lineVec);
            syntax = syntax + thisLineNumber + " REM " + QString::fromStdString(remark.remark) + "\n";
        }
        else if(lineVec[0]=="INPUT"){
            qDebug() << "input";
            // exists
            if(lineVec.size()==2)
                syntax = syntax + thisLineNumber + " INPUT\n    " + QString::fromStdString(lineVec[1]) + "\n";
            else
                syntax = syntax + thisLineNumber + " ERROR" + "\n";
        }
        else if(lineVec[0]=="PRINT"){
            try{
                printStatement state(lineVec);
                Expression *node = state.exp->root;
                syntax = syntax + thisLineNumber+ " PRINT\n";
                syntax = syntax + syntaxTree(node);
            }
            catch(...){
                qDebug() << "exception";
                syntax = syntax + thisLineNumber+ " ERROR\n";
                qDebug() << thisLineNumber;
            }
        }
        else if(lineVec[0]=="END" && lineVec.size()==1){
            syntax = syntax + thisLineNumber+ " END\n";
        }
        else if(lineVec[0]=="LET"){
            try{
                letStatement state(lineVec);
                Expression *node = state.exp->root;
                syntax = syntax + thisLineNumber + " LET";
                syntax = syntax + syntaxTree(node);
            }
            catch(...){
                syntax = syntax + thisLineNumber+ " ERROR\n";
            }
        }
        else if(lineVec[0]=="GOTO"){
            index++;
            if(lineVec.size()==2)
                syntax = syntax + thisLineNumber + " GOTO\n" + "     " + QString::fromStdString(lineVec[1]) + "\n";
            else
                syntax = syntax + thisLineNumber + " ERROR\n";
        }
        else if(lineVec[0]=="IF"){
            try{
                ifStatement state(lineVec);
                Expression *node = state.exp->root;
                syntax = syntax + thisLineNumber + " IF THEN\n";
                syntax = syntax + syntaxTree(node) + "   " + QString::number(state.nextLine) + "\n";
            }
            catch(...){
                syntax = syntax + thisLineNumber + " ERROR\n";
            }
        }
        else {
            syntax = syntax + thisLineNumber + " ERROR\n";
        }
        ui->browserStructure->setText(syntax);
    }
}

void MainWindow::HandldInput()
{
    // input mode
    if(handlingInput && ui->textInput->text().length()==0){
        ui->browserResult->setText("You must input a number begin with '? '");
        ui->textInput->setText("? ");
        return;
    }
    // handle input
    QString tmpQS = ui->textInput->text();
    if(tmpQS.length()==0) return;
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
        else if(handlingInput && lineVec[0] == "?" && lineVec.size() == 2){
            gotInput = true;
            handlingInput = false;
            handlingVal = std::stoi(lineVec[1]);
        }
        else if(handlingInput && lineVec[0] == "?" && lineVec.size() != 2){
            gotInput = true;
            handlingInput = false;
            throw myException("invalid input");
        }
        else if(handlingInput && lineVec[0] != "?"){
            ui->browserResult->setText("You must input a number begin with '? '");
            ui->textInput->setText("? ");
            return;
        }
        else if(!handlingInput && lineVec[0] == "?"){
            throw myException("unexpected input");
        }
        else if(lineVec[0]=="LOAD" && lineVec.size() == 1)
            loadFile();
        else if(lineVec[0]=="RUN" && lineVec.size() == 1)
            runBegin();
        else if(lineVec[0]=="CLEAR" && lineVec.size() == 1)
            clearCode();
        else if(lineVec[0]=="HELP" && lineVec.size() == 1)
            helpMessage();
        else if(lineVec[0]=="QUIT" && lineVec.size() == 1)
            this->close();
        else
            throw myException("invalid command "+tmpS);
    }
    else insertLine(lineNumber, lineVec);
}
