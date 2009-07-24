TEMPLATE = lib 

LANGUAGE = C++
QT += xml
QWT_ROOT = ../../

OBJECTS_DIR = .obj

PLATFORM = win32

INCLUDEPATH += $${QWT_ROOT}/headers 

INCLUDE_DIR = $${QWT_ROOT}/headers

HEADERS = $${INCLUDE_DIR}/CGPPopulation.h \
					$${INCLUDE_DIR}/Individual.h \
					$${INCLUDE_DIR}/MathFunctions.h \
					$${INCLUDE_DIR}/HFCPopulation.h \
					$${INCLUDE_DIR}/CGPPopulationBase.h \
					$${INCLUDE_DIR}/Gene.h \
					$${INCLUDE_DIR}/GeneFactory.h \ 
					$${INCLUDE_DIR}/SimpleProblems.h \
					$${INCLUDE_DIR}/FunctionFactory.h \
					$${INCLUDE_DIR}/Function.h \
					$${INCLUDE_DIR}/SingletonCreator.h \
					$${INCLUDE_DIR}/RunInfo.h

SOURCES =     CGPPopulation.cpp \
   Individual.cpp \
   MathFunctions.cpp \
   HFCPopulation.cpp \
   CGPPopulationBase.cpp \
   Gene.cpp \
   GeneFactory.cpp \
   SimpleProblems.cpp \
   FunctionFactory.cpp \
   Function.cpp  \
   RunInfo.cpp

TARGET = ../$${QWT_ROOT}/lib/CGP
