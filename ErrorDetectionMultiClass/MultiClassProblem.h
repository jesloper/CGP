#ifndef ErrorDetection_H_
#define ErrorDetection_H_
#include <QtDebug>
#include "Problem.h"
#include <QFileDialog>
#include "TwoDArray.h"
#include <Macros.h>
//#include "InputDialog.h"
/**
\class ClassificationProblem
*/
class MultiClassProblem : public QObject,public Problem{
	Q_OBJECT
	Q_INTERFACES(Problem)
public:
	MultiClassProblem();
	virtual ~MultiClassProblem();
    virtual double setFitness(double* output);
	virtual double answer();
	virtual double* getCurrentInputs();
	virtual void inputStringValues(std::vector<std::string>& inp);
	virtual QString description();
	QString getName(){ return "ErrorDetectionMultiClass";} ///< \brief this should return the name of the problem
	void Init();
private:
	void setInputs();
	void GetInputData();
	int current;
        int currentOut;
	double x,y,z;
	TwoDArray<double>* m_inputArray;	///< \brief holds the input and output values
	bool m_dataLoaded;	///< \brief flag whether data has been loaded yet
	double* m_fitnessFactor; ///< \brief multiplier of fitness value. Used in order to 'direct' the classification
        QString m_fitnessFunction; ///< \brief chooses the type of fitness evaluation
        int m_fitFunc;
};
#endif /*ErrorDetection_H_*/
