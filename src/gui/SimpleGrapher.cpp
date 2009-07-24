#include "SimpleGrapher.h"
#include "MathFunctions.h"
#include "Macros.h"
QColor colortable[3] = { QColor(Qt::red), QColor(Qt::black), QColor(Qt::cyan) };

SimpleGrapher::SimpleGrapher(QWidget* parent) :
	QFrame(parent), crv(0), m_values(1, 1), m_x_values(0) {
}

void SimpleGrapher::updatePlot() {
	for (size_t i = 0; i < m_values.rows(); i++) {
		crv[i].setData(m_x_values, m_values[i], m_values.cols());
		if (findUnique(m_values[i], m_values.cols()) < 5) { //if less than 5 unique we use a step curve
			crv[i].setStyle(QwtPlotCurve::Steps);
		}else{
			crv[i].setStyle(QwtPlotCurve::Lines);
		}
	}
	xMap.setScaleInterval(0, m_values.cols());
	yMap.setScaleInterval(findMin(m_values[0], m_values.cols()) - .1, findMax(m_values[0], m_values.cols()) + .1);
	this->repaint();
}
/**
 * Sets up the plot
 */
void SimpleGrapher::initPlot() {
	if (crv)
		delete[] crv;
	if (m_x_values)
		delete[] m_x_values;

	crv = new QwtPlotCurve[m_values.rows()];
	setFrameStyle(QFrame::Box | QFrame::Raised);
	m_x_values = new double[m_values.cols()];
	for (size_t x = 0; x < m_values.cols(); x++)
		m_x_values[x] = x;

	int markerSize = (int) (200.0 / (double) (m_values.cols() + 1)); //ajust marker size according to number of values
	for (size_t i = 0; i < m_values.rows(); i++) {
		QwtSymbol sym;// = new QwtSymbol;
		sym.setStyle(QwtSymbol::Cross);
		sym.setSize(QSize(markerSize, markerSize));
                sym.setPen(colortable[i%3]);
		crv[i].setSymbol(sym);
                crv[i].setPen(colortable[i%3]);
		crv[i].setStyle(QwtPlotCurve::Lines);
		crv[i].setRawData(m_x_values, m_values[i], m_values.cols());
		if (findUnique(m_values[i], m_values.cols()) < 3) { //if less than 3 unique we use a step curve
			crv[i].setStyle(QwtPlotCurve::Steps);
		}
	}
	// set the correct scale
	xMap.setScaleInterval(0, m_values.cols());
	yMap.setScaleInterval(findMin(m_values[0], m_values.cols()) - .1, findMax(m_values[0], m_values.cols()) + .1);
}
void SimpleGrapher::paintEvent(QPaintEvent *event) {
	QFrame::paintEvent(event);

	QPainter painter(this);
	painter.setClipRect(contentsRect());
	drawContents(&painter);

}

/**
 * Actually draws the graph
 */
void SimpleGrapher::drawContents(QPainter *painter) {
	if (!m_x_values)
		return;
	QRect r = contentsRect();
	xMap.setPaintInterval(r.left(), r.right());
	yMap.setPaintInterval(r.bottom(), r.top());
	for (size_t i = 0; i < m_values.rows(); i++) {
		crv[i].draw(painter, xMap, yMap, 0, m_values.cols());
	}

}
