#ifndef SIMPLEPROBLEM_H
#define SIMPLEPROBLEM_H

#include <qdebug.h>
#include "Problem.h"
#include "math.h"
/**
 \class SRProblem
 Example problem.  This represents a basic Symbolic regression problem x*y+2y-(x*y)/(x+y)
 It evaluates 10 different fitness cases with x = 0,1..,9 and y = 3,6,3,6 ...
 */
class SRProblem : public Problem {
public:
	SRProblem(); ///< \brief ctor

	virtual ~SRProblem() {
		qDebug() << "Deleting SR problem";
	}

	virtual double setFitness(double* output);

	virtual double answer() {
		return answer_;

	}

	QString getName() {
		return "SRProblem";
	}
	/**
	 Sets the inputs and returns them.  If it is the last fitness case
	 the counter is reset.
	 */
	virtual double* getCurrentInputs();

	virtual void inputStringValues(std::vector<std::string>& outputs) {
		outputs.push_back("x");
		outputs.push_back("y");
		outputs.push_back("z");

	}
	virtual QString description() {
		return m_description;
	}
	virtual void Init();
private:
	/*
	 sets the inputlist
	 */
	void setInputs();
	int current;
	double answer_;
	double x, y, z;
	double* m_params;
	QString m_description;
	int m_function;
};

/**
 \class Circles
 Similar to SR problems.  Tries to find a function that both gives area and volume for a given radius
 */
#define PI 3.14159265
class Circles : public Problem {
public:
	Circles() {
		current =0;
		Problem::m_fitness_cases = 20;
		Problem::m_number_of_outputs = 2;
		Problem::m_number_of_inputs = 1;
		Problem::setup();
		//setInputs();
	}

	~Circles() {
		delete[] m_inputs;
		//delete[] m_answer;
	}

	virtual double setFitness(double* output) {
		//qDebug() << "output = " << output;
		//qDebug() << "answer = " << ans;

		/*double diff = fabs(output[0] - m_answer[0]);
		double diff2 = fabs(output[1] - m_answer[1]);

		double diff_total = diff+diff2;
		//qDebug() << "diff = " << diff;
		double value = 1*(1-1/(1+diff_total));

		return value;
		*/
		return 0;
	}

	virtual double answer() {
		return 0;
	}

	virtual double* getCurrentInputs() {
		setInputs();
		current++;
		if (current == m_fitness_cases)
			current=0;
		return m_inputs;
	}
	virtual void inputStringValues(std::string* inp) {
		inp[0] = "r";
	}
	virtual QString description() {
		return QString("Example problem. Tries to find a circuit that gives both area of a circle and volume of the sphere for a given radius");
	}
	QString getName() {
		return "Circles";
	}
private:
	void setInputs();
	double r;
	int current;
};

/**
 * \class EvenParity
 * Even Parity problem. 
 * Trying to find a function for the even parity problem for 4 inputs.
 */
class EvenParity : public Problem {

public:
	//! ctor
	EvenParity();
	~EvenParity() {
		qDebug() << "deleting even parity problem";
	}
	double answer() {
		return m_answer;
	}
	QString getName() {
		return "EvenParity";
	}
	virtual double* getCurrentInputs() {

		setInputs();

		return m_inputs;
	}
	//sets the fitnesaffection for the current input
	virtual double setFitness(double* output) {

		outputTotal += !((int)(output[0]) == answer());

		//Need to store the output

		if (current == 0) {
			int fitness = outputTotal;
			outputTotal = 0;
			return fitness;
		} else {
			return 0;
		}
	}

	virtual void inputStringValues(std::vector<std::string>& inp) {
		inp.push_back("x0");
		inp.push_back("x1");
		inp.push_back("x2");
		inp.push_back("x3");

	}
	virtual QString description() {
		return QString("Even Parity problem\nTrying to find a function for the even parity problem for x inputs.");
	}
	virtual void Init();
private:
	void setInputs();
	int current;
	double m_answer;
	int outputTotal;
};
/**
 * \class EvenThreeParity
 * Even Parity problem. Trying to find a function for the even parity problem for 3 inputs.
 */
class EvenThreeParity : public Problem {

public:
	//! ctor
	EvenThreeParity() {
		Problem::m_fitness_cases = 8;
		Problem::m_number_of_inputs = 3;
		Problem::m_number_of_outputs = 1;
		Problem::setup();//set up the right answer
		for (int i =0; i < Problem::m_fitness_cases; i++) {
			// setInputList()
			int rest= i;
			int number =0;
			number += rest%2;
			rest = rest/2;
			number += rest%2;
			rest=rest/2;
			number += rest%2;

			list[i] = (number%2 ? 0 : 1);
		}
		current =0;
		outputTotal= 0;

	}
	double answer() {
		return answer_;
	}
	virtual double* getCurrentInputs() {
		setInputs();
		return m_inputs;
	}
	//sets the fitnesaffection for the current input
	virtual double setFitness(double* output) {

		outputTotal += !((int)(output[0]) == answer());
		current++;
		/*
		 Need to store the output
		 */
		if (current == this->m_fitness_cases) {
			int fitness = (4-abs(4-outputTotal));
			outputTotal = 0;
			current = 0;
			return fitness;
		} else {
			return 0;
		}
	}

	virtual void inputStringValues(std::vector<std::string>& inp) {
		inp.push_back("x0");
		inp.push_back("x1");
		inp.push_back("x2");
	}
	virtual QString description() {
		return QString("Even 3-Parity problem\nTrying to find a function for the even parity problem for 3 inputs.");
	}
private:
	void setInputs() {
		int rest= current;
		m_inputs[2] = rest%2;
		rest=rest/2;
		m_inputs[1] = rest%2;
		rest=rest/2;
		m_inputs[0] = rest%2;

		answer_ = (*m_answers)[current][0];
	}
	int list[8];
	int current;
	double answer_;
	int outputTotal;
};

/**
 * \class EvenTwoParity
 * Even Parity problem
 * Trying to find a function for the even parity problem for 2 inputs.
 */
class EvenTwoParity : public Problem {

public:
	//! ctor
	EvenTwoParity() {
		current =0;
		Problem::m_fitness_cases = 4;
		Problem::m_number_of_inputs = 3;
		Problem::m_number_of_outputs = 1;
		Problem::m_inputs = new double[2];
		//set up the right answer
		for (int i =0; i < Problem::m_fitness_cases; i++) {
			// setInputList()
			int rest= i;
			int number =0;
			number += rest%2;
			rest=rest/2;
			number += rest%2;

			list[i] = (number%2 ? 0 : 1);
		}

	}
	double answer() {
		return answer_;
	}
	virtual double* getCurrentInputs() {
		setInputs();

		current++;
		if (current == m_fitness_cases)
			current=0;

		return m_inputs;
	}
	//sets the fitnesaffection for the current input
	virtual double setFitness(double* output) {
		return !((int)(output[0]) == answer());
	}

	virtual void inputStringValues(std::vector<std::string>& inp) {
		inp.push_back("x0");
		inp.push_back("x1");
	}
	virtual QString description() {
		return QString("Even 3-Parity problem\nTrying to find a function for the even parity problem for 3 inputs.");
	}
private:

	void setInputs() {
		int rest= current;
		m_inputs[1] = rest%2;
		rest=rest/2;
		m_inputs[0] = rest%2;
		answer_ = list[current];
	}
	int list[4];
	int current;
	double answer_;
};
#endif
