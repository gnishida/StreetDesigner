/********************************************************************************
** Form generated from reading UI file 'PropertyWidget.ui'
**
** Created: Thu Apr 10 16:45:15 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROPERTYWIDGET_H
#define UI_PROPERTYWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PropertyWidget
{
public:
    QWidget *dockWidgetContents;
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *lineEditEdgeType;
    QLabel *label_2;
    QLineEdit *lineEditEdgeLanes;
    QLabel *label_3;
    QLineEdit *lineEditEdgeOneWay;
    QLineEdit *lineEditEdgeLink;
    QLabel *label_9;
    QLabel *label_10;
    QLineEdit *lineEditEdgeRoundabout;
    QGroupBox *groupBox_5;
    QLabel *label_6;
    QLineEdit *lineEditVertexDesc;
    QLabel *label_7;
    QLineEdit *lineEditVertexPos;
    QLabel *label_8;
    QTextEdit *textEditVertexNeighbors;
    QLabel *label_11;
    QLineEdit *lineEditKernel;
    QLabel *label_12;
    QLabel *label_13;
    QLineEdit *lineEditVertexParent;
    QTextEdit *textEditVertexUncles;
    QLabel *label_14;
    QLineEdit *lineEditVertexOnBoundary;

    void setupUi(QDockWidget *PropertyWidget)
    {
        if (PropertyWidget->objectName().isEmpty())
            PropertyWidget->setObjectName(QString::fromUtf8("PropertyWidget"));
        PropertyWidget->resize(192, 831);
        PropertyWidget->setMinimumSize(QSize(192, 240));
        PropertyWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 320, 171, 131));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 31, 16));
        lineEditEdgeType = new QLineEdit(groupBox);
        lineEditEdgeType->setObjectName(QString::fromUtf8("lineEditEdgeType"));
        lineEditEdgeType->setGeometry(QRect(80, 20, 81, 20));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 40, 51, 16));
        lineEditEdgeLanes = new QLineEdit(groupBox);
        lineEditEdgeLanes->setObjectName(QString::fromUtf8("lineEditEdgeLanes"));
        lineEditEdgeLanes->setGeometry(QRect(80, 40, 81, 20));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 60, 51, 16));
        lineEditEdgeOneWay = new QLineEdit(groupBox);
        lineEditEdgeOneWay->setObjectName(QString::fromUtf8("lineEditEdgeOneWay"));
        lineEditEdgeOneWay->setGeometry(QRect(80, 60, 81, 20));
        lineEditEdgeLink = new QLineEdit(groupBox);
        lineEditEdgeLink->setObjectName(QString::fromUtf8("lineEditEdgeLink"));
        lineEditEdgeLink->setGeometry(QRect(80, 80, 81, 20));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 80, 51, 16));
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 100, 61, 16));
        lineEditEdgeRoundabout = new QLineEdit(groupBox);
        lineEditEdgeRoundabout->setObjectName(QString::fromUtf8("lineEditEdgeRoundabout"));
        lineEditEdgeRoundabout->setGeometry(QRect(80, 100, 81, 20));
        groupBox_5 = new QGroupBox(dockWidgetContents);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 10, 171, 291));
        label_6 = new QLabel(groupBox_5);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 20, 31, 16));
        lineEditVertexDesc = new QLineEdit(groupBox_5);
        lineEditVertexDesc->setObjectName(QString::fromUtf8("lineEditVertexDesc"));
        lineEditVertexDesc->setGeometry(QRect(80, 20, 81, 20));
        label_7 = new QLabel(groupBox_5);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 40, 51, 16));
        lineEditVertexPos = new QLineEdit(groupBox_5);
        lineEditVertexPos->setObjectName(QString::fromUtf8("lineEditVertexPos"));
        lineEditVertexPos->setGeometry(QRect(80, 40, 81, 20));
        label_8 = new QLabel(groupBox_5);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 190, 51, 16));
        textEditVertexNeighbors = new QTextEdit(groupBox_5);
        textEditVertexNeighbors->setObjectName(QString::fromUtf8("textEditVertexNeighbors"));
        textEditVertexNeighbors->setGeometry(QRect(80, 190, 81, 61));
        label_11 = new QLabel(groupBox_5);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 260, 51, 16));
        lineEditKernel = new QLineEdit(groupBox_5);
        lineEditKernel->setObjectName(QString::fromUtf8("lineEditKernel"));
        lineEditKernel->setGeometry(QRect(80, 260, 81, 20));
        label_12 = new QLabel(groupBox_5);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 60, 51, 16));
        label_13 = new QLabel(groupBox_5);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 120, 51, 16));
        lineEditVertexParent = new QLineEdit(groupBox_5);
        lineEditVertexParent->setObjectName(QString::fromUtf8("lineEditVertexParent"));
        lineEditVertexParent->setGeometry(QRect(80, 60, 81, 20));
        textEditVertexUncles = new QTextEdit(groupBox_5);
        textEditVertexUncles->setObjectName(QString::fromUtf8("textEditVertexUncles"));
        textEditVertexUncles->setGeometry(QRect(80, 120, 81, 61));
        label_14 = new QLabel(groupBox_5);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 80, 71, 16));
        lineEditVertexOnBoundary = new QLineEdit(groupBox_5);
        lineEditVertexOnBoundary->setObjectName(QString::fromUtf8("lineEditVertexOnBoundary"));
        lineEditVertexOnBoundary->setGeometry(QRect(80, 80, 81, 20));
        PropertyWidget->setWidget(dockWidgetContents);

        retranslateUi(PropertyWidget);

        QMetaObject::connectSlotsByName(PropertyWidget);
    } // setupUi

    void retranslateUi(QDockWidget *PropertyWidget)
    {
        groupBox->setTitle(QApplication::translate("PropertyWidget", "Edge", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PropertyWidget", "Type", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("PropertyWidget", "Lanes", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PropertyWidget", "One Way", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("PropertyWidget", "Link", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("PropertyWidget", "Roundabout", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("PropertyWidget", "Vertex", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("PropertyWidget", "ID", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("PropertyWidget", "Location", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("PropertyWidget", "Neighbors", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("PropertyWidget", "Example", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("PropertyWidget", "Parent", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("PropertyWidget", "Uncles", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("PropertyWidget", "On boundary?", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(PropertyWidget);
    } // retranslateUi

};

namespace Ui {
    class PropertyWidget: public Ui_PropertyWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROPERTYWIDGET_H
