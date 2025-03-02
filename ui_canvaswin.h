/********************************************************************************
** Form generated from reading UI file 'canvaswin.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CANVASWIN_H
#define UI_CANVASWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CanvasWin
{
public:
    QLabel *labelTime;
    QLabel *labelTips;
    QLabel *CanvasWidget;

    void setupUi(QWidget *CanvasWin)
    {
        if (CanvasWin->objectName().isEmpty())
            CanvasWin->setObjectName(QString::fromUtf8("CanvasWin"));
        CanvasWin->resize(599, 461);
        CanvasWin->setStyleSheet(QString::fromUtf8("background-color: rgb(32, 46, 37);"));
        labelTime = new QLabel(CanvasWin);
        labelTime->setObjectName(QString::fromUtf8("labelTime"));
        labelTime->setGeometry(QRect(290, 10, 301, 141));
        labelTime->setStyleSheet(QString::fromUtf8("font: 20pt \"Consolas\";\n"
"color: rgb(255, 255, 255);"));
        labelTime->setAlignment(Qt::AlignRight|Qt::AlignTop|Qt::AlignTrailing);
        labelTips = new QLabel(CanvasWin);
        labelTips->setObjectName(QString::fromUtf8("labelTips"));
        labelTips->setGeometry(QRect(90, 60, 281, 111));
        labelTips->setStyleSheet(QString::fromUtf8("font: 24pt \"\351\273\221\344\275\223\";\n"
"color: rgb(255, 255, 255);\n"
"background-color: rgba(0, 0, 0, 150);\n"
"border-radius: 15px;\n"
"padding: 25px;"));
        labelTips->setAlignment(Qt::AlignCenter);
        CanvasWidget = new QLabel(CanvasWin);
        CanvasWidget->setObjectName(QString::fromUtf8("CanvasWidget"));
        CanvasWidget->setGeometry(QRect(340, 250, 161, 101));
        CanvasWidget->raise();
        labelTime->raise();
        labelTips->raise();

        retranslateUi(CanvasWin);

        QMetaObject::connectSlotsByName(CanvasWin);
    } // setupUi

    void retranslateUi(QWidget *CanvasWin)
    {
        CanvasWin->setWindowTitle(QCoreApplication::translate("CanvasWin", "LemonxNote", nullptr));
        labelTime->setText(QString());
        labelTips->setText(QCoreApplication::translate("CanvasWin", "X: 0, Y: 0", nullptr));
        CanvasWidget->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CanvasWin: public Ui_CanvasWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANVASWIN_H
