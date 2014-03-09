/********************************************************************************
** Form generated from reading UI file 'UrbanMain.ui'
**
** Created: Sun Mar 9 11:54:55 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_URBANMAIN_H
#define UI_URBANMAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UrbanMain
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *UrbanMain)
    {
        if (UrbanMain->objectName().isEmpty())
            UrbanMain->setObjectName(QString::fromUtf8("UrbanMain"));
        UrbanMain->resize(800, 600);
        centralwidget = new QWidget(UrbanMain);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setGeometry(QRect(0, 21, 800, 560));
        UrbanMain->setCentralWidget(centralwidget);
        menubar = new QMenuBar(UrbanMain);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        UrbanMain->setMenuBar(menubar);
        statusbar = new QStatusBar(UrbanMain);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        statusbar->setGeometry(QRect(0, 581, 800, 19));
        UrbanMain->setStatusBar(statusbar);

        retranslateUi(UrbanMain);

        QMetaObject::connectSlotsByName(UrbanMain);
    } // setupUi

    void retranslateUi(QMainWindow *UrbanMain)
    {
        UrbanMain->setWindowTitle(QApplication::translate("UrbanMain", "UrbanMain", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UrbanMain: public Ui_UrbanMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_URBANMAIN_H
