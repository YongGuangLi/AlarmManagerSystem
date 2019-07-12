#-------------------------------------------------
#
# Project created by QtCreator 2017-05-24T18:38:15
#
#-------------------------------------------------

QT       += core gui xml sql concurrent network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = AlarmDisp
TEMPLATE    = app
MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj
DESTDIR     = bin

include($$PWD/log4qt/log4qt.pri)

SOURCES     += main.cpp \
    framelesshelper.cpp \
    loginwindow.cpp \
    loginnetsetwindow.cpp \
    rotatewidget.cpp \
    basewindow.cpp \
    dbpage.cpp \
    modifypasswdwindow.cpp \
    databasehelper.cpp \
    alarmdata.cpp \
    RtdbMessage.pb.cpp \
    alarmreportparse.cpp \
    historyalarmquery.cpp \
    RedisHelper.cpp \
    baseconfig.cpp \
    statisticanalysis.cpp \
    floatingwindow.cpp \
    titlebar.cpp \
    userconfig.cpp \
    settingdialog.cpp \
    datamodel.cpp \
    roleconfig.cpp \
    confighelper.cpp \
    mainwindow.cpp \
    frmmessagebox.cpp \
    iconhelper.cpp \
    realtimealarm.cpp \
    filterbay.cpp \
    versiondialog.cpp \
    pushbutton.cpp

HEADERS     += iconhelper.h \
    framelesshelper.h \
    loginwindow.h \
    loginnetsetwindow.h \
    rotatewidget.h \
    basewindow.h \
    dbpage.h \
    modifypasswdwindow.h \
    versiondialog.h \
    databasehelper.h \
    alarmdata.h \
    RtdbMessage.pb.h \
    alarmreportparse.h \
    historyalarmquery.h \
    RedisHelper.h \
    baseconfig.h \
    statisticanalysis.h \
    floatingwindow.h \
    titlebar.h \
    userconfig.h \
    settingdialog.h \
    datamodel.h \
    CommonObject.h \
    roleconfig.h \
    confighelper.h \
    mainwindow.h \
    myhelper.h \
    frmmessagebox.h \
    realtimealarm.h \
    filterbay.h \
    pushbutton.h

FORMS       += MainWindow.ui \
    loginwindow.ui \
    loginnetsetwindow.ui \
    modifypasswdwindow.ui \
    historyalarmquery.ui \
    baseconfig.ui \
    statisticanalysis.ui \
    floatingwindow.ui \
    userconfig.ui \
    settingdialog.ui \
    roleconfig.ui \
    mainwindow.ui \
    frmmessagebox.ui \
    realtimealarm.ui \
    filterbay.ui \
    versiondialog.ui

RESOURCES   += \
    resource.qrc
RESOURCES   +=
CONFIG      += qt warn_off
INCLUDEPATH += $$PWD

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/protobuf-3.3.0/lib/release/ -lprotobuf-lite
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/protobuf-3.3.0/lib/debug/ -lprotobuf-lite
else:unix: LIBS += -L$$PWD/protobuf-3.3.0/lib/ -lprotobuf-lite

INCLUDEPATH += $$PWD/protobuf-3.3.0/include
DEPENDPATH += $$PWD/protobuf-3.3.0/include


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libacl/lib/release/ -lacl_cpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libacl/lib/debug/ -lacl_cpp
else:unix: LIBS += -L$$PWD/libacl/lib/ -lacl_cpp


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libacl/lib/release/ -lacl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libacl/lib/debug/ -lacl
else:unix: LIBS += -L$$PWD/libacl/lib/ -lacl

INCLUDEPATH += $$PWD/libacl/include
DEPENDPATH += $$PWD/libacl/include


#INCLUDEPATH += /home/tools/wireshark-2.0.16
#LIBS += -L/usr/local/wireshark-2.0.16/lib -lwireshark
