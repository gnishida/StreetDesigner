/********************************************************************************
** Form generated from reading UI file 'ControlWidget.ui'
**
** Created: Thu Mar 27 17:06:39 2014
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
#include <QtGui/QDial>
#include <QtGui/QDockWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlWidget
{
public:
    QWidget *dockWidgetContents;
    QGroupBox *groupBox_8;
    QPushButton *pushButtonGenerateKDE;
    QDial *dialOrientation;
    QLabel *label_2;
    QPushButton *pushButtonPerturb;
    QLineEdit *lineEditPerturbationFactor;
    QLabel *label_6;
    QPushButton *pushButtonClear;
    QGroupBox *groupBox;
    QCheckBox *checkBoxAddAvenuesOnBoundary;
    QLabel *label;
    QLineEdit *lineEditNumIterations;
    QCheckBox *checkBoxLocalStreets;
    QLabel *label_8;
    QLineEdit *lineEditOrganicFactor;
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
    QGroupBox *groupBox_4;
    QRadioButton *radioButtonCartesianCoordinate;
    QRadioButton *radioButtonPolarCoordinate;
    QGroupBox *groupBox_5;
    QPushButton *pushButtonConnect;
    QPushButton *pushButtonMerge;
    QSlider *horizontalSliderExactSimilarityFactor;
    QLabel *label_7;
    QButtonGroup *buttonGroup_2;
    QButtonGroup *buttonGroup;

    void setupUi(QDockWidget *ControlWidget)
    {
        if (ControlWidget->objectName().isEmpty())
            ControlWidget->setObjectName(QString::fromUtf8("ControlWidget"));
        ControlWidget->resize(190, 977);
        ControlWidget->setMinimumSize(QSize(190, 240));
        ControlWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        groupBox_8 = new QGroupBox(dockWidgetContents);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        groupBox_8->setGeometry(QRect(10, 510, 171, 221));
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
        pushButtonClear = new QPushButton(groupBox_8);
        pushButtonClear->setObjectName(QString::fromUtf8("pushButtonClear"));
        pushButtonClear->setGeometry(QRect(20, 180, 131, 31));
        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 171, 121));
        checkBoxAddAvenuesOnBoundary = new QCheckBox(groupBox);
        checkBoxAddAvenuesOnBoundary->setObjectName(QString::fromUtf8("checkBoxAddAvenuesOnBoundary"));
        checkBoxAddAvenuesOnBoundary->setGeometry(QRect(10, 80, 151, 17));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 46, 21));
        lineEditNumIterations = new QLineEdit(groupBox);
        lineEditNumIterations->setObjectName(QString::fromUtf8("lineEditNumIterations"));
        lineEditNumIterations->setGeometry(QRect(100, 20, 61, 20));
        lineEditNumIterations->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        checkBoxLocalStreets = new QCheckBox(groupBox);
        checkBoxLocalStreets->setObjectName(QString::fromUtf8("checkBoxLocalStreets"));
        checkBoxLocalStreets->setGeometry(QRect(10, 100, 131, 17));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 40, 81, 21));
        lineEditOrganicFactor = new QLineEdit(groupBox);
        lineEditOrganicFactor->setObjectName(QString::fromUtf8("lineEditOrganicFactor"));
        lineEditOrganicFactor->setGeometry(QRect(100, 40, 61, 20));
        lineEditOrganicFactor->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox_2 = new QGroupBox(dockWidgetContents);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 410, 171, 91));
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
        groupBox_3->setGeometry(QRect(10, 200, 171, 121));
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
        checkBoxAreaScaling->setGeometry(QRect(10, 100, 111, 17));
        checkBoxConnectAvenues = new QCheckBox(groupBox_3);
        checkBoxConnectAvenues->setObjectName(QString::fromUtf8("checkBoxConnectAvenues"));
        checkBoxConnectAvenues->setGeometry(QRect(20, 60, 121, 17));
        checkBoxCropping = new QCheckBox(groupBox_3);
        checkBoxCropping->setObjectName(QString::fromUtf8("checkBoxCropping"));
        checkBoxCropping->setGeometry(QRect(20, 80, 121, 17));
        groupBox_4 = new QGroupBox(dockWidgetContents);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 330, 171, 71));
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
        groupBox_5->setGeometry(QRect(10, 740, 171, 101));
        pushButtonConnect = new QPushButton(groupBox_5);
        pushButtonConnect->setObjectName(QString::fromUtf8("pushButtonConnect"));
        pushButtonConnect->setGeometry(QRect(20, 60, 131, 31));
        pushButtonMerge = new QPushButton(groupBox_5);
        pushButtonMerge->setObjectName(QString::fromUtf8("pushButtonMerge"));
        pushButtonMerge->setGeometry(QRect(20, 20, 131, 31));
        horizontalSliderExactSimilarityFactor = new QSlider(dockWidgetContents);
        horizontalSliderExactSimilarityFactor->setObjectName(QString::fromUtf8("horizontalSliderExactSimilarityFactor"));
        horizontalSliderExactSimilarityFactor->setGeometry(QRect(20, 170, 151, 20));
        horizontalSliderExactSimilarityFactor->setOrientation(Qt::Horizontal);
        label_7 = new QLabel(dockWidgetContents);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 150, 46, 13));
        ControlWidget->setWidget(dockWidgetContents);

        retranslateUi(ControlWidget);

        QMetaObject::connectSlotsByName(ControlWidget);
    } // setupUi

    void retranslateUi(QDockWidget *ControlWidget)
    {
        groupBox_8->setTitle(QApplication::translate("ControlWidget", "KDE Pattern", 0, QApplication::UnicodeUTF8));
        pushButtonGenerateKDE->setText(QApplication::translate("ControlWidget", "Generate", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ControlWidget", "Orientation:", 0, QApplication::UnicodeUTF8));
        pushButtonPerturb->setText(QApplication::translate("ControlWidget", "Perturb", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("ControlWidget", "Perturbation Factor:", 0, QApplication::UnicodeUTF8));
        pushButtonClear->setText(QApplication::translate("ControlWidget", "Clear", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ControlWidget", "Options", 0, QApplication::UnicodeUTF8));
        checkBoxAddAvenuesOnBoundary->setText(QApplication::translate("ControlWidget", "Add Avenues on Boundary", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ControlWidget", "Iteration:", 0, QApplication::UnicodeUTF8));
        checkBoxLocalStreets->setText(QApplication::translate("ControlWidget", "Generate Local Streets", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("ControlWidget", "Organic Factor:", 0, QApplication::UnicodeUTF8));
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
        groupBox_4->setTitle(QApplication::translate("ControlWidget", "Coordinates System", 0, QApplication::UnicodeUTF8));
        radioButtonCartesianCoordinate->setText(QApplication::translate("ControlWidget", "Cartesian Coordinates", 0, QApplication::UnicodeUTF8));
        radioButtonPolarCoordinate->setText(QApplication::translate("ControlWidget", "Polar Coordinates", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("ControlWidget", "Finalize", 0, QApplication::UnicodeUTF8));
        pushButtonConnect->setText(QApplication::translate("ControlWidget", "Connect", 0, QApplication::UnicodeUTF8));
        pushButtonMerge->setText(QApplication::translate("ControlWidget", "Merge", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("ControlWidget", "Similarity:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ControlWidget);
    } // retranslateUi

};

namespace Ui {
    class ControlWidget: public Ui_ControlWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLWIDGET_H
