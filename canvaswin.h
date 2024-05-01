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

typedef QList<QPair<QPoint, /* <-偏移量, 即offset */QList/* 每一个线条 */<QPair<QColor/* <-绘图颜色 */, QList<QPoint> /* 线里面的每个点坐标 */>>>> Pages;

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

    Pages pages = {{}};

    QList<QPair<int/* 这个int表示页码 */, Pages>> history = {};

    void Clean(QPoint, QPoint);
    int page = 0;

    void setPage(int p){
        this->page = p;
        update();
    }

    float zoom = 1;

private:
    Ui::CanvasWin *ui;

    //QPoint offset = QPoint(0, 0);
    QPoint mousePos = QPoint(0, 0);
    bool PressMouse = false;

    int eraserR = 0;

    bool timerInited = false;

    void SetTime();
    qint64 getPagesSize();


protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override {
        if(event->delta()>0){
            zoom += 0.1;
        } else {
            zoom -= 0.1;
        }
pages.replace(page, {pages.at(page).first+QPoint(event->x()*(zoom-1), event->y()*(zoom-1)), pages.at(page).second});
update();
    };

public slots:
    void handleTimeout();  //超时处理函数
private:
    QTimer *Timer;
signals:
    void setToolBarPostion(int);
    void OnDrawCompleted();
};

#endif // CANVASWIN_H
