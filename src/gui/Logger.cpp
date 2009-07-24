#include "Logger.h"

//SingletonCreator<Logger> _creator_;

/**
 * access to singleton instance
 */
Logger& Logger::instance(){
	static Logger* log = new Logger();
	return *log;
	
}

/**
 * Ctor
 */
Logger::Logger(){
	m_mutex = new QMutex();
	QMutexLocker locker(m_mutex);
	//_creator_.dummy();
	editbox = new QTextEdit();
	editbox->resize(600,200);
	editbox->show();
	
}
/**
 * Appends a log message to the debug window
 */
void Logger::log(QString s){
	if(m_mutex->tryLock()){
		editbox->append("[" + QTime::currentTime().toString() + "] " + s);
		m_mutex->unlock();
	}
}
/**
 * closes the edit box
 */
void Logger::close(){
	QMutexLocker locker(m_mutex);
	editbox->close();
}
