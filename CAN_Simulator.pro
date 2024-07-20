QT       += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    app_settings.cpp \
    db_manager.cpp \
    log_manager.cpp \
    main.cpp \
    mainwindow.cpp \
    modifyvehicleinfo.cpp \
    newvehicleinfo.cpp \
    utils.cpp

HEADERS += \
    app_settings.h \
    db_manager.h \
    defines.h \
    exprtk.hpp \
    log_manager.h \
    mainwindow.h \
    modifyvehicleinfo.h \
    newvehicleinfo.h \
    utils.h

FORMS += \
    mainwindow.ui \
    modifyvehicleinfo.ui \
    newvehicleinfo.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc
