/********************************************************************************
** Form generated from reading UI file 'dialogclean.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCLEAN_H
#define UI_DIALOGCLEAN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_DialogClean
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QSlider *horizontalSlider;
    QPushButton *cancel;

    void setupUi(QDialog *DialogClean)
    {
        if (DialogClean->objectName().isEmpty())
            DialogClean->setObjectName(QString::fromUtf8("DialogClean"));
        DialogClean->resize(405, 95);
        gridLayout_2 = new QGridLayout(DialogClean);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(DialogClean);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font: 14pt;"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 0, 1, 1);

        horizontalSlider = new QSlider(DialogClean);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setMinimumSize(QSize(0, 50));
        horizontalSlider->setStyleSheet(QString::fromUtf8("QSlider::groove:horizontal{ \n"
"		height: 30px; \n"
"		left: 0px; \n"
"		right: 0px; \n"
"		border:0px;\n"
"		background:rgba(0,0,0,50);\n"
" } \n"
" QSlider::handle:horizontal{ \n"
"		width:  20px; \n"
"		height: 30px; \n"
"padding: 2px;\n"
"padding-left: 4px;\n"
"margin: 3px;\n"
"margin-left: 10px;\n"
"	background-color: rgb(255, 255, 255);\n"
"border-radius: 20px;\n"
"} \n"
"QSlider::sub-page:horizontal{\n"
"	   background:rgba(80,166,234,1);\n"
"margin: 5px;\n"
"\n"
"}"));
        horizontalSlider->setMaximum(100);
        horizontalSlider->setPageStep(0);
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider, 0, 0, 1, 5);

        cancel = new QPushButton(DialogClean);
        cancel->setObjectName(QString::fromUtf8("cancel"));

        gridLayout->addWidget(cancel, 1, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(DialogClean);

        QMetaObject::connectSlotsByName(DialogClean);
    } // setupUi

    void retranslateUi(QDialog *DialogClean)
    {
        DialogClean->setWindowTitle(QCoreApplication::translate("DialogClean", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("DialogClean", ">>> Swipe to clear  >>>", nullptr));
        cancel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DialogClean: public Ui_DialogClean {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCLEAN_H
