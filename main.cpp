#include "mainwindow.h"
#include "log_manager.h"
#include "defines.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(ORG_NAME);
    QCoreApplication::setOrganizationDomain(ORG_DOMAIN);
    QCoreApplication::setApplicationName(APP_NAME);
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/Res/Icons/icon.ico"));//Add Application Icon
    /***Initialize Logger with rotational file handling ***///Reference:https://andydunkel.net/2017/11/08/qt_log_file_rotation_with_qdebug/
    LogManager *lmngr;
    lmngr = LogManager::getInstance();
    lmngr->initLogging();
    /**********************************/
    MainWindow w;
    w.show();
    return a.exec();
}
