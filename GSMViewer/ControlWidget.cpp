#include <road/GraphUtil.h>
#include "ControlWidget.h"
#include "MainWindow.h"
#include "GLWidget.h"
#include "RoadGraphEditor.h"

ControlWidget::ControlWidget(MainWindow* mainWin) : QDockWidget("Control Widget", (QWidget*)mainWin) {
	this->mainWin = mainWin;

	// set up the UI
	ui.setupUi(this);
	ui.checkBoxShowHighway->setChecked(true);
	ui.checkBoxShowBoulevard->setChecked(true);
	ui.checkBoxShowAvenue->setChecked(true);
	ui.checkBoxShowLocalStreet->setChecked(true);
	ui.lineEditSimplifyThreshold->setText("10");
	ui.lineEditRemoveShortDeadendThreshold->setText("10");

	// register the event handlers
	connect(ui.checkBoxShowHighway, SIGNAL(stateChanged(int)), this, SLOT(showRoad(int)));
	connect(ui.checkBoxShowBoulevard, SIGNAL(stateChanged(int)), this, SLOT(showRoad(int)));
	connect(ui.checkBoxShowAvenue, SIGNAL(stateChanged(int)), this, SLOT(showRoad(int)));
	connect(ui.checkBoxShowLocalStreet, SIGNAL(stateChanged(int)), this, SLOT(showRoad(int)));
	connect(ui.pushButtonClean, SIGNAL(clicked()), this, SLOT(clean()));
	connect(ui.pushButtonSimplify, SIGNAL(clicked()), this, SLOT(simplify()));
	connect(ui.pushButtonReduce, SIGNAL(clicked()), this, SLOT(reduce()));
	connect(ui.pushButtonRemoveShortDeadend, SIGNAL(clicked()), this, SLOT(removeShortDeadend()));

	hide();
}

void ControlWidget::setRoadVertex(RoadVertexDesc vertexDesc, RoadVertexPtr selectedVertex) {
	QString desc("");
	QString location("");
	QString neighbors("");

	if (selectedVertex != NULL) {
		desc.setNum(vertexDesc);

		location = QString("(%1, %2)").arg(selectedVertex->pt.x(), 0, 'f', 0).arg(selectedVertex->pt.y(), 0, 'f', 0);

		std::vector<RoadVertexDesc> n = GraphUtil::getNeighbors(mainWin->glWidget->editor->roads, vertexDesc);
		for (int i = 0; i < n.size(); i++) {
			QString str;
			str.setNum(n[i]);

			neighbors += str;
			if (i < n.size() - 1) neighbors += ",";
		}
	}

	ui.lineEditVertexDesc->setText(desc);
	ui.lineEditVertexPos->setText(location);
	ui.lineEditVertexNeighbors->setText(neighbors);
}

/**
 * Display the selected edge information.
 */
void ControlWidget::setRoadEdge(RoadEdgePtr selectedEdge) {
	QString type;
	QString numLanes;
	QString oneWay;
	QString link;
	QString roundabout;

	if (selectedEdge != NULL) {
		switch (selectedEdge->type) {
		case RoadEdge::TYPE_HIGHWAY:
			type = "Highway";
			break;
		case RoadEdge::TYPE_BOULEVARD:
			type = "Boulevard";
			break;
		case RoadEdge::TYPE_AVENUE:
			type = "Avenue";
			break;
		case RoadEdge::TYPE_STREET:
			type = "Street";
			break;
		default:
			type = "";
			break;
		}

		numLanes.setNum(selectedEdge->lanes);

		oneWay = selectedEdge->oneWay ? "Yes" : "No";
		link = selectedEdge->link ? "Yes" : "No";
		roundabout = selectedEdge->roundabout ? "Yes" : "No";
	}

	ui.lineEditEdgeType->setText(type);
	ui.lineEditEdgeLanes->setText(numLanes);
	ui.lineEditEdgeOneWay->setText(oneWay);
	ui.lineEditEdgeLink->setText(link);
	ui.lineEditEdgeRoundabout->setText(roundabout);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event handlers

void ControlWidget::showRoad(int flag) {
	mainWin->glWidget->editor->roads.showHighways = ui.checkBoxShowHighway->isChecked();
	mainWin->glWidget->editor->roads.showBoulevards = ui.checkBoxShowBoulevard->isChecked();
	mainWin->glWidget->editor->roads.showAvenues = ui.checkBoxShowAvenue->isChecked();
	mainWin->glWidget->editor->roads.showLocalStreets = ui.checkBoxShowLocalStreet->isChecked();
	mainWin->glWidget->editor->roads.setModified();

	mainWin->glWidget->updateGL();
}

/**
 * Event handler for button [Clean]
 */
void ControlWidget::clean() {
	GraphUtil::clean(mainWin->glWidget->editor->roads);

	mainWin->glWidget->updateGL();
}

/**
 * Event handler for button [Simplify]
 */
void ControlWidget::simplify() {
	float threshold = ui.lineEditSimplifyThreshold->text().toFloat();
	mainWin->glWidget->editor->simplify(threshold);

	mainWin->glWidget->updateGL();
}

/**
 * Event handler for button [Reduce]
 */
void ControlWidget::reduce() {
	mainWin->glWidget->editor->reduce();

	mainWin->glWidget->updateGL();
}

/**
 * Event handler for button [Remove Short Dead-End]
 */
void ControlWidget::removeShortDeadend() {
	float threshold = ui.lineEditRemoveShortDeadendThreshold->text().toFloat();
	mainWin->glWidget->editor->removeShortDeadend(threshold);

	mainWin->glWidget->updateGL();
}
