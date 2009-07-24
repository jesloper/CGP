#ifndef MACROS_H_
#define MACROS_H_
#include <ctime>
#include <QtDebug>
#include <QDateTime>
#define DEBUG_LEVEL 1
#define MYDEBUG( level, debug_statement ) { \
if ( DEBUG_LEVEL >= level ) \
{ \
    debug_statement; \
} }

#define LOG(text){\
	HERE_T(-1,text)\
}

#define HERE_T(level,text) { \
if ( DEBUG_LEVEL >= level ) \
{ \
	qDebug() << "[" << QDateTime::currentDateTime().toString() << "  " << __PRETTY_FUNCTION__ << " " << __FILE__  << ":" << __LINE__ << "] " << text; \
} }

#define HERE(level) { \
	HERE_T(level,"") \
}

#define TIME_STATEMENT(function,agg_time){ \
if (DEBUG_LEVEL > 0) \
{\
clock_t timer = clock(); \
function; \
agg_time += (double)(clock() - timer)/CLOCKS_PER_SEC; \
}\
else \
{\
    function;\
} }

#endif /*MACROS_H_*/
