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
    util/modelbase.cpp \
    util/models.cpp \
    mainwindow_init.cpp \
    dialog/activitydialog.cpp \
    util/urlservice.cpp \
    util/httpservice.cpp \
    dialog/downloaddialog.cpp \
    view/downloaditemwidget.cpp \
    view/networkiconlabel.cpp \
    dialog/blurdialog.cpp \
    util/qgraphicsgloweffect.cpp

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
    util/modelbase.h \
    util/models.h \
    dialog/activitydialog.h \
    util/urlservice.h \
    util/httpservice.h \
    dialog/downloaddialog.h \
    view/downloaditemwidget.h \
    view/networkiconlabel.h \
    dialog/blurdialog.h \
    util/qgraphicsgloweffect.h

FORMS    += mainwindow.ui \
    dialog/aboutdialog.ui \
    dialog/PreferencesDialog.ui \
    dialog/activitydialog.ui \
    dialog/downloaddialog.ui \
    view/downloaditemwidget.ui

RC_FILE += VRSteam.rc

mac {
    HEADERS += notification/osx/osxnotificationservice.h \
        notification/osx/notificationcenterdelegate.h

    OBJECTIVE_SOURCES += notification/osx/osxnotificationservice.mm \
        notification/osx/notificationcenterdelegate.mm \
        mainwindow_osx.mm

    ICON = ./artwork/icon/icon.icns

    LIBS += -framework Foundation -framework AppKit

    QMAKE_POST_LINK += sed -i -e "s/@VERSION@/$$VERSION/g" "$$DESTDIR/$${TARGET}.app/Contents/Info.plist";
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
include ($$PWD/3rd-party/QtJson/src/QtJson.pri)
include ($$PWD/3rd-party/http/http.pri)

DISTFILES += \
    README.MD \
    html/activitys.json

TRANSLATIONS = language/zh_CN.ts
