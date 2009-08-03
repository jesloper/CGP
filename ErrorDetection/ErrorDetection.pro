TEMPLATE = lib
CONFIG += plugin debug
LANGUAGE = C++

INCLUDEPATH += ../headers/cgp \
               ../headers \
    		   ../src
HEADERS += ./ClassificationProblem.h
SOURCES += ./ClassificationProblem.cpp
TARGET = ../../plugins/ErrorDetection
FORMS += InputDialog.ui
