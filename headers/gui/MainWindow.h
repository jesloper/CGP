#ifndef CGPWINDOW
#define CGPWINDOW

/**
 * \mainpage CGP-QT
 *
 *
 * This program provides an implementation of CGP using QT as a framework for displaying progress, managing settings and driving the CGP.
 * 
 * \version 0.9
 * first version
 *
 * \author john erik sloper
 *
 * \par license
 * This code is absolutely free to use and modify. The code is provided "as is" with
 * no expressed or implied warranty. The author accepts no liability if it causes
 * any damage to your computer, causes your pet to fall ill, increases baldness
 * or makes your car start emitting strange noises when you start it up.
 * This code has no bugs, just undocumented features!
 * 
 */
#include "ui_cgpWindow.h"
#include "CompThread.h"
//#include "StoreThread.h"
#include "RunInfo.h"
#include "RunInfoTree.h"
#include "GridViewer.h"
#include "Individual.h"
#include "Function.h"
#include <QtXml/QDomDocument>

class DataGrapher;
class Problem;
class GenerationPlot;
/**
 * \brief Main window.
 * This is the main window seen when running the program. 
 * It contains all the information necessary to set-up, run and store results from a CGP experiment.
 */
class CGPWindow : public QMainWindow, private Ui::MainWindow
    {
        Q_OBJECT

    public:
        CGPWindow(QMainWindow *parent = 0); ///< \brief ctor
		virtual ~CGPWindow();				///< \brief dtor
		void ReadSettings(); 					///< \brief loads any stored settings
		void loadPlugins(); 					///< \brief loads all available plugins from the plugin directory
			  		  	
    protected:
		void closeEvent(QCloseEvent* event);
	private:
		CompThread thread;		///< \brief Thread to drive the CGP
		//StoreThread m_storeThread; ///< \brief Thread to store the run information to file
		RunInfo runInfo;		///< \brief Current run information
		bool saved;				///< \brief flag indicating whether last run was saved or not
		RunInfoTree* infoTree;	///< \brief Used to browse information from previous runs
		DataGrapher* win;		///< \brief Graphs the progress of the CGP
		GenerationPlot* plot;
		GridViewer* gViewer;	///<\brief  Shows the CGP in graphical format
		void setupTable();		///< \brief Sets up the table
		void setupGrapher();	///< \brief sets up the grapher
		std::map<std::string,Function<double>*> availableFunctions; ///<\brief  Functions available
		std::map<std::string,Function<double>*> chosenFunctions;	///< \brief Functions currently chosen
		std::map<std::string,Problem*> problems;	///< \brief Available problems
		Problem* selectedProblem;					///< \brief Currently selected problem
		void addMenuProblem(std::string);			///< \brief Adds a problem to the menu	
		void startRun();		///< \brief Starts a new run
		void execBatch();		///< \brief executes a new batch run
		int batchNumber;		///< \brief Current batch number
		QString batchFile;		///< \brief File to store the batch results in
		QTextEdit* textbox;		///< \brief Used to show different results (code in matlab syntax, etc)
    private slots:
        void on_selectionMethod_activated(int);
	  void update(int,double,double,double,IndividualInformation);
	  void finished(); 
	  void on_StartButton_clicked();		///< \brief starts a new run
	  void on_StopButton_clicked(); 		///< \brief stops the current run
	  void on_PauseButton_clicked(); 	///< \brief called when the pause button is pressed. Pauses the current run
	  void on_RestartButton_clicked(); 	///< \brief called when the restart button is pressed
	  void on_ChooseFunctions_clicked(); ///< \brief called when the choose function button is pressed
	  void on_reInitProblem_clicked(); 	/// \brief called when the reinit button is pressed
	  void save(); 							///< \brief saves the current data
	  void saveData(QString filename);	 ///< \brief saves current data to file
	  void open();  							///< \brief opens a saved file
	  void selectProblem(QAction*); 		///< \brief called when a problem is selected
	  void on_actionLoad_Problem_triggered(); ///< \brief called then the load problem is selected from the menu
	  void batchRun();						///< \brief initiates a new batch run
	  void WriteSettings();					///< \brief writes setting to user configuration file
	  void setRunInformation(); 			///< \brief fills in the RunInfo structure
	  void on_actionDebugWindow_toggled(bool checked); ///< \brief called when the debug window is toggled
	  void loadDll(QString filename); 		///< \brief loads a given dll file
	  
    };
#endif
