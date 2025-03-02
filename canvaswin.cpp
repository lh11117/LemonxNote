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

    Page *p = new Page;
    p->zoom = 1.0;
    p->offset = QPoint(0,0);
    p->lines << new Line();
    pages<<p;

    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QRect screenGeometry = primaryScreen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    this->resize(screenWidth, screenHeight);
    this->move(0,0);

    Pix = new QPixmap(screenWidth, screenHeight);

    ui->CanvasWidget->resize(screenWidth, screenHeight);
    ui->CanvasWidget->move(0,0);

    //ui->CanvasWidget->installEventFilter(this);

    ui->labelTime->setAttribute(Qt::WA_TranslucentBackground);
    ui->labelTips->setVisible(false);

    Timer = new QTimer(this);
    connect(Timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    handleTimeout();

    SetTime();

    QPoint O(100, 200);
    double r = 50;
    QPoint P(50, 300);
    QPoint Q(200, 100);
    bool _;

    auto intersects = circleLineIntersect(O, r, P, Q, _);
    QPoint N = intersects.first;
    QPoint M = intersects.second;
    qDebug()<<N<<M;
}

qint64 CanvasWin::getPagesSize(){
    qint64 totalBytes = 0;
    /*for (const auto &page : history){
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
    }*/

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
    Pix->fill(QColor(32, 46, 37));
    QPainter painter(Pix);
    //painter.setRenderHint(QPainter::Antialiasing);
    if (isThereBG && cameraPage == page){
        //qDebug()<<*bgPixmap;
        QPointF point = QPointF(-bgSize.width() + width(), -bgSize.height() + height())*zoom()/2+offset()*zoom();
        QSizeF size = bgSize*zoom();
        QRectF sourceRect;
        //if(fmod(cameraAngle, 180.0) != 0.0)
        //    sourceRect = QRectF(0, 0, bgSize.height(), bgSize.width()); // Adjust the source rectangle as needed
        //else
            sourceRect = QRectF(0, 0, bgSize.width(), bgSize.height());
        painter.drawPixmap(QRectF(point.x(), point.y(), size.width(), size.height()), *bgPixmap, sourceRect);
    }
    if (getpage()->lines.size()!=0){
        //qDebug()<<offset;
        for (Line*line: getpage()->lines) {
            if (line->points.count()!=0){
                QPoint last = *line->points.at(0);
                for (QPoint*p: line->points) {
                    QPoint point = *p;
                    QPen pen(line->color);
                    pen.setBrush(line->color);
                    pen.setWidth(line->size*zoom());
                    QPoint start = last+offset(), end = point+offset();
                    start *= zoom(), end *= zoom();
                    painter.setPen(pen);
                    painter.drawLine(start,end);
                    last = point;
                }
            }
        }
    }
    if (eraserR!=0){
        QPen pen1(QColor(255,255,255,128));
        pen1.setWidth(5);
        painter.setPen(pen1);
        painter.drawEllipse(mousePos + offset(), eraserR, eraserR);
    }
    SetTime();

    ui->CanvasWidget->setPixmap(*Pix);
}

#define max(a,b) a>b?a:b
#define min(a,b) a>b?b:a

// 计算两点之间的欧几里得距离
double dist(const QPoint &p1, const QPoint &p2) {
    return sqrt(pow(p1.x() - p2.x(), 2) + pow(p1.y() - p2.y(), 2));
}

// 判断点 p 是否在线段 p1p2 上
bool onSegment(const QPoint &p1, const QPoint &p2, const QPoint &p) {
    if (p.x() <= qMax(p1.x(), p2.x()) && p.x() >= qMin(p1.x(), p2.x()) &&
        p.y() <= qMax(p1.y(), p2.y()) && p.y() >= qMin(p1.y(), p2.y())) {
        return true;
    }
    return false;
}

// 求解圆和线段的两个交点
std::pair<QPoint, QPoint> circleLineIntersect(const QPoint &O, double r, const QPoint &P, const QPoint &Q, bool &has_result) {
    std::pair<QPoint, QPoint> result;

    // 将线段 PQ 平移至过原点
    QPoint p1 = P - O, p2 = Q - O;

    double a = p2.y() - p1.y();
    double b = p1.x() - p2.x();
    double c = -a * p1.x() - b * p1.y();

    double x0 = -a * c / (a * a + b * b), y0 = -b * c / (a * a + b * b);

    // 没有交点
    if (c * c > r * r * (a * a + b * b)) {
        has_result = false;
        return result;
    }

    double d = r * r - c * c / (a * a + b * b);
    double mult = sqrt(d / (a * a + b * b));

    double ax, ay, bx, by;
    ax = x0 + b * mult;
    bx = x0 - b * mult;
    ay = y0 - a * mult;
    by = y0 + a * mult;

    result.first = QPoint(ax + O.x(), ay + O.y());
    result.second = QPoint(bx + O.x(), by + O.y());

    has_result = true;

    // 处理端点情况
    if (onSegment(P, Q, result.first)) {
        return result;
    }
    if (onSegment(P, Q, result.second)) {
        std::swap(result.first, result.second);
        return result;
    }

    // 交点在线段延长线上
    return result;
}


//原来的
Page filterOutsideCircle(const Page& pointLists, const QPoint& center, int radius) {
    Page outsideCircle;
    outsideCircle.zoom = pointLists.zoom;
    outsideCircle.offset = pointLists.offset;
    outsideCircle.lines << new Line();
    for (const Line* pointList : pointLists.lines) {
        QList<QPoint> outsideList;
        for (QPoint*const point : pointList->points) {
            int dx = point->x() - center.x();
            int dy = point->y() - center.y();
            int distanceSquared = dx * dx + dy * dy;
            if (distanceSquared > radius * radius) {
                outsideList.append(*point);
            } else {
                if (!outsideList.isEmpty()) {
                    Line*l = new Line();
                    l->color = pointList->color;
                    l->size = pointList->size;
                    for(QPoint p: outsideList){
                        l->points << new QPoint(p);
                    }
                    outsideCircle.lines.append(l);
                    outsideList = {};
                    continue;
                    delete pointList;
                }
            }
        }
        if (!outsideList.isEmpty()) {
            Line*l = new Line();
            l->color = pointList->color;
            l->size = pointList->size;
            for(QPoint p: outsideList){
                l->points << new QPoint(p);
            }
            outsideCircle.lines.append(l);
        }
        delete pointList;
    }
    return outsideCircle;
}

//实验功能，有巨大的bug (有没有好心人帮我修一下QAQ)
//Page filterOutsideCircle(const Page& pointLists, const QPoint& center, int radius) {
//    Page outsideCircle;
//    outsideCircle.zoom = pointLists.zoom;
//    outsideCircle.offset = pointLists.offset;
//    outsideCircle.lines << new Line();
//    for (const Line* pointList : pointLists.lines) {
//        QList<QPoint> outsideList;
//        QPoint*last = pointList->points.first();
//        int i=0;
//        Line*l = new Line();
//        l->color = pointList->color;
//        l->size = pointList->size;
//        for (QPoint*const point : pointList->points) {
//            bool has_result;
//            auto p = circleLineIntersect(center, radius, *last, *point, has_result);
//            if(has_result && ++i!=1){
//                l->points << last<< &p.first;
//                outsideCircle.lines.append(l);
//                Line*l = new Line();
//                l->color = pointList->color;
//                l->size = pointList->size;
//                l->points<<point << &p.second;
//            } else{
//                l->points<<point;
//            }
//            last = point;
//        }
//        outsideCircle.lines.append(l);
//    }
//    return outsideCircle;
//}
void CanvasWin::Clean(QPoint last, QPoint p){
    int len = sqrt(pow((last).x()-(p-offset()).x(),2)+pow((last).y()-(p-offset()).y(),2))*5/zoom();    // 对于显示
    eraserR = max(len, 10);
    eraserR = eraserR>100?100:eraserR;
    last = (last + offset())/zoom() - offset();
    p/=zoom();
    int len2 = sqrt(pow((last).x()-(p-offset()).x(),2)+pow((last).y()-(p-offset()).y(),2))*5/zoom();  // 对于擦除
    int eraserR2 = max(len2, 10);
    eraserR2 = eraserR2>100?100:eraserR2;
//    auto original = pages.at(page);
    pages.replace(page, new Page(filterOutsideCircle(*getpage(), p - offset(), eraserR2)));
//    if(original) {
//        delete original;
//        original = nullptr;
//    }
    //qDebug()<<pages.at(page).second.count();
    update();
}

void CanvasWin::mousePressEvent(QMouseEvent *event) {
    PressMouse = true;
    history<<QPair<int, Pages>{page,pages};
    emit OnUpdateHistory();
    mousePos = event->pos()/(Mode==0?zoom():1) - offset();
    if (event->button() == Qt::LeftButton && Mode==1) {
        Line*l = new Line();
        l->color = drawColor;
        l->size = 3.0/zoom();
        l->points << new QPoint(event->pos()/zoom() - offset());
//        qDebug()<<getpage()->lines;
        getpage()->lines << l;
        //_page << QPair<QColor, QList<QPoint>>{drawColor, {event->pos()/zoom - pages.at(page).second.first}};  // 新建一个空的线条
        //pages.replace(page, {pages.at(page).first ,{pages.at(page).second.first, _page}});  // 替换!
        update();
    } else if((event->button() == Qt::LeftButton && Mode==2) || (event->button() == Qt::RightButton && Mode==1)) {
        Clean(mousePos, event->pos());
        mousePos = event->pos() - offset();
    }
}

void CanvasWin::mouseMoveEvent(QMouseEvent *event) {
    emit this->setToolBarPostion(event->x()<width()*1/6?1:(event->x()>width()*5/6?2:0));
    if (PressMouse && Mode==1 && event->buttons() & Qt::LeftButton) {
        //QList<QPair<QColor, QList<QPoint>>> _page = pages.at(page).second; // 获取页面
        //_page.last().second << event->pos()/zoom - pages.at(page).first; // 这个页面最后一个线条, 新增点的记录
        //pages.replace(page, {pages.at(page).first, _page}); // 替换页面数据
        getpage()->lines.last()->points << new QPoint(event->pos()/zoom() - offset());
        update(); // 请求重绘
    } else if(PressMouse && Mode==0) {
        offset(QPoint(event->pos()/zoom() - mousePos));
        //pages.replace(page, {event->pos()/zoom() - mousePos, pages.at(page).second}); // 移动页面
        ui->labelTips->setVisible(true);
        ui->labelTips->setText(QString("X: %1, Y: %2").arg(offset().x()).arg(offset().y()));
        ui->labelTips->adjustSize();
        ui->labelTips->move(width()/2-ui->labelTips->width()/2,height()/2-ui->labelTips->height()/2);  // 显示位置相关
        update();
    } else if ((PressMouse && Mode==2)||(PressMouse&&event->buttons() & Qt::RightButton&&Mode==1)) {
        Clean(mousePos, event->pos());
        mousePos = event->pos() - offset();
    }
}

void CanvasWin::mouseReleaseEvent(QMouseEvent *event){
    ui->labelTips->setVisible(false);
    if (Mode==1 && event->buttons() & Qt::LeftButton) {
        //QList<QPair<QColor, QList<QPoint>>> _page = pages.at(page).second;  // 绘制完成后，处理线条, 避免再次绘制连在一起
        //_page << QPair<QColor, QList<QPoint>>{drawColor,QList<QPoint>()};
        //pages.replace(page, {pages.at(page).first, _page});
        getpage()->lines.last()->points << new QPoint(event->pos()/zoom() - offset());
    }
    emit OnDrawCompleted();
    PressMouse = false;
    eraserR = 0;
    update(); // 请求重绘
}

bool CanvasWin::eventFilter(QObject *watched, QEvent *event)  {
    if(watched == ui->CanvasWidget && event->type() == QEvent::Paint)
        {
           paintEvent(nullptr);
        }
        return QWidget::eventFilter(watched,event);
}
