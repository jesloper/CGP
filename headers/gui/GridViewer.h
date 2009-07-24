#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "ui_gridViewer.h"
#include "Individual.h"

#include <QWidget>
#include <QBrush>
#include <QPen>

class GridViewer : public QWidget, private Ui::gridViewer {
Q_OBJECT

public:
	GridViewer(QWidget* parent = 0);
	~GridViewer();
	void setupViewer(int r, int c, int i) {
		rows = r;
		cols = c;
		inputs = i;
	}

	void updateIndividual(QString s);
	void updateIndividual(Individual i) {
		m_individual = i;
		this->Fitness->display(m_individual.getFitness());
			this->repaint();
	}

protected:
	void paintEvent(QPaintEvent *event);
private:
	void setup();
	QPen* m_regularPen;
	QPen* m_heavyPen;
	QBrush* brush;
	QBrush* m_grayBrush;
	QPainter* m_painter;
	Individual m_individual;
	int node_size;
	int rows;
	int cols;
	int inputs;
	int spacing;
	int x_size;
	int y_size;
	int fontsize;
	void drawNodeLines(Gene& g, int row,int col);
	void drawNodes();
private slots:
	void on_loadInd_clicked() {
		qDebug() << "Clicked ";
		/*qDebug() << "old one:" << QString(m_individual.toString().c_str());
		m_individual.fromString(textInd->text().toStdString());
		rows =Rows->value();
		cols = Cols->value();
		inputs = Inputs->value();
		this->repaint();
		*/
	}
	void on_nodeSize_valueChanged(int s);
	void on_spacing_valueChanged(int s) {
		spacing = s;
		this->repaint();
	}
	void on_drawInactive_stateChanged(int b){
		this->repaint();
	}
};

#endif /*MAINWINDOW_H_*/
