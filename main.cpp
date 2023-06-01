#include "mainwindow.h"

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QTextCodec>
#include <iostream>

void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QFile file("log.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        switch (type) {
        case QtDebugMsg:
            stream << "Debug: " << timestamp << " - " << msg << Qt::endl;
            break;
        case QtInfoMsg:
            stream << "Info: " << timestamp << " - " << msg << Qt::endl;
            break;
        case QtWarningMsg:
            stream << "Warning: " << timestamp << " - " << msg << Qt::endl;
            break;
        case QtCriticalMsg:
            stream << "Critical: " << timestamp << " - " << msg << Qt::endl;
            break;
        case QtFatalMsg:
            stream << "Fatal: " << timestamp << " - " << msg << Qt::endl;
            break;
        }
    }
}

int main(int argc, char* argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    qInstallMessageHandler(customMessageHandler);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
