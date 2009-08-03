#include <QtGui>
#include <QPixmap>
#include "MainWindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef WIN32
void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
#endif
/**
 * Create CGPWindow and pass control to QT
 * \todo: redirect output to text window instead of console
 */
int main( int argc, char ** argv ) {
	QApplication a(argc, argv);
	QPixmap pixmap(":/icons/splash");
	pixmap = pixmap.scaled(500,500);
	QSplashScreen splash(pixmap);
	splash.show();
	splash.showMessage("Loading Plugins!",Qt::AlignLeft,Qt::white);
	a.processEvents();
	CGPWindow thing;
	thing.loadPlugins();
	splash.showMessage("Reading settings!",Qt::AlignLeft,Qt::white);
	a.processEvents();
	thing.ReadSettings();
	splash.showMessage("All ready!",Qt::AlignLeft,Qt::white);
	a.processEvents();
	
	thing.show();
	sleep(1000);
	splash.finish(&thing);
	return a.exec();
	qInstallMsgHandler(0);

}
