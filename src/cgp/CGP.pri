SRC_DIR = ./CGP

INCLUDE_DIR = ../headers/CGP/
INCLUDEPATH += $${INCLUDE_DIR}
HEADERS += $${INCLUDE_DIR}/Individual.h \
					$${INCLUDE_DIR}/HFCPopulation.h \
					$${INCLUDE_DIR}/CGPPopulationBase.h \
					$${INCLUDE_DIR}/Gene.h \
					$${INCLUDE_DIR}/GeneFactory.h \ 
					$${INCLUDE_DIR}/SimpleProblems.h \
					$${INCLUDE_DIR}/FunctionFactory.h \
					$${INCLUDE_DIR}/Function.h \
					$${INCLUDE_DIR}/SingletonCreator.h \
					$${INCLUDE_DIR}/RunInfo.h \
					$${INCLUDE_DIR}/Problem.h 
					

SOURCES +=  $${SRC_DIR}/CGPPopulation.cpp \
   $${SRC_DIR}/Individual.cpp \
   $${SRC_DIR}/HFCPopulation.cpp \
   $${SRC_DIR}/CGPPopulationBase.cpp \
  $${SRC_DIR}/Gene.cpp \
   $${SRC_DIR}/GeneFactory.cpp \
   $${SRC_DIR}/SimpleProblems.cpp \
   $${SRC_DIR}/FunctionFactory.cpp \
   $${SRC_DIR}/Function.cpp  \
  $${SRC_DIR}/RunInfo.cpp

