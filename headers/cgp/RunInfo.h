#ifndef RUNINFO_H
#define RUNINFO_H
#include <iostream>
#include <QTextStream>
#include <QtXml/QDomDocument>
#include "Function.h"
#include <QtDebug>
#include <map>
#include "TwoDArray.h"
#include "Problem.h"
#include "Macros.h"
/**
 * Class to keep all info and parameters about current run.
 * Supports functionlaity for storing the data in the xml format.
 */
class RunInfo {
	/**
	 * Nested class to keep all GP parameters
	 */
	struct GPParameters {
		int Individuals;
		int Generations;
		int MutationProbability;
		int CrossoverProbability;
		int PointMutationProbability;
		int Nodes;
		bool Grid;
		bool HFC;
		int Rows;
		int Cols;
		int Connections;
		bool treeCrossover;
		void store(QDomElement parent, QDomDocument doc);
	};

	/**
	 *	Nested class to keep all selection paramaters
	 */
	struct SelecetionParameters {
		bool Elitism;
		int Selection;
		int QTournamentSize;
	};

	/**
	 *	Nested class to keep data about each generation
	 */
	struct RunData {
		int CompletedGenerations;
		TwoDArray<double> Stats;
		std::vector<QString> BestInds;
		std::vector<QString> ActiveNodes;
		std::vector<QString> FunctionUsage;

		void store(QDomElement parent, QDomDocument doc);
		RunData() {
		}
		;
		~RunData() {
		}
		;
	};

public:
	RunInfo();
	~RunInfo(void) {
	}
	;
	std::map<std::string, Function<double>*> functions; ///< \brief available functions in this run
	std::map<std::string, int> functionHeuristics; ///< \brief keeps count of mostly used functions
	std::string getRandomFunction() {
		std::map<std::string, Function<double>*>::iterator it;
		int rand = getRandInt(0, functions.size() - 1);
		it = functions.begin();
		for (int k = 0; k < rand; k++) {
			it++;
		}
		return it->first;
	}
	/**
	 * Stores all data in the DomDocument
	 */
	void store(QDomDocument& doc);
	void setGPInfo(int ind, int gen, int nodes, int mut, int cross, int point,
			int back, bool tree);
	void setGridInfo(bool grid, int Rows, int Cols, int cons);
	void setSelectionInfo(bool elitism, int selType);
	void setRunData(int generations);
	void updateData(int gen, double best, double avg, double worst,
			QString bestInd, QString activeNodes,QString functionUsage);
	void reset();
	GPParameters gp;
	SelecetionParameters sel;
	RunData rd;
	QString bestInd;
	QString activeNodes;
	QString m_functionUsage;
	Problem* problem;
};

#endif
