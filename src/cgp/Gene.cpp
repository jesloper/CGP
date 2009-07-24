#include <Gene.h>
#include <GeneFactory.h>
#include <qdebug>

static GeneRegistrator<Gene> Gene_instance("Gene");

std::string Gene::m_class_name = "Gene";

/**
 * Constructor taking two inputs and function
 */
Gene::Gene(int f, int s, std::string func) :
	m_function(0) {

	this->setFunction(func);
	if (m_function.get()) {
		inputs[0] = f;
		inputs[1] = s;
	} else {
		FunctionFactory<double>::instance().print();
		QMessageBox::warning(0, QString("Aborting!"), QString("Function %1 does not exist!").arg(func.c_str()));
		abort();
	}
}

/**
 * default constructor
 */
Gene::Gene() : m_function(0) {
}

/**
 * Method for explisit comparison operator
 */
bool Gene::equals(const Gene& other) const {
	if (this->getNumberOfInputs() != other.getNumberOfInputs())
		return false;
	for (int i = 0; i < getNumberOfInputs(); i++) {
		if (inputs[i] != other.inputs[i])
			return false;
	}
	if (m_function->name() != other.m_function->name())
		return false;

	return true;
}

/**
 * Sets the node function to f
 */
void Gene::setFunction(std::string f) {
	m_function.reset(FunctionFactory<double>::instance().create(f));
}
/**
 * Copies another Gene
 */
void Gene::copy(const Gene& other) {

	//copy all inputs
	for (int i = 1; i <= 2; i++) {
		this->setInput(i, other.getInput(i));
	}
	this->setFunction(other.getFunction()->name());

}

/**
 * copy ctor
 */
Gene::Gene(const Gene& other) :
	m_function(0) {
	this->setFunction(other.m_function->name());
	m_function->copy(other.m_function.get());
	//copy all inputs
	for (int i = 1; i <= 2; i++) {
		this->setInput(i, other.getInput(i));
	}

}

/**
 * Assignement operator.
 * Deletes previous help input values.
 */
Gene& Gene::operator =(const Gene &other) {
	this->setFunction(other.m_function->name());
	m_function->copy(other.m_function.get());
	//copy all inputs
	for(int i = 1; i <= 2;i++) {
		this->setInput(i,other.getInput(i));
	}
	return *this;
}
/**
 * comparison operator
 */
bool Gene::operator ==(const Gene& other) {
	if (this->getNumberOfInputs() != other.getNumberOfInputs()) {
		return false;
	}
	for (int i = 0; i < getNumberOfInputs(); i++) {
		if (inputs[i] != other.inputs[i])
		return false;
	}
	if (m_function->name() != other.m_function->name()) {
		return false;
	}
	return true;
}
/**
 * Calculates the output of this gene given the inputs we have
 */
double Gene::calculate(double* inputs, RunInfo &ri) const {

	double result = m_function->calculate(inputs);
	if (isnan(result)) {
		qDebug() << "======= Got NAN result ===============";
		qDebug() << " Function = " << m_function->name().c_str();
		qDebug() << " Inputs   = [" << inputs[0] << " , " << inputs[1] << "]";
		qDebug() << " Input 1  = " << inputs[0];
		qDebug() << " Input 2  = " << inputs[1];
		qDebug() << "=================================";
		abort();
	}
	//qDebug() << "Setting value in " << (void*)this;

	return result;
}

/**
 * Prints the gene to the qDebug stream
 */
void Gene::print() const {
	qDebug() << inputs[0] << "," << inputs[1] << " [" << m_function->name().c_str() << "]";
}

/**
 * Transforms the gene to stdstring
 */
std::string Gene::toStdString() {
	std::ostringstream str;
	if (inputs[0] < 0)
		qDebug() << "BAAD";
	str << inputs[0] << " " << inputs[1] << " " << m_function->name();
	return str.str();
}
/**
 * Prints the gene on a given stream
 * \param out ostringstream to print the gene on
 */
void Gene::toOutStream(std::ostringstream out) {
	out << getNumberOfInputs() << " ";
	for (int i = 0; i < getNumberOfInputs(); i++) {
		out << inputs[i] << " ";
	}
	out << m_function->name().c_str();
}

