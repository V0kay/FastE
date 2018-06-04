QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FastE
TEMPLATE = app


SOURCES += main.cpp\
        faste.cpp

HEADERS  += faste.h \
    plaintextedit.h

FORMS    += faste.ui
