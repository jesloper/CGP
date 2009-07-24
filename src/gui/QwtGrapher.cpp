#include "QwtGrapher.h"
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>

QwtGrapher::QwtGrapher(QWidget* parent):
	QwtPlot(parent) {
	m_values = new TwoDArray<double>(3,100);
	plotLayout()->setAlignCanvasToScales(true);
	QwtLegend *legend = new QwtLegend;
	legend->setItemMode(QwtLegend::CheckableItem);
	insertLegend(legend, QwtPlot::RightLegend);

	setAxisTitle(QwtPlot::xBottom, " FitnessCases");
	/*setAxisScaleDraw(QwtPlot::xBottom, 
	 new TimeScaleDraw(cpuStat.upTime()));
	 setAxisScale(QwtPlot::xBottom, 0, HISTORY);
	 setAxisLabelRotation(QwtPlot::xBottom, -50.0);
	 setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);
	 */
	/*
	 In situations, when there is a label at the most right position of the
	 scale, additional space is needed to display the overlapping part
	 of the label would be taken by reducing the width of scale and canvas.
	 To avoid this "jumping canvas" effect, we add a permanent margin.
	 We don't need to do the same for the left border, because there
	 is enough space for the overlapping label below the left scale.
	 */

	QwtScaleWidget *scaleWidget = axisWidget(QwtPlot::xBottom);
	const int fmh = QFontMetrics(scaleWidget->font()).height();
	scaleWidget->setMinBorderDist(0, fmh / 2);

	setAxisTitle(QwtPlot::yLeft, "Ouput") ;
	//setAxisScale(QwtPlot::yLeft, 0, 100);

	// Background *bg = new Background();
	//bg->attach(this);

	/*CpuPieMarker *pie = new CpuPieMarker();
	 pie->attach(this);
	 */
	QwtPlotCurve *curve;

	curve = new QwtPlotCurve("Best");
	curve->setPen(QColor(Qt::green));
	curve->attach(this);
	m_curves[0] = curve;

	curve = new QwtPlotCurve("Average");
	curve->setPen(QColor(Qt::blue));
	curve->setZ(curve->z() - 1);
	curve->attach(this);
	m_curves[1] = curve;

	curve = new QwtPlotCurve("Worst");
	curve->setPen(QColor(Qt::black));
	curve->setZ(curve->z() - 2);
	curve->attach(this);
	m_curves[2] = curve;

	showCurve(m_curves[0], true);
	showCurve(m_curves[1], true);
	showCurve(m_curves[2], true);

	connect(this, SIGNAL(legendChecked(QwtPlotItem *, bool)), 
	SLOT(showCurve(QwtPlotItem *, bool)));
}

void QwtGrapher::showCurve(QwtPlotItem *item, bool on) {
	item->setVisible(on);
	QWidget *w = legend()->find(item);
	if (w && w->inherits("QwtLegendItem") )
		((QwtLegendItem *)w)->setChecked(on);

	replot();
}

void QwtGrapher::updateData(TwoDArray<double> data) {
	(*m_values) = data;
	m_x_values = new double[m_values->cols()];
	for (size_t x =0; x < m_values->cols(); x++)
		m_x_values[x] = x;
	m_curves[0]->setData(m_x_values, (*m_values)[0], m_values->cols());
	m_curves[1]->setData(m_x_values, (*m_values)[1], m_values->cols());
	m_curves[2]->setData(m_x_values, (*m_values)[2], m_values->cols());
	replot();

}