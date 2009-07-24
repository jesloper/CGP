#ifndef StoreThread
#define StoreThread
#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>
#include <qlist.h>
#include "RunInfo.h"
#include "Function.h"
#include "Problem.h"
#include "Individual.h"

class PopulationInterface;

/**
 * \class StoreThread
 * \brief Worker thread used to store the info from the CGP.
 * Initializes the correct population type based on the RunInfo structure.
 * Emits update signals which can be connected to by the owner of the thread.
 */
class StoreThread : public QThread {
Q_OBJECT

public:
	addInfo(RunInfo info){
	}
	setFile(QString filename){
		QFile file(filename);
		bool exists = file.exists();

		if (!file.open(QIODevice::ReadWrite | QFile::Text)) {
			QMessageBox::warning(this, tr("SAX Bookmarks"), tr("Cannot read file %1:\n%2.")
			.arg(filename)
			.arg(file.errorString()));
			return;
		}

		QDomDocument domDocument;
		//tries to load the document
		if (exists) {
			QString errorStr;
			int errorLine;
			int errorColumn;
			qDebug() << "reading old file";
			if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
				QMessageBox::information(window(), tr("DOM Bookmarks"), tr("Parse error at line %1, column %2:\n%3")
				.arg(errorLine)
				.arg(errorColumn)
				.arg(errorStr));
				return;
			}

			QDomElement root = domDocument.documentElement();
			if (root.tagName() != "xbel") {
				QMessageBox::information(window(), tr("DOM Bookmarks"), tr("The file is not an XBEL file."));
				return;
			}
			if (file.seek(0)) {
				qDebug() << "seek worked";
			}
		} else {
			QDomElement root = domDocument.createElement("xbel");
			root.setAttribute("version", "1.0");
			domDocument.appendChild(root);
		}

		//QDomDocument domDocument;
		runInfo.store(domDocument);
		QTextStream out(&file);
		domDocument.save(out, 4);
		saved = true;
	}
protected:
	void run(); ///< \brief starts the thread

private:
	QList<>RunInfo>m_list; /// information to be stored to file
	QFile m_file; /// file to store the information to

};
#endif
