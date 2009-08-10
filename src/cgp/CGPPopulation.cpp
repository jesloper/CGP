#include "CGPPopulation.h"
#include "MathFunctions.h"
#include "Problem.h"
#include "Function.h"
#include "SimpleProblems.h"
#include "Macros.h"
#include <QtDebug>
#include <sstream>
#include <algorithm>

/**
 Creates a new population.
 */
void CGPPopulation::createPopulation() {
	qDebug() << "[CGPPopulation::createPopulation] setting up";
	srand((unsigned)time(0));

	this->numberOfInputs = ri.problem->NumberOfInputs();
	qDebug() << "Number of inputs = " << this->numberOfInputs;
	this->numberOfFunctions = ri.functions.size();
	qDebug() << "[CGPPopulation::createPopulation] number of inputs are " << this->numberOfInputs;
	qDebug() << "[CGPPopulation::createPopulation] qSize is  " <<ri.sel.QTournamentSize;
	CGPPopulationBase::inputList = new double[numberOfInputs+ri.gp.Nodes+10];
	try {
		int limit = 10;
		for(int i = 0; i < ri.gp.Individuals;i++) {
			if( (i*100)/ri.gp.Individuals> limit) {
				qDebug() << "[CGPPopulation::createPopulation] "<< limit << "% of individuals finished";
				limit += 10;
			}
			Individual thing;
			this->createIndividual(thing);
			pop.push_back(thing);
		}
	} catch(...) {
		qDebug() << "[CGPPopulation::createPopulation]something wrong in initialization";
	}

	qDebug() << "[CGPPopulation::createPopulation]computing fitness";
	computeFitness(this->pop);
	qDebug() << "[CGPPopulation::createPopulation]sorting...";
	std::sort(pop.begin(), pop.end(), AscendingSort());

	QMapIterator<QString,Function<double>*> it(ri.functions);
	//populate the map of available functions with the default list
	qDebug() << "[CGPPopulation::createPopulation]number of functions = " << this->numberOfFunctions;
	while(it.hasNext()){
		it.next();
		qDebug() << "[CGPPopulation::createPopulation] " << it.key();
	
	}
	
}

/**
 * Evolves the next generation.
 */
void CGPPopulation::NewGeneration() {
        QVector<Individual> newGen;
	ti.reset();

	//Save the best ind if elitism is set
	if (ri.sel.Elitism){
		Individual elite = this->getBest();
		elite.setFitness(this->getFitness(elite));
		newGen.push_back(elite);
	}
        int i = pop.size();

	/*
	 1. Select an individual
	 2. Check if it should be crossovered or mutated
	 3. If crossover get another ind and perform
	 4. If a mutation, mutate the ind
	 5. Add the new ind to the population
	 */
        while (newGen.size() < i) {
		Individual dude;
		getIndividual(dude);
		int random = getRandInt(0, 99);
		if (random < ri.gp.CrossoverProbability) {//crossover or reproduce
			Individual dude2;
			getIndividual(dude2);
			Individual newInd;
			CrossOver(dude, dude2, newInd);
			dude = newInd;
		}
		random = getRandInt(0, 99);
		if (random < ri.gp.MutationProbability) {//mutate
			//Individual dude2;
			//TIME_STATEMENT(dude2 = dude,ti.copy_time);
			mutate(dude);
			//we only keep helpful mutations
			// if(getFitness(dude2) >= getFitness(dude)){
			//	MYDEBUG(3,qDebug() << "Mutatation led to worse ind. keeping old  " << getFitness(dude2) << " vs " << dude.fitness);
			//}else{
			//TIME_STATEMENT(dude = dude2,ti.copy_time);
		}
		newGen.push_back(dude);
	}

	//remove the old population and copy in the new
	pop.clear();
	pop=newGen;
	computeFitness(pop);
	sortPool(pop, true);
	//ti.printInfo();
}

/**
 * prints all individuals and their fitness
 */
void CGPPopulation::PrintGeneration() {
	QVector<Individual>::iterator it;
	for (it = pop.begin(); it != pop.end(); it++) {
		(*it).printGenes();
		double fitness = (*it).getFitness();
		qDebug() << "Fitness = " << fitness;

	}
	qDebug() << "BestFitness = " << getBestFitness() ;
	qDebug() << "AverageFitness = " << getAvgFitness() ;

}

/**
 * Prints the best individual
 */
void CGPPopulation::PrintBest() {
	Individual best = getBest();
        for (int i = 0; i < best.getGenes().size(); i++) {
		int first = best.getGenes().at(i).getInput(1);
		int second = best.getGenes().at(i).getInput(2);
		QString function = best.getGenes().at(i).getFunction()->name();
		qDebug() << "#" << i+ri.problem->NumberOfInputs() << "  " << first << function << second;
	}
	this->HumanReadable(best);
}

