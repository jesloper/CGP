#include "SimpleProblems.h"
#include <math.h>
#include "SR_ProblemSelecter.h"
#include <QInputDialog>

/**
 * Constructor. Sets up the symbolic regression problem
 * \todo allow user to choose between several different problems
 */
SRProblem::SRProblem() {

}
void SRProblem::Init() {
	current = 0;
	SR_ProblemSelecter* p_sel = new SR_ProblemSelecter(0);
	m_function = p_sel->exec();
	Problem::m_fitness_cases = 100;
	Problem::m_number_of_outputs = 1;
	Problem::m_number_of_inputs = 1;
	Problem::setup();
	m_params = new double[Problem::m_fitness_cases];
	switch (m_function) {
	case 1:

		for (int i = 0; i < Problem::m_fitness_cases; i++) {
			m_params[i] = (double) (i - 50) / 100.0;
			(*m_answers)[i][0] = pow(m_params[i], 6) - 2* pow (m_params[i], 4) + pow(m_params[i], 2);
		}
		m_description = "Symbolic regression. Function we are trying to find is: x^6-2x^4+x^2";
		break;
	case 2:
		for (int i = 0; i < Problem::m_fitness_cases; i++) {
			m_params[i] = (double) (i - 50) / 13.0;
			(*m_answers)[i][0] = 2*pow(m_params[i], 4)-3*pow(m_params[i], 7)+pow(m_params[i], 5) - 2* pow (m_params[i], 3) + m_params[i];
		}
		m_description = "Symbolic regression. Function we are trying to find is: 2*x^4-3*x^7+x^5-2x^3+x";
		break;
	case 3:
		qDebug() << "not implemented yet!";
		break;
	case 4:
		for (int i = 0; i < Problem::m_fitness_cases; i++) {
			m_params[i] = (double) (i - 50) / 100.0;
			(*m_answers)[i][0] = pow(m_params[i], 2) - 2* m_params [i] + 1;//x^2-2x+1
		}
		m_description = "Symbolic regression. Function we are trying to find is: x^2-2x+1";
		break;
	default:
		qDebug() << "This should not happen";
	}
	delete p_sel;
}
/**
 * Sets the current inputs
 */
double* SRProblem::getCurrentInputs() {
	m_inputs[0] = m_params[current];
	answer_ = (*m_answers)[current][0];
	current++;
	if (current == m_fitness_cases)
		current = 0;
	return m_inputs;
}

/**
 * Returns the fitness for a given output
 */
double SRProblem::setFitness(double* output) {
	//qDebug() << "answer is " <<  this->answer();
	double diff = fabs(output[0] - this->answer());
	double fit;
	if (diff <= 1) {
		fit = diff;
	} else {
		fit = log(sqrt(diff)) + 1.0; //we define fitness using logaritmic value
	}
	return fit;
}

void SRProblem::setInputs() {
	answer_ = (*m_answers)[current][0]; //function to be found
}

EvenParity::EvenParity() {

}
void EvenParity::Init() {
	current = 0;

	int evenX = QInputDialog::getInteger(0, "Even parity", "How many bits?");

	Problem::m_fitness_cases = (int) pow(evenX, 2);
	Problem::m_number_of_inputs = evenX;
	Problem::m_number_of_outputs = 1;

	Problem::setup();
	//set up the right answer
	for (int i = 0; i < m_fitness_cases; i++) {
		// setInputList()
		int rest = i;
		int number = 0;
		for (int j = 0; j < m_number_of_inputs; j++) {
			number += rest % 2;
			rest = rest / 2;
		}
		(*m_answers)[i][0] = (number % 2 ? 0 : 1);
	}
	outputTotal = 0;
}
/**
 * sets the inputs for the current fitness case
 */
void EvenParity::setInputs() {

	int rest = current;
	for (int j = 1; j <= m_number_of_inputs; j++) {
		m_inputs[m_number_of_inputs - j] = rest % 2;
		rest = rest / 2;
	}
	/*m_inputs[3] = rest%2;
	 rest = rest/2;
	 m_inputs[2] = rest%2;
	 rest=rest/2;
	 m_inputs[1] = rest%2;
	 rest=rest/2;
	 m_inputs[0] = rest%2;
	 */
	m_answer = (*m_answers)[current][0];
	current++;
	if (current == this->NumberOfFitnessCases()) {
		current = 0;
	}
}

void Circles::setInputs() {
	r = current + 1 / 10.0;
	m_inputs[0] = r;
	//m_answer[0] = r*r*PI; //area of circle
	//m_answer[1] = 4*pow(r, 3)*PI/3; //volume of sphere
}
