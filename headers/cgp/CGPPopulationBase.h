#ifndef POP_BASE_H
#define POP_BASE_H

#include "Individual.h"
#include "Function.h"
#include "PopulationInterface.h"
#include "RunInfo.h"
#include <ctime>

class Problem;

/**
 * \brief Interface class for all CGP population classes. 
 * Contains all common functionality for CGP populations such as keeping track of run information, retrieving human readable version of individuals, etc.
 */
class CGPPopulationBase : public PopulationInterface {
public:
	CGPPopulationBase() :
		inputList(0) {
	}
	;/**< \brief Ctor */
	virtual ~CGPPopulationBase(); /**< \brief Dtor */
	virtual void createPopulation() = 0; /**< \brief creates a population of a given size and with a set number of nodes */
	virtual void NewGeneration() = 0; /** \brief evolves the next generation */
	virtual void PrintGeneration() = 0; /** \brief prints the current generation */
	virtual void setInput(int number, double value); /**< \brief sets a particular input */
	virtual double getBestFitness(); /**< \brief returns the best fitness */
	virtual double getWorstFitness(); /**< \brief returns the worst fitness */
	virtual double getAvgFitness(); /**< \brief returns the average fitness of all individuals */
	virtual Individual& getBest(); /**< \brief returns the best fit individual */
	virtual Individual& getWorst(); /**< \brief returns the least fit individual */
	virtual void setInfo(RunInfo& info) { /**< \brief sets the run information */
		ri = info;
		
	}
	;
	virtual TwoDArray<double> getBestOutput();
        RunInfo ri;
	QString HumanReadable(Individual &ind);

	/*
	 Defines how we select the individuals when creating a new Generation
	 */
	enum SelectionType {QTournament,RoundRobin};
	SelectionType selType;
protected:
		RunInfo* params; /**< \brief parameters for the current run */
	double* inputList; /**< \brief placeholder when calculating the fitness for individuals */
	QVector<Individual> pop; /**< \brief temporary storage */

	int numberOfInputs;
	int numberOfFunctions;
	double FitnessTotal;
	double m_ind_outputs[20]; //TODO: this is a hard-limit on number of outputs (but reasonable enough)
	double m_ind_inputs[100]; //TODO: this is a hard-limit, but ok for now
	double m_ind_fitnessTotal; ///< \brief fitness total of a single individual
	double m_ind_fit;	///< \brief fitness for a single individual for a single fitnesscase
	QMap<QString, double> m_fitnessMap; ///< \brief contains already calculated values. Used to improve performance
	double getFitness(Individual& it);

	void mutate(Gene& it, int number); ///< \brief mutates a Gene
	void mutate(Individual& it); ///< \brief mutates an individual
	void CrossOver(Individual& first, Individual &second, Individual &result); ///< \brief performs a crossover of two individuals and stores the result in the third parameter
	void regularCrossOver(Individual& first, Individual& second, Individual& result); ///< \brief performs a 'regular crossover' in CGP
	void treeCrossOver(Individual& first, Individual &second, Individual &result);
	void simpleCrossOver(Individual& first, Individual& second, Individual& result); ///< \brief performs a 'simple' crossover in CGP
	void directedGraphTreeCrossOver(Individual& first, Individual &second, Individual &result);
	void gridTreeCrossOver(Individual& first, Individual &second, Individual &result);
		
	void getNodes(int at, Individual& parent, QVector<int> &nodes);	///< \brief used by the tree crossover methods
	void computeFitness(QVector<Individual> &pool); ///< \brief computes the fitness of all individuals
        void regularCompute(QVector<Individual> &pool); ///< \brief Regular computation of fitness
        void hashCompute(QVector<Individual> &pool); /// \brief use of hash to speed up computation
	void getIndividual(Individual& ind);
        Individual& tournamentSelection(int q,QVector<Individual> &pool);
        Individual& randomSelection(QVector<Individual> &pool);
        Individual& rankSelection(QVector<Individual> &pool);
	void insertInLevel(int level, QVector<Individual>& buffer);
	void computeThresholds();
	double getAvg(QVector<Individual>& pool); /**< \brief returns the average fitness of all individuals */
	int validInput(int node);
	void sortPool(QVector<Individual>& pool, bool); ///< \brief sorts a population
	void SanityCheck(Individual& ind);
	void createIndividual(Individual& ind); ///< \brief Creates a new random individual

	/** 
	 * \brief contains timing information used for debugging purposes
	 */
	struct timingInfo {
		void reset() {
			Crossovers = 0;
			Mutations = 0;
			cross_time = 0;
			mut_time = 0;
			sorting_time =0;
			fitness_time = 0;
			retrieving_time = 0;
			rand_num = 0;
			functionStart =:: clock();
			copy_time = 0;
		}
		int Crossovers;
		int Mutations;
		double cross_time;
		double mut_time;
		double sorting_time;
		double fitness_time;
		double retrieving_time;
		double rand_num;
		double functionStart;
		double copy_time;
		void printInfo() {
			qDebug() << "[TimingInfo] Finsihed";
			qDebug() << "[TimingInfo] Crossovers = " << Crossovers << " total time = " << cross_time << "  per crossover = " << cross_time/(double)(Crossovers);
			qDebug() << "[TimingInfo] Mutations  = " << Mutations << " mut  time = " << mut_time << " per mut = " << mut_time/(double)(Mutations);
			qDebug() << "[TimingInfo] Sorting    = " << sorting_time;
			qDebug() << "[TimingInfo] Computing fitness = " << fitness_time;
			qDebug() << "[TimingInfo] Getting individuals = " << retrieving_time;
			//qDebug() << "[TimingInfo] Getting rands = " << rand_num;
					qDebug() << "[TimingInfo] Copying = " << copy_time;

					qDebug() << "[TimingInfo] Total timed functions = " << cross_time + mut_time + sorting_time + fitness_time + retrieving_time;

					qDebug() << "[TimingInfo] Total function = " << (double)(clock() - functionStart)/CLOCKS_PER_SEC;

				}
			};
			timingInfo ti;
		};

#endif //POP_BASE_H
