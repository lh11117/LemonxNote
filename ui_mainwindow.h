/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QFrame *frame;
    QGridLayout *gridLayout_3;
    QSpacerItem *verticalSpacer_2;
    QGridLayout *gridLayout;
    QFrame *line;
    QFrame *line_2;
    QPushButton *point;
    QPushButton *undo;
    QFrame *line_3;
    QPushButton *back_page;
    QPushButton *quitButton;
    QPushButton *eraser;
    QPushButton *page;
    QPushButton *next_page;
    QPushButton *save_file;
    QPushButton *pen;
    QPushButton *open_file;
    QSpacerItem *verticalSpacer;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(110, 943);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        verticalSpacer_2 = new QSpacerItem(20, 144, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        line = new QFrame(frame);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 7, 0, 1, 1);

        line_2 = new QFrame(frame);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 13, 0, 1, 1);

        point = new QPushButton(frame);
        point->setObjectName(QString::fromUtf8("point"));
        point->setMinimumSize(QSize(70, 70));
        point->setMaximumSize(QSize(70, 70));
        point->setStyleSheet(QString::fromUtf8("border-radius: 35px;\n"
"padding: 5px;"));

        gridLayout->addWidget(point, 0, 0, 1, 1);

        undo = new QPushButton(frame);
        undo->setObjectName(QString::fromUtf8("undo"));
        undo->setMinimumSize(QSize(70, 70));
        undo->setMaximumSize(QSize(70, 70));
        undo->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"border: none;"));

        gridLayout->addWidget(undo, 4, 0, 1, 1);

        line_3 = new QFrame(frame);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_3, 3, 0, 1, 1);

        back_page = new QPushButton(frame);
        back_page->setObjectName(QString::fromUtf8("back_page"));
        back_page->setMinimumSize(QSize(70, 70));
        back_page->setMaximumSize(QSize(70, 70));
        back_page->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"border: none;"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/back_page.png"), QSize(), QIcon::Normal, QIcon::Off);
        back_page->setIcon(icon);

        gridLayout->addWidget(back_page, 9, 0, 1, 1);

        quitButton = new QPushButton(frame);
        quitButton->setObjectName(QString::fromUtf8("quitButton"));
        quitButton->setMinimumSize(QSize(70, 70));
        quitButton->setMaximumSize(QSize(70, 70));
        quitButton->setStyleSheet(QString::fromUtf8("border-radius: 35px;\n"
"padding: 5px;"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/quit.png"), QSize(), QIcon::Normal, QIcon::Off);
        quitButton->setIcon(icon1);

        gridLayout->addWidget(quitButton, 14, 0, 1, 1);

        eraser = new QPushButton(frame);
        eraser->setObjectName(QString::fromUtf8("eraser"));
        eraser->setMinimumSize(QSize(70, 70));
        eraser->setMaximumSize(QSize(70, 70));
        eraser->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"border: none;"));

        gridLayout->addWidget(eraser, 2, 0, 1, 1);

        page = new QPushButton(frame);
        page->setObjectName(QString::fromUtf8("page"));
        page->setMinimumSize(QSize(70, 70));
        page->setMaximumSize(QSize(70, 70));
        page->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"border: none;"));

        gridLayout->addWidget(page, 10, 0, 1, 1);

        next_page = new QPushButton(frame);
        next_page->setObjectName(QString::fromUtf8("next_page"));
        next_page->setMinimumSize(QSize(70, 70));
        next_page->setMaximumSize(QSize(70, 70));
        next_page->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"border: none;"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new_page.png"), QSize(), QIcon::Normal, QIcon::Off);
        next_page->setIcon(icon2);

        gridLayout->addWidget(next_page, 12, 0, 1, 1);

        save_file = new QPushButton(frame);
        save_file->setObjectName(QString::fromUtf8("save_file"));
        save_file->setMinimumSize(QSize(70, 70));
        save_file->setMaximumSize(QSize(70, 70));
        save_file->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"border: none;"));

        gridLayout->addWidget(save_file, 6, 0, 1, 1);

        pen = new QPushButton(frame);
        pen->setObjectName(QString::fromUtf8("pen"));
        pen->setMinimumSize(QSize(70, 70));
        pen->setMaximumSize(QSize(70, 70));
        pen->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"border: none;"));

        gridLayout->addWidget(pen, 1, 0, 1, 1);

        open_file = new QPushButton(frame);
        open_file->setObjectName(QString::fromUtf8("open_file"));
        open_file->setMinimumSize(QSize(70, 70));
        open_file->setMaximumSize(QSize(70, 70));
        open_file->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);\n"
"border: none;"));

        gridLayout->addWidget(open_file, 5, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 136, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 2, 0, 1, 1);


        gridLayout_2->addWidget(frame, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        point->setText(QString());
        undo->setText(QString());
        back_page->setText(QString());
        quitButton->setText(QString());
        eraser->setText(QString());
        page->setText(QCoreApplication::translate("MainWindow", "1/1", nullptr));
        next_page->setText(QString());
        save_file->setText(QString());
        pen->setText(QString());
        open_file->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
