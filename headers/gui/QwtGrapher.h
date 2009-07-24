#ifndef QWTGRAPHER_H_
#define QWTGRAPHER_H_
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <TwoDArray.h>
class QwtGrapher : public QwtPlot{
	Q_OBJECT
public:
	QwtGrapher(QWidget * parent = 0);
	virtual ~QwtGrapher(){};
protected:
	void drawContents(QPainter *p);
	void updateData(TwoDArray<double> data);
	
private slots:
    void showCurve(QwtPlotItem *, bool on);
private:
	QwtPlotCurve* m_curves[3];
	TwoDArray<double>* m_values;
	double * m_x_values;
};
#endif /*QWTGRAPHER_H_*/
