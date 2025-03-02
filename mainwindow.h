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
#include <QFileDialog>
#include "save_open_lnbf.h"
#include "canvaswin.h"
#include <QSaveFile>
#include "appinfo.h"
#include <QCheckBox>
#include <QStyle>
#include <QComboBox>
#include <QSettings>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QIcon>
#include <QPainter>
#include <QPainterPath>
#include <QPainterPathStroker>
#include <QPixmap>
#include "qmenu.h"
#include "qevent.h"


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

    QIcon ShadowIcon(QString);


    QColor drawColor;

    int Mode = 0;


    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    QNetworkAccessManager *dl_nam = new QNetworkAccessManager(this);

    QString lnbf_path = "";

    int CameraId = -1;
    bool IsCameraFixed = false;
    int CameraId_back = -1;

    QIcon setIconColor(QIcon, QColor);

    double cameraAngle = 0;


private:
    Ui::MainWindow *ui;

    CanvasWin *cw = new CanvasWin();

    void OnModeChange();

    void OnPageChanges();

    void removeHistory(int);

    bool isUpdateDownloaded = false;
    QString update_exe_path = "", install_bat_script_path = "";

    QTranslator *m_pTranslator = new QTranslator;

    QSettings *setting = new QSettings("HKEY_CURRENT_USER\\Software\\LemonxNote", QSettings::NativeFormat);

    QIcon addDropShadowToIcon(const QIcon& , const QColor& , int , int , int );

    QTimer*timer = new QTimer();


    QCameraImageCapture* CameraImageCapture = nullptr;
    QCamera* Camera = nullptr;
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    QTimer *Timer;

public slots:
    void checkUpdateWithData(QNetworkReply*);
    void OnDownloadCompleted(QNetworkReply*);
    void onSetToolbarPositon(int);
    void OnDrawCompleted();
    void setLanguage(int);
    void OnCameraChange();
    void take_photo(int, const QImage &);

protected:
    void closeEvent(QCloseEvent*) override;
};


int compareVersions(const QString&, const QString&);


void tintButtonBackground(QPushButton *, const QColor &, const QSize & = QSize(20,20));

class MyMenu: public QMenu {
public:
    MyMenu(){
        setStyleSheet("QMenu {"
                               "    background-color : rgb(253,253,254);"
                               " padding:5px;"
                               " border-radius:15px;"
                               "}"
                               "QMenu::item {"
                               "    font-size:10pt;"
                               "    color: rgb(0,0,0);"
                               "    background-color:rgb(253,253,254);"
                               "    padding: 8px 25px 6px 10px;"
                               "    margin: 4px 1px;"
                               "}"
                               "QMenu::item:selected {"
                               "    background-color : rgb(236,236,237);"
                               "}"
                               "QMenu::icon:checked {"
                               "    background: rgb(253,253,254);"
                               "    position: absolute;"
                               "    top: 1px;"
                               "    right: 1px;"
                               "    bottom: 1px;"
                               "    left: 1px;"
                               "}"
                               "QMenu::icon:checked:selected {"
                               "    background-color : rgb(236,236,237);"
                               "    background-image: url(:/choose.png);"
                               "}"
                               "QMenu::separator {"
                               "    height: 2px;"
                               "    background: rgb(235,235,236);"
                               "    margin-left: 10px;"
                               "    margin-right: 10px;"
                               "}");
    }
protected:
    void mouseReleaseEvent(QMouseEvent*e) override {
        QAction *action = this->actionAt(e->pos());
        if (action){
            action->activate(QAction::Trigger);
        } else {
            QMenu::mouseReleaseEvent(e);
        }
    };
};


#endif // MAINWINDOW_H
