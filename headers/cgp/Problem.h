#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>
#include <vector>
#include <qdebug.h>
#include "../TwoDArray.h"
#include <QtPlugin>

/**
 This is a generic base class for all problems to be solved by the CGP framework.
 New problems must inherit from this class and implement all abstract methods.
 The framework can then be used to solve the problem.

 Example


 Some example Problems are available in the examples dirsectory.
 */
class Problem {
public:
	Problem():m_inputs(0),m_answers(0) {
		qDebug() << "Creating new problem";
	}
	;
	virtual ~Problem() {
		if(m_answers)
			delete m_answers;
		if(m_inputs)
			delete[] m_inputs;
		qDebug() << "deleting problem";
	}
	;
	void setup() {
		Problem::m_inputs = new double[m_number_of_inputs];
		if(m_answers)
			delete m_answers;
		m_answers = new TwoDArray<double>(m_fitness_cases,m_number_of_outputs);
	}
	; ///< \brief sets up the problem according to number of inputs, outputs, etc.
	virtual double setFitness(double* output) =0; ///< \brief sets the fitness affection for the current input */
	virtual double answer() = 0; ///< \brief should return the correct answer for a given inputs
	virtual double* getCurrentInputs() = 0; ///< \brief this should return the current inputs
	virtual std::string getName() = 0; ///< \brief this should return the name of the problem
	virtual void reset() {
	}
	; ///< \brief should reset any data if needed
	virtual std::string description() {
		return std::string("No description is available");
	}
	virtual void inputStringValues(std::vector<std::string>& inp) {
		for (int i = 0; i < m_number_of_inputs; i++) {
			QString s = QString("x%1").arg(i);
			inp.push_back(s.toStdString());
		}
	}
	;
	/**
	 * \returns number of fitness cases
	 */
	int NumberOfFitnessCases() {
		return m_fitness_cases;
	}
	/**
	 * \returns number of inputs
	 */
	int NumberOfInputs() {
		return m_number_of_inputs;
	}
	/**
	 * \returns number of outputs
	 */
	int NumberOfOutputs() {
		return m_number_of_outputs;
	}
	/**
	 * any initialization that might be needed
	 */
	virtual void Init() {
		qDebug() << "Initing problem";
	}
	TwoDArray<double> getAnswers() {
		return *m_answers;
	}
protected:
	/**
	 * hold the number of cases to check per generation (how many different inputs are evaluated)
	 *  for example for a symbolic regression we might compare the value of the program with the real value for
	 *  100 different inputs.
	 */
	int m_fitness_cases;
	int m_number_of_inputs; ///< holds the default number of inputs for the problem (this can be modified from the GUI)
	int m_number_of_outputs; ///< holds the default number of output for the problem (this can be modified from the GUI)
	double* m_inputs; ///< this should hold the current set of inputs
	TwoDArray<double>* m_answers; ///< this has size num_fitness_cases*num_outputs and holds all answers for the problem
};

Q_DECLARE_INTERFACE(Problem,"Problem/0.1")


#endif
