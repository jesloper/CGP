#ifndef HFCPOPULATION_H_
#define HFCPOPULATION_H_
#include "CGPPopulationBase.h"
#include "Function.h"

class RunInfo;

class HFCPopulation: public CGPPopulationBase {
public:

	HFCPopulation(); ///< \brief Ctor
	virtual ~HFCPopulation(); ///< \brief Dtor
	virtual void createPopulation(); ///< \brief creates a population of a given size and with a set number of nodes
	virtual void setInput(int number, double value); ///< \brief sets a particular input
	virtual void NewGeneration(); ///< \brief evolves the next generation
	virtual void PrintGeneration(); ///< \brief prints the current generation
	virtual double getBestFitness(); ///< \brief returns the best fitness
	virtual double getWorstFitness(); ///< \brief returns the worst fitness
	virtual double getAvgFitness(); ///< \brief returns the average fitness of all individuals
	virtual Individual& getBest(); ///< \brief returns the best fit individual
	virtual Individual& getWorst(); ///< \brief returns the least fit individual
	virtual void resetCalculations(); ///< \brief resets the remebered calulations
private:
	RunInfo* params; ///< \brief parameters for the current run
	QVector<Individual> pop; ///< \brief temporary storage
	QVector<QVector<Individual>*> pools; ///< \brief contains the pools of individuals

	double* thresholds; ///< \brief current thresholds for all levels
	int numPools; ///< \brief number of pools

	void addIndividual(Individual& ind); ///< \todo: move to CGPPopulationBase

        void insertInLevel(int level, QVector<Individual>& buffer); ///< \brief inserts a vector of individuals at a given level
	void computeThresholds(); ///< \brief computes new threshold for all levels
	double getAvg(QVector<Individual>& pool); ///< \brief returns the average fitness of all individuals
};

#endif /*HFCPOPULATION_H_*/
