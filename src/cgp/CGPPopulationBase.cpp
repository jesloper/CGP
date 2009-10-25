#ifdef WIN32
#define isnan(x) ((x) != (x))
#endif
#include "CGPPopulationBase.h"
#include "Individual.h"
#include "MathFunctions.h"
#include "Problem.h"
#include "Function.h"
#include "SimpleProblems.h"
#include "Macros.h"
#include <QtDebug>
#include <QtGui>
#include <sstream>
#include <ctime>
#include <algorithm>

/**
 * Destructor.
 * deletes the inputList
 */
CGPPopulationBase::~CGPPopulationBase() {
    if (inputList)
        delete[] inputList;
}

double CGPPopulationBase::threadedGetFitness(Individual& it){
    QFuture<double> t = QtConcurrent::run(this,&CGPPopulationBase::getFitness,it);
    return t.result();
}
/**
 * Gets the fitness of a single Individual.
 * Loops over all fitnesscases, computes individual output and retrives fitness from problem.
 */
double CGPPopulationBase::getFitness(Individual& it) {
    TwoDArray<double>* input = ri.problem->getAllInputs();
    TwoDArray<double> results(input->rows(),ri.problem->NumberOfOutputs());
    for(size_t i = 0;i < input->rows();++i){
        double* problemInputs = (*input)[i];
        it.getOutput(m_ind_outputs, this->ri, problemInputs);
        for(int j = 0; j < ri.problem->NumberOfOutputs();++j){
            results[i][j] = m_ind_outputs[j];
        }
    }
    return ri.problem->setFitness(results);

}

/*
 Calculates the average fitness for all the individuals in the population.
 */
double CGPPopulationBase::getAvgFitness() {
    QVector<Individual>::iterator it;
    double avg = 0;
    for (it = pop.begin(); it != pop.end(); it++) {
        avg += (*it).getFitness();
    }
    //qDebug() << "avg total = " << avg << "  and avg = " << avg/double(pop.size());
    return avg / double(pop.size());
}

/**
 * mutates a Gene
 * \param g reference to the Gene to be mutate
 * \param number	where in the individual the Gene is located (starting from 0 to number of genes-1.  This is needed to know which connections are legal.
 */
void CGPPopulationBase::mutate(Gene &g, int number) {
    int mutPoint = getRandInt(1, 3);
    switch (mutPoint) {
    case 1: //first input is changed into a new legal value
        g.setInput(1, this->validInput(number));
        break;
    case 2: //second input is changed into a new legal value
        g.setInput(2, this->validInput(number));//getRandInt(max(0,numberOfInputs+number-5),numberOfInputs+number-2);
        break;
    case 3: //the function is changed into a new legal function type
        g.setFunction(ri.getRandomFunction());
        break;
    default:
        QMessageBox::warning(0, QString("Aborting!"),
                             QString("Wrong mutation!"));
        abort(); //not possible
    }

    //  if((g.getInput(1) >= numberOfInputs+number-1) || (g.getInputTwo() >= numberOfInputs+number-1))
    //     qDebug() << "AHA!! (in mutating)" ;
}

/**
 * Mutates every gene in an individual with prob = geneMutationFactor
 */
void CGPPopulationBase::mutate(Individual& it) {
    ti.Mutations++;
    for (int i = 0; i < ri.gp.Nodes; i++) {
        if (getRandInt(0, 100) < ri.gp.PointMutationProbability) {
            mutate(it.getGenes()[i], i);
        }
    }
}

/**
 * This kind of crossover will give yield copies of parents if parents are identical (as for Genetic algorithms).
 * Does a 'tree crossover'
 */
void CGPPopulationBase::CrossOver(Individual& first, Individual& second,
                                  Individual& result) {
    ti.Crossovers++;
    if (this->ri.gp.treeCrossover)
        treeCrossOver(first, second, result);
    else
        regularCrossOver(first, second, result);

}

/**
 * Does a 'regular' crossover of two individuals
 */
void CGPPopulationBase::regularCrossOver(Individual& first, Individual &second,
                                         Individual &result) {
    //qDebug() << "Crossover";
    //int size = getRandInt(1,Genes-1);
    int pos1 = getRandInt(1, ri.gp.Nodes - 2);
    int pos2 = getRandInt(pos1 + 1, ri.gp.Nodes - 1);
    Individual child1, child2;
    //qDebug() << "Copying";
    child1 = first;
    child2 = second;
    //qDebug() << "swapping genes";
    try {
        child1.swapGenes(child2, pos1, pos2);
    } catch (OutOfRange &e) {
        qDebug() << "swapping failed!!!";
        result = first;
        return;
    }
    child1.setFitness(this->threadedGetFitness(child1));
    child2.setFitness(this->threadedGetFitness(child2));
    //return the child with the best fitness

    //if (child1.getFitness() < child2.getFitness())
    result = child1;
    //else
    //result =child2;
}

/**
 * Does a 'tree' crossover of two individuals
 */
void CGPPopulationBase::treeCrossOver(Individual& first, Individual &second,
                                      Individual &result) {
    if (ri.gp.Grid) {
        this->gridTreeCrossOver(first, second, result);
    } else {
        this->directedGraphTreeCrossOver(first, second, result);
    }
}
/**
 * Does a 'simple' crossover of two individuals.
 * Swaps a single node between the individuals
 */
void CGPPopulationBase::simpleCrossOver(Individual& first, Individual &second,
                                        Individual &result) {
    int pos = getRandInt(1, ri.gp.Nodes - 1);
    Individual child1, child2;
    child1 = first;
    child2 = second;
    try {
        child1.swapGenes(child2, pos, pos);
    } catch (OutOfRange &e) {
        qDebug() << "swapping failed!!!";
        result = first;
        return;
    }
    child1.setFitness(this->threadedGetFitness(child1));
    //child2.setFitness(this->getFitness(child2));
    result = child1;
}

void CGPPopulationBase::directedGraphTreeCrossOver(Individual& first,
                                                   Individual &second, Individual &result) {
    int max = ri.gp.Nodes;
    int node = getRandInt(0, max - 1);//get a start node
    QVector<int> nodes;
    getNodes(node, first, nodes);//get a vector of node indexes from first individual
    std::sort(nodes.begin(), nodes.end());//sort the vector
    QVector<int>::iterator end = std::unique(nodes.begin(), nodes.end()); //remove duplicate nodes. Now we can copy all these nodes
    QVector<int>::iterator it;
    Individual child1, child2;
    child1 = first;
    child2 = second;
    for (it = nodes.begin(); it != end; it++) {
        child2.swapGenes(child1, (*it), (*it));
    }
    child1.setFitness(this->threadedGetFitness(child1));
    child2.setFitness(this->threadedGetFitness(child2));
    //return the child with the best fitness
    if (child1.getFitness() < child2.getFitness())
        result = child1;
    else
        result = child2;

}
void CGPPopulationBase::gridTreeCrossOver(Individual& first,
                                          Individual &second, Individual &result) {
    int col = getRandInt(1, ri.gp.Cols); //which row to take the start node from.
    int row = getRandInt(1, ri.gp.Rows);//which col to take the start node from.

    QVector<int> nodes;
    int c_node = (col - 1) * ri.gp.Rows + (row - 1); //current node (from [0- numNodes-1]);
    getNodes(c_node, first, nodes);//get a vector of node indexes
    std::sort(nodes.begin(), nodes.end());//sort the vector
    QVector<int>::iterator end = std::unique(nodes.begin(), nodes.end()); //remove duplicate nodes. Now we can copy all these nodes

    Individual child1, child2;
    child1 = first;
    child2 = second;
    QVector<int>::iterator it;

    for (it = nodes.begin(); it != end; it++) {
        child2.swapGenes(child1, (*it), (*it));
    }
    child1.setFitness(this->threadedGetFitness(child1));
    child2.setFitness(this->threadedGetFitness(child2));
    //return the child with the best fitness
    if (child1.getFitness() < child2.getFitness())
        result = child1;
    else
        result = child2;
}
/**
 * Gets all nodes back for a set number of levels
 */
void CGPPopulationBase::getNodes(int at, Individual& parent,
                                 QVector<int> &nodes) {

    if (at >= 0) {
        nodes.push_back(at);
        if (ri.gp.Grid) {
            if (at >= ri.gp.Rows) {
                Gene g = parent.getGenes().at(at);
                getNodes(g.getInput(1) - ri.problem->NumberOfInputs(), parent,
                         nodes);
                getNodes(g.getInput(2) - ri.problem->NumberOfInputs(), parent,
                         nodes);
            }
        } else if (at >= ri.problem->NumberOfInputs()) {//directed graph
            Gene g = parent.getGenes().at(at);
            getNodes(g.getInput(1) - ri.problem->NumberOfInputs(), parent,
                     nodes);
            getNodes(g.getInput(2) - ri.problem->NumberOfInputs(), parent,
                     nodes);
        }
    }//if

}

/*
 computes the fitness for all individual
 \todo: Implement hashing of results so that identical individuals only gets calculated once.
 */
void CGPPopulationBase::computeFitness(QVector<Individual> &pool) {
    //regularCompute(pool);
    hashCompute(pool);
    //       QtConcurrent::mappedReduced ( const Sequence & sequence, MapFunction mapFunction, ReduceFunction reduceFunction, QtConcurrent::ReduceOptions reduceOptions = UnorderedReduce | SequentialReduce )
}
/**
 * Calculates the fitness for all individuals.
 */
void CGPPopulationBase::regularCompute(QVector<Individual> &pool) {
    QVector<Individual>::iterator it;
    for (it = pool.begin(); it != pool.end(); it++) {
        (*it).setFitness(this->threadedGetFitness((*it)));
    }
}

/**
 * Calculates the fitness of all individuals, but optimized for using a hash map of already stored individuals
 */
void CGPPopulationBase::hashCompute(QVector<Individual> &pool) {
    //m_fitnessMap.clear();
    QVector<Individual>::iterator it;
    int mapUse = 0;
    for (it = pool.begin(); it != pool.end(); it++) {
        //see if we have already calculated fitness for this exact individual /todo optimize to only look at 'active' nodes
        QMap<QString, double>::iterator map_it =
                this->m_fitnessMap.find((*it).singleLineString());
        if (map_it == m_fitnessMap.end()) {
            (*it).setFitness(threadedGetFitness(*it));
            m_fitnessMap[(*it).singleLineString()] = (*it).getFitness();
        } else {
            (*it).setFitness(map_it.value());
            mapUse++;
        }
    }
    LOG( m_fitnessMap.size() << " unique solutions tested so far");
}
/*
 returns the indicvidual with the lowest fitness
 */
Individual& CGPPopulationBase::getBest() {
    return pop.front();
}

/*
 returns the individual with the worst fitness
 */
Individual& CGPPopulationBase::getWorst() {
    return pop.back();
}

TwoDArray<double> CGPPopulationBase::getBestOutput() {
    TwoDArray<double> output(this->ri.problem->NumberOfFitnessCases(),
                             this->ri.problem->NumberOfOutputs());
    //loop over number of cases
    Individual it = getBest();
    for (int i = 0; i < ri.problem->NumberOfFitnessCases(); i++) {
        double* problemInputs = ri.problem->getCurrentInputs();
        for (int j = 0; j < ri.problem->NumberOfInputs(); j++) {
            m_ind_inputs[j] = problemInputs[j];
        }
        it.getOutput(m_ind_outputs, ri, m_ind_inputs);
        for (int k = 0; k < ri.problem->NumberOfOutputs(); k++) {
            output[i][k] = m_ind_outputs[k];
        }
    }
    return output;
}
void CGPPopulationBase::setInput(int number, double value) {
    inputList[number] = value;
}

/**
 *
 * \param &ind individual to be parsed
 * \return the ouput of the final node as a program-tree
 */
QString CGPPopulationBase::HumanReadable(Individual &ind) {
    QString function;
    QVector<QString> strings;
    ri.problem->inputStringValues(strings);
    for (int i = 0; i < ind.getGenes().size(); i++) {
        function = ind.getGenes().at(i).getFunction()->name();
        QString str;
        QTextStream stream(&str);
        stream << "(" << function;
        for (int j = 0; j < ri.functions[function]->numberOfInputs(); j++) {
            stream << " " << strings.at(ind.getGenes().at(i).getInput(j + 1));
        }
        stream << ")";
        strings.push_back(str);
    }

    return strings[ind.getGenes().size() + ri.problem->NumberOfInputs() - 1]; //this returns only the last
}

/*
 Retrieves the fitness of the best fit individual
 */
double CGPPopulationBase::getBestFitness() {
    Individual best = getBest();
    return best.getFitness();
}

/*
 Retrives the fitness of the least fit individual
 */
double CGPPopulationBase::getWorstFitness() {
    Individual worst = getWorst();
    return worst.getFitness();

}

/**
 * gets an individual using the set selectionMethod
 * The individual is copied into the passed variable
 */
void CGPPopulationBase::getIndividual(QVector<Individual> &pool,Individual& ind) {
    switch (ri.sel.Selection) {
    case PopulationInterface::QTournament:
        ind = tournamentSelection(ri.sel.QTournamentSize,pool);
        break;
    case PopulationInterface::Random:
        ind = randomSelection(pool);
        break;
    case PopulationInterface::Roulette:
        ind = rouletteSelection(pool);
        break;
    case PopulationInterface::Rank:
        ind = rankSelection(pool);
        break;
    default:
        QMessageBox::warning(0, QString("Aborting!"),QString("Impossible selection type: %1").arg(ri.sel.Selection));
        qDebug() << "selection impossible!! " << this->ri.sel.Selection;
        abort();
    }
    return;
}

/**
 * Gets two individuals among the top q%, then return the best one
 * This of course assumes the population to be sorted
 * \param q percentage of population to pick individual from
 * \param pool the pool of individuals to do the selection from
 */
Individual& CGPPopulationBase::tournamentSelection(int q, QVector<Individual>& pool) {
    int max = (pop.size() * q) / 100;
    int rand = getRandInt(0, max - 1);
    int rand2 = getRandInt(0, max - 1);
    if (pool.at(rand).getFitness() < pool.at(rand2).getFitness())
        return pool[rand];
    return pool[rand2];
}

/**
  * Selects a random individual from the population
  * \param pool the pool of individuals to do the selection from
 */
Individual& CGPPopulationBase::randomSelection(QVector<Individual>& pool) {
    int rand = getRandInt(0, pool.size() - 1);
    return pool[rand];
}

/**
  * Selects an individual from the population using ROULETTE selection.
  * \todo implement properly. Currently deferring to rank selection
  * \param pool the pool of individuals to do the selection from
 */
Individual& CGPPopulationBase::rouletteSelection(QVector<Individual>& pool) {
    return rankSelection(pool);
}

/**
  * Selects an individual from the population using RANK selection
  * \param pool the pool of individuals to do the selection from. Pool MUST be sorted in ascending order
 */
Individual& CGPPopulationBase::rankSelection(QVector<Individual>& pool) {
    int total = pool.size()*(pool.size()+1)/2;
    int selection = getRandInt(1,total);
    double var = (sqrt(1+8*selection)-1)/2;
    int num = pool.size() - (int)(ceil(var));
    return pool[num];
}


/**
 * prints all individuals and their fitness
 */
void CGPPopulationBase::PrintGeneration() {
    QVector<Individual>::iterator it;
    for (it = pop.begin(); it != pop.end(); it++) {
        (*it).printGenes();
        double fitness = (*it).getFitness();
        qDebug() << "Fitness = " << fitness;

    }
    qDebug() << "BestFitness = " << getBestFitness();
    qDebug() << "AverageFitness = " << getAvgFitness();

}

/**
 * returns a random valid input for a Gene in the given position
 * \param node position of the Gene
 * \returns int valid input to the node
 */
int CGPPopulationBase::validInput(int node) {
    //check type of CGP
    //if grid
    // 0 3 6
    // 1 4 7
    // 2 5 8
    int minimum, maximum;
    if (ri.gp.Grid == true) {

        int col = node / ri.gp.Rows;
        maximum = col * ri.gp.Rows - 1 + ri.problem->NumberOfInputs();
        if (col == 0)
            minimum = 0;
        else {
            minimum = max(0, ((col - ri.gp.Connections) * ri.gp.Rows)
                          + ri.problem->NumberOfInputs());

        }
        /*	qDebug() << "Node" << node << "\n"
                 << "col " << col  << "\n"
                 << "minimum " << minimum << "\n"
                 << "ri.gp.Rows" << ri.gp.Rows << "\n"
                 << "ri.gp.Connections" << ri.gp.Connections << "\n"
                 << "sum " << ((col - ri.gp.Connections) * ri.gp.Rows) + ri.problem->NumberOfInputs() << "\n";
                 */
    } else {
        //calculate valid nodes
        minimum = max(0, numberOfInputs + node - ri.gp.Connections); //ensure that we don't have a negative input
        maximum = numberOfInputs - 1 + node; //-1 since we enumerate from 0 and can't connect to ourself
    }
    //qDebug() << "Input for " << node << " to be " << minimum << " -> " << maximum;
    int r = getRandInt(minimum, maximum);
    return r;
}
/**
 * Sorts a vector of individuals
 * \param pop vector to be sorted
 * \param ascending if true the pop is sorted in ascending order, otherwise in descending
 */
void CGPPopulationBase::sortPool(QVector<Individual>& pop, bool ascending) {
    if (ascending) {
        std::sort(pop.begin(), pop.end(), AscendingSort());
    } else {
        std::sort(pop.begin(), pop.end(), DescendingSort());
    }
}

/**
 *
 */
void CGPPopulationBase::SanityCheck(Individual& ind) {
    for (int i = 1; i < 100; i++) {
        double a = this->threadedGetFitness(ind);
        double b = this->threadedGetFitness(ind);
        if (a != b) {
            qDebug() << "Sanity check FAILED!";
            abort();
        }
    }

}

/**
 */
void CGPPopulationBase::createIndividual(Individual& newInd) {
    HERE(2);
    for (int j = 0; j < ri.gp.Nodes; j++) {
        int inputOne = validInput(j);//getRandInt(max(0,numberOfInputs+j-5),numberOfInputs-1+j);
        int inputTwo = validInput(j);//getRandInt(max(0,numberOfInputs+j-5),numberOfInputs-1+j);
        int func = getRandInt(0, numberOfFunctions - 1);
        QMap<QString, Function<double>*>::iterator it = ri.functions.begin();
        for (int k = 0; k < func; k++) {
            it++;
        }
        newInd.insertGene(inputOne, inputTwo, it.key());
    }
}
