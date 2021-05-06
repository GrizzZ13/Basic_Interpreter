# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = BasicInterpreter

QT = core gui widgets

HEADERS = \
   $$PWD/buffer.h \
   $$PWD/Evaluation.h \
   $$PWD/expression.h \
   $$PWD/mainwindow.h \
   $$PWD/myexception.h \
   $$PWD/statement.h \
   $$PWD/tree.h \
   $$PWD/variables.h

SOURCES = \
   $$PWD/buffer.cpp \
   $$PWD/Evaluation.cpp \
   $$PWD/expression.cpp \
   $$PWD/main.cpp \
   $$PWD/mainwindow.cpp \
   $$PWD/statement.cpp \
   $$PWD/tree.cpp \
   $$PWD/variables.cpp

INCLUDEPATH = \
    $$PWD/.

#DEFINES = 

FORMS += \
    mainwindow.ui

