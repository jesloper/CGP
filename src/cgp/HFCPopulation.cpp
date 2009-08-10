#include <HFCPopulation.h>
#include <MathFunctions.h>
#include <QtDebug>
#include <sstream>
#include "Macros.h"
#include <ctime>
#include <algorithm>
int poolNum = 0;

/**
 * ctor.
 * Creates a new population object.
 * Clears the function list
 */
HFCPopulation::HFCPopulation() {
	qDebug() << __FUNCTION__ << "Creating population class";
}

/**
 * Destructor
 */
HFCPopulation::~HFCPopulation() {
	qDebug() << __FUNCTION__ << "Deleting HFCPopulation";
	delete[] this->thresholds;
	for (int i = 0; i < numPools; i++) {
		//delete subpool #i
		delete pools[i];
	}

}

/**
 * Calculates the average fitness for all the individuals in the population.
 * NOTE: this should never be called before all individuals have been assigned a fitness
 */
double HFCPopulation::getAvgFitness() {

	HERE(2);//qDebug() << __FUNCTION__ <<"getAvgFitness()";
	QVector<Individual>::iterator it;
	double avg = 0;
	int num = 0;
	for (int i = 0; i < numPools; i++) {
		//MYDEBUG(1,qDebug() << "Poolsize = " << pools[i]->size());
		for (it = pools[i]->begin(); it != pools[i]->end(); it++) {
			avg += (*it).getFitness();
			num++;
		}
	}
	MYDEBUG(2,qDebug() << "Got avg for " << num << "individuals");
	return avg / double(ri.gp.Individuals);
}

/**
 * returns the indicvidual with the lowest fitness
 * NOTE: this should never be called if the population is not sorted
 */
Individual& HFCPopulation::getBest() {
	Individual* best = &pools[0]->front();
	for (int i = 1; i < numPools; i++) {
		if (pools[i]->front().getFitness() < best->getFitness())
			best = &pools[i]->front();
	}
	return *best;
}

/**
 * returns the individual with the worst fitness
 * NOTE: this should never be called if the population is not sorted
 */
Individual& HFCPopulation::getWorst() {
	Individual* worst = &pools[0]->back();
	HERE(2);//MYDEBUG(1,qDebug() << "[HFCPopulation::getWorst]");
	for (int i = 1; i < numPools; i++) {
		if (pools[i]->back().getFitness() > worst->getFitness())
			worst = &pools[i]->back();
	}
	return *worst;
}

/**
 * Creates a new population.
 */
void HFCPopulation::createPopulation() {
	srand((unsigned) time(0));

	//store some parameters locally
	this->numberOfInputs = ri.problem->NumberOfInputs();
	this->numberOfFunctions = ri.functions.size();
	this->numPools = 3;

	this->thresholds = new double[numPools];
	//create the pools.
	HERE(2);
	for (int i = 0; i < numPools; i++) {
		//create subpool #i
		QVector<Individual>* pop = new QVector<Individual>();
		pools.push_back(pop);
	}

	//assign memory for inputs and genes
	CGPPopulationBase::inputList = new double[numberOfInputs + ri.gp.Nodes + 10];

	HERE(2);
	//here we assigne the random individuals
	try {
		for (int i = 0; i < ri.gp.Individuals; i++) {
			Individual thing;
			this->createIndividual(thing);
			pop.push_back(thing);
		}
	} catch (...) {
		HERE(0);
		QMessageBox::warning(0, QString("Aborting!"), QString("Could not create ind!!"));

		abort();
	}
	MYDEBUG(0, qDebug() << "[HFCPopulation::createPopulation]computing fitness");

	//compute fitness of initial population then sort it
	computeFitness(pop);
	HERE(2);
	std::sort(pop.begin(), pop.end(), DescendingSort());

	int num = 0;
	int poolNum = 0;
	double total = numPools * (numPools + 1) / 2.0;
	double v = 1;
	double share = v / total;
	//use a default three pools
	qDebug() << __FUNCTION__ << "[HFCPopulation::createPopulation] adding to pool num = " << poolNum << "  Share = " << share;
	qDebug() << __FUNCTION__ << "[HFCPopulation::createPopulation] ind left = " << pop.size();
	//qDebug() << "[HFCPopulation::createPopulation] Individuals = " << ri.gp.Individuals;
	//qDebug() << "[HFCPopulation::createPopulation] test = " << (((num > share*ri.gp.Individuals) && (poolNum < numPools-1))? "TRUE":"FALSE")  ;

	//m_population.pop_back();

	while (!pop.empty()) {

		if (num > (share * ri.gp.Individuals) && (poolNum < numPools - 1)) {
			poolNum++;
			v++;
			share = v / total;
			qDebug() << "[HFCPopulation::createPopulation] adding to pool num = " << poolNum << "  Share = " << share;
			qDebug() << "[HFCPopulation::createPopulation] ind left = " << pop.size();
			num = 0;
		}
		pools[poolNum]->push_back(pop.back());
		pop.pop_back();
		num++;
	}
	computeThresholds();
	qDebug() << "[HFCPopulation::createPopulation] finished pools";
	//this->resetCalculations();
}

void HFCPopulation::addIndividual(Individual&) {
	//add the ind to the correct pool
}

/**
 * Sets an input
 * \param number which input (first, second, etc)
 * \param value  value of the input
 */
void HFCPopulation::setInput(int number, double value) {
	inputList[number] = value;
}

/**
 * Retrieves the fitness of the best fit individual
 */
double HFCPopulation::getBestFitness() {
	HERE(2);
	Individual best = getBest();
	HERE(2);
	return best.getFitness();
}

/**
 * Retrives the fitness of the least fit individual
 */
double HFCPopulation::getWorstFitness() {
	HERE(2);
	Individual worst = getWorst();
	//worst.printGenes();
	HERE(2);
	return worst.getFitness();

}
/**
 * Gets an individual from the given pool using the currently set selection mechanism
 * \param pool sub-pool to select from
 * \param ind reference to individual to be filled with return value
 */
void HFCPopulation::getIndividual(QVector<Individual>& pool, Individual& ind) {
	switch (this->ri.sel.Selection) {
	case PopulationInterface::QTournament:
		ind = tournamentSelection(ri.sel.QTournamentSize, pool);
		break;
	case PopulationInterface::Random:
		ind = this->randomSelection(pool);
		break;
	default:
		qDebug() << "Selection type is: " << ri.sel.Selection;
		qDebug() << "Possible values are: " << PopulationInterface::QTournament << " and " << PopulationInterface::Random;
		HERE(0)
		;
		abort();
	}
}

/**
 * Gets two individuals among the top q%, then return the best one
 * This of course assumes the population to be sorted
 * \param q percentage of population to pick individual from
 * \param pool sub-pool to select from
 *
Individual& HFCPopulation::tournamentSelection(int q, QVector<Individual>& pool) {
	int max = (pool.size() * q) / 100;
	int rand = getRandInt(0, max - 1);
	int rand2 = getRandInt(0, max - 1);
	//	qDebug() << "Getting " << rand << " or " << rand2 << "(max = " << max << "  q = " << q << " )";
	if (pool.at(rand).getFitness() < pool.at(rand2).getFitness())
		return pool[rand];
	return pool[rand2];
}

Individual& HFCPopulation::randomSelection(QVector<Individual>& pool) {

	int rand = getRandInt(0, pool.size() - 1);

	if (poolNum == 0)
		MYDEBUG(2,qDebug() << "returning ind at " << rand);

	return pool[rand];
}
*/
void printPool(QVector<Individual> pop) {
	QVector<Individual>::iterator it;
	for (it = pop.begin(); it != pop.end(); it++) {
		double fitness = (*it).getFitness();
		MYDEBUG(0,qDebug() << "Fitness = " << fitness);
	}
}

/**
 * 	Evolves the next generation.
 */
void HFCPopulation::NewGeneration() {
	static int genNum = 0;
	genNum++;
	QVector<Individual> newGen;

	//Save the best ind if elitism is set
	if (ri.sel.Elitism) {
		Individual save;
		save = this->getBest();
		save.setFitness(this->getFitness(save));
		newGen.push_back(save);
	}
	//first we evolve, then we check for admissions
        for (int i = 0; i < pools.size(); i++) {
		poolNum = i;
                int size = pools[i]->size();
		int random;
		while (newGen.size() < size) {
			Individual parent;
			getIndividual(*pools[i], parent);//retrieve from population
			random = getRandInt(0, 99);

			if (random < ri.gp.CrossoverProbability) {
				Individual parent2;
				getIndividual(*pools[i], parent2);
				int trys = 0;
				while (parent2.equals(parent)) {
					trys++;
					if (trys == 10) {
						break;
					}
					getIndividual(*pools[i], parent2);
				}
				CrossOver(parent, parent2, parent);//result

			}//if
			//perform mutation with prob =  ri.gp.MutationProbability
			if (getRandInt(0, 99) < ri.gp.MutationProbability) {
				mutate(parent);
			}
			newGen.push_back(parent);
		}//while

		//qDebug() << "Clearing pool #" << i;
		sortPool(newGen, true);
		pools[i]->clear();
		*pools[i] = newGen;
		QVector<Individual>()= newGen; // release all memory
		computeFitness(*pools[i]);
		sortPool(*pools[i], true);
	}

	//We will only do admissions every 5 generations:
	if (genNum % 3 == 0) {
		qDebug() << "Starting swap!";
		//sort all pools in descending order
                for (int i = 0; i < pools.size(); i++) {
			sortPool(*pools[i], false);
		}
                int* size = new int[numPools]; //placeholder for poolsizes
                for (int i = 0; i < pools.size(); i++) {
			size[i] = pools[i]->size();
		}

		//create an admission buffer
		QVector<Individual> bufferUp;
		//loop over pools and move individuals
		for (size_t i = numPools - 1; i > 0; i--) {
			qDebug() << "Checking " << pools[i]->back().getFitness() << " vs " << thresholds[i - 1];
			while (pools[i]->back().getFitness() < thresholds[i - 1] && (bufferUp.size() < pools[i]->size() / 2)) {
				//MYDEBUG(0,qDebug() << "This is ( " << pools[i]->back().getFitness() << " ) a candidate to move from " << i << " to " << i-1);
				bufferUp.push_back(pools[i]->back()); //copy the last to the next level buffer
				pools[i]->pop_back(); //remove it from the current
			}
			//sort the buffer
			sortPool(bufferUp, false);
			//add the buffer to the new level
			insertInLevel(i - 1, bufferUp);
			bufferUp.clear();

			//if there are not enough indviduals in the pool we will fill it up with random ones
			while (pools[i]->size() < size[i]) {
				Individual fresh;
				this->createIndividual(fresh);
				fresh.setFitness(this->getFitness(fresh));
				pools[i]->push_back(fresh);
			}

			//sort the new level (best at the back)
			sortPool(*pools[i - 1], false);

			//trimming extra individuals from the pool
			while (pools[i]->size() > size[i]) {
				pools[i]->pop_back();
			}

		}

		//sortAllPools
                for (int i = 0; i < pools.size(); i++) {
			sortPool(*pools[i], true);
		}

		//trimming the top pool
		while (pools[0]->size() > size[0]) {
			MYDEBUG(2,qDebug() << "Trimming pool " << 0 << " of ind with fitness " << pools[0]->back().getFitness());
			pools[0]->pop_back();
		}
		delete[] size;
	}

	//update new thresholds
	computeThresholds();

}

//prints all individuals and their fitness
void HFCPopulation::PrintGeneration() {
	qDebug() << __FUNCTION__ << "Not Implemnted! ";

}

/**
 * Inserts a buffer of individuals into a level
 */
void HFCPopulation::insertInLevel(int level, QVector<Individual> &buffer) {

	//now we will copy all into this level
        int size = buffer.size();

	if (size > 0) {
		qDebug() << __FUNCTION__ << "inserting in level " << level << "buffersize" << buffer.size();

		if (size > (pools[level]->size() / 2)) {
			qDebug() << __FUNCTION__ << "Too many inds in buffer (" << size << ") cutting size";
			size = (pools[level]->size() / 2);
			qDebug() << __FUNCTION__ << "New size = (" << size << ")";

		}
                for (int i = 0; i < size; i++) {
			//qDebug() << __FUNCTION__ <<"Inserting something with fitness = " << buffer.back().getFitness() << " into level " << level;
			pools[level]->push_back(buffer.back());
			buffer.pop_back();
		}

	}

}

/**
 * computes the new thresholds for each level
 */
void HFCPopulation::computeThresholds() {
	//compute threshold as the average of thats levels fitness - half the distance of its average to the best
	for (int i = 0; i < numPools; i++) {
		double best = pools[i]->front().getFitness();
		double avg = getAvg(*pools[i]);
		double t = avg;
		thresholds[i] = (t + best) / 2;
		//qDebug() << __FUNCTION__ << "Size = " << pools[i]->size() << " Best = " << best << " Avg = " << avg << "T#" << i << " = " << thresholds[i];

	}
}

double HFCPopulation::getAvg(QVector<Individual> &pool) {
	QVector<Individual>::iterator it;
	double avg = 0.;
	for (it = pool.begin(); it != pool.end(); it++) {
		avg += (*it).getFitness();
		//qDebug() << __FUNCTION__ <<"ind = " << (*it).getFitness();
	}
	return avg / double(pool.size());
}
/*double HFCPopulation::getStdDev(QVector<Individual> &pool, double mean) {
	double sum = 0.;
	for (it = pool.begin(); it != pool.end(); it++) {
		double diff = (*it).getFitness - mean;
		sum += diff *diff;
	}
	double std_dev = sqrt(sum/double(pool.size()));
	return std_dev;
}
*/
/**
 * Loop over all individuals and reset all remembered valus
 */
void HFCPopulation::resetCalculations() {
	for (int i = 0; i < numPools; i++) {
		QVector<Individual>::iterator it;
		QVector<Individual> pool = *pools[i];
		for (it = pool.begin(); it != pool.end(); it++) {
			//(*it).reset();
		}
	}

}
