#ifndef CANVASWIN_H
#define CANVASWIN_H

#include <QWidget>
#include <QPainter>
#include <QEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QList>
#include <QPoint>
#include <QDebug>
#include <cmath>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "appinfo.h"
#include <QLabel>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraInfo>

struct Line{
    QColor color;
    int size;
    QList<QPoint*> points;
};

struct Page{
    double zoom;
    QPoint offset;
    QList<Line*> lines;
};

typedef QList<Page*> Pages;

//typedef QList<QPair<double /* <- zoom 缩放 */,QPair<QPoint, /* <-偏移量, 即offset */QList/* 每一个线条 */<QPair<QPair<QColor/* <-绘图颜色 */, int/* <- 线条粗细*/>, QList<QPoint> /* 线里面的每个点坐标 */>>>>> Pages;

QString format_bytes(qint64 size_bytes);

namespace Ui {
class CanvasWin;
}

class CanvasWin : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasWin(QWidget *parent = nullptr);
    ~CanvasWin();

    int Mode = 0;

    QColor drawColor = QColor(255,0,0);

    Pages pages;

    QList<QPair<int/* 这个int表示页码 */, Pages>> history = {};

    void Clean(QPoint, QPoint);
    int page = 0;

    void setPage(int p){
        this->page = p;
        update();
    }

    //float zoom = 1;

    qint64 getPagesSize();

    Ui::CanvasWin *ui;

    QCameraViewfinder* CameraViewFinder = nullptr;
    QPixmap *bgPixmap = new QPixmap();
    int cameraPage = 0;

    QSize bgSize;
    bool isThereBG = false;

    double cameraAngle = 0.0;

    Page* getpage() {return pages.at(page);}
    double zoom() {return getpage()->zoom; }
    void zoom(double new_zoom) { getpage()->zoom = qMax(new_zoom, 0.1); }
    void zoom(int new_zoom) { getpage()->zoom = qMax((double)new_zoom, 0.1); }
    QPoint offset() {return getpage()->offset;}
    void offset(QPoint new_offset) { getpage()->offset = new_offset; }
    void offset(int x, int y) { getpage()->offset = QPoint(x, y); }

private:
    //QPoint offset = QPoint(0, 0);
    QPoint mousePos = QPoint(0, 0);
    bool PressMouse = false;

    int eraserR = 0;

    bool timerInited = false;

    QPixmap*Pix;

    void SetTime();


protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override {
        if(event->delta()>0){
            zoom(zoom() + 0.1);
        } else {
            zoom(zoom() - 0.1);
        }
        update();
    };
    bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    void handleTimeout();  //超时处理函数
private:
    QTimer *Timer;
signals:
    void setToolBarPostion(int);
    void OnDrawCompleted();
    void OnUpdateHistory();
};

std::pair<QPoint, QPoint> circleLineIntersect(const QPoint &O, double r, const QPoint &P, const QPoint &Q, bool&hsd);

#endif // CANVASWIN_H
