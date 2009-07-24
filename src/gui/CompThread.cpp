#include <QtGui>
#include <math.h>
#include <ctime>
#include "CompThread.h"
#include "CGPPopulation.h"
#include "HFCPopulation.h"
#include <QtDebug>
#include "Logger.h"
#include "TwoDArray.h"
class TimeInfo {
public:
	double totalTime;
	double avgTime;

};

/**
 * Constructor.
 * \param *parent QObject in control of the thread
 */
CompThread::CompThread(QObject *parent) :
	QThread(parent), m_population(0) {
	qDebug() << "Created thread";
	m_abort = false;
	m_pause=false;
	popType =0;
	stopValue = 0;
}

/**
 * Destructor. Makes sure the thread is finished its last cycle before terminating.
 */
CompThread::~CompThread() {
	qDebug() << "deleting thread";
	mutex.lock();
	qDebug() << "mutex locked";
	m_abort = true;
	condition.wakeOne();
	qDebug() << "condition awoken";
	mutex.unlock();
	qDebug() << "deletion successful";
}

/**
 * Starts the computation
 */
void CompThread::calculate() {
	QMutexLocker locker(&mutex);

	if (!isRunning()) {
		m_abort = false;
		m_pause= false;
		start(LowPriority); //the thread will run with low priority
	}
}

/**
 * Stops the thread.
 */
void CompThread::stop() {
	if (isRunning()) {
		m_abort = true;
		if (m_pause)
			restart();
	}

}

/**
 * Pauses the thread until specifically told to wake up.
 */
void CompThread::pause() {
	m_pause= true;
}

/**
 * Restarts the thread if sleeping
 */
void CompThread::restart() {
	condition.wakeOne();
}

double avgTime = 0;
/**
 * Actually runs the thread
 */
void CompThread::run() {
	qDebug() << "Running thread";
	if (m_population)
		delete m_population; // delete the old one
	//create the new population
	if (ri.gp.HFC) {
		m_population = new HFCPopulation();
	} else {
		m_population = new CGPPopulation();
	}
	//pass along information
	m_population->setInfo(this->ri);

	//create a new population
	qDebug() << "Creating population ....";
	m_population->createPopulation();
	qDebug() << "Finished creating";

	std::string bestInd = "";
	int pass =0;
	//qDebug() << "Getting best individual";
	IndividualInformation indInfo;
	indInfo.m_ind = m_population->getBest();
	indInfo.m_answers = ri.problem->getAnswers();
	indInfo.m_outputs = m_population->getBestOutput();
	indInfo.m_generation = pass;
emit 											update(pass, m_population->getBestFitness(), m_population->getWorstFitness(), m_population->getAvgFitness(), indInfo);
	//loop for the set number of times or until an ind with fitness <= stopValue is found
	qDebug() << "Starting new generations";
	TimeInfo ti;
	ti.totalTime = 0;
	ti.avgTime = 0;
	try {
		while (pass < ri.gp.Generations && (m_population->getBestFitness()> this->stopValue)) {
			clock_t start = clock();
			mutex.lock();
			//check if thread has been paused
			if (m_pause) {
				condition.wait(&mutex);
			}
			m_pause = false;
			mutex.unlock();

			//check if thread has been stopped
			if (m_abort) {
				break;
			}
			//Create the next generations
			m_population->NewGeneration();

			//increment number of generations
			++pass;

			//get the best individual
			//qDebug() << "Getting human readable";
			Individual ind = m_population->getBest();

			//send information to the GUI
			indInfo.m_ind = m_population->getBest();
			indInfo.m_answers = ri.problem->getAnswers();
			indInfo.m_outputs = m_population->getBestOutput();
			indInfo.m_generation = pass;
			emit update(pass, m_population->getBestFitness(), m_population->getWorstFitness(), m_population->getAvgFitness(), indInfo);
			clock_t end = clock();
			ti.totalTime += ((double)(end - start)/CLOCKS_PER_SEC);
			ti.avgTime = ti.totalTime/(double)(pass);

			int totalSecs =(int)(ti.avgTime*(ri.gp.Generations - pass));
			int hours = totalSecs/3600;
			totalSecs %=3600;
			int mins = totalSecs/60;
			totalSecs %= 60;
			QString msg = QString("New Generation took : %1s. Estimated time left: %2h %3m %4s").arg(((end - start)/CLOCKS_PER_SEC)).arg(hours).arg(mins).arg(totalSecs);
			HERE_T(0,msg);

		}
	} catch(std::exception& e) {
		std::cerr << "Caught std::exception : " << e.what();
	}
	qDebug() << "Evolution took : " << ti.totalTime << "seconds";
	//tell the GUI that thread has finished
emit 	finished();

}
