message("Building src")
TEMPLATE = app
QT += xml
 PLATFORM = win32   
 LANGUAGE = C++
 CONFIG += debug \
 					  windows
 MOC_DIR = .moc
 OBJECTS_DIR = .obj

 QWT_ROOT = ../
 INCLUDEPATH += ../headers \
		    ../headers/cgp \
		    ../headers/gui \
		    ../ui 
								
include(./gui/GUI.pri)
include(./cgp/CGP.pri )
RESOURCES = ../icons/sources.qrc
HEADERS += ../headers/TwoDArray.h \
           ../headers/MathFunctions.h \
           ../headers/Macros.h
SOURCES +=  main.cpp \
						MathFunctions.cpp


CONFIG(debug, debug|release) {
     TARGET = ../$${QWT_ROOT}bin/CGP
 } else {
     TARGET = ../$${QWT_ROOT}bin/release/CGP
 } 
 message("My target is $${TARGET}")
  
