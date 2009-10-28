#include "ClassificationProblem.h"
#include "Macros.h"
#include <QTextStream>
#include <QString>
#include <QApplication>
#include <QInputDialog>
#include <QtPlugin>
#include <cmath>
#include <QMessageBox>


ClassificationProblem::ClassificationProblem():m_dataLoaded(false),m_fitnessFactor(0) {
}

ClassificationProblem::~ClassificationProblem() {
    delete m_fitnessFactor;
    m_fitnessFactor =0;
    delete m_inputs;
    m_inputs = 0;
}

/**
 * This will load a set of data for developing the CGP
 */
void ClassificationProblem::GetInputData() {
    //HERE(0);
    QFileDialog* fd = new QFileDialog(0, QString("Open data File"), QApplication::applicationDirPath(), QString("Data File (*.txt)"));
    fd->setFileMode(QFileDialog::AnyFile);
    fd->setModal(true);
    QString fileName;
    QStringList items;
    items << tr("Roundoff") << tr("SSE");
    bool ok = false;
    while(!ok){
        m_fitnessFunction = QInputDialog::getItem(0, tr("Choose fitness evaluation type"),
                                                  tr("Evaluation function:"), items, 0, false, &ok);
    }


    if (fd->exec() == QDialog::Accepted) {
        fileName = fd->selectedFiles().at(0);
    } else {
        return;
    }
    if (fileName.isEmpty()){
        return;
    }
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        LOG("Could not open file!: " << fileName);
        return;
    }
    QTextStream input(&file);
    int rows, cols, outputs;
    input >> rows >> cols >> outputs;
    //create holders for input data
    LOG( " rows " << rows);
    LOG( " cols " << cols);
    LOG( " outputs " << outputs);
    m_inputArray = new TwoDArray<double> (rows, cols - outputs);
    Problem::m_fitness_cases = rows;
    Problem::m_number_of_inputs = cols - outputs;
    Problem::m_number_of_outputs = outputs;
    Problem::setup();
    m_fitnessFactor = new double[m_fitness_cases];
    double num = 0;
    double ans;

    for (int i = 0; i < rows; i++) {
        LOG( " row# " << i);
        for (int j = 0; j < m_number_of_inputs; j++) {
            input >> num;
            (*m_inputArray)[i][j] = num;
        }
        LOG( "finished reading input nums. ");
        for (int k = 0; k < m_number_of_outputs; k++) {
            input >> ans;
            (*m_answers)[i][k] = ans;
        }

        input >> ans;
        LOG( " reading fitness factor: " << ans);
        m_fitnessFactor[i] = ans;
        LOG( " row# " << i << " finished");

    }

    file.close();

    m_dataLoaded = true;
    if(m_fitnessFunction.compare("Roundoff") == 0){
        m_fitFunc = 0;
    }else{
        m_fitFunc = 1;
    }
}
double ClassificationProblem::setFitnessRoundOff(TwoDArray<double>& output){
    double TP,TN,FP,FN;

    double factor = 1;
    double sens, spec,ppv,npv;
    for (int c = 0; c < this->m_number_of_outputs; ++c) { //for each output
        TP = TN = FP = FN=0;
        for(int r = 0; r < Problem::NumberOfFitnessCases();++r){//for all fitness cases
            if((*m_answers)[r][c] == 1) {
                if (output[r][c] < 0.5) {
                    ++FN;//fit+= m_fitnessFactor[currentOut];
                }else{
                    ++TP;
                }
            } else {
                if (output[r][c] >= 0.5) {
                    ++FP;//fit+= m_fitnessFactor[currentOut];
                }else{
                    ++TN;
                }
            }
        }
        sens = TP/(TP+FN);
        spec = TN/(TN+FP);
        ppv = (TP > 0 ? TP/(TP+FP): 0);
        npv = (TN > 0 ? TN/(TN+FN): 0);
        factor *= sens*spec*ppv*npv;
    }

    if(factor >=1){
        qDebug() << "no way!";
    }

    return (1-factor);
    //return (TP+TN)/((double)output.rows()); //percentage
}
double ClassificationProblem::setFitnessSSE(TwoDArray<double>& output){
   //double fit = 0;
    //for (int i = 0; i < this->m_number_of_outputs; i++) {
        //double err = fabs((*m_answers)[currentOut][i]-output[i]);
      //  fit += m_fitnessFactor[currentOut]*err*err;

    //}
    return 0;
}
/**
  *  Calculates fitness value for a given output
  */
double ClassificationProblem::setFitness(TwoDArray<double>& output) {
    /* Fitness function checks if answer is >=, < output */
    if(m_fitFunc == 0){
        return setFitnessRoundOff(output);
    }else if(m_fitFunc == 1){ // calculates absolute error
        return setFitnessSSE(output);
    }else{
        QMessageBox::warning(0, QString("Aborting!"),QString("Wrong fit function!"));
        abort();
    }

}

/**
  * gets the current target value
  */
double ClassificationProblem::answer() {
    return (*m_inputArray)[current][Problem::m_number_of_inputs];
}

/**
 Sets the inputs and returns them.  If it is the last fitness case
 the counter is reset.
 */
double* ClassificationProblem::getCurrentInputs() {
    setInputs();
    currentOut = current;
    current++;
    if (current == Problem::m_fitness_cases)
        current = 0;
    return m_inputs;
}

void ClassificationProblem::inputStringValues(QVector<QString>& inp) {
    for (int i = 0; i < Problem::m_number_of_inputs; i++) {
        QString s = QString("x%1").arg(i);
        inp.push_back(s);
    }
}

QString ClassificationProblem::description() {
    HERE(0);
    return QString("Detecting errors from rate data");
}

void ClassificationProblem::setInputs() {
    for (int i = 0; i < Problem::m_number_of_inputs; i++) {
        m_inputs[i] = (*m_inputArray)[current][i];
    }

}

void ClassificationProblem::Init() {
    LOG("Initing Classification problem");
    GetInputData();
    current = 0;
}
Q_EXPORT_PLUGIN2(ClassificationProblem,ClassificationProblem)



