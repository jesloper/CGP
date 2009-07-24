#ifndef POPULATION_H
#define POPULATION_H

#include "Individual.h"
#include "Function.h"
#include "CGPPopulationBase.h"


class Problem;
/**
 * \brief This class is used to represent the population of individuals using 'regular' CGP.
 * 
*/
class CGPPopulation : public CGPPopulationBase {
public:
	
	virtual void createPopulation();	///< \brief creates a population of a given size and with a set number of nodes
	virtual void NewGeneration();		///< \brief evolves a new generation
    virtual void PrintGeneration();		///< \brief prints the entire generation
	void PrintBest();					///< \brief prints the best individual
};

#endif
