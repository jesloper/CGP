#include "FunctionSelecter.h"
#include "Function.h"
#include "Macros.h"
#include <iostream>


/**
  *	Constructor
  * Sets widget and custom connections
  */
FunctionSelecter::FunctionSelecter(QWidget *parent,std::map<std::string,Function<double>*> &available ,std::map<std::string,Function<double>*> &chosen)
: QDialog(parent)
{
	setupUi(this);

	qDebug("Setting up Function selector");
	fMap = available;
	cMap = &chosen;
	std::map<std::string,Function<double>*>::iterator it = available.begin();

    QTreeWidgetItem* none = new QTreeWidgetItem(AvailableFunctions,QStringList(tr("No input")));
    QTreeWidgetItem* single   = new QTreeWidgetItem(AvailableFunctions,QStringList(tr("Single input")));
    QTreeWidgetItem* multiple = new QTreeWidgetItem(AvailableFunctions,QStringList(tr("Multiple input")));

	//populate the map of available functions with the default Tree
	for(;it != available.end();it++){
        if((*it).second->numberOfInputs() ==0)
		  new QTreeWidgetItem(none, QStringList((*it).first.c_str()));
        else if((*it).second->numberOfInputs() ==1)
          new QTreeWidgetItem(single, QStringList((*it).first.c_str()));
        else
          new QTreeWidgetItem(multiple, QStringList((*it).first.c_str()));

    }
	//AvailableFunctions->sortItems();


	for(it = chosen.begin();it != chosen.end();it++){
		new QListWidgetItem(tr((*it).first.c_str()), ChosenFunctions);
	}
	ChosenFunctions->sortItems();

	connect(AvailableFunctions,SIGNAL( itemActivated(QTreeWidgetItem*,int)),this,SLOT(itemActivated(QTreeWidgetItem*)));
	connect(AvailableFunctions,SIGNAL( itemClicked(QTreeWidgetItem*,int)),this,SLOT(itemClicked(QTreeWidgetItem*)));
    connect(ChosenFunctions,SIGNAL( itemActivated(QListWidgetItem*)),this,SLOT(deleteItem(QListWidgetItem*)));

}

void FunctionSelecter::on_Finish_clicked(){
	//emit signal with the list of chosen functions (this will be picked up in the mainWindow
	(*cMap).clear();
	std::map<std::string,Function<double>*>::iterator it;
	for(int i = 0; i < ChosenFunctions->count();i++){
		ChosenFunctions->setCurrentRow(i);
		it = fMap.find(ChosenFunctions->currentItem()->text().toStdString());
		//if the function exists in the available list add it to the chosen list
		if(it != fMap.end()){
			(*cMap)[(*it).first] = (*it).second;
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

    std::map<std::string,Function<double>*>::iterator it;
    it = fMap.find(item->text(0).toStdString());
	if(it != fMap.end())
		Description->setPlainText(QString((*it).second->description().c_str()));

}
void FunctionSelecter::deleteItem(QListWidgetItem *){
	ChosenFunctions->takeItem(ChosenFunctions->currentRow());
}
