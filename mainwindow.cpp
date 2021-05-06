#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    handlingStr = "";
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
    handlingStr = "";
    handlingInput = false;
    gotInput = true;
    varTable.clear();
    strTable.clear();
    typeTable.clear();
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
        for(auto itr = tmp.begin();itr!=tmp.end();++itr){
            if(*itr=='='||*itr=='>'||*itr=='<') throw myException("invalid expression after PRINT");
        }
        Tree *expressionTree = new Tree(tmp);
        string result = expressionTree->eval().toString();
        ui->browserResult->setText(QString::fromStdString(result));
        //qDebug() << "delete expression tree";
        delete expressionTree;
        //qDebug() << "successfully delete expression tree";
        return;
    }
    else if(lineVec[0] == "PRINTF"){
        string result = printfStatement(lineVec).getValue();
        ui->browserResult->setText(QString::fromStdString(result));
        //qDebug() << "successfully delete expression tree";
        return;
    }

    else if(lineVec[0] == "LET"){// assignment
        //qDebug() << "assigment";
        int count = 0;
        for(auto itr = lineVec.begin()+1;itr != lineVec.end();++itr){
            tmp = tmp + *itr;
        }
        for(auto itr = tmp.begin();itr!=tmp.end();++itr){
            if(*itr=='=') count++;
            if(*itr=='>'||*itr=='<') throw myException("invalid expression after LET");
        }
        if(count != 1) throw myException("invalid expression after LET");
        string dfvar="";
        string exp = tmp;
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
        bool success= expressionTree->eval().toBoolean();
        if(!success)
            ui->browserResult->setText("Syntax error");

        //qDebug() << "delete expression tree";
        delete expressionTree;
        //qDebug() << "successfully delete expression tree";
        return;
    }
    else if(lineVec[0] == "INPUT"){
        if(lineVec.size()==1)
            throw myException("need variable here");
        if(lineVec.size()>=3)
            throw myException("too many arguments");
        for(auto itr=lineVec[1].begin();itr!=lineVec[1].end();++itr){
            if(!(*itr>='a'&&*itr<='z') && !(*itr>='A'&& *itr<='Z') && !(*itr>='0'&&*itr<='9'))
                throw myException("illegal variable");
        }
        if(lineVec[1][0]>='0'&&lineVec[1][0]<='9') throw myException("illegal variable");

        handlingVar = lineVec[1];

        // whether this variable exists or not
        auto itr = typeTable.find(handlingVar);
        if(itr == typeTable.end()){
            typeTable.insert(pair<string, variableType>(handlingVar, _INT));
        }
        else if(typeTable[handlingVar]==_STRING){
            handlingVar = "";
            throw myException("input type error");
        }
        if(varTable.find(handlingVar)==varTable.end()){
            varTable.insert(pair<string, int>(handlingVar, 0x7fffffff));
            strTable.insert(pair<string, string>(handlingVar, ""));
        }
        gotInput = false;
        handlingInput = true;

        qDebug() << "begin to input";
        ui->textInput->setText("? ");
        ui->textInput->setFocus();
        while(!gotInput){
            QEventLoop loop;
            QTimer::singleShot(300, &loop, SLOT(quit()));
            loop.exec();
            // after the slot function returns, the handling Value will be changed
        }

        varTable[handlingVar] = handlingVal;
        handlingVar = "";
        handlingVal = 0x7fffffff;
    }
    else if(lineVec[0] == "INPUTS"){
        if(lineVec.size()==1)
            throw myException("need variable here");
        if(lineVec.size()>=3)
            throw myException("too many arguments");
        for(auto itr=lineVec[1].begin();itr!=lineVec[1].end();++itr){
            if(!(*itr>='a'&&*itr<='z') && !(*itr>='A'&& *itr<='Z') && !(*itr>='0'&&*itr<='9'))
                throw myException("illegal variable");
        }
        if(lineVec[1][0]>='0'&&lineVec[1][0]<='9') throw myException("illegal variable");

        handlingVar = lineVec[1];

        // whether this variable exists or not
        auto itr = typeTable.find(handlingVar);
        if(itr == typeTable.end()){
            typeTable.insert(pair<string, variableType>(handlingVar, _STRING));
        }
        else if(typeTable[handlingVar]==_INT){
            handlingVar = "";
            throw myException("inputs type error");
        }
        if(strTable.find(handlingVar)==strTable.end()){
            varTable.insert(pair<string, int>(handlingVar, 0x7fffffff));
            strTable.insert(pair<string, string>(handlingVar, ""));
        }
        gotInput = false;
        handlingInput = true;

        qDebug() << "begin to inputs";
        ui->textInput->setText("?? ");
        ui->textInput->setFocus();
        while(!gotInput){
            QEventLoop loop;
            QTimer::singleShot(300, &loop, SLOT(quit()));
            loop.exec();
            // after the slot function returns, the handling String will be changed
        }

        strTable[handlingVar] = handlingStr;
        handlingVar = "";
        handlingStr = "";
    }
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
    handlingStr = "";
    strTable.clear();
    varTable.clear();
    typeTable.clear();
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

        /*********************************************************/

//        ss << thisLine;
//        while(ss >> fragment) lineVec.push_back(fragment);

        int parsingIndex = 0;
        int maxIndex = thisLine.length();
        vector<string> lineVec;
        string buf = "";
        while(parsingIndex<maxIndex){
            buf = "";
            while(parsingIndex < maxIndex && thisLine[parsingIndex]!=' ' && thisLine[parsingIndex]!=','){
                if(thisLine[parsingIndex]=='\"'){
                    if(!buf.empty()){
                        lineVec.push_back(buf);
                        buf="";
                    }
                    buf += thisLine[parsingIndex];
                    parsingIndex++;
                    while(parsingIndex < maxIndex && thisLine[parsingIndex]!='\"'){
                        buf += thisLine[parsingIndex];
                        parsingIndex++;
                    }
                    if(thisLine[parsingIndex]=='\"'){
                        buf += thisLine[parsingIndex];
                        parsingIndex++;
                    }
                    lineVec.push_back(buf);
                    std::cout << buf << std::endl;
                    buf = "";
                }
                else if(thisLine[parsingIndex]=='\''){
                    if(!buf.empty()){
                        lineVec.push_back(buf);
                        buf="";
                    }
                    buf += thisLine[parsingIndex];
                    parsingIndex++;
                    while(parsingIndex < maxIndex && thisLine[parsingIndex]!='\''){
                        buf += thisLine[parsingIndex];
                        parsingIndex++;
                    }
                    if(thisLine[parsingIndex]=='\''){
                        buf += thisLine[parsingIndex];
                        parsingIndex++;
                    }
                    lineVec.push_back(buf);
                    buf = "";
                }
                else{
                    buf += thisLine[parsingIndex];
                    parsingIndex++;
                }
            }
            if(!buf.empty())
                lineVec.push_back(buf);
            if(thisLine[parsingIndex]==' ' || thisLine[parsingIndex]==','){
                parsingIndex++;
            }
        }

        /*********************************************************/

        // remark
        if(lineVec.size()==0){
            index++;
            continue;
        }
        if(lineVec[0]=="REM"){
            remarkStatement remark(lineVec);
            index++;
            end = false;
        }
        else if(lineVec[0] == "INPUT" && lineVec.size()==2){

            for(auto itr=lineVec[1].begin();itr!=lineVec[1].end();++itr){
                if(!(*itr>='a'&&*itr<='z') && !(*itr>='A'&& *itr<='Z') && !(*itr>='0'&&*itr<='9'))
                    throw myException("illegal variable");
            }
            if(lineVec[1][0]>='0'&&lineVec[1][0]<='9') throw myException("illegal variable");

            handlingVar = lineVec[1];

            // whether this variable exists or not
            auto itr = typeTable.find(handlingVar);
            if(itr == typeTable.end()){
                typeTable.insert(pair<string, variableType>(handlingVar, _INT));
            }
            else if(typeTable[handlingVar]==_STRING){
                handlingVar = "";
                throw myException("input type error");
            }
            if(varTable.find(handlingVar)==varTable.end()){
                varTable.insert(pair<string, int>(handlingVar, 0x7fffffff));
                strTable.insert(pair<string, string>(handlingVar, ""));
            }
            gotInput = false;
            handlingInput = true;

            qDebug() << "begin to input";
            ui->textInput->setText("? ");
            ui->textInput->setFocus();
            while(!gotInput){
                QEventLoop loop;
                QTimer::singleShot(300, &loop, SLOT(quit()));
                loop.exec();
                // after the slot function returns, the handling Value will be changed
            }

            varTable[handlingVar] = handlingVal;
            handlingVar = "";
            handlingVal = 0x7fffffff;
        }
        else if(lineVec[0] == "INPUTS" && lineVec.size()==2){

            for(auto itr=lineVec[1].begin();itr!=lineVec[1].end();++itr){
                if(!(*itr>='a'&&*itr<='z') && !(*itr>='A'&& *itr<='Z') && !(*itr>='0'&&*itr<='9'))
                    throw myException("illegal variable");
            }
            if(lineVec[1][0]>='0'&&lineVec[1][0]<='9') throw myException("illegal variable");

            handlingVar = lineVec[1];

            // whether this variable exists or not
            auto itr = typeTable.find(handlingVar);
            if(itr == typeTable.end()){
                typeTable.insert(pair<string, variableType>(handlingVar, _STRING));
            }
            else if(typeTable[handlingVar]==_INT){
                handlingVar = "";
                throw myException("inputs type error");
            }
            if(strTable.find(handlingVar)==strTable.end()){
                varTable.insert(pair<string, int>(handlingVar, 0x7fffffff));
                strTable.insert(pair<string, string>(handlingVar, ""));
            }
            gotInput = false;
            handlingInput = true;

            qDebug() << "begin to inputs";
            ui->textInput->setText("?? ");
            ui->textInput->setFocus();
            while(!gotInput){
                QEventLoop loop;
                QTimer::singleShot(300, &loop, SLOT(quit()));
                loop.exec();
                // after the slot function returns, the handling String will be changed
            }

            strTable[handlingVar] = handlingStr;
            handlingVar = "";
            handlingStr = "";
        }
        else if(lineVec[0]=="PRINT"){
            string tmp="";
            for(auto itr = lineVec.begin()+1;itr != lineVec.end();++itr){
                tmp = tmp + *itr;
            }
            for(auto itr = tmp.begin();itr!=tmp.end();++itr){
                if(*itr=='='||*itr=='>'||*itr=='<'||*itr=='\''||*itr=='\"')
                    throw myException("invalid expression after PRINT");
            }
            printStatement state(lineVec);
            displayBuffer = displayBuffer + QString::fromStdString(state.getValue()) + '\n';
            ui->browserResult->setText(displayBuffer);
            index++;
            end = false;
        }
        else if(lineVec[0]=="PRINTF"){
            // to be continued
            string tmp = printfStatement(lineVec).getValue();
            displayBuffer = displayBuffer + QString::fromStdString(tmp) + '\n';
            ui->browserResult->setText(displayBuffer);
            index++;
            end = false;
        }
        else if(lineVec[0]=="END" && lineVec.size()==1){
            end = true;
        }
        else if(lineVec[0]=="LET"){
            int count = 0;
            string tmp = "";
            for(auto itr = lineVec.begin()+1;itr != lineVec.end();++itr){
                tmp = tmp + *itr;
            }
            for(auto itr = tmp.begin();itr!=tmp.end();++itr){
                if(*itr=='=') count++;
                if(*itr=='>'||*itr=='<') throw myException("invalid expression after LET");
            }
            if(count != 1) throw myException("invalid expression after LET");
            // declare variable
            string dfvar="";
            string exp = tmp;
            int len = exp.length();
            for(int i=0;i < len;++i){
                if(!((exp[i]<='Z'&&exp[i]>='A')||(exp[i]<='z'&&exp[i]>='a')||(exp[i]<='9'&&exp[i]>='0')))
                    break;
                dfvar+=exp[i];
            }
            auto itr=varTable.find(dfvar);
            if(itr==varTable.end()){
                varTable.insert(pair<string,int>(dfvar, 0x7fffffff));
                strTable.insert(pair<string, string>(dfvar, ""));
            }

            letStatement state(lineVec);
            if(state.getSuccess()){
                index++;
                end = false;
            }
            else
                throw myException("invalid assignment");
        }
        else if(lineVec[0]=="GOTO" && lineVec.size()==2){
            gotoStatement state(lineVec);
            int nextLine = state.toLine();
            bool flag = false;
            for(int i = 0;i < length;++i){
                if(NumData[i].line == nextLine){
                    index = i;
                    end = false;
                    flag = true;
                    break;
                }
            }
            if(!flag)
                throw myException("invalid line number after GOTO");
        }
        else if(lineVec[0]=="IF"){
            ifStatement state(lineVec);
            int nextLine = state.toLine().i;
            bool jump = state.toLine().b;
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
        }
        else throw myException("invalid instruction");
    }
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
            try{
                if(lineVec.size()==2){
                    for(auto itr=lineVec[1].begin();itr!=lineVec[1].end();++itr){
                        if(!(*itr>='a'&&*itr<='z') && !(*itr>='A'&& *itr<='Z') && !(*itr>='0'&&*itr<='9'))
                            throw myException("illegal variable");
                    }
                    if(lineVec[1][0]>='0'&&lineVec[1][0]<='9') throw myException("illegal variable");
                    syntax = syntax + thisLineNumber + " INPUT\n    " + QString::fromStdString(lineVec[1]) + "\n";
                }
                else
                    throw myException("illegal variable");
            }
            catch(...){
                syntax = syntax + thisLineNumber+ " ERROR\n";
                qDebug() << thisLineNumber;
            }
        }
        else if(lineVec[0]=="INPUTS"){
            try{
                if(lineVec.size()==2){
                    for(auto itr=lineVec[1].begin();itr!=lineVec[1].end();++itr){
                        if(!(*itr>='a'&&*itr<='z') && !(*itr>='A'&& *itr<='Z') && !(*itr>='0'&&*itr<='9'))
                            throw myException("illegal variable");
                    }
                    if(lineVec[1][0]>='0'&&lineVec[1][0]<='9') throw myException("illegal variable");
                    syntax = syntax + thisLineNumber + " INPUTS\n    " + QString::fromStdString(lineVec[1]) + "\n";
                }
                else
                    throw myException("illegal variable");
            }
            catch(...){
                syntax = syntax + thisLineNumber+ " ERROR\n";
                qDebug() << thisLineNumber;
            }
        }
        else if(lineVec[0]=="PRINT"){
            try{
                string tmp = "";
                for(auto itr = lineVec.begin()+1;itr != lineVec.end();++itr){
                    tmp = tmp + *itr;
                }
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
        else if(lineVec[0]=="PRINTF"){
            syntax = syntax + thisLineNumber + " PRINTF\n";
        }
        else if(lineVec[0]=="END" && lineVec.size()==1){
            syntax = syntax + thisLineNumber + " END\n";
        }
        else if(lineVec[0]=="LET"){
            try{
                string tmp = "";
                int count = 0;
                for(auto itr = lineVec.begin()+1;itr != lineVec.end();++itr){
                    tmp = tmp + *itr;
                }
                for(auto itr = tmp.begin();itr!=tmp.end();++itr){
                    if(*itr=='=') count++;
                    if(*itr=='>'||*itr=='<') throw myException("invalid expression after LET");
                }
                if(count != 1) throw myException("invalid expression after LET");

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
        if(typeTable[handlingVar]==_INT){
            ui->browserResult->setText("You must input a number begin with '? '");
            ui->textInput->setText("? ");
        }
        else{
            ui->browserResult->setText("You must input a string begin with '?? '");
            ui->textInput->setText("?? ");
        }
        return;
    }
    // handle input
    QString tmpQS = ui->textInput->text();
    if(tmpQS.length()==0) return;
    ui->textInput->clear();
    string tmpS = tmpQS.toStdString();

    //
    int parsingIndex = 0;
    int maxIndex = tmpS.length();
    int lineNumber = -1;
    vector<string> lineVec;
    string buf = "";
    char delim = ' ';
    while(parsingIndex<maxIndex){
        buf = "";
        while(parsingIndex < maxIndex && tmpS[parsingIndex]!=delim){
            if(tmpS[parsingIndex]=='\"'){
                if(!buf.empty()){
                    lineVec.push_back(buf);
                    buf="";
                }
                buf += tmpS[parsingIndex];
                parsingIndex++;
                while(parsingIndex < maxIndex && tmpS[parsingIndex]!='\"'){
                    buf += tmpS[parsingIndex];
                    parsingIndex++;
                }
                if(tmpS[parsingIndex]=='\"'){
                    buf += tmpS[parsingIndex];
                    parsingIndex++;
                }
                lineVec.push_back(buf);
                buf = "";
            }
            else if(tmpS[parsingIndex]=='\''){
                if(!buf.empty()){
                    lineVec.push_back(buf);
                    buf="";
                }
                buf += tmpS[parsingIndex];
                parsingIndex++;
                while(parsingIndex < maxIndex && tmpS[parsingIndex]!='\''){
                    buf += tmpS[parsingIndex];
                    parsingIndex++;
                }
                if(tmpS[parsingIndex]=='\''){
                    buf += tmpS[parsingIndex];
                    parsingIndex++;
                }
                lineVec.push_back(buf);
                buf = "";
            }
            else{
                buf += tmpS[parsingIndex];
                parsingIndex++;
            }
        }
        if(!buf.empty())
            lineVec.push_back(buf);
        if(tmpS[parsingIndex]==delim){
            parsingIndex++;
        }
        if(lineVec[0]=="PRINTF")
            delim = ',';
    }

    stringstream sstmp(lineVec[0]);
    if(!(sstmp >> lineNumber)){
        if(lineVec[0] == "LET" || lineVec[0] == "PRINT" || lineVec[0] == "INPUT" || lineVec[0] == "PRINTF" || lineVec[0]=="INPUTS")
            doitNow(lineVec);
        else if(handlingInput){
            if(typeTable[handlingVar]==_INT){// input
                if(lineVec[0] == "?" && lineVec.size() == 2) {
                    gotInput = true;
                    handlingInput = false;
                    handlingVal = std::stoi(lineVec[1]);
                }
                else{
                    ui->browserResult->setText("You must input a number begin with '? '");
                    ui->textInput->setText("? ");
                    return;
                }
            }
            else{ // inputs
                if(lineVec[0] == "??" && lineVec.size() == 2) {
                    if(isString(lineVec[1])){
                        gotInput = true;
                        handlingInput = false;
                        string trimmed = trimString(lineVec[1]);
                        handlingStr = trimmed;
                    }
                    else{
                        ui->browserResult->setText("not a string / string format error");
                        ui->textInput->setText("?? ");
                        return;
                    }
                }
                else{
                    ui->browserResult->setText("You must input a string begin with '?? '");
                    ui->textInput->setText("?? ");
                    return;
                }
            }
        }
        else if(!handlingInput && lineVec[0] == "?"){
            throw myException("unexpected input");
        }
        else if(!handlingInput && lineVec[0] == "??"){
            throw myException("unexpected inputs");
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

bool MainWindow::isString(string str) {
    int strlen = str.length();
    if(strlen > 2){
        if((str[0]=='\'' && str[strlen-1]=='\'')||(str[0]=='\"' && str[strlen-1]=='\"')){
            for(int i=1;i< strlen-1;++i){
                if(str[i]=='\''||str[i]=='\"')
                    return false;
            }
            return true;
        }
        else return false;
    }
    else return false;
}

string MainWindow::trimString(string str) {
    return str.substr(1, str.length()-2);
}
