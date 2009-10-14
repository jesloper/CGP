#ifndef POPULATION_INTERFACE_H_
#define POPULATION_INTERFACE_H_

#include "Individual.h"
#include "Problem.h"
#include "RunInfo.h"
#include "TwoDArray.h"

/**
 * Abstract class.
 * This class defines the interface for all different types of populations
 */
class PopulationInterface {
public:
        enum SelectionType {QTournament,Random,Roulette,Rank}; ///< \brief different selection types available
        virtual ~PopulationInterface(){}
	virtual void createPopulation() = 0; /**< \brief creates a population of a given size and with a set number of nodes */
	virtual void setInput(int number, double value) = 0; /**< \brief sets a particular input */
	virtual void NewGeneration() = 0; /** \brief evolves the next generation */
	virtual void PrintGeneration() = 0; /** \brief prints the current generation */
	virtual double getBestFitness() = 0;/** \brief returns the best fitness */
	virtual double getWorstFitness() = 0; /** \brief returns the worst fitness */
	virtual double getAvgFitness() = 0; /**< \brief returns the average fitness of all individuals */
	virtual Individual& getBest() = 0; /**< \brief returns the best fit individual */
	virtual Individual& getWorst() = 0; /**< \brief returns the least fit individual */
	virtual void setInfo(RunInfo& ri) = 0;/**< \brief sets the current run information */
	virtual QString HumanReadable(Individual&)= 0; /**< \brief returns a Human readable string of an ind */
	virtual TwoDArray<double> getBestOutput() = 0; ///< \brief retrieves a vector of outputs from the best individual
private:
};

#endif /*POPULATION_INTERFACE_H_*/
