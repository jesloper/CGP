#ifndef GENE_H
#define GENE_H
#include <iostream>
#include <QtDebug>
#include <QtGui>
#include "FunctionFactory.h"
#include "RunInfo.h"
#include "Macros.h"
#include <utility>

/**
 * \todo: create a hierarchy that allows for modules (possible ONLY use modules?  with possible size of 1).
 * Each module must be able to keep track of number of nodes in it and its inputs/outputs
 * By using a global inputlist (like now) we should be able to still just reference a specific input/output (e.g. the
 */
/**
 * \brief building block of individual
 * Represents a part of an individual (this could be a 'simple' function or a module)
 */
class Gene {
public:
    /**
         * default ctor
         */
    Gene();

    /**
         * Ctor.
         * \param f address of first input
         * \param s address of second input
         * \param func type of node/function (add, div, and, or,sub,...)
         */
    Gene(int f, int s, std::string func);

    bool equals(const Gene& other) const ;


    void setFunction(std::string f);

    /**
         * Gets the node function
         * \returns current function associated with the gene
         */
    Function<double>* getFunction() const {
        return m_function.get();

    }

    /**
         * Gets a input
         * \param inp input number to get
         */
    int getInput(int inp) const {
        if (inp <= 0){
            HERE(0);
            QMessageBox::warning(0, QString("Aborting!"),QString("Wrong getting of input!!"));

            abort();
        }

        if (inp > 2){
            HERE(0);
            QMessageBox::warning(0, QString("Aborting!"),QString("Wrong getting of input!!"));

            abort();
        }
        return inputs[inp-1];
    }
    /**
         * sets a input
         * \param pos position of new input
         * \param newInp input to set
         */
    void setInput(int pos, int newInp) {
        if(newInp < 0){
            QMessageBox::warning(0, QString("Aborting!"),QString("Wrong setting of input!!"));

            abort();
        }
        inputs[pos-1] = newInp;
    }

    double calculate(double* inputs, RunInfo &ri) const; ///< \brief calculates the output given a list of inputs
    void print() const;	///< \brief prints the gene to qDebug()
    int getNumberOfInputs() const{ return m_function->numberOfInputs(); }///< \brief returns the number of inputs to the Gene

    std::string toStdString();	///< \brief returns gene as std::string
    void toOutStream(std::ostringstream out); ///< \brief prints the gene to the given stream
    void copy(const Gene& other); ///< \brief explicit copy of object
    Gene(const Gene& other); ///<\brief copy constructor. Needed as we have arrays!
    Gene& operator=(const Gene &other); ///< \brief assignment operator
    bool operator==(const Gene& other);///< \brief comparison operator
    QString toMatlabCode(QString inp1,QString inp2){
        return m_function->toMatlabCode(inp1,inp2);
    }
protected:
    static std::string m_class_name;
private:

    int inputs[2]; ///< location of inputs to this function
    std::auto_ptr<Function<double> > m_function; ///< pointer to function (using auto_ptr for safety)

};//Gene

/** represents the simplest form of gene (single function consisting of inputs and outputs}
 class SimpleNode{


 }
 */

#endif
