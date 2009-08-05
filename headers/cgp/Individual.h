#ifndef PENOTYPE_H
#define PENOTYPE_H

#include <list>
#include <vector>
#include "Gene.h"
#include <RunInfo.h>
#include <map>
#include <QtDebug>
#include <exception>

class OutOfRange: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Asked for gene out of range";
  }
};

/**
 * \author john erik sloper
 *
 */
class Individual
{
public:
 Individual(){
	this->numberOfNodes=0;
	Genes.clear();
 };
 ~Individual(void);	///< \ brief dtor
 void copy(const Individual &rhs);	///< copies and individual.  We cannot use shallow copy because of the vector
 void insertGene(int f, int s, QString func); //< \brief inserts a new gene at the back of the individual
 void swapGenes(Individual& other, int f, int s) throw(OutOfRange);
 void printGenes() const; ///< \brief prints the genes using qDebug
 bool equals(const Individual& other) const; ///< \brief check if two individuals are equal
 QString toString();	///< \brief converts the Individual to a string representation
 void fromString(QString);	///< \brief updates the individual based on a string representation
 QString toMatlabCode(int inputs);	///< \brief converts the Ind to a matlab friendly representation
 QString singleLineString();	///< \brief single line string representation

 /**
  * \brief Returns the number of nodes in the individual
  */
 int getNumberOfNodes() const{
	 return this->numberOfNodes;
 }
 /**
  * returns a reference to the vector of genes representing the individual
  */
 QVector<Gene>& getGenes(){
	 return this->Genes;
 }
 /**
  * Returns the fitness value of the individual
  * \return fitness value
  */
 double getFitness() const{
	 return this->fitness;
 }
 /**
  * Sets the fitness of the individual
  * \param fit double representing the new fitness
  */
 void setFitness(double fit){
	 this->fitness = fit;
 }

 void getOutput(double* out,RunInfo& ri,double* inp,bool track = false);
 Individual(const Individual& other);
 Individual& operator=(const Individual& other);
 QVector<int> getActiveNodes(){
	 return m_activeNodes;
 }
private:
 double getOutput(Gene g,RunInfo& ri,double* inp, bool track = false);
 int numberOfNodes;
 int lengthOfNode;
 QVector<Gene> Genes;
 double fitness;
 QMap<int,double> h_map;
 QVector<int> m_activeNodes; ///< \brief holds the currently active nodes as of last time output was calculated

};

/** This class is used to pass around extra information about and individual. This is used in cases
 *  where we want to do extra plotting, etc.
 */
class IndividualInformation{
public:
	IndividualInformation():m_outputs(1,1),m_answers(1,1){};
	Individual m_ind;
	TwoDArray<double> m_outputs;
	TwoDArray<double> m_answers;
	int m_generation;
};
/**
 * Sorts a list of Individuals
 */
struct AscendingSort {
    bool operator() (const Individual& F, const Individual& S) {
        return F.getFitness() < S.getFitness();
    }
};

/**
 * Sorts the Individuals in Decending order
 */
struct DescendingSort{
	  bool operator() (const Individual& F, const Individual& S) {
        return F.getFitness() > S.getFitness();
    }
};
#endif
