/**
	FunctionSelecter.h

	Allows for a selection of functions to be used for the individuals
*/
#ifndef FUNCTIONSELECTER_H
#define FUNCTIONSELECTER_H

#include "ui_FunctionChooser.h"

#include "Function.h"
#include <map>
#include <QWidget>

class FunctionSelecter : public QDialog, private Ui::FSelector
    {
        Q_OBJECT

    public:
        FunctionSelecter(QWidget* parent,QMap<QString,Function<double>*>& available,QMap<QString,Function<double>*>& chosen );
		
	private:
		QMap<QString,Function<double>*> fMap;
		QMap<QString,Function<double>*>* cMap;
	private slots:
	  void on_Finish_clicked();
	  void on_Cancel_clicked();
	  void itemActivated(QTreeWidgetItem * item);
	  void itemClicked(QTreeWidgetItem * item);
	  void deleteItem(QListWidgetItem * item);
    };
#endif
