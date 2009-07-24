#ifndef SR_PROBLEMSELECTER_H_
#define SR_PROBLEMSELECTER_H_

#include "../.ui/ui_SR_ProblemSelecter.h"

class SR_ProblemSelecter: public QDialog, private Ui::SR_ProblemSelecter{
	public:
	SR_ProblemSelecter(QWidget* w):QDialog(w){
		setupUi(this);
	};
	
	virtual void accept(){
		qDebug() << "in accept method";
		if(radioButton->isChecked())
			done(1);
		else if(this->radioButton_2->isChecked())
			done(2);
		else if(this->radioButton_3->isChecked())
			done(3);
		else if(this->radioButton_4->isChecked())
			done(4);
	}
};
#endif /*SR_PROBLEMSELECTER_H_*/
