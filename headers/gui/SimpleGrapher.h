#ifndef SIMPLEGRAPHER_H_
#define SIMPLEGRAPHER_H_

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
#include "TwoDArray.h"

class SimpleGrapher : public QFrame{
public:
	SimpleGrapher(QWidget * parent = 0);
	virtual ~SimpleGrapher(){
		if(m_x_values)
			delete []m_x_values;
		if (crv)
				delete[] crv;
        }
	//Add values to the plot
	void setValues(TwoDArray<double> array) {
		m_values = array;
		updatePlot();
	}
	void initializePlot(TwoDArray<double> array){
		m_values = array;
		initPlot();
	}
protected:
	virtual void paintEvent(QPaintEvent *);
	void drawContents(QPainter *p);

private:
	void updatePlot();
	void initPlot();
	QwtPlotCurve* crv;
	TwoDArray<double> m_values;
	double* m_x_values;
	double* yValues;
	QwtScaleMap xMap;
	QwtScaleMap yMap;
};

#endif /*SIMPLEGRAPHER_H_*/
