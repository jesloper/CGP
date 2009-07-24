#ifndef DATAGRAPHER_H
#define DATAGRAPHER_H

#include "qwt_scale_map.h"
#include "qwt_plot_curve.h"
#include "qwt_symbol.h"
#include "qwt_plot.h"
#include "qwt_legend.h"
#include "qcolor.h"
#include "qpainter.h"
#include "qpaintengine.h"
#include "math.h"
#include "qframe.h"


/**
 * 	Graphs best, worst and average fitness of the individuals
 * 	\todo: Add legend and scales (numbers on the sides).
 */
class DataGrapher : public QFrame {
	Q_OBJECT
public:
	DataGrapher(QWidget * parent = 0);
	virtual ~DataGrapher();
	void init(int size = 100, int maxY = 0);
	//Add values to the plot
	void setValues(int gen, double best, double avg, double worst) {
		if (gen>size)
			return;
		bestval[gen] = best;
		avgval[gen] = avg;
		worstval[gen] = worst;
		xval[gen] = gen;
		current = gen;
		xMap.setScaleInterval(0, gen+2);
		updateScaleInterval();
	}
public slots:
	void drawBest(bool value);
	void drawWorst(bool value) {
		m_draw_worst = value;
		this->repaint();
	}
	void drawAverage(bool value) {
		m_draw_average= value;
		this->repaint();
	}

protected:
	virtual void paintEvent(QPaintEvent *);
	void drawContents(QPainter *p);

private:
	void shiftDown(QRect &rect, int offset) const;
	double findMin(double* array);
	double findMax(double* array);
	void updateScaleInterval();
	QwtPlotCurve* crv;
	double* xval;
	double* yval;
	double* avgval;
	double* bestval;
	double* worstval;
	int size;
	int current;
	QwtScaleMap xMap;
	QwtScaleMap yMap;
	bool m_draw_best;
	bool m_draw_average;
	bool m_draw_worst;
};

#endif
