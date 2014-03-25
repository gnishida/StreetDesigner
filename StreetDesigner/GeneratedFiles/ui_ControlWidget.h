/********************************************************************************
** Form generated from reading UI file 'ControlWidget.ui'
**
** Created: Tue Mar 25 12:56:55 2014
**      by: Qt User Interface Compiler version 4.8.1
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
#include <QtGui/QDial>
#include <QtGui/QDockWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlWidget
{
public:
    QWidget *dockWidgetContents;
    QCheckBox *checkBoxRoadTypeAvenue;
    QCheckBox *checkBoxRoadTypeLocalStreet;
    QGroupBox *groupBox_8;
    QPushButton *pushButtonGenerateKDE;
    QDial *dialOrientation;
    QLabel *label_2;
    QPushButton *pushButtonPerturb;
    QLineEdit *lineEditPerturbationFactor;
    QLabel *label_6;
    QGroupBox *groupBox;
    QCheckBox *checkBoxAddAvenuesOnBoundary;
    QLabel *label;
    QLineEdit *lineEditNumIterations;
    QCheckBox *checkBoxLocalStreets;
    QCheckBox *checkBoxInvadingCheck;
    QCheckBox *checkBoxSaveSnappingImages;
    QCheckBox *checkBoxRoadTypeHighway;
    QCheckBox *checkBoxRoadTypeBoulevard;
    QGroupBox *groupBox_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *lineEditWeightEdge;
    QLineEdit *lineEditWeightLocation;
    QLineEdit *lineEditWeightRepetition;
    QGroupBox *groupBox_3;
    QRadioButton *radioButtonMultiSeeds;
    QRadioButton *radioButtonOneSeed;
    QCheckBox *checkBoxAreaScaling;
    QCheckBox *checkBoxConnectAvenues;
    QCheckBox *checkBoxCropping;
    QCheckBox *checkBoxSaveConnectingImages;
    QGroupBox *groupBox_4;
    QRadioButton *radioButtonCartesianCoordinate;
    QRadioButton *radioButtonPolarCoordinate;
    QGroupBox *groupBox_5;
    QPushButton *pushButtonConnect;
    QPushButton *pushButtonMerge;
    QPushButton *pushButtonClear;
    QPushButton *pushButtonGeneratePM;
    QButtonGroup *buttonGroup;
    QButtonGroup *buttonGroup_2;

    void setupUi(QDockWidget *ControlWidget)
    {
        if (ControlWidget->objectName().isEmpty())
            ControlWidget->setObjectName(QString::fromUtf8("ControlWidget"));
        ControlWidget->resize(190, 916);
        ControlWidget->setMinimumSize(QSize(190, 240));
        ControlWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        checkBoxRoadTypeAvenue = new QCheckBox(dockWidgetContents);
        checkBoxRoadTypeAvenue->setObjectName(QString::fromUtf8("checkBoxRoadTypeAvenue"));
        checkBoxRoadTypeAvenue->setGeometry(QRect(10, 30, 70, 17));
        checkBoxRoadTypeLocalStreet = new QCheckBox(dockWidgetContents);
        checkBoxRoadTypeLocalStreet->setObjectName(QString::fromUtf8("checkBoxRoadTypeLocalStreet"));
        checkBoxRoadTypeLocalStreet->setGeometry(QRect(90, 30, 91, 17));
        groupBox_8 = new QGroupBox(dockWidgetContents);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        groupBox_8->setGeometry(QRect(10, 550, 171, 221));
        pushButtonGenerateKDE = new QPushButton(groupBox_8);
        pushButtonGenerateKDE->setObjectName(QString::fromUtf8("pushButtonGenerateKDE"));
        pushButtonGenerateKDE->setGeometry(QRect(20, 80, 131, 31));
        dialOrientation = new QDial(groupBox_8);
        dialOrientation->setObjectName(QString::fromUtf8("dialOrientation"));
        dialOrientation->setGeometry(QRect(80, 20, 71, 51));
        dialOrientation->setMaximum(359);
        dialOrientation->setSingleStep(10);
        dialOrientation->setValue(180);
        dialOrientation->setOrientation(Qt::Vertical);
        dialOrientation->setInvertedAppearance(false);
        dialOrientation->setInvertedControls(false);
        dialOrientation->setNotchTarget(3.7);
        dialOrientation->setNotchesVisible(true);
        label_2 = new QLabel(groupBox_8);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 20, 61, 16));
        pushButtonPerturb = new QPushButton(groupBox_8);
        pushButtonPerturb->setObjectName(QString::fromUtf8("pushButtonPerturb"));
        pushButtonPerturb->setGeometry(QRect(20, 140, 131, 31));
        lineEditPerturbationFactor = new QLineEdit(groupBox_8);
        lineEditPerturbationFactor->setObjectName(QString::fromUtf8("lineEditPerturbationFactor"));
        lineEditPerturbationFactor->setGeometry(QRect(120, 120, 41, 20));
        label_6 = new QLabel(groupBox_8);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 120, 101, 16));
        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 60, 171, 141));
        checkBoxAddAvenuesOnBoundary = new QCheckBox(groupBox);
        checkBoxAddAvenuesOnBoundary->setObjectName(QString::fromUtf8("checkBoxAddAvenuesOnBoundary"));
        checkBoxAddAvenuesOnBoundary->setGeometry(QRect(10, 70, 151, 17));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 46, 21));
        lineEditNumIterations = new QLineEdit(groupBox);
        lineEditNumIterations->setObjectName(QString::fromUtf8("lineEditNumIterations"));
        lineEditNumIterations->setGeometry(QRect(70, 20, 91, 20));
        lineEditNumIterations->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        checkBoxLocalStreets = new QCheckBox(groupBox);
        checkBoxLocalStreets->setObjectName(QString::fromUtf8("checkBoxLocalStreets"));
        checkBoxLocalStreets->setGeometry(QRect(10, 90, 131, 17));
        checkBoxInvadingCheck = new QCheckBox(groupBox);
        checkBoxInvadingCheck->setObjectName(QString::fromUtf8("checkBoxInvadingCheck"));
        checkBoxInvadingCheck->setGeometry(QRect(10, 50, 151, 17));
        checkBoxSaveSnappingImages = new QCheckBox(groupBox);
        checkBoxSaveSnappingImages->setObjectName(QString::fromUtf8("checkBoxSaveSnappingImages"));
        checkBoxSaveSnappingImages->setGeometry(QRect(10, 110, 141, 17));
        checkBoxRoadTypeHighway = new QCheckBox(dockWidgetContents);
        checkBoxRoadTypeHighway->setObjectName(QString::fromUtf8("checkBoxRoadTypeHighway"));
        checkBoxRoadTypeHighway->setGeometry(QRect(10, 10, 70, 17));
        checkBoxRoadTypeBoulevard = new QCheckBox(dockWidgetContents);
        checkBoxRoadTypeBoulevard->setObjectName(QString::fromUtf8("checkBoxRoadTypeBoulevard"));
        checkBoxRoadTypeBoulevard->setGeometry(QRect(90, 10, 91, 17));
        groupBox_2 = new QGroupBox(dockWidgetContents);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 450, 171, 91));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 20, 81, 21));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 40, 81, 21));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 60, 81, 21));
        lineEditWeightEdge = new QLineEdit(groupBox_2);
        lineEditWeightEdge->setObjectName(QString::fromUtf8("lineEditWeightEdge"));
        lineEditWeightEdge->setGeometry(QRect(90, 20, 71, 20));
        lineEditWeightEdge->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditWeightLocation = new QLineEdit(groupBox_2);
        lineEditWeightLocation->setObjectName(QString::fromUtf8("lineEditWeightLocation"));
        lineEditWeightLocation->setGeometry(QRect(90, 40, 71, 20));
        lineEditWeightLocation->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditWeightRepetition = new QLineEdit(groupBox_2);
        lineEditWeightRepetition->setObjectName(QString::fromUtf8("lineEditWeightRepetition"));
        lineEditWeightRepetition->setGeometry(QRect(90, 60, 71, 20));
        lineEditWeightRepetition->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox_3 = new QGroupBox(dockWidgetContents);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 210, 171, 151));
        radioButtonMultiSeeds = new QRadioButton(groupBox_3);
        buttonGroup = new QButtonGroup(ControlWidget);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(radioButtonMultiSeeds);
        radioButtonMultiSeeds->setObjectName(QString::fromUtf8("radioButtonMultiSeeds"));
        radioButtonMultiSeeds->setGeometry(QRect(10, 40, 121, 17));
        radioButtonOneSeed = new QRadioButton(groupBox_3);
        buttonGroup->addButton(radioButtonOneSeed);
        radioButtonOneSeed->setObjectName(QString::fromUtf8("radioButtonOneSeed"));
        radioButtonOneSeed->setGeometry(QRect(10, 20, 121, 17));
        checkBoxAreaScaling = new QCheckBox(groupBox_3);
        checkBoxAreaScaling->setObjectName(QString::fromUtf8("checkBoxAreaScaling"));
        checkBoxAreaScaling->setGeometry(QRect(10, 120, 111, 17));
        checkBoxConnectAvenues = new QCheckBox(groupBox_3);
        checkBoxConnectAvenues->setObjectName(QString::fromUtf8("checkBoxConnectAvenues"));
        checkBoxConnectAvenues->setGeometry(QRect(20, 60, 121, 17));
        checkBoxCropping = new QCheckBox(groupBox_3);
        checkBoxCropping->setObjectName(QString::fromUtf8("checkBoxCropping"));
        checkBoxCropping->setGeometry(QRect(20, 100, 121, 17));
        checkBoxSaveConnectingImages = new QCheckBox(groupBox_3);
        checkBoxSaveConnectingImages->setObjectName(QString::fromUtf8("checkBoxSaveConnectingImages"));
        checkBoxSaveConnectingImages->setGeometry(QRect(20, 80, 141, 17));
        groupBox_4 = new QGroupBox(dockWidgetContents);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 370, 171, 71));
        radioButtonCartesianCoordinate = new QRadioButton(groupBox_4);
        buttonGroup_2 = new QButtonGroup(ControlWidget);
        buttonGroup_2->setObjectName(QString::fromUtf8("buttonGroup_2"));
        buttonGroup_2->addButton(radioButtonCartesianCoordinate);
        radioButtonCartesianCoordinate->setObjectName(QString::fromUtf8("radioButtonCartesianCoordinate"));
        radioButtonCartesianCoordinate->setGeometry(QRect(10, 20, 131, 17));
        radioButtonPolarCoordinate = new QRadioButton(groupBox_4);
        buttonGroup_2->addButton(radioButtonPolarCoordinate);
        radioButtonPolarCoordinate->setObjectName(QString::fromUtf8("radioButtonPolarCoordinate"));
        radioButtonPolarCoordinate->setGeometry(QRect(10, 40, 131, 17));
        groupBox_5 = new QGroupBox(dockWidgetContents);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 820, 171, 101));
        pushButtonConnect = new QPushButton(groupBox_5);
        pushButtonConnect->setObjectName(QString::fromUtf8("pushButtonConnect"));
        pushButtonConnect->setGeometry(QRect(20, 60, 131, 31));
        pushButtonMerge = new QPushButton(groupBox_5);
        pushButtonMerge->setObjectName(QString::fromUtf8("pushButtonMerge"));
        pushButtonMerge->setGeometry(QRect(20, 20, 131, 31));
        pushButtonClear = new QPushButton(dockWidgetContents);
        pushButtonClear->setObjectName(QString::fromUtf8("pushButtonClear"));
        pushButtonClear->setGeometry(QRect(30, 730, 131, 31));
        pushButtonGeneratePM = new QPushButton(dockWidgetContents);
        pushButtonGeneratePM->setObjectName(QString::fromUtf8("pushButtonGeneratePM"));
        pushButtonGeneratePM->setGeometry(QRect(30, 780, 131, 31));
        ControlWidget->setWidget(dockWidgetContents);

        retranslateUi(ControlWidget);

        QMetaObject::connectSlotsByName(ControlWidget);
    } // setupUi

    void retranslateUi(QDockWidget *ControlWidget)
    {
        checkBoxRoadTypeAvenue->setText(QApplication::translate("ControlWidget", "Avenues", 0, QApplication::UnicodeUTF8));
        checkBoxRoadTypeLocalStreet->setText(QApplication::translate("ControlWidget", "Local Streets", 0, QApplication::UnicodeUTF8));
        groupBox_8->setTitle(QApplication::translate("ControlWidget", "KDE Pattern", 0, QApplication::UnicodeUTF8));
        pushButtonGenerateKDE->setText(QApplication::translate("ControlWidget", "Generate", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ControlWidget", "Orientation:", 0, QApplication::UnicodeUTF8));
        pushButtonPerturb->setText(QApplication::translate("ControlWidget", "Perturb", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("ControlWidget", "Perturbation Factor:", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ControlWidget", "Options", 0, QApplication::UnicodeUTF8));
        checkBoxAddAvenuesOnBoundary->setText(QApplication::translate("ControlWidget", "Add Avenues on Boundary", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ControlWidget", "Iteration:", 0, QApplication::UnicodeUTF8));
        checkBoxLocalStreets->setText(QApplication::translate("ControlWidget", "Generate Local Streets", 0, QApplication::UnicodeUTF8));
        checkBoxInvadingCheck->setText(QApplication::translate("ControlWidget", "Invading Check", 0, QApplication::UnicodeUTF8));
        checkBoxSaveSnappingImages->setText(QApplication::translate("ControlWidget", "Save Snapping Images", 0, QApplication::UnicodeUTF8));
        checkBoxRoadTypeHighway->setText(QApplication::translate("ControlWidget", "Highways", 0, QApplication::UnicodeUTF8));
        checkBoxRoadTypeBoulevard->setText(QApplication::translate("ControlWidget", "Boulevards", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("ControlWidget", "Weight", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ControlWidget", "Edge Geometry:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ControlWidget", "Location:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("ControlWidget", "Repetition:", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("ControlWidget", "For Larger Area", 0, QApplication::UnicodeUTF8));
        radioButtonMultiSeeds->setText(QApplication::translate("ControlWidget", "Multiple Seeds", 0, QApplication::UnicodeUTF8));
        radioButtonOneSeed->setText(QApplication::translate("ControlWidget", "One Seed", 0, QApplication::UnicodeUTF8));
        checkBoxAreaScaling->setText(QApplication::translate("ControlWidget", "Area Scaling", 0, QApplication::UnicodeUTF8));
        checkBoxConnectAvenues->setText(QApplication::translate("ControlWidget", "Connect Avenues", 0, QApplication::UnicodeUTF8));
        checkBoxCropping->setText(QApplication::translate("ControlWidget", "Cropping", 0, QApplication::UnicodeUTF8));
        checkBoxSaveConnectingImages->setText(QApplication::translate("ControlWidget", "Save Connecting Images", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("ControlWidget", "Coordinates System", 0, QApplication::UnicodeUTF8));
        radioButtonCartesianCoordinate->setText(QApplication::translate("ControlWidget", "Cartesian Coordinates", 0, QApplication::UnicodeUTF8));
        radioButtonPolarCoordinate->setText(QApplication::translate("ControlWidget", "Polar Coordinates", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("ControlWidget", "Finalize", 0, QApplication::UnicodeUTF8));
        pushButtonConnect->setText(QApplication::translate("ControlWidget", "Connect", 0, QApplication::UnicodeUTF8));
        pushButtonMerge->setText(QApplication::translate("ControlWidget", "Merge", 0, QApplication::UnicodeUTF8));
        pushButtonClear->setText(QApplication::translate("ControlWidget", "Clear", 0, QApplication::UnicodeUTF8));
        pushButtonGeneratePM->setText(QApplication::translate("ControlWidget", "PM", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ControlWidget);
    } // retranslateUi

};

namespace Ui {
    class ControlWidget: public Ui_ControlWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLWIDGET_H
