INCLUDEPATH +=  C:\qwt-5.1.1\include
        
LIBS += -LC:\qwt-5.1.1\lib  -lqwt5

INCLUDEDIR = ../headers/GUI
INCLUDEPATH += $${INCLUDEDIR}
SRC_DIR=./GUI
HEADERS += $${INCLUDEDIR}/CompThread.h \
					$${INCLUDEDIR}/DataGrapher.h \
					$${INCLUDEDIR}/MainWindow.h \
					$${INCLUDEDIR}/GridViewer.h \
					$${INCLUDEDIR}/RunInfoTree.h \
					$${INCLUDEDIR}/FunctionSelecter.h \
					$${INCLUDEDIR}/Logger.h \
					$${INCLUDEDIR}/SimpleGrapher.h \
					$${INCLUDEDIR}/SR_ProblemSelecter.h \
					$${INCLUDEDIR}/qdebugstream.h			
					

SOURCES += $${SRC_DIR}/CompThread.cpp \   
					$${SRC_DIR}/DataGrapher.cpp \
					$${SRC_DIR}/FunctionSelecter.cpp \
					$${SRC_DIR}/GridViewer.cpp \
					$${SRC_DIR}/MainWindow.cpp \
					$${SRC_DIR}/RunInfoTree.cpp \
					$${SRC_DIR}/Logger.cpp \
				    $${SRC_DIR}/SimpleGrapher.cpp \
				    $${SRC_DIR}/qdebugstream.cpp
					

UI_DIR = ../.ui
UI_HEADERS_DIR = $${UI_DIR}
FORMS += $${UI_DIR}/cgpWindow.ui \
    $${UI_DIR}/gridViewer.ui \
    $${UI_DIR}/FunctionChooser.ui \
    $${UI_DIR}/SR_ProblemSelecter.ui

