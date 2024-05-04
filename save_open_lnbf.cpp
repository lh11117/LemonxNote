#include "save_open_lnbf.h"

/*namespace SAVE_OPEN_LNBF {
    // QWaitCondition: Destroyed while threads are still waiting
    // terminate called after throwing an instance of 'std::bad_alloc'
    //   what():  std::bad_alloc
    bool save(Pages p, QString path) {
        QFile f(path);
        if (!f.open(QIODevice::ReadWrite | QFile::Text)) {
            return false;
        }
        QDomDocument doc;
        QDomProcessingInstruction instruction;
        instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
        doc.appendChild(instruction);
        // 创建根节点
        QDomElement root = doc.createElement("Blackboard");
        doc.appendChild(root);
        int i = 0;
        for (auto page: p) {
            QPoint offset = page.first;
            QDomElement one_page = doc.createElement("Page");
            one_page.setAttribute("num", i++);
            QDomElement Offset = doc.createElement("Offset");
            Offset.setAttribute("x", offset.x());
            Offset.setAttribute("y", offset.y());
            QDomElement lines = doc.createElement("Lines");
            int j = 0;
            for (auto line: page.second){
                QDomElement one_line = doc.createElement("Line");
                one_line.setAttribute("num", j++);
                QDomElement color = doc.createElement("Color");
                color.setAttribute("r", line.first.red());
                color.setAttribute("g", line.first.green());
                color.setAttribute("b", line.first.blue());
                one_line.appendChild(color);
                QDomElement points = doc.createElement("Points");
                for (QPoint p: line.second){
                    QDomElement one_point = doc.createElement("p");
                    one_point.setAttribute("x", p.x());
                    one_point.setAttribute("y", p.y());
                    points.appendChild(one_point);
                }
                one_line.appendChild(points);
                lines.appendChild(one_line);
            }
            one_page.appendChild(Offset);
            one_page.appendChild(lines);
            root.appendChild(one_page);
        }
        f.write(doc.toString().toLocal8Bit().data());
        f.close();
        return true;
    }
}*/


#include <QByteArray>
#include <QDataStream>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace SAVE_OPEN_LNBF {
QString save(const Pages& pages) {
    QJsonArray pagesArray;
    for (const auto& page : pages) {
        const QPoint& offset = page.first;
        const QList<QPair<QColor, QList<QPoint>>>& lines = page.second;

        QJsonObject pageObject;
        pageObject["offset"] = QJsonObject{{"x", offset.x()}, {"y", offset.y()}};

        QJsonArray linesArray;
        for (const auto& line : lines) {
            const QColor& color = line.first;
            const QList<QPoint>& points = line.second;

            QJsonObject lineObject;
            lineObject["color"] = QJsonObject{
                {"r", color.red()}, {"g", color.green()}, {"b", color.blue()}};

            QJsonArray pointsArray;
            for (const QPoint& point : points) {
                pointsArray.append(QJsonObject{{"x", point.x()}, {"y", point.y()}});
            }
            lineObject["points"] = pointsArray;

            linesArray.append(lineObject);
        }
        pageObject["lines"] = linesArray;

        pagesArray.append(pageObject);
    }

    QJsonDocument doc(pagesArray);
    QString jsonData = doc.toJson(QJsonDocument::Compact);
    return jsonData;
}

void load(const QString& jsonData, Pages &p) {
    Pages pages;

    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    if (doc.isNull()) {
        return;
    }

    QJsonArray pagesArray = doc.array();

    for (const QJsonValue& pageValue : pagesArray) {
        QJsonObject pageObject = pageValue.toObject();

        QPoint offset;
        QJsonObject offsetObject = pageObject["offset"].toObject();
        offset.setX(offsetObject["x"].toInt());
        offset.setY(offsetObject["y"].toInt());

        QList<QPair<QColor, QList<QPoint>>> lines;
        QJsonArray linesArray = pageObject["lines"].toArray();
        for (const QJsonValue& lineValue : linesArray) {
            QJsonObject lineObject = lineValue.toObject();

            QColor color;
            QJsonObject colorObject = lineObject["color"].toObject();
            color.setRed(colorObject["r"].toInt());
            color.setGreen(colorObject["g"].toInt());
            color.setBlue(colorObject["b"].toInt());

            QList<QPoint> points;
            QJsonArray pointsArray = lineObject["points"].toArray();
            for (const QJsonValue& pointValue : pointsArray) {
                QJsonObject pointObject = pointValue.toObject();
                QPoint point(pointObject.value("x").toInt(), pointObject.value("y").toInt());
                points.append(point);
            }

            lines.append(QPair<QColor, QList<QPoint>>(color, points));
        }

        pages.append(QPair<QPoint, QList<QPair<QColor, QList<QPoint>>>>(offset, lines));
    }

    p = pages;
}
}
