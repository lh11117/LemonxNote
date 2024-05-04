#include "canvaswin.h"
#include "ui_canvaswin.h"

#include <QDateTime>
#include "QScreen"


CanvasWin::CanvasWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CanvasWin)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);//需要去掉标题栏


    setMouseTracking(true);

    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QRect screenGeometry = primaryScreen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    this->resize(screenWidth, screenHeight);
    this->move(0,0);

    ui->labelTime->setAttribute(Qt::WA_TranslucentBackground);
    ui->labelTips->setVisible(false);

    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    handleTimeout();

    SetTime();

}

qint64 CanvasWin::getPagesSize(){
    qint64 totalBytes = 0;
    for (const auto &page : history){
        totalBytes += sizeof(page.first);
        for (const auto &outerPair : page.second) {
            totalBytes += sizeof(QPair<QPoint, QList<QPair<QColor, QList<QPoint>>>>);
            totalBytes += sizeof(outerPair.first);
            for (const auto &innerPair : outerPair.second) {
                totalBytes += sizeof(QPair<QColor, QList<QPoint>>);
                totalBytes += sizeof(innerPair.first);
                for (const QPoint &pt : innerPair.second) {
                    totalBytes += sizeof(pt);
                }
            }
        }
    }

    for (const auto &outerPair : pages) {
        totalBytes += sizeof(QPair<QPoint, QList<QPair<QColor, QList<QPoint>>>>);
        totalBytes += sizeof(outerPair.first);
        for (const auto &innerPair : outerPair.second) {
            totalBytes += sizeof(QPair<QColor, QList<QPoint>>);
            totalBytes += sizeof(innerPair.first);
            for (const QPoint &pt : innerPair.second) {
                totalBytes += sizeof(pt);
            }
        }
    }

    return totalBytes;
}

QString format_bytes(qint64 size_bytes){
    // 为了更好地处理小数点，我们使用浮点数进行计算
    float size = size_bytes;
    // 定义单位
    QList<QString> units {"B", "KB", "MB", "GB", "TB"};
    // 逐步缩小单位，并转换为相应的单位
    for(QString unit: units) {
        if(size < 1024.0)
            return QString("%1%2").arg(QString::number(size, 'f', 5)).arg(unit);
        size /= 1024.0;
    }
    return "";
}

void CanvasWin::SetTime(){
    QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间
    QString timeStr = dateTime.toString("hh:mm:ss");//格式化时间
    ui->labelTime->setText(QString("%1   %2").arg(format_bytes(getPagesSize())).arg(timeStr));
    ui->labelTime->adjustSize();
    ui->labelTime->move(width()-ui->labelTime->width()-10,10);
}

void CanvasWin::handleTimeout()
{
     SetTime();
     if (!timerInited) {
         timerInited = true;
         Timer->stop();
         qint64 timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch(); //毫秒级
         Timer->start(1000-timestamp%1000);
     }
 }



CanvasWin::~CanvasWin()
{
    delete ui;
}


void CanvasWin::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (pages.at(page).second.count()!=0){
        //qDebug()<<offset;
        for (QPair<QColor, QList<QPoint>> line: pages.at(page).second) {
            if (line.second.count()!=0){
                QPoint last = line.second.at(0);
                foreach (QPoint point, line.second) {
                    QPen pen(line.first);
                    pen.setBrush(line.first);
                    pen.setWidth(3*zoom);
                    QPoint start = last+pages.at(page).first, end = point+pages.at(page).first;
                    start *= zoom, end *= zoom;
                    painter.setPen(pen);
                    painter.drawLine(start,end);
                    last = point;
                }
            }
        }
    }
    if (eraserR!=0){
        QPen pen1(QColor(255,255,255,80));
        pen1.setWidth(5);
        painter.setPen(pen1);
        painter.drawEllipse(mousePos + pages.at(page).first, eraserR, eraserR);
    }
    SetTime();
}

#define max(a,b) a>b?a:b
#define min(a,b) a>b?b:a

QList<QPair<QColor, QList<QPoint>>> filterOutsideCircle(const QList<QPair<QColor, QList<QPoint>>>& pointLists, const QPoint& center, int radius) {
    QList<QPair<QColor, QList<QPoint>>> outsideCircle;

    for (const QPair<QColor, QList<QPoint>>& pointList : pointLists) {
        QList<QPoint> outsideList;

        for (const QPoint& point : pointList.second) {
            int dx = point.x() - center.x();
            int dy = point.y() - center.y();
            int distanceSquared = dx * dx + dy * dy;

            if (distanceSquared > radius * radius) {
                outsideList.append(point);
            } else {
                if (!outsideList.isEmpty()) {
                    outsideCircle.append({pointList.first, outsideList});
                    outsideList = {};
                    continue;
                }
            }
        }

        if (!outsideList.isEmpty()) {
            outsideCircle.append(QPair<QColor, QList<QPoint>> {pointList.first, outsideList});
        }
    }

    return outsideCircle;
}


void CanvasWin::Clean(QPoint last, QPoint p){
    int len = sqrt(pow((last).x()-(p-pages.at(page).first).x(),2)+pow((last).y()-(p-pages.at(page).first).y(),2))/zoom;    // 对于显示
    eraserR = max(len, 10);
    eraserR = eraserR>100?100:eraserR;
    last = (last + pages.at(page).first)/zoom - pages.at(page).first;
    p/=zoom;
    int len2 = sqrt(pow((last).x()-(p-pages.at(page).first).x(),2)+pow((last).y()-(p-pages.at(page).first).y(),2))/zoom;  // 对于擦除
    int eraserR2 = max(len2, 10);
    eraserR2 = eraserR2>100?100:eraserR2;
    pages.replace(page, {pages.at(page).first, filterOutsideCircle(pages.at(page).second, p - pages.at(page).first, eraserR2)});
    //qDebug()<<pages.at(page).second.count();
    update();
}

void CanvasWin::mousePressEvent(QMouseEvent *event) {
    PressMouse = true;
    history<<QPair<int, Pages>{page,pages};
    emit OnUpdateHistory();
    mousePos = event->pos()/(Mode==0?zoom:1) - pages.at(page).first;
    if (event->button() == Qt::LeftButton && Mode==1) {
        QList<QPair<QColor, QList<QPoint>>> _page = pages.at(page).second; // 获取
        _page << QPair<QColor, QList<QPoint>>{drawColor, {event->pos()/zoom - pages.at(page).first}};  // 新建一个空的线条
        pages.replace(page, {pages.at(page).first, _page});  // 替换!
        update();
    } else if(event->button() == Qt::LeftButton && Mode==2) {
        Clean(mousePos, event->pos());
        mousePos = event->pos() - pages.at(page).first;
    }
}

void CanvasWin::mouseMoveEvent(QMouseEvent *event) {
    emit this->setToolBarPostion(event->x()<width()*1/6?1:(event->x()>width()*5/6?2:0));
    if (Mode==1 && event->buttons() & Qt::LeftButton) {
        QList<QPair<QColor, QList<QPoint>>> _page = pages.at(page).second; // 获取页面
        _page.last().second << event->pos()/zoom - pages.at(page).first; // 这个页面最后一个线条, 新增点的记录
        pages.replace(page, {pages.at(page).first, _page}); // 替换页面数据
        update(); // 请求重绘
    } else if(PressMouse && Mode==0) {
        pages.replace(page, {event->pos()/zoom - mousePos, pages.at(page).second}); // 移动页面
        ui->labelTips->setVisible(true);
        ui->labelTips->setText(QString("X: %1, Y: %2").arg(pages.at(page).first.x()).arg(pages.at(page).first.y()));
        ui->labelTips->adjustSize();
        ui->labelTips->move(width()/2-ui->labelTips->width()/2,height()/2-ui->labelTips->height()/2);  // 显示位置相关
        update();
    } else if (PressMouse && Mode==2) {
        Clean(mousePos, event->pos());
        mousePos = event->pos() - pages.at(page).first;
    }
}

void CanvasWin::mouseReleaseEvent(QMouseEvent *event){
    ui->labelTips->setVisible(false);
    if (Mode==1 && event->buttons() & Qt::LeftButton) {
        QList<QPair<QColor, QList<QPoint>>> _page = pages.at(page).second;  // 绘制完成后，处理线条, 避免再次绘制连在一起
        _page << QPair<QColor, QList<QPoint>>{drawColor,QList<QPoint>()};
        pages.replace(page, {pages.at(page).first, _page});
    }
    emit OnDrawCompleted();
    PressMouse = false;
    eraserR = 0;
    update(); // 请求重绘
}



