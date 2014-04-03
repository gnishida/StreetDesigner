/********************************************************************************
** Form generated from reading UI file 'ControlWidget.ui'
**
** Created: Thu Apr 3 10:42:21 2014
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
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlWidget
{
public:
    QWidget *dockWidgetContents;
    QLineEdit *lineEditElevation;
    QLabel *label;
    QPushButton *pushButtonApply;
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
        lineEditElevation = new QLineEdit(dockWidgetContents);
        lineEditElevation->setObjectName(QString::fromUtf8("lineEditElevation"));
        lineEditElevation->setGeometry(QRect(72, 10, 101, 20));
        label = new QLabel(dockWidgetContents);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 61, 16));
        pushButtonApply = new QPushButton(dockWidgetContents);
        pushButtonApply->setObjectName(QString::fromUtf8("pushButtonApply"));
        pushButtonApply->setGeometry(QRect(34, 40, 121, 31));
        ControlWidget->setWidget(dockWidgetContents);

        retranslateUi(ControlWidget);

        QMetaObject::connectSlotsByName(ControlWidget);
    } // setupUi

    void retranslateUi(QDockWidget *ControlWidget)
    {
        label->setText(QApplication::translate("ControlWidget", "Elevation:", 0, QApplication::UnicodeUTF8));
        pushButtonApply->setText(QApplication::translate("ControlWidget", "Apply", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ControlWidget);
    } // retranslateUi

};

namespace Ui {
    class ControlWidget: public Ui_ControlWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLWIDGET_H
