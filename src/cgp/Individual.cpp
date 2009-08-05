#include "Individual.h"
#include <QTextStream>
#include <QtDebug>

/**
 * default ctor
 */
Individual::~Individual(void) {
}

/**
 * Inserts a new gene to the Individual
 */
void Individual::insertGene(int f, int s, std::string func) {
	if ((f < 0) || (s < 0))
		std::cout << "ILLEAGAL!!";
	this->Genes.push_back(Gene(f, s, func));
	this->numberOfNodes++;
}

/**
 * Prints the genes of the Individual to std::cout
 */
void Individual::printGenes() const {
	qDebug() << "This is ME (number of nodes " << this->numberOfNodes << ")";
	for (unsigned int i = 0; i < this->Genes.size(); i++) {
		qDebug() << "#" << i << " = ";
		Genes.at(i).print();
		qDebug() << " /  ";

	}
}

/**
 * Copies another Individual
 * \param rhs constant reference to the Individual to be copied
 */
void Individual::copy(const Individual &rhs) {
	this->Genes.clear();
	for (unsigned int i = 0; i < rhs.Genes.size(); i++) {
		Gene newGene = rhs.Genes.at(i);
		//newGene.copy();
		Genes.push_back(newGene);
	}
	this->numberOfNodes = rhs.numberOfNodes;
	this->lengthOfNode = rhs.lengthOfNode;
	this->fitness = rhs.fitness;
	this->m_activeNodes = rhs.m_activeNodes;
	//qDebug() << "Fitness = " << rhs.fitness;

}

/**
 * Checks whether this is equal to another Individual
 * \param other reference to the other Individual
 */
bool Individual::equals(const Individual& other) const {
	for (unsigned int i = 0; i < other.Genes.size(); i++) {
		if (!Genes.at(i).equals(other.Genes.at(i)))
			return false;
	}
	return true;
}

/**
 * Swap  genes with another Individual. Swaps all genes between the given positions.
 * This method is called for Crossover.
 * \param other reference to the other Individual
 * \param f first gene to copy
 * \param s last gene to copy
 */
void Individual::swapGenes(Individual& other, int f, int s) throw(OutOfRange) {
	if (f < 0 || s >= this->numberOfNodes) {
		OutOfRange ex;
		throw ex;
	}
	assert(s < this->numberOfNodes);
	for (int i = f; i <= s; i++) {
		Gene temp = this->getGenes().at(i);
		this->getGenes().at(i) = other. getGenes().at(i);
		other. getGenes().at(i) = temp;

	}
}

/**
 * serializes the Individual to a string
 * \returns the Individual represented as a string
 */
std::string Individual::toString() {
	QString out;
	QTextStream str(&out);
	str << this->numberOfNodes << endl;
	str << this->fitness << endl;
	for (unsigned int i = 0; i < Genes.size(); i++) {
		str << Genes.at(i).toStdString().c_str() << endl;
	}
	return out.toStdString();
}

/**
 * Transforms the individual to a single line string.
 * Uses 'Polish' notation.
 * \returns the individual represented as a single string.
 */
std::string Individual::singleLineString() {
	QString out;
	QTextStream str(&out);
	str << "(";
	for (unsigned int i = 0; i < Genes.size(); i++) {
		str << "( " << Genes.at(i).getFunction()->name().c_str();
		for (int j = 1; j <= Genes.at(i).getNumberOfInputs(); j++) {
			str << "  " << Genes.at(i).getInput(j);
		}
		str << " )";
	}
	str << ")";
	return out.toStdString();
}
/**
 * serializes the individual to matlab friendly code
 * \param inputs number of input values for the problem (to properly format the string)
 */
QString Individual::toMatlabCode(int inputs) {
	QString out;
	QTextStream str(&out);
	for (unsigned int i = 0; i < Genes.size(); i++) {
		int i1 = Genes.at(i).getInput(1);
		int i2 = Genes.at(i).getInput(2);
		std::string func = Genes.at(i).getFunction()->name().c_str();
		QString inp1, inp2;
		if (i1 < inputs) {
			inp1 = QString("data(k,%1)").arg(i1 + 1);
		} else {
			inp1 = QString("n%1").arg((i1 - inputs));
		}
		if (i2 < inputs) {
			inp2 = QString("data(k,%1)").arg(i2 + 1);
		} else {
			inp2 = QString("n%1").arg((i2 - inputs));
		}
		QString gout = Genes.at(i).toMatlabCode(inp1, inp2);
		QString line = QString("n%1 = %2").arg(i).arg(gout);//.arg(func.c_str()).arg(inp2);
		str << line << endl;
	}
	return out;
}

/**
 * restores and individual from a string
 * \param object the string representation of the object
 */
void Individual::fromString(std::string object) {
	QString in(object.c_str());
	QTextStream str(&in);
	Genes.clear();
	this->numberOfNodes = 0;
	int nodes;
	str >> nodes;
	str >> this->fitness;

	for (int i = 0; i < nodes; i++) {
		int i1, i2;
		QString func;
		str >> i1 >> i2 >> func;
		insertGene(i1, i2, func.toStdString());
	}

}
/**
 * gets the outputs of the Individual
 * \param out pointer to the result vector
 * \param ri RunInfo structure containing needed information
 * \param inp Input vector containing inputs for the current fitness case
 * \param track used for debugging. \todo remove this
 */
void Individual::getOutput(double* out, RunInfo& ri, double* inp, bool track) {
	h_map.clear();
	m_activeNodes.clear();
	//start at back and retrieve outputs
	for (int i = 0; i < ri.problem->NumberOfOutputs(); i++) {
		m_activeNodes.push_back(numberOfNodes - i - 1);
		Gene g = this->Genes.at(numberOfNodes - i - 1);
		out[i] = this->getOutput(g, ri, inp, track);

	}

}

/** gets the output from a specific Gene. Stores output from previousely gathered outputs in a hash map for efficiency
 * \param g Gene to get the output from
 * \param ri RunInfo structure for current run
 * \param inputlist array containing input for this particular fitness case
 * \param track used for debugging.
 * \todo rewrite for arbitrary input size
 */
double Individual::getOutput(Gene g, RunInfo& ri, double* inputlist, bool track) {
	if (track) {
		qDebug() << "tracking";
	}
	double inps[10];
	for (int i = 0; i < g.getNumberOfInputs(); i++) {
		int inp = g.getInput(i+1);
		double result;
		std::map<int, double>::const_iterator it = h_map.find(inp);
		if (it == h_map.end()) {
			if (inp < ri.problem->NumberOfInputs()) {
				result = inputlist[inp];
			} else {
				m_activeNodes.push_back(inp - ri.problem->NumberOfInputs()); //keep track of 'active' nodes
				result = getOutput(this->Genes.at(inp - ri.problem->NumberOfInputs()), ri, inputlist, track);
			}
			h_map[inp] = result;
		} else {
			result = it->second;
		}
		inps[i] = result;
	}
	try {
		return g.calculate(inps, ri);
	} catch (...) {
		this->printGenes();
                //abort();
	}
/*

	int first = g.getInput(1);
	int second = g.getInput(2);
	double f_result, s_result, final_result;
	std::map<int, double>::const_iterator it;
	it = h_map.find(first);
	if (it == h_map.end()) {
		if (first < ri.problem->NumberOfInputs()) {
			f_result = inputlist[first];
		} else {
			f_result = getOutput(this->Genes.at(first - ri.problem->NumberOfInputs()), ri, inputlist, track);
		}
		h_map[first] = f_result;
	} else {
		f_result = it->second;
	}
	it = h_map.find(second);
	if (it == h_map.end()) {
		if (second < ri.problem->NumberOfInputs()) {
			s_result = inputlist[second];
		} else {
			s_result = getOutput(this->Genes.at(second - ri.problem->NumberOfInputs()), ri, inputlist, track);
		}
		h_map[second] = s_result;
	} else {
		s_result = it->second;
	}
	try {

		final_result = g.calculate(f_result, s_result, ri);
		if (track) {
			qDebug() << "calculating " << f_result << " " << g.getFunction()->name().c_str() << " " << s_result << "   = " << final_result;
		}
	} catch (...) {
		this->printGenes();
                //abort();
	}
	return final_result;*/
}

/**
 * Copy constructor.
 * \param other constant reference to the Individual to copy
 */
Individual::Individual(const Individual& other) {

	this->Genes.clear();
	for (size_t i = 0; i < other.Genes.size(); i++) {
		Genes.push_back(other.Genes.at(i));
	}
	this->numberOfNodes = other.numberOfNodes;
	this->lengthOfNode = other.lengthOfNode;
	this->fitness = other.fitness;
	this->m_activeNodes = other.m_activeNodes;
}

/**
 * Assignment operator
 * \param other constant reference to the Individual to copy
 */
Individual& Individual::operator =(const Individual & other) {
	this->Genes.clear();
	for (unsigned int i = 0; i < other.Genes.size(); i++) {
		Genes.push_back(other.Genes.at(i));
	}
	this->numberOfNodes = other.numberOfNodes;
	this->lengthOfNode = other.lengthOfNode;
	this->fitness = other.fitness;
	this->m_activeNodes = other.m_activeNodes;
	return *this;
}

