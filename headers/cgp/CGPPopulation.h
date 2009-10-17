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
private:
    class MutateHelper : public QRunnable
    {

    public:
        MutateHelper(QVector<Individual>& pop, RunInfo& ri,CGPPopulation* that):m_pop(pop),m_ri(ri),m_that(that){}
    private:
        void run()
        {
            QVectorIterator<Individual> it(m_pop);
            while(it.hasNext()){
                Individual dude = it.next();
                int random = getRandInt(0, 99);
                if (random < m_ri.gp.MutationProbability) {//mutate
                    m_that->mutate(dude);
                }
            }

        }
        QVector<Individual>& m_pop;
        RunInfo& m_ri;
        CGPPopulation* m_that;
     };
};

#endif
