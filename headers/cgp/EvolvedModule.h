/*
 * EvolvedModule.h
 *
 *  Created on: May 5, 2009
 *      Author: sloper
 */
#include <Function.h>
#ifndef EVOLVEDMODULE_H_
#define EVOLVEDMODULE_H_
/**
 * Holds all currently available Modules
 */
class EvolvedModuleFactory{

};

/**
 * Class to represent a module (behaves as a normal node, but consists of a sub-program)
 */
class EvolvedModule : public Function {
public:
	EvolvedModule(std::vector<Gene> genes) {
		//calculate distinct inputs
		//update the global reference counter
	}
	virtual ~EvolvedModule(){
		//Update the global reference counter
		;}

	/**
	 *  \brief returns the number of nodes in the module
	 */
	int getNumberOfNodes() {
		return genes.size();
	}
	virtual double calculate(double inp1, double inp2) const ///< \brief calculates the output of this module
	{return 0.0;}



private:
	static int nodeNumber = 0;
	int inputs[20];
	std::vector<Gene> genes; ///< \brief holds the Genes belonging to this module
};
void getNodes(int at, Individual& parent,
		std::vector<int> &nodes) {


}
static EvolvedModule ExtractModule(Individual& ind){
		//define a max length of the function
		int max = 3;
		std::vector<int> activeNodes= ind.getActiveNodes();
		std::vector<int>::reverse_iterator rit= activeNodes.rbegin();

		//for(;rit != activeNodes.rend();it++){
		qDebug() << "We are now storing node x";
		Gene start = ind.getGenes().at(*rit);
		int inputs = start.getNumberOfInputs();
		for(int i = 0; i < inputs; i++){

		}
		genes.push_back(ind.getGenes().at(*rit));
		genes.push_back()
		//}
		//start at end and go x steps back
		//extract all nodes. Now find all inputs that are not nodes and rearrange them to go from 0-to-x

	}



#endif /* EVOLVEDMODULE_H_ */
