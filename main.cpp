#include "mainwindow.h"
#include <QApplication>

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 忽略QPainter::setPen: Painter not active的警告
    if (msg.startsWith("QPainter::setPen: Painter not active"))
        return;
    if (msg.startsWith("QPainter::begin: Paint"))
        return;
    if (msg.startsWith("QPainter::begin"))
        return;

    // 对于其他消息,使用Qt默认的消息处理器
    QByteArray localMsg = msg.toLocal8Bit();
    /*switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        break;
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        abort();
    }*/
    if (type == QtDebugMsg)
    fprintf(stdout, "%s\n", localMsg.constData());
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName(QString("LemonxTech"));
    QCoreApplication::setApplicationName(QString("LemonxNote"));
    //qInstallMessageHandler(myMessageHandler);
    MainWindow w;
    w.show();
    return a.exec();
}
