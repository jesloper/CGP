TEMPLATE = lib
CONFIG += plugin
LANGUAGE = C++

INCLUDEPATH += ../headers \
               ../headers/cgp \
    		   ../src
HEADERS += ./MultiClassProblem.h
SOURCES += ./MultiClassProblem.cpp
TARGET = ../../plugins/ErrorDetectionMulti
