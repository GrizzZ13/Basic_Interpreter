/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_4;
    QTextBrowser *browserCode;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_3;
    QTextBrowser *browserResult;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QTextBrowser *browserStructure;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_5;
    QTextBrowser *browserVariable;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonLoad;
    QPushButton *buttonRun;
    QPushButton *buttonDebug;
    QPushButton *buttonClear;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLineEdit *textInput;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QMenu *menuBASIC_Interrpeter;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(950, 879);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout_3 = new QHBoxLayout(centralwidget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetNoConstraint);
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setSizeConstraint(QLayout::SetNoConstraint);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_6->addWidget(label_4);

        browserCode = new QTextBrowser(centralwidget);
        browserCode->setObjectName(QString::fromUtf8("browserCode"));
        browserCode->setAutoFillBackground(false);

        verticalLayout_6->addWidget(browserCode);


        horizontalLayout_2->addLayout(verticalLayout_6);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setSizeConstraint(QLayout::SetNoConstraint);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_4->addWidget(label_3);

        browserResult = new QTextBrowser(centralwidget);
        browserResult->setObjectName(QString::fromUtf8("browserResult"));

        verticalLayout_4->addWidget(browserResult);


        horizontalLayout_2->addLayout(verticalLayout_4);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetNoConstraint);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_3->addWidget(label_2);

        browserStructure = new QTextBrowser(centralwidget);
        browserStructure->setObjectName(QString::fromUtf8("browserStructure"));

        verticalLayout_3->addWidget(browserStructure);


        horizontalLayout_6->addLayout(verticalLayout_3);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        verticalLayout_5->addWidget(label_5);

        browserVariable = new QTextBrowser(centralwidget);
        browserVariable->setObjectName(QString::fromUtf8("browserVariable"));

        verticalLayout_5->addWidget(browserVariable);


        horizontalLayout_6->addLayout(verticalLayout_5);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        buttonLoad = new QPushButton(centralwidget);
        buttonLoad->setObjectName(QString::fromUtf8("buttonLoad"));

        horizontalLayout->addWidget(buttonLoad);

        buttonRun = new QPushButton(centralwidget);
        buttonRun->setObjectName(QString::fromUtf8("buttonRun"));

        horizontalLayout->addWidget(buttonRun);

        buttonDebug = new QPushButton(centralwidget);
        buttonDebug->setObjectName(QString::fromUtf8("buttonDebug"));

        horizontalLayout->addWidget(buttonDebug);

        buttonClear = new QPushButton(centralwidget);
        buttonClear->setObjectName(QString::fromUtf8("buttonClear"));

        horizontalLayout->addWidget(buttonClear);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetNoConstraint);
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        textInput = new QLineEdit(centralwidget);
        textInput->setObjectName(QString::fromUtf8("textInput"));

        verticalLayout_2->addWidget(textInput);


        verticalLayout->addLayout(verticalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 950, 22));
        menuBASIC_Interrpeter = new QMenu(menubar);
        menuBASIC_Interrpeter->setObjectName(QString::fromUtf8("menuBASIC_Interrpeter"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuBASIC_Interrpeter->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Code", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Result", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Structure", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Variables", nullptr));
        buttonLoad->setText(QCoreApplication::translate("MainWindow", "LOAD", nullptr));
        buttonRun->setText(QCoreApplication::translate("MainWindow", "RUN", nullptr));
        buttonDebug->setText(QCoreApplication::translate("MainWindow", "DEBUG", nullptr));
        buttonClear->setText(QCoreApplication::translate("MainWindow", "CLEAR", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Command Line", nullptr));
        menuBASIC_Interrpeter->setTitle(QCoreApplication::translate("MainWindow", "BASIC Interrpeter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
