#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QPixmap>
#include <QPainter>
#include <QScreen>
#include <QDebug>
#include <QMessageBox>
#include <QColorDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#include <QVersionNumber>
#include <QStringList>
#include <QDir>
#include <QProcess>
#include <QTranslator>
#include <QFile>

#include "canvaswin.h"

const QString version = "1.0.0";


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Point();
    void Pen();
    void Eraser();

    void InitIcons();


    QColor drawColor;

    int Mode = 0;


    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    QNetworkAccessManager *dl_nam = new QNetworkAccessManager(this);


private:
    Ui::MainWindow *ui;

    CanvasWin *cw = new CanvasWin();

    void OnModeChange();

    void OnPageChanges();

    bool isUpdateDownloaded = false;
    QString update_exe_path = "", install_bat_script_path = "";

public slots:
    void checkUpdateWithData(QNetworkReply*);
    void OnDownloadCompleted(QNetworkReply*);
    void onSetToolbarPositon(int);
    void OnDrawCompleted();

protected:
    void closeEvent(QCloseEvent*) override;
};


int compareVersions(const QString&, const QString&);


#endif // MAINWINDOW_H
