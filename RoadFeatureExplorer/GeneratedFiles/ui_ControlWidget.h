/********************************************************************************
** Form generated from reading UI file 'ControlWidget.ui'
**
** Created: Fri Mar 28 09:42:23 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLWIDGET_H
#define UI_CONTROLWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDockWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlWidget
{
public:
    QWidget *dockWidgetContents;
    QCheckBox *checkBoxRoadTypeAvenue;
    QCheckBox *checkBoxRoadTypeLocalStreet;
    QGroupBox *groupBox_9;
    QPushButton *pushButtonExtractKDEFeature;
    QCheckBox *checkBoxPerturbation;
    QCheckBox *checkBoxRotation;
    QCheckBox *checkBoxExactCut;
    QCheckBox *checkBoxRoadTypeHighway;
    QCheckBox *checkBoxRoadTypeBoulevard;
    QGroupBox *groupBox;
    QPushButton *pushButtonExtractGenericFeature;

    void setupUi(QDockWidget *ControlWidget)
    {
        if (ControlWidget->objectName().isEmpty())
            ControlWidget->setObjectName(QString::fromUtf8("ControlWidget"));
        ControlWidget->resize(218, 888);
        ControlWidget->setMinimumSize(QSize(218, 240));
        ControlWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        checkBoxRoadTypeAvenue = new QCheckBox(dockWidgetContents);
        checkBoxRoadTypeAvenue->setObjectName(QString::fromUtf8("checkBoxRoadTypeAvenue"));
        checkBoxRoadTypeAvenue->setGeometry(QRect(20, 40, 70, 17));
        checkBoxRoadTypeLocalStreet = new QCheckBox(dockWidgetContents);
        checkBoxRoadTypeLocalStreet->setObjectName(QString::fromUtf8("checkBoxRoadTypeLocalStreet"));
        checkBoxRoadTypeLocalStreet->setGeometry(QRect(100, 40, 91, 17));
        groupBox_9 = new QGroupBox(dockWidgetContents);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        groupBox_9->setGeometry(QRect(10, 90, 201, 141));
        pushButtonExtractKDEFeature = new QPushButton(groupBox_9);
        pushButtonExtractKDEFeature->setObjectName(QString::fromUtf8("pushButtonExtractKDEFeature"));
        pushButtonExtractKDEFeature->setGeometry(QRect(30, 100, 141, 31));
        checkBoxPerturbation = new QCheckBox(groupBox_9);
        checkBoxPerturbation->setObjectName(QString::fromUtf8("checkBoxPerturbation"));
        checkBoxPerturbation->setGeometry(QRect(20, 50, 101, 17));
        checkBoxRotation = new QCheckBox(groupBox_9);
        checkBoxRotation->setObjectName(QString::fromUtf8("checkBoxRotation"));
        checkBoxRotation->setGeometry(QRect(20, 70, 101, 17));
        checkBoxExactCut = new QCheckBox(groupBox_9);
        checkBoxExactCut->setObjectName(QString::fromUtf8("checkBoxExactCut"));
        checkBoxExactCut->setGeometry(QRect(20, 30, 101, 17));
        checkBoxRoadTypeHighway = new QCheckBox(dockWidgetContents);
        checkBoxRoadTypeHighway->setObjectName(QString::fromUtf8("checkBoxRoadTypeHighway"));
        checkBoxRoadTypeHighway->setGeometry(QRect(20, 10, 70, 17));
        checkBoxRoadTypeBoulevard = new QCheckBox(dockWidgetContents);
        checkBoxRoadTypeBoulevard->setObjectName(QString::fromUtf8("checkBoxRoadTypeBoulevard"));
        checkBoxRoadTypeBoulevard->setGeometry(QRect(100, 10, 70, 17));
        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 250, 201, 71));
        pushButtonExtractGenericFeature = new QPushButton(groupBox);
        pushButtonExtractGenericFeature->setObjectName(QString::fromUtf8("pushButtonExtractGenericFeature"));
        pushButtonExtractGenericFeature->setGeometry(QRect(30, 30, 141, 31));
        ControlWidget->setWidget(dockWidgetContents);

        retranslateUi(ControlWidget);

        QMetaObject::connectSlotsByName(ControlWidget);
    } // setupUi

    void retranslateUi(QDockWidget *ControlWidget)
    {
        checkBoxRoadTypeAvenue->setText(QApplication::translate("ControlWidget", "Avenues", 0, QApplication::UnicodeUTF8));
        checkBoxRoadTypeLocalStreet->setText(QApplication::translate("ControlWidget", "Local Streets", 0, QApplication::UnicodeUTF8));
        groupBox_9->setTitle(QApplication::translate("ControlWidget", "Extract KDE Feature", 0, QApplication::UnicodeUTF8));
        pushButtonExtractKDEFeature->setText(QApplication::translate("ControlWidget", "Extract KDE Feature", 0, QApplication::UnicodeUTF8));
        checkBoxPerturbation->setText(QApplication::translate("ControlWidget", "Perturbation", 0, QApplication::UnicodeUTF8));
        checkBoxRotation->setText(QApplication::translate("ControlWidget", "Rotation", 0, QApplication::UnicodeUTF8));
        checkBoxExactCut->setText(QApplication::translate("ControlWidget", "Exact Cut", 0, QApplication::UnicodeUTF8));
        checkBoxRoadTypeHighway->setText(QApplication::translate("ControlWidget", "Highways", 0, QApplication::UnicodeUTF8));
        checkBoxRoadTypeBoulevard->setText(QApplication::translate("ControlWidget", "Boulevard", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ControlWidget", "Generic Feature", 0, QApplication::UnicodeUTF8));
        pushButtonExtractGenericFeature->setText(QApplication::translate("ControlWidget", "Extract Generic Feature", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ControlWidget);
    } // retranslateUi

};

namespace Ui {
    class ControlWidget: public Ui_ControlWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLWIDGET_H
