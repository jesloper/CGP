/**
 */

#include "MainWindow.h"

//executing thread
#include "CompThread.h"
#include "QwtGrapher.h"
//available and chosen functions
#include "Function.h"
#include "FunctionFactory.h"
#include <QtPlugin>

//console window for windows applications
//#include "DebugWindow.h"
//form for selecting functions
#include "FunctionSelecter.h"

//different problem specifications
#include "SimpleProblems.h"
#include "PopulationInterface.h"
#include "Macros.h"
#include "FunctionFactory.h"
#include "Logger.h"
#include <QtGui>
#include "DataGrapher.h"
#include "Problem.h"
#include "qdebugstream.h"

void myMessageOutput(QtMsgType type, const char *msg) {
    std::cout << msg << std::endl;
    //Logger::instance().log(msg);
}

/**
 * Constructor
 * Sets widget and custom connections
 */
CGPWindow::CGPWindow(QMainWindow *parent) :
        QMainWindow(parent) {
    setupUi(this);
    saved = true;
    qInstallMsgHandler(myMessageOutput);
    QDebugStream* qout = new QDebugStream(std::cout,
                                          Logger::instance().getWindow());

    runInfo.setGPInfo(Individuals->value(), Generations->value(),
                      Nodes->value(), Mutation->value(), Crossover->value(),
                      pointProb->value(), Back->value(), treeCheck->isChecked());

    setupTable();
    setupGrapher();

    availableFunctions.clear();
    chosenFunctions.clear();
    qRegisterMetaType<IndividualInformation> ("IndividualInformation");
    connect(&thread,
            SIGNAL(update(int,double,double,double,IndividualInformation)),
            this, SLOT(update(int,double,double,double,IndividualInformation)));
    connect(&thread, SIGNAL(finished()), this, SLOT(finished()));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    //connect(actionLoad_Problem, SIGNAL(triggered()), this, SLOT(loadDll()));
    connect(actionBatch_Run, SIGNAL(triggered()), this, SLOT(batchRun()));
    //connect(action_close_debug,SIGNAL(triggered()))
    connect(menuProblems, SIGNAL(triggered(QAction*)), this,
            SLOT(selectProblem(QAction*)));

    SRProblem* p = new SRProblem();
    problems[p->getName()] = p;
    EvenParity* e = new EvenParity();
    problems[e->getName()] = e;

    std::map<std::string, Problem*>::iterator pit;

    for (pit = problems.begin(); pit != problems.end(); pit++) {
        addMenuProblem((*pit).first);
    }

    selectedProblem = 0;
    batchNumber = -1;
    batchFile = "";
    qDebug() << "finished setup";
    gViewer = 0;
    textbox = 0;
    batch_progressbar->setVisible(false);
    batch_label->setVisible(false);

}

CGPWindow::~CGPWindow() {
    qDebug() << "Deleting myself";
    std::map<std::string, Problem*>::iterator pit;
    qDebug() << "deleting " << problems.size() << " problems";
    for (pit = problems.begin(); pit != problems.end(); pit++) {
        qDebug() << "deleting problem '" << (*pit).first.c_str() << "'";
        delete (*pit).second;
    }

}

/**
 * Called when the window is asked to exit.
 * Stores the settings.
 */
void CGPWindow::closeEvent(QCloseEvent *event) {
    if (!QMessageBox::question(this, tr("Warning"), tr(
            "Do you really want to exit?"), tr("&Yes"), tr("&No"), QString(),
                               0, 1)) {
        WriteSettings();
        if (gViewer) {
            gViewer->close();
            delete gViewer;
        }
        if (textbox) {
            textbox->close();
            delete textbox;
        }
        thread.stop();
        while(thread.isRunning()){
        }

        Logger::instance().close();
        event->accept();
    } else {
        event->ignore();
    }
}
/**
 * Sets up the datatable showing key results for each generation
 */
void CGPWindow::setupTable() {
    DataTable->clear();
    DataTable->setRowCount(1);
    DataTable->setColumnCount(4);

    //setup horizontal headers
    QHeaderView* header = new QHeaderView(Qt::Horizontal, DataTable);
    header->setResizeMode(QHeaderView::Stretch);
    DataTable->setHorizontalHeader(header);

    //setup vertical header
    QHeaderView* vHeader = new QHeaderView(Qt::Vertical, DataTable);
    vHeader->hide();
    DataTable->setVerticalHeader(vHeader);

    //setup horizontal header items
    QTableWidgetItem *gensHeader = new QTableWidgetItem(tr("Gens"));
    QTableWidgetItem *worstHeader = new QTableWidgetItem(tr("Worst"));
    QTableWidgetItem *avgHeader = new QTableWidgetItem(tr("Average"));
    QTableWidgetItem *bestHeader = new QTableWidgetItem(tr("Best"));
    DataTable->setHorizontalHeaderItem(0, gensHeader);
    DataTable->setHorizontalHeaderItem(1, bestHeader);
    DataTable->setHorizontalHeaderItem(2, worstHeader);
    DataTable->setHorizontalHeaderItem(3, avgHeader);
    header->show();

    //enable table
    DataTable->setEnabled(true);

}
/**
 * Sets up the graph frame showing progress over generations.
 */
void CGPWindow::setupGrapher() {
    //frame = new DataGrapher(t);
    frame->init(0, 0);
    connect(checkBest, SIGNAL(toggled(bool)), frame, SLOT(drawBest(bool)));
    connect(checkWorst, SIGNAL(toggled(bool)), frame, SLOT(drawWorst(bool)));
    connect(checkAverage, SIGNAL(toggled(bool)), frame, SLOT(drawAverage(bool)));
}
/**
 Sets the appropriate selection methods
 */
void CGPWindow::on_selectionMethod_activated(int sel) {
    //if tournament selection is chosen set the size and label active
    this->tournamentSize->setEnabled((sel == 0));
    this->tournamentLabel->setEnabled((sel == 0));
}

/**
 * slots for signal emitted from calculating thread.
 * Updates appropriate displays with info.
 */
void CGPWindow::update(int gens, double best, double worst, double avg,
                       IndividualInformation indInfo) {
    GenerationNumber->display(gens);
    AvgFitness->display(avg);
    BestFitness->display(best);
    WorstFitness->display(worst);
    progressBar->setValue(gens);
    frame->setValues(gens, best, avg, worst);
    frame->repaint();

    QTableWidgetItem *gensItem = new QTableWidgetItem(tr("%1").arg(gens));
    QTableWidgetItem *bestItem = new QTableWidgetItem(tr("%1").arg(best));
    QTableWidgetItem *worstItem = new QTableWidgetItem(tr("%1").arg(worst));
    QTableWidgetItem *avgItem = new QTableWidgetItem(tr("%1").arg(avg));

    DataTable->insertRow(gens + 1);
    DataTable->setItem(gens, 0, gensItem);
    DataTable->setItem(gens, 1, bestItem);
    DataTable->setItem(gens, 2, worstItem);
    DataTable->setItem(gens, 3, avgItem);
    DataTable->scrollToItem(gensItem);
    std::vector<int> activeNodes = indInfo.m_ind.getActiveNodes();
    std::map<std::string,int> functions;
    QString active = "";
    for(size_t i =0; i < activeNodes.size();i++){
        int node = activeNodes.at(i);
        active.append(QString("n%1; ").arg(node));
        std::string functionName = indInfo.m_ind.getGenes().at(node).getFunction()->name();
        if(functions.find(functionName) == functions.end()){
            functions[functionName] = 1;
        }else{
            functions[functionName]++;
        }
    }

    std::map<std::string,int>::iterator it = functions.begin();
    QString functionUse = "";
    for(;it != functions.end();it++){
        //qDebug() << it->first.c_str() << " = " << it->second;
        QString s = QString("%1=%2; ").arg(it->first.c_str()).arg(it->second);
        functionUse.append(s);
    }
    runInfo.updateData(gens, best, avg, worst, indInfo.m_ind.toMatlabCode(runInfo.problem->NumberOfInputs()),active,functionUse);

    //bestIndividual->setText(bestInd);
    if (runInfo.gp.Grid) {
        gViewer->updateIndividual(indInfo.m_ind);
    }

    if (runInfo.problem->NumberOfOutputs() == 1) {
        m_solution_grapher->setVisible(true);
        TwoDArray<double> graphData = indInfo.m_answers;
        graphData.horzCat(indInfo.m_outputs);
        graphData.inverse();
        m_solution_grapher->setValues(graphData);
    }else{
        m_solution_grapher->setVisible(false);
    }
}

/**
 Performs appropriate actions when current run has finished
 */
void CGPWindow::finished() {
    qDebug() << "Run finished";
    StartButton->setEnabled(true);
    //StopButton->setEnabled(false);
    if (!textbox)
        textbox = new QTextEdit(0);
    textbox->append(QString("Fitness %1").arg(
            thread.get_Population()->getBest().getFitness()));
    textbox->append(thread.get_Population()->getBest().toMatlabCode(
            thread.getRunInfo().problem->NumberOfInputs()));
    textbox->show();

    qDebug() << "batch number" << batchNumber;
    if (batchNumber >= 0) {
        while (thread.isRunning())
            ;
        saveData(batchFile);
        execBatch();
        return;
    }
    saved = false;
}
/**
Saves the RunInfo used for the previous Run
*/
void CGPWindow::save() {

    QString fileName;

    QFileDialog* fd =
            new QFileDialog(this, tr("Open Bookmark File"),
                            QApplication::applicationDirPath(), tr(
                                    "XBEL Files (*.xbel *.xml)"));
    fd->setFileMode(QFileDialog::AnyFile);
    fd->setModal(true);
    if (fd->exec() == QDialog::Accepted) {
        fileName = fd->selectedFiles().at(0);
    } else {
        return;
    }

    if (fileName.isEmpty())
        return;

    saveData(fileName);
}

/**
 * Saves any data from last run to file.
 * \param filename file to store the data in
 */
void CGPWindow::saveData(QString filename) {
    QFile file(filename);
    bool exists = file.exists();

    if (!file.open(QIODevice::ReadWrite | QFile::Text)) {
        QMessageBox::warning(this, tr("SAX Bookmarks"), tr(
                "Cannot read file %1:\n%2.") .arg(filename) .arg(
                        file.errorString()));
        return;
    }

    QDomDocument domDocument;
    //tries to load the document
    if (exists) {
        QString errorStr;
        int errorLine;
        int errorColumn;
        qDebug() << "reading old file";
        if (!domDocument.setContent(&file, true, &errorStr, &errorLine,
                                    &errorColumn)) {
            QMessageBox::information(
                    window(),
                    tr("DOM Bookmarks"),
                    tr("Parse error at line %1, column %2:\n%3") .arg(errorLine) .arg(
                            errorColumn) .arg(errorStr));
            return;
        }

        QDomElement root = domDocument.documentElement();
        if (root.tagName() != "xbel") {
            QMessageBox::information(window(), tr("DOM Bookmarks"), tr(
                    "The file is not an XBEL file."));
            return;
        }
        if (file.seek(0)) {
            qDebug() << "seek worked";
        }
    } else {
        QDomElement root = domDocument.createElement("xbel");
        root.setAttribute("version", "1.0");
        domDocument.appendChild(root);
    }

    //QDomDocument domDocument;
    runInfo.store(domDocument);
    QTextStream out(&file);
    domDocument.save(out, 4);
    saved = true;
}
/**
 * Opens and shows saved data.
 * Asks the user to select file name.
 */
void CGPWindow::open() {
    QString errorStr;
    int errorLine;
    int errorColumn;
    QString fileName = QFileDialog::getOpenFileName(this, tr(
            "Open Bookmark File"), QDir::currentPath(), tr(
                    "XBEL Files (*.xbel *.xml)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SAX Bookmarks"), tr(
                "Cannot read file %1:\n%2.") .arg(fileName) .arg(
                        file.errorString()));
        return;
    }

    QDomDocument domDocument;
    //tries to load the document
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information(window(), tr("DOM Bookmarks"), tr(
                "Parse error at line %1, column %2:\n%3") .arg(errorLine) .arg(
                        errorColumn) .arg(errorStr));
        return;
    }

    QDomElement root = domDocument.documentElement();
    if (root.tagName() != "xbel") {
        QMessageBox::information(window(), tr("DOM Bookmarks"), tr(
                "The file is not an XBEL file."));
        return;
    } else if (root.hasAttribute("version") && root.attribute("version")
        != "1.0") {
        QMessageBox::information(window(), tr("DOM Bookmarks"), tr(
                "The file is not an XBEL version 1.0 "
                "file."));
        return;
    }
    infoTree = new RunInfoTree(domDocument);
    infoTree->resize(400, 300);
    infoTree->show();

}

/**
 * Called when the load problem is selected from the menu.
 * Opens file dialog for user to select a plug-in problem
 */
void CGPWindow::on_actionLoad_Problem_triggered() {

    QString errorStr;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open DLL"),
                                                    QDir::currentPath(), tr("Lib Files (*.dll *.so)"));

    this->loadDll(fileName);

}
/**
 Starts a new run.
 Checks if data is saved.
 Initializes thread, stores rin info and starts thread.
 */
void CGPWindow::on_StartButton_clicked() {
    qDebug() << "Check if saving is needed";

    /*
         Check if last run was saved.
         */
    if (!saved
        && !QMessageBox::question(this, tr("Save?"), tr(
                "Last run has not been saved."
                "Do you want save it?"), tr("&Yes"), tr("&No"),
                                  QString(), 0, 1)) {
        save();
    }

    //If no functions are chosen then we output warning and refuse to start.
    if (this->chosenFunctions.size() == 0) {
        QMessageBox::warning(
                this,
                tr("Functions!"),
                tr(
                        "No functions have been chosen.\n  Please choose a set of functions for the problem"));
        return;
    }
    //make sure a problem is selected
    if (this->selectedProblem == 0) {
        QMessageBox::warning(this, tr("No Problem!"), tr(
                "No problem have been selected.\n  Please choose a problem"));
        return;
    }

    qDebug() << "Init the thread";
    double stopV = 0;
    if (stopCheck->isChecked())
        stopV = stopValue->value();

    qDebug() << "Tournament size = " << tournamentSize->value();

    setRunInformation();

    //If old view exists, delete it
    if (gViewer) {
        delete gViewer;
        gViewer = 0;
    }
    //If we are running in grid mode, create a new viewer
    if (runInfo.gp.Grid) {
        qDebug() << " Creating grid viewer";
        gViewer = new GridViewer(0);
        gViewer->setupViewer(Rows->value(), Cols->value(),
                             selectedProblem->NumberOfInputs());
        gViewer->show();

    }

    TwoDArray<double> data = selectedProblem->getAnswers();
    TwoDArray<double> outData(selectedProblem->NumberOfFitnessCases(),
                              selectedProblem->NumberOfOutputs());
    data.horzCat(outData);
    data.inverse();
    m_solution_grapher->initializePlot(data);

    //thread.setQSize(tournamentSize->value());
    qDebug() << " Setting problem = " << selectedProblem->description().c_str();
    thread.init(runInfo, stopV);

    progressBar->reset();
    progressBar->setRange(0, Generations->value());

    //reset the datatable
    qDebug() << "Reset the datatable";
    setupTable();

    //setup the grapher
    frame->init(Generations->value());
    qDebug() << "Start calculations";

    //start the calculation
    thread.calculate();
    StartButton->setEnabled(false);
}

/**
 stops thread and calls finished.
 */
void CGPWindow::on_StopButton_clicked() {
    batchNumber = -1;
    batch_progressbar->setVisible(false);
    batch_label->setVisible(false);
    thread.stop();

}
/**
 Pauses the thread
 */
void CGPWindow::on_PauseButton_clicked() {
    thread.pause();
}
/**
 Restarts the thread if paused
 */
void CGPWindow::on_RestartButton_clicked() {
    thread.restart();
}

/**
 Opens the window for selection of functions
 */
void CGPWindow::on_ChooseFunctions_clicked() {
    std::vector<std::string> all =
            FunctionFactory<double>::instance().getAllFunctions();
    std::vector<std::string>::iterator fit = all.begin();
    for (; fit != all.end(); fit++) {
        Function<double>* f =
                FunctionFactory<double>::instance().create((*fit));
        availableFunctions[(*fit)] = f;
    }
    FunctionSelecter* fsel = new FunctionSelecter(0, availableFunctions,
                                                  chosenFunctions);
    fsel->setWindowModality(Qt::ApplicationModal);
    fsel->exec();
    std::map<std::string, Function<double>*>::iterator it =
            chosenFunctions.begin();
    functionList->clear();
    for (; it != chosenFunctions.end(); it++) {
        new QListWidgetItem(tr((*it).first.c_str()), functionList);
    }
    functionList->sortItems();

}

/**
 * Sends a init command to the currently selected problem
 */
void CGPWindow::on_reInitProblem_clicked() {
    if (this->selectedProblem) {
        this->selectedProblem->Init();
    }
}
/**
 * Called whenever a Problem is selected from the menu.
 * The problem description is set and the Init method of the problem is called.
 */
void CGPWindow::selectProblem(QAction* a) {
    qDebug() << "This problem was selected" << a->text();
    std::map<std::string, Problem*>::iterator it = problems.find(
            a->text().toStdString());
    if (it == problems.end()) {
        qDebug() << "Could not find problem in the problem list";
    } else {
        qDebug() << "New problem has been selected";
        selectedProblem = it->second;
        selectedProblem->Init();

        problemDescription->setPlainText(it->second->description().c_str());
        qDebug() << "Description has been set.";

    }
}

/**
 * Adds an entry in the list of problems (from the menu)
 * \param name	name of the problem to be added
 */
void CGPWindow::addMenuProblem(std::string name) {
    qDebug() << "adding " << name.c_str() << " to menu";
    menuProblems->addAction(name.c_str());
}

/**
 * Runs with the current params
 */
void CGPWindow::batchRun() {
    //open dialog to select filename to store data in
    QString fileName;

    QFileDialog* fd = new QFileDialog(this, tr("Open Bookmark File"),
                                      QDir::currentPath(), tr("XBEL Files (*.xbel *.xml)"));
    fd->setFileMode(QFileDialog::AnyFile);
    fd->setModal(true);
    if (fd->exec() == QDialog::Accepted) {
        fileName = fd->selectedFiles().at(0);
    } else {
        return;
    }

    if (fileName.isEmpty())
        return;
    batchFile = fileName;

    batchNumber = QInputDialog::getInteger(this, tr("Please provide input"),
                                           ("Number of runs"), 100);
    batch_progressbar->setRange(0, batchNumber);
    batch_progressbar->setVisible(true);
    batch_label->setVisible(true);
    execBatch();
}

/**
 * Executes a new bactch run.
 */
void CGPWindow::execBatch() {
    batch_progressbar->setValue(batch_progressbar->maximum() - batchNumber);
    batchNumber--;
    if (batchNumber >= 0)
        this->on_StartButton_clicked();
}

/**
 * Writes the settings to the settings file
 */
void CGPWindow::WriteSettings() {
    QSettings settings("NoName", "CGP");
    settings.beginGroup("MainWindow - Layout");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("splitterSizes", splitter->saveState());
    settings.setValue("splitterSizes2", splitter_2->saveState());
    settings.setValue("splitterSizes3", splitter_3->saveState());
    settings.setValue("splitterSizes4", m_two_graph_splitter->saveState());

    settings.setValue("graph_splitter", graph_splitter->saveState());
    settings.setValue("table_splitter", table_splitter->saveState());

    settings.endGroup();
    settings.beginGroup("MainWindow - CGP");

    settings.setValue("Individuals", Individuals->value());
    settings.setValue("Generations", Generations->value());
    settings.setValue("Nodes", Nodes->value());
    settings.setValue("Mutation", Mutation->value());
    settings.setValue("Crossover", Crossover->value());
    settings.setValue("pointProb", pointProb->value());
    settings.setValue("Back", Back->value());
    settings.setValue("TreeCross", treeCheck->isChecked());
    settings.setValue("options_index", options->currentIndex());
    settings.setValue("CGP_type", CGP_type->currentIndex());
    settings.setValue("PopulationType", PopulationType->currentIndex());
    settings.setValue("Elite", Elitism->isChecked());
    settings.endGroup();

}
/**
 * Reads the settings from the settings file
 */
void CGPWindow::ReadSettings() {
    QSettings settings("NoName", "CGP");
    settings.beginGroup("MainWindow - Layout");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    splitter->restoreState(settings.value("splitterSizes").toByteArray());
    splitter_2->restoreState(settings.value("splitterSizes2").toByteArray());
    splitter_3->restoreState(settings.value("splitterSizes3").toByteArray());
    m_two_graph_splitter->restoreState(
            settings.value("splitterSizes4").toByteArray());
    graph_splitter->restoreState(settings.value("graph_splitter").toByteArray());
    table_splitter->restoreState(settings.value("table_splitter").toByteArray());

    settings.endGroup();
    settings.beginGroup("MainWindow - CGP");
    Individuals->setValue(settings.value("Individuals").toInt());
    Generations->setValue(settings.value("Generations").toInt());
    Mutation->setValue(settings.value("Mutation").toInt());
    Crossover->setValue(settings.value("Crossover").toInt());
    pointProb->setValue(settings.value("pointProb").toInt());
    Back->setValue(settings.value("Back").toInt());
    treeCheck->setChecked(settings.value("TreeCross").toBool());
    options->setCurrentIndex(settings.value("options_index").toInt());
    CGP_type->setCurrentIndex(settings.value("CGP_type").toInt());
    PopulationType->setCurrentIndex(settings.value("PopulationType").toInt());
    Elitism->setChecked(settings.value("Elite").toBool());
    settings.endGroup();
}

/**
 * This fills in the RunInfo structure with the correct infomration
 */
void CGPWindow::setRunInformation() {
    runInfo.reset();
    runInfo.setGridInfo(CGP_type->currentIndex(), Rows->value(), Cols->value(),
                        Back->value());
    int nodes;
    if (runInfo.gp.Grid) {
        nodes = (Rows->value() * Cols->value());
    } else {
        nodes = Nodes->value();
    }
    runInfo.setGPInfo(Individuals->value(), Generations->value(), nodes,
                      Mutation->value(), Crossover->value(), pointProb->value(),
                      Back->value(), treeCheck->isChecked());
    runInfo.setSelectionInfo(Elitism->isChecked(),
                             this->selectionMethod->currentIndex());
    runInfo.functions = chosenFunctions;
    runInfo.gp.HFC = PopulationType->currentIndex();
    runInfo.problem = this->selectedProblem;
    runInfo.sel.QTournamentSize = tournamentSize->value();

}

/**
 * Toggles whether the debug window should be shown or not
 */
void CGPWindow::on_actionDebugWindow_toggled(bool checked) {
    if (checked) {
        Logger::instance().show();
    } else {
        Logger::instance().hide();
    }
}

/**
 * Loads all plugins in ../plugins directory
 */
void CGPWindow::loadPlugins() {
    QDir pluginDir(QApplication::applicationDirPath());
    if (!pluginDir.cd("../plugins"))
        return;
    foreach(QString fileName, pluginDir.entryList(QDir::Files))
    {
        this->loadDll(pluginDir.absoluteFilePath(fileName));
    }
}

/**
 * Loads a plugin given by filename
 * \fileName name of the plugin file
 */
void CGPWindow::loadDll(QString fileName) {

    if (fileName.isEmpty())
        return;
    qDebug() << "Attempting to load file: " << fileName;
    #ifdef WIN32
    if (!fileName.contains(".dll")) {
        HERE_T(0,"Plugin is not a dll!");
        return;
    }
    #else
    if (!fileName.contains(".so")) {
        HERE_T(0,"Plugin is not a so file!");
        return;
    }
    #endif
    QPluginLoader loader(fileName);
    QObject* plugin = loader.instance();
    if (plugin) {
        Problem *interface = dynamic_cast<Problem *> (plugin);
        if (interface) {
            qDebug() << "plugin ok";
            problems[interface->getName()] = interface;
            addMenuProblem(interface->getName());
        } else {
            qDebug() << "not a proper plugin";
        }
    } else {
        qDebug() << loader.errorString();
    }
}
