#include "RunInfo.h"
void RunInfo::GPParameters::store(QDomElement parent, QDomDocument doc) {
	QDomElement ind = doc.createElement("Individuals");
	ind.setAttribute("value", Individuals);
	QDomElement gen = doc.createElement("Generations");
	gen.setAttribute("value", Generations);
	QDomElement mut = doc.createElement("MutationProbability");
	mut.setAttribute("value", MutationProbability);
	QDomElement cro = doc.createElement("CrossoverProbability");
	cro.setAttribute("value", CrossoverProbability);
	QDomElement poi = doc.createElement("PointMutationProbability");
	poi.setAttribute("value", PointMutationProbability);
	QDomElement nod = doc.createElement("Nodes");
	nod.setAttribute("value", Nodes);
	QDomElement tree = doc.createElement("TreeCrossover");
	tree.setAttribute("value", treeCrossover);
	QDomElement con = doc.createElement("Backward-connections");
	con.setAttribute("value", Connections);

	parent.appendChild(ind);
	parent.appendChild(gen);
	parent.appendChild(mut);
	parent.appendChild(cro);
	parent.appendChild(poi);
	parent.appendChild(nod);
	parent.appendChild(tree);
	parent.appendChild(con);

}
void RunInfo::RunData::store(QDomElement parent, QDomDocument doc) {
	for (int i = 0; i < CompletedGenerations; i++) {
		qDebug() << "Storing for generation" << i;
		QDomElement gen = doc.createElement("Generation");
		gen.setAttribute("value", i);
		QDomElement bestInd = doc.createElement("BestInd");
		bestInd.setAttribute("value", BestInds.at(i).replace(QString("\n"),
				QString(" ")));
		QDomElement activeInd = doc.createElement("ActiveNodes");
		activeInd.setAttribute("value", ActiveNodes.at(i).replace(
				QString("\n"), QString(" ")));
		QDomElement best = doc.createElement("Best");
		best.setAttribute("value", Stats[i][0]);
		QDomElement avg = doc.createElement("Avg");
		avg.setAttribute("value", Stats[i][1]);
		QDomElement worst = doc.createElement("Worst");
		worst.setAttribute("value", Stats[i][2]);
		gen.appendChild(bestInd);
		gen.appendChild(activeInd);
		gen.appendChild(best);
		gen.appendChild(avg);
		gen.appendChild(worst);
		parent.appendChild(gen);
	}
	BestInds.clear();
}

RunInfo::RunInfo() {

}

void RunInfo::store(QDomDocument& doc) {
	QDomElement root = doc.documentElement();

	QDomElement ind = doc.createElement("BestInd");
	ind.setAttribute("value", this->bestInd);
	QDomElement nodes = doc.createElement("ActiveNodes");
	nodes.setAttribute("value", this->activeNodes);
	QDomElement fUsage = doc.createElement("FunctionUsage");
	fUsage.setAttribute("value", this->m_functionUsage);

	//store the GP parameters
	QDomElement gp = doc.createElement("GPParameters");
	this->gp.store(gp, doc);

	//store the selection parameters
	QDomElement sel = doc.createElement("SelectionParameters");
	sel.setAttribute("Elitism", (this->sel.Elitism ? "TRUE" : "FALSE"));
	sel.setAttribute("SelectionType", this->sel.Selection);

	//store the rd paramters
	QDomElement rd = doc.createElement("RunData");
	//rd.setAttribute("CompletedGenerations", this->rd.CompletedGenerations);
	this->rd.store(rd, doc);
	//setup the model
	QDomElement ri = doc.createElement("RunInfo");
	ri.appendChild(ind);
	ri.appendChild(nodes);
	ri.appendChild(fUsage);
	ri.appendChild(gp);
	ri.appendChild(sel);
	ri.appendChild(rd);
	root.appendChild(ri);

}

void RunInfo::setGPInfo(int ind, int gen, int nodes, int mut, int cross,
		int point, int back, bool tree) {
	gp.Individuals = ind;
	gp.Generations = gen;
	gp.MutationProbability = mut;
	gp.CrossoverProbability = cross;
	gp.PointMutationProbability = point;
	gp.Nodes = nodes;
	gp.Connections = back;
	gp.treeCrossover = tree;
	qDebug() << "resize-ing stats";
	rd.Stats.resize(gen + 1, 3);
}
;

void RunInfo::setGridInfo(bool grid, int Rows, int Cols, int cons) {
	gp.Grid = grid;
	gp.Cols = Cols;
	gp.Rows = Rows;
	gp.Connections = cons;
}

void RunInfo::setSelectionInfo(bool elitism, int selType) {
	sel.Elitism = elitism;
	sel.Selection = selType;
}
;

void RunInfo::setRunData(int generations) {
	rd.CompletedGenerations = generations;

}
void RunInfo::updateData(int gen, double best, double avg, double worst,
		QString bestInd, QString activeNodes, QString functionUsage) {
	rd.CompletedGenerations++;
	rd.Stats[gen][0] = best;
	rd.Stats[gen][1] = avg;
	rd.Stats[gen][2] = worst;
	//HERE_T(0,"Got updates " << bestInd << " == " << activeNodes);
	rd.BestInds.push_back(bestInd);
	rd.ActiveNodes.push_back(activeNodes);
	this->bestInd = bestInd;
	this->m_functionUsage = functionUsage;
	this->bestInd.replace(QString("\n"), QString(" , "));
	this->activeNodes = activeNodes;
}

void RunInfo::reset() {
	qDebug() << "setting compgen = 0";
	rd.CompletedGenerations = 0;
	rd.BestInds.clear();
	rd.ActiveNodes.clear();

}
