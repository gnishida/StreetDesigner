/********************************************************************************
** Form generated from reading UI file 'PMControlWidget.ui'
**
** Created: Thu Apr 17 11:21:18 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PMCONTROLWIDGET_H
#define UI_PMCONTROLWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PMControlWidget
{
public:
    QWidget *dockWidgetContents;
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *lineEditNumIterations;
    QPushButton *pushButtonGenerate;
    QPushButton *pushButtonClear;
    QPushButton *pushButtonMerge;
    QLabel *label_2;
    QLineEdit *lineEditOrganicFactor;
    QLabel *label_3;
    QLineEdit *lineEditSnapFactor;
    QLabel *label_4;
    QLineEdit *lineEditMinAngle;

    void setupUi(QDockWidget *PMControlWidget)
    {
        if (PMControlWidget->objectName().isEmpty())
            PMControlWidget->setObjectName(QString::fromUtf8("PMControlWidget"));
        PMControlWidget->resize(190, 916);
        PMControlWidget->setMinimumSize(QSize(190, 240));
        PMControlWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 20, 171, 231));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 46, 21));
        lineEditNumIterations = new QLineEdit(groupBox);
        lineEditNumIterations->setObjectName(QString::fromUtf8("lineEditNumIterations"));
        lineEditNumIterations->setGeometry(QRect(90, 20, 71, 20));
        lineEditNumIterations->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButtonGenerate = new QPushButton(groupBox);
        pushButtonGenerate->setObjectName(QString::fromUtf8("pushButtonGenerate"));
        pushButtonGenerate->setGeometry(QRect(20, 110, 131, 31));
        pushButtonClear = new QPushButton(groupBox);
        pushButtonClear->setObjectName(QString::fromUtf8("pushButtonClear"));
        pushButtonClear->setGeometry(QRect(20, 150, 131, 31));
        pushButtonMerge = new QPushButton(groupBox);
        pushButtonMerge->setObjectName(QString::fromUtf8("pushButtonMerge"));
        pushButtonMerge->setGeometry(QRect(20, 190, 131, 31));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 40, 81, 21));
        lineEditOrganicFactor = new QLineEdit(groupBox);
        lineEditOrganicFactor->setObjectName(QString::fromUtf8("lineEditOrganicFactor"));
        lineEditOrganicFactor->setGeometry(QRect(90, 40, 71, 20));
        lineEditOrganicFactor->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 60, 81, 21));
        lineEditSnapFactor = new QLineEdit(groupBox);
        lineEditSnapFactor->setObjectName(QString::fromUtf8("lineEditSnapFactor"));
        lineEditSnapFactor->setGeometry(QRect(90, 60, 71, 20));
        lineEditSnapFactor->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 80, 81, 21));
        lineEditMinAngle = new QLineEdit(groupBox);
        lineEditMinAngle->setObjectName(QString::fromUtf8("lineEditMinAngle"));
        lineEditMinAngle->setGeometry(QRect(90, 80, 71, 20));
        lineEditMinAngle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        PMControlWidget->setWidget(dockWidgetContents);

        retranslateUi(PMControlWidget);

        QMetaObject::connectSlotsByName(PMControlWidget);
    } // setupUi

    void retranslateUi(QDockWidget *PMControlWidget)
    {
        groupBox->setTitle(QApplication::translate("PMControlWidget", "PM Generation", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PMControlWidget", "Iteration:", 0, QApplication::UnicodeUTF8));
        pushButtonGenerate->setText(QApplication::translate("PMControlWidget", "Generate", 0, QApplication::UnicodeUTF8));
        pushButtonClear->setText(QApplication::translate("PMControlWidget", "Clear", 0, QApplication::UnicodeUTF8));
        pushButtonMerge->setText(QApplication::translate("PMControlWidget", "Merge", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("PMControlWidget", "Organic factor:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PMControlWidget", "Snap factor:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("PMControlWidget", "Min angle:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(PMControlWidget);
    } // retranslateUi

};

namespace Ui {
    class PMControlWidget: public Ui_PMControlWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PMCONTROLWIDGET_H
