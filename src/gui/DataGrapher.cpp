#include "DataGrapher.h"
#include  <math.h>
const int CurvCnt = 3;

double min(double f, double s) {
	return (f < s ? f : s);
}
double max(double f, double s) {
	return (f > s ? f : s);
}


DataGrapher::DataGrapher(QWidget * parent) :
	QFrame(parent), m_draw_best(true), m_draw_average(true), m_draw_worst(true) {
	current = 0;
	xval = new double[1];
	bestval = new double[1];
	avgval = new double[1];
	worstval = new double[1];
	//connect(parent,SIGNAL(finished()),this,SLOT(finished()));

}

DataGrapher::~DataGrapher() {
	delete[] xval;
	delete[] bestval;
	delete[] avgval;
	delete[] worstval;
	delete[] crv;
}

double DataGrapher::findMin(double* array) {
	double min = 0;
	for (int i = 0; i <= current; i++) {
		if (array[i] < min)
			min = array[i];
	}
	return min;
}
double DataGrapher::findMax(double* array) {
	double max = 0;
	for (int i = 0; i <= current; i++) {
		if (array[i] > max)
			max = array[i];
	}
	return max;
}

void DataGrapher::updateScaleInterval() {
	double min_val = 0;
	double max_val = 0;
	if (m_draw_best) {
		min_val =  findMin(bestval);
	} else if (m_draw_average) {
		min_val = findMin(avgval);
	} else if (m_draw_worst) {
		min_val = findMin(worstval);
	}
	if (m_draw_worst) {
		max_val =  findMax(worstval);
	} else if (m_draw_average) {
		max_val =findMax(avgval);
	} else if (m_draw_best) {
		max_val = findMax(bestval);
	}

	yMap.setScaleInterval(min_val, max_val);
}
void DataGrapher::drawBest(bool value) {
	m_draw_best = value;
	this->repaint();
}
void DataGrapher::init(int size, int maxY) {
	delete[] xval;
	delete[] bestval;
	delete[] avgval;
	delete[] worstval;
	this->size = size;
	xval = new double[size+1];
	bestval = new double[size+1];
	avgval = new double[size+1];
	worstval = new double[size+1];

	crv = new QwtPlotCurve[CurvCnt];
	xMap.setScaleInterval(0, size+1);
	yMap.setScaleInterval(maxY-1, maxY);

	//
	// Frame style
	//  
	setFrameStyle(QFrame::Box|QFrame::Raised);
	//
	// Calculate values
	//
	int i;
	for (i=0; i<=size; i++) {
		xval[i] = 0;
		avgval[i] = 0;
		bestval[i] = 0;
		worstval[i] = 0;
	}

	//
	//  define curve styles
	// 
	i = 0;

	crv[i].setPen(QColor(Qt::darkGreen));
	crv[i].setStyle(QwtPlotCurve::Lines);
	i++;

	crv[i].setPen(QColor(Qt::red));
	crv[i].setStyle(QwtPlotCurve::Lines);
	i++;

	crv[i].setPen(QColor(Qt::darkBlue));
	crv[i].setStyle(QwtPlotCurve::Lines);

	//
	// attach data
	//
	crv[0].setRawData(xval, bestval, size);
	crv[1].setRawData(xval, avgval, size);
	crv[2].setRawData(xval, worstval, size);
}

void DataGrapher::shiftDown(QRect &rect, int offset) const {
	rect.translate(0, offset);

}

void DataGrapher::paintEvent(QPaintEvent *event) {
	QFrame::paintEvent(event);

	QPainter painter(this);
	painter.setClipRect(contentsRect());
	drawContents(&painter);

}

//
//  REDRAW CONTENTS
//
void DataGrapher::drawContents(QPainter *painter) {
	QRect r = contentsRect();
	xMap.setPaintInterval(r.left(), r.right());
	yMap.setPaintInterval(r.bottom(), r.top());

	//
	//  draw curves
	//
	if (m_draw_best) {
		crv[0].draw(painter, xMap, yMap, 0, current);
	}
	if (m_draw_average) {
		crv[1].draw(painter, xMap, yMap, 0, current);
	}
	if (m_draw_worst) {
		crv[2].draw(painter, xMap, yMap, 0, current);
	}
}
