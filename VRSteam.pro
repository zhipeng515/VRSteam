#-------------------------------------------------
#
# Project created by QtCreator 2015-08-27T00:27:12
#
#-------------------------------------------------

QT       += core gui webchannel

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webenginewidgets

CONFIG += c++11

TARGET = VRSteam
TEMPLATE = app
VERSION = 0.2.1

CONFIG(debug, debug|release) {
    DESTDIR = bin/debug
} else {
    DESTDIR = bin/release
}

OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/rcc
UI_DIR = $$DESTDIR/ui

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += main.cpp\
    mainwindow.cpp \
    view/webpage.cpp \
    notification/notificationservice.cpp \
    notification/nullnotificationservice.cpp \
    notification/TrayNotificationService.cpp \
    dialog/AboutDialog.cpp \
    dialog/PreferencesDialog.cpp \
    util/Preferences.cpp \
    download/downloadmanager.cpp \
    download/downloadmanagerFTP.cpp \
    download/downloadmanagerHTTP.cpp \
    download/download.cpp \
    view/webview.cpp \
    mainwindow_service.cpp \
    util/regexputils.cpp \
    util/localappmanager.cpp \
    util/appinfo.cpp

HEADERS  += mainwindow.h \
    notification/notificationservice.h \
    notification/nullnotificationservice.h \
    notification/TrayNotificationService.h \
    view/webpage.h \
    dialog/AboutDialog.h \
    dialog/PreferencesDialog.h \
    util/Preferences.h \
    download/downloadmanager.h \
    download/downloadmanagerFTP.h \
    download/downloadmanagerHTTP.h \
    download/download.h \
    view/webview.h \
    util/regexputils.h \
    util/localappmanager.h \
    util/appinfo.h

FORMS    += mainwindow.ui \
    dialog/aboutdialog.ui \
    dialog/PreferencesDialog.ui

RC_FILE += VRSteam.rc

mac {
    HEADERS += notification/osx/osxnotificationservice.h \
        notification/osx/notificationcenterdelegate.h

    OBJECTIVE_SOURCES += notification/osx/osxnotificationservice.mm \
        notification/osx/notificationcenterdelegate.mm \
        mainwindow_osx.mm

    ICON = ./artwork/icon/icon.icns

    LIBS += -framework Foundation -framework AppKit
}

unix:!mac {
    QT += dbus

    SOURCES += notification/linux/FreedesktopNotificationService.cpp

    HEADERS += notification/linux/FreedesktopNotificationService.h

    DBUS_INTERFACES =  notification/linux/org.freedesktop.Notifications.xml
}

RESOURCES += \
    resources.qrc

include ($$PWD/3rd-party/QSimpleUpdater/QSimpleUpdater.pri)
