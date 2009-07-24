#ifndef LOGGER_H_
#define LOGGER_H_
#include <QtGui>
#include <SingletonCreator.h>

class Logger {
public:
	static Logger& instance();
	void show(){
		QMutexLocker locker(m_mutex);
		editbox->show();
	}
	void hide(){
		QMutexLocker locker(m_mutex);
		editbox->hide();
	}
	void log(QString s);

	void clear(){
		QMutexLocker locker(m_mutex);
		editbox->clear();
	}

	void  close();
	QTextEdit* getWindow(){
		return editbox;
	}
private:
	Logger();
	QTextEdit* editbox;
	QMutex* m_mutex;
};

#endif /*LOGGER_H_*/
