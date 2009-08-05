#include "GridViewer.h"
#include "Macros.h"
#include <QtGui>

GridViewer::GridViewer(QWidget* parent) :
	QWidget(parent) {
	setup();
}

void GridViewer::setup() {
	setupUi(this);
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);

	m_regularPen = new QPen();
	brush = new QBrush(Qt::white);
	m_grayBrush = new QBrush(Qt::gray);
	m_heavyPen = new QPen(Qt::black);
	m_heavyPen->setWidth(2);
	Individual i;
	i.insertGene(0, 1, "+");
	updateIndividual(i);

	node_size = 1000;
	spacing = 30;
	inputs = 2;
	rows = 4;
	cols = 3;
}

GridViewer::~GridViewer() {
	delete m_regularPen;
	delete brush;
}

void GridViewer::updateIndividual(QString s) {
	m_individual.fromString(s);
	this->Fitness->display(m_individual.getFitness());
	this->repaint();
}

void GridViewer::drawNodeLines(Gene& g, int row, int col) {

	for (int k = 0; k < g.getNumberOfInputs(); k++) {
		int i1 = g.getInput(k + 1) - inputs;
		int r, c;
		if (i1 >= 0) {
			r = i1 % rows;
			c = i1 / rows;
			m_painter->drawLine(col * x_size + col * spacing, row * y_size
					+ row * spacing + (1 + 2* k ) * y_size / 4, c * x_size + c
					* spacing + x_size, r * y_size + r * spacing + y_size / 2);
		}
	}
}

void GridViewer::drawNodes() {
	int row = 0;
	int col = 0;
	QRect rect(0, 0, x_size, y_size);
	QVector<int> active = m_individual.getActiveNodes();
	int index = -1;
	for (int i = 1; i <= m_individual.getNumberOfNodes(); i++) {

		m_painter->save();

		if (i > rows) {
			index = active.indexOf(i-1);
			if (index != -1) {
				m_painter->setPen(*m_heavyPen);
			}
			if (index != -1 || drawInactive->isChecked()) {

				Gene g = m_individual.getGenes().at(i - 1);
				this->drawNodeLines(g, row, col);
			}
		}

		row++;
		if (i % rows == 0) {
			col++;
			row = 0;
		}
		m_painter->restore();
	}
	row = 0;
	col = 0;
	for (int i = 1; i <= m_individual.getNumberOfNodes(); i++) {
		m_painter->save();
		index = active.indexOf(i - 1);
		if (index == -1) {
			m_painter->setBrush(*m_grayBrush);
		}
		if (index != -1 || drawInactive->isChecked()) {

			m_painter->translate(col * x_size + col * spacing, row * y_size
					+ row * spacing);

			m_painter->drawRect(rect);
			m_painter->drawText(
					x_size / 2,
					y_size / 2 + fontsize / 2,
					QString("%1").arg(
							m_individual.getGenes().at(i - 1).getFunction()->name()));

			for (int k = 0; k
					< m_individual.getGenes().at(i - 1).getNumberOfInputs(); k++) {
				m_painter->drawText(x_size / 10, (1 + 2* k ) * y_size / 4
						+ fontsize / 2, QString("%1").arg(
						m_individual.getGenes().at(i - 1).getInput(k + 1)));

			}
		}
		//m_painter->drawText(x_size / 2-fontsize/2, y_size / 5
		//		+ fontsize / 2,QString("[%1]").arg(i-1));
		row++;
		if (i % rows == 0) {
			col++;
			row = 0;
		}
		m_painter->restore();
	}
}

void GridViewer::paintEvent(QPaintEvent * /* event */) {
	x_size = node_size / m_individual.getNumberOfNodes();
	y_size = node_size / m_individual.getNumberOfNodes();
	fontsize = 12 + (x_size - 50) / 5;
	QRect rect(0, 0, x_size, y_size);

	m_painter = new QPainter(this);
	m_painter->setPen(*m_regularPen);
	m_painter->setBrush(*brush);
	QFont f("Helvetica", fontsize);
	m_painter->setFont(f);

	m_painter->translate(10, 75);
	drawNodes();

	m_painter->setPen(palette().dark().color());
	m_painter->setBrush(Qt::NoBrush);
	m_painter->drawRect(QRect(-5, -5, width() - 15, height() - 80));
	delete m_painter;
}

void GridViewer::on_nodeSize_valueChanged(int s) {
	node_size = s;
	this->repaint();
}

