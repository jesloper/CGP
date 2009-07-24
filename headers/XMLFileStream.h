#ifndef XMLFILESTREAM_H
#define XMLFILESTREAM_H
#include <QTextStream>
#include "../headers/RunInfo.h"

class XMLFileStream : public QTextStream{

	operator<< (RunInfo info){
		//store the RunInfo
	};
};

#endif