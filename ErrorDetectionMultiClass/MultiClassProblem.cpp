#include "MultiClassProblem.h"
#include "Macros.h"
#include <QTextStream>
#include <QString>
#include <QApplication>
#include <QInputDialog>
#include <QtPlugin>
#include <math.h>

MultiClassProblem::MultiClassProblem():m_inputArray(0),m_dataLoaded(false),m_fitnessFactor(0) {
}

MultiClassProblem::~MultiClassProblem() {
    delete m_inputArray;
    m_inputArray = 0;
    delete m_fitnessFactor;
    m_fitnessFactor =0;
    delete m_inputs;
    m_inputs = 0;
}

/**
 * This will load a set of data for developing the CGP
 */
void MultiClassProblem::GetInputData() {
    //HERE(0);
    QFileDialog* fd = new QFileDialog(0, QString("Open data File"), QApplication::applicationDirPath(), QString("Data File (*.txt)"));
    fd->setFileMode(QFileDialog::AnyFile);
    fd->setModal(true);
    QString fileName;

    QStringList items;
    items << tr("Roundoff") << tr("SSE");
    bool ok = false;
    while(!ok){
        m_fitnessFunction = QInputDialog::getItem(0, tr("Choose fitness evaluation type:"),
                                                  tr("Evaluation function:"), items, 0, false, &ok);
    }
    if (fd->exec() == QDialog::Accepted) {
        fileName = fd->selectedFiles().at(0);
    } else {
        return;
    }
    if (fileName.isEmpty())
        exit(0);
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        LOG("Could not open file!: " << fileName);
        exit(0);
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
    Problem::m_number_of_inputs = cols - 1;
    Problem::m_number_of_outputs = 1;
    Problem::setup();
    Problem::m_inputs = new double[rows - 1]; //for later use
    m_fitnessFactor = new double[rows-1];
    double num = 0;
    double ans;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols - outputs; j++) {
            input >> num;
            (*m_inputArray)[i][j] = num;
        }
        LOG( "finished reading input nums. ");
        for (int k = 0; k < outputs; k++) {
            input >> ans;
            LOG( "got answer: " << ans);
            (*m_answers)[i][k] = ans;
        }


        input >> ans;
        LOG( " reading fitness factor: " << ans);
        m_fitnessFactor[i] = ans;

    }

    file.close();
    m_dataLoaded = true;
    if(m_fitnessFunction.compare("Roundoff") == 0){
        m_fitFunc = 0;
    }else{
        m_fitFunc = 1;
    }
    QApplication::restoreOverrideCursor();
}

double MultiClassProblem::setFitness(double* output) {
    double fit = 0;

    if(m_fitFunc == 0){

        double out = floor(3*output[0]+0.5);
        if(out != (*m_answers)[currentOut][0]){
            fit+= m_fitnessFactor[currentOut];
        }
    }else if(m_fitFunc == 1){
        double err = fabs((*m_answers)[currentOut][0]-output[0]);
        fit += m_fitnessFactor[currentOut]*err*err;
    }else{
       LOG("fitness function is BAAAD!");
    }

    return fit;
}

double MultiClassProblem::answer() {
    return (*m_inputArray)[current][Problem::m_number_of_inputs];
}

/**
 Sets the inputs and returns them.  If it is the last fitness case
 the counter is reset.
 */
double* MultiClassProblem::getCurrentInputs() {
    setInputs();
    currentOut = current;
    current++;
    if (current == Problem::m_fitness_cases)
        current = 0;
    return m_inputs;
}

void MultiClassProblem::inputStringValues(std::vector<std::string>& inp) {
    for (int i = 0; i < Problem::m_number_of_inputs; i++) {
        QString s = QString("x%1").arg(i);
        inp.push_back(s.toStdString());
    }
}

QString MultiClassProblem::description() {
    HERE(0);
    return QString("Detecting errors from rate data");
}

void MultiClassProblem::setInputs() {
    for (int i = 0; i < Problem::m_number_of_inputs; i++) {
        m_inputs[i] = (*m_inputArray)[current][i];
    }

}

void MultiClassProblem::Init() {
    LOG("Initing Classification problem");
    GetInputData();
    current = 0;
}
Q_EXPORT_PLUGIN2(MultiClassProblem,MultiClassProblem)



