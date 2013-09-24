#-------------------------------------------------
#
# Project created by QtCreator 2013-08-16T09:38:17
#
#-------------------------------------------------

QT       += sql script

QT       -= gui

TARGET = rtreePg
TEMPLATE = lib

DEFINES += RTREEPG_LIBRARY

SOURCES += rtreepg.cpp \
    logger.cpp

HEADERS += rtreepg.h\
        rtreePg_global.h \
    logger.h \
    utils.h

INCLUDEPATH += /usr/include/qt4 /home/pedro/NetBeansProjects/rtreePgIndex/src /home/pedro/NetBeansProjects/rtreePgIndex/include
INCLUDEPATH += /home/pedro/NetBeansProjects/rtreePgIndex/include/gbdi
INCLUDEPATH += /usr/include/postgresql/9.1/server/

QMAKE_CXXFLAGS_RELEASE +=-fpic -shared

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE3F53382
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = rtreePg.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
