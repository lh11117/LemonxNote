#include "save_open_lnbf.h"
#include <QByteArray>
#include <QDataStream>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace SAVE_OPEN_LNBF {
QString save(const Pages& pages) {
    QJsonArray pagesArray;
    for (const Page* page : pages) {
        QJsonObject pageObject;
        pageObject["zoom"] = page->zoom;
        pageObject["offset"] = QJsonObject{{"x", page->offset.x()}, {"y", page->offset.y()}};

        QJsonArray linesArray;
        for (const Line* line : page->lines) {
            QJsonObject lineObject;
            lineObject["color"] = QJsonObject{
                {"r", line->color.red()}, {"g", line->color.green()}, {"b", line->color.blue()}};
            lineObject["size"] = line->size;

            QJsonArray pointsArray;
            for (const QPoint* point : line->points) {
                pointsArray.append(QJsonObject{{"x", point->x()}, {"y", point->y()}});
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

void load(const QString& jsonData, Pages& pages) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    if (doc.isNull()) {
        return;
    }

    QJsonArray pagesArray = doc.array();

    for (const QJsonValue& pageValue : pagesArray) {
        QJsonObject pageObject = pageValue.toObject();

        Page* page = new Page;
        page->zoom = pageObject["zoom"].toDouble();
        QJsonObject offsetObject = pageObject["offset"].toObject();
        page->offset = QPoint(offsetObject["x"].toInt(), offsetObject["y"].toInt());

        QJsonArray linesArray = pageObject["lines"].toArray();
        for (const QJsonValue& lineValue : linesArray) {
            QJsonObject lineObject = lineValue.toObject();

            Line* line = new Line;
            QJsonObject colorObject = lineObject["color"].toObject();
            line->color = QColor(colorObject["r"].toInt(), colorObject["g"].toInt(), colorObject["b"].toInt());
            line->size = lineObject["size"].toInt();

            QJsonArray pointsArray = lineObject["points"].toArray();
            for (const QJsonValue& pointValue : pointsArray) {
                QJsonObject pointObject = pointValue.toObject();
                line->points.append(new QPoint(pointObject["x"].toInt(), pointObject["y"].toInt()));
            }

            page->lines.append(line);
        }

        pages.append(page);
    }
}
}
