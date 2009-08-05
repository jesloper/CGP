#include "FunctionSelecter.h"
#include "Function.h"
#include "Macros.h"
#include <iostream>


/**
  *	Constructor
  * Sets widget and custom connections
  */
FunctionSelecter::FunctionSelecter(QWidget *parent,QMap<QString,Function<double>*> &available ,QMap<QString,Function<double>*> &chosen)
: QDialog(parent)
{
	setupUi(this);

        LOG("Setting up Function selector");
	fMap = available;
	cMap = &chosen;
	QMapIterator<QString,Function<double>*> i(available);
	
	
    QTreeWidgetItem* none = new QTreeWidgetItem(AvailableFunctions,QStringList(tr("No input")));
    QTreeWidgetItem* single   = new QTreeWidgetItem(AvailableFunctions,QStringList(tr("Single input")));
    QTreeWidgetItem* multiple = new QTreeWidgetItem(AvailableFunctions,QStringList(tr("Multiple input")));

	while(i.hasNext()){
		i.next();
		if(i.value()->numberOfInputs() == 0){
		 new QTreeWidgetItem(none, QStringList(i.key()));
		}else if(i.value()->numberOfInputs() == 1){
		new QTreeWidgetItem(single, QStringList(i.key()));
		}else{
		new QTreeWidgetItem(multiple, QStringList(i.key()));
		}
	}
	          
      //AvailableFunctions->sortItems();

	i = chosen;
	while(i.hasNext()){
		i.next();
		new QListWidgetItem(i.key(), ChosenFunctions);
	}
	
	ChosenFunctions->sortItems();
	connect(AvailableFunctions,SIGNAL( itemActivated(QTreeWidgetItem*,int)),this,SLOT(itemActivated(QTreeWidgetItem*)));
	connect(AvailableFunctions,SIGNAL( itemClicked(QTreeWidgetItem*,int)),this,SLOT(itemClicked(QTreeWidgetItem*)));
        connect(ChosenFunctions,SIGNAL( itemActivated(QListWidgetItem*)),this,SLOT(deleteItem(QListWidgetItem*)));
}

void FunctionSelecter::on_Finish_clicked(){
	//emit signal with the list of chosen functions (this will be picked up in the mainWindow
	(*cMap).clear();
	QMap<QString,Function<double>*>::iterator it;
	for(int i = 0; i < ChosenFunctions->count();i++){
		ChosenFunctions->setCurrentRow(i);
		it = fMap.find(ChosenFunctions->currentItem()->text());
		//if the function exists in the available list add it to the chosen list
		if(it != fMap.end()){
			(*cMap)[it.key()] = it.value();
		}

	}
	this->close();
}
void FunctionSelecter::on_Cancel_clicked(){
	this->close();
}

//called on double click on item
void FunctionSelecter::itemActivated(QTreeWidgetItem * item){
	ChosenFunctions->insertItem(0,item->text(0));
	ChosenFunctions->sortItems();
}
//called when an item is selected by a clicked
void FunctionSelecter::itemClicked(QTreeWidgetItem * item){
	if(item->text(0) == "Single input" || item->text(0) == "Multiple input")
       return;

    QMap<QString,Function<double>*>::iterator it;
    it = fMap.find(item->text(0));
    if(it != fMap.end()){
	Description->setPlainText(QString(it.value()->description()));
    }
}
void FunctionSelecter::deleteItem(QListWidgetItem *){
	ChosenFunctions->takeItem(ChosenFunctions->currentRow());
}
