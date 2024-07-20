#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <QObject>

#define LOG_FILE_SIZE  1024*100
#define LOG_FILE_COUNT 2

class LogManager
{
private:

    QString logFileName;
    QString logFolderName;
    static LogManager s_instance;

    void initLogFileName(void);
    void deleteOldLogs(void);
    void rotateLogFiles(void);
public:
    LogManager();
    static LogManager *getInstance();

    bool initLogging(void);
    static void messageHandler(QtMsgType type,const QMessageLogContext &context,const QString &msg);
};

#endif // LOG_MANAGER_H
