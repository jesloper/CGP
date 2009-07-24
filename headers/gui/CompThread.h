#ifndef COMPTHREAD
#define COMPTHREAD
#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>
#include "RunInfo.h"
#include "Function.h"
#include "Problem.h"
#include "Individual.h"

class PopulationInterface;

/**
 * \class CompThread
 * \brief Worker thread used to run the GP.
 * Initializes the correct population type based on the RunInfo structure.
 * Emits update signals which can be connected to by the owner of the thread.
 */
class CompThread : public QThread {
Q_OBJECT

public:
	CompThread(QObject *parent = 0); ///< \brief ctor
	~CompThread();///< \brief dtor

	/**
	 * \brief initalizes the thread
	 * \param info RunInfo structure containin all parameters for the current run
	 * \param v the run should be stopped if best fitness is equal or below v
	 */
	void init(RunInfo info, double v) {
		ri = info;
		stopValue = v;
	}
	;
	RunInfo getRunInfo() {
		return ri;
	}
	/**
	 * \brief sets the q tournament size
	 * \param q tournament size
	 */
	void setQSize(int q) {
		qDebug() << "setting qSize to " << q;
		qSize = q;
	}
	;

	void calculate(); ///< \brief starts the thread calculations (creating new generations)
	void stop(); ///< \brief requests the thread to stop
	void pause(); ///< \brief requests the thread to pause
	void restart(); ///< \brief requests the thread to restart (no action if not paused)

	/**
	 * \brief Access to the population
	 * \returns Pointer to the PopulationInterface
	 */
	PopulationInterface* get_Population() {
		return m_population;

	}
signals:
	void update(int gen, double best, double worst, double avg, IndividualInformation);///< \brief emitted at the end of each new generation cycel
	void finished(); ///< \brief emitted when the thread has finished execution

protected:
	void run(); ///< \brief starts the thread

private:
	QWaitCondition condition; ///< \brief used to pause and restart the thread
	QMutex mutex;

	bool m_abort; ///< \brief this flag is set if an m_abort is requested
	bool m_pause; ///< \brief This flag is set if a pause is requested
	//CGP specific parameters	
	int individuals_;
	int generations_;
	RunInfo ri;
	double stopValue;
	std::map<std::string,Function<double>*>* functions;
	int qSize;
	int popType;
	PopulationInterface* m_population;

};
#endif
