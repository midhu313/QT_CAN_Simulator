#include "log_manager.h"
#include <QObject>
#include <QString>
#include <QtDebug>
#include <QDate>
#include <QTime>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QFileInfoList>
#include <iostream>

LogManager LogManager::s_instance;

LogManager::LogManager()
{
    logFolderName = "logs";
    logFileName = logFolderName + "/logCurrent.txt";
}

LogManager *LogManager::getInstance()
{
    return &s_instance;
}

void LogManager::initLogFileName()
{
    logFileName = logFolderName + "/logCurrent.txt";
}

void LogManager::deleteOldLogs()
{
    QDir dir;
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Time | QDir::Reversed);
    dir.setPath(logFolderName);

    QFileInfoList list = dir.entryInfoList();
    if(list.size()>=LOG_FILE_COUNT){
        for(int i = LOG_FILE_COUNT;i<list.size();i++){
            QFile file(list.at(i).absoluteFilePath());
            file.remove();
        }
    }
}

void LogManager::rotateLogFiles()
{
    QString newLogFileName = QString(logFolderName + "/Log_%1_%2.txt")
            .arg(QDate::currentDate().toString("ddMMyyyy"))
            .arg(QTime::currentTime().toString("hh_mm_ss_zzz"));

    QFile::rename(logFileName, newLogFileName);
    deleteOldLogs();
    initLogFileName();
}

bool LogManager::initLogging()
{
    // Create folder for logfiles if not exists
    if(!QDir(logFolderName).exists())
    {
        QDir().mkdir(logFolderName);
    }
    deleteOldLogs();
    initLogFileName();
    QFile outFile(logFileName);
    if(outFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qInstallMessageHandler(LogManager::messageHandler);
        return true;
    }
    return false;
}

void LogManager::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type);
    Q_UNUSED(context);
    QFile outFile(s_instance.logFileName);
    if (outFile.size() > LOG_FILE_SIZE) {
        outFile.close();
        s_instance.rotateLogFiles();
        outFile.setFileName(s_instance.logFileName);
    }

    if (outFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream ts(&outFile);
        ts << msg << Qt::endl;
    }
}
