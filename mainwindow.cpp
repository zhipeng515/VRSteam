/*
 * This file is part of WhatsQt.
 *
 * WhatsQt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WhatsQt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WhatsQt.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QRegExp>
#include <QSettings>
#include <QPropertyAnimation>
#include <QDesktopWidget>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QSimpleUpdater.h"
#include "dialog/activitydialog.h"
#include "util/models.h"
#include "util/httpservice.h"


JSNotifcationWrapper::JSNotifcationWrapper(NotificationService *service, QObject *parent)
    : QObject(parent),
      notificationService(service)
{
    connect(notificationService, &NotificationService::notificationClicked, this, &JSNotifcationWrapper::onNotificationClicked);
    connect(notificationService, &NotificationService::notificationReplied, this, &JSNotifcationWrapper::onNotificationReplied);
}

void JSNotifcationWrapper::setNotificationService(NotificationService *notificationService) {
    this->notificationService = notificationService;
}

void JSNotifcationWrapper::deliverNotification(const QString &title, const QMap<QString, QVariant> &options)
{
    qDebug() << "Received notification: " << title << " " << options;

    if(qApp->activeWindow())
    {
        qDebug() << "Ignoring notification due to active window";
        return;
    }

    Notification notification;
    notification.setTitle(title);
    notification.setInformativeText(options["body"].toString());
    notification.setIdentifier(options["tag"].toString());

    notificationService->deliverNotification(notification);
}

void JSNotifcationWrapper::onNotificationClicked(const Notification &notification)
{
    QMap<QString, QVariant> options;
    options["body"] = notification.getInformativeText();
    options["tag"] = notification.getIdentifier();
    emit notificationClicked(notification.getTitle(), options);
}

void JSNotifcationWrapper::onNotificationReplied(const Notification &notification, const QString &reply)
{
    QMap<QString, QVariant> options;
    options["body"] = notification.getInformativeText();
    options["tag"] = notification.getIdentifier();
    emit notificationReplied(notification.getTitle(), options, reply);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    notificationService(nullptr),
    notificationWrapper(nullptr),
    beginDrag(false)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

#ifndef Q_OS_OSX
    setWindowIcon(QIcon(":/artwork/icon/icon256.png"));
#endif
    initActions();
//    initMenus();
    initTitleBar();

    initMainWebView();
    initWebService();
    initHttpService();

    readSettings();

    nativeSetup();

    checkSelfUpdate();
    checkActivitys();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();

#ifdef Q_OS_OSX
    hide();
    event->ignore();
#else
    QMainWindow::closeEvent(event);
#endif
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
    settings.endGroup();
}

static const QString DEFS_URL = "https://raw.githubusercontent.com/"
                                "zhipeng515/QSimpleUpdater/master/tutorial/"
                                "definitions/updates.json";
void MainWindow::checkSelfUpdate()
{
    QSimpleUpdater * updater = QSimpleUpdater::getInstance();
    /* Apply the settings */
    updater->setModuleVersion(DEFS_URL, APP_VERSION);
    updater->setNotifyOnFinish(DEFS_URL, true);
    updater->setNotifyOnUpdate(DEFS_URL, true);
    updater->setDownloaderEnabled(DEFS_URL, true);

    /* Check for updates */
    updater->checkForUpdates(DEFS_URL);

    connect (updater, SIGNAL (checkingFinished (QString)),
             this,    SLOT (updateCheckingFinished (QString)));
    connect (updater, SIGNAL (downloadFinished (QString, QString)),
             this,    SLOT (updateDownloadFinished (QString, QString)));
}

void MainWindow::checkActivitys()
{
    QMap<QString, QString> params;
//    HttpService::getInstance()->request("activity", params,
//        [&](const QByteArray &bytes){
            ActivityInfo activityInfo;
            QFile file(":/html/activitys.json");
            file.open(QIODevice::ReadOnly);
            activityInfo.importFromJson(file.readAll());
            if(activityInfo.urls().count() > 0){
                auto activityDialog = new ActivityDialog(this);
                activityDialog->setActivityUrls(activityInfo.urls());
                activityDialog->open();
            }
//        },
//        [&](const QString &message){
//            qDebug() << message;
//        }
//    );
}

void MainWindow::updateCheckingFinished(QString url)
{
    QSimpleUpdater * updater = QSimpleUpdater::getInstance();
    qDebug() << __FUNCTION__ << updater->getChangelog(url);
}

void MainWindow::updateDownloadFinished(QString url, QString filepath)
{
    qDebug() << __FUNCTION__ << url << filepath;
}

bool MainWindow::event(QEvent *event)
{
    if(event->type() == QEvent::WindowActivate)
    {
        qDebug() << "Window becoming active";
        notificationService->dismissNotifications();
    }

    return QMainWindow::event(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        beginDrag = true;
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton && beginDrag) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    beginDrag = false;
}

void MainWindow::showEvent(QShowEvent *event)
{
//    QDesktopWidget* desktopWidget = QApplication::desktop();
//    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
//    animation->setDuration(2000);
//    int w = desktopWidget->availableGeometry().width();
//    int h = desktopWidget->availableGeometry().height();
//    animation->setStartValue(QRect(w /2 - size().width()/2,
//                                   0,
//                                   size().width(), size().height()));
//    animation->setEndValue(QRect(w /2 - size().width()/2,
//                                 h / 2 - size().height() / 2,
//                                 size().width(), size().height()));
//    animation->setEasingCurve(QEasingCurve::OutElastic);
//    animation->start();

//    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
//    animation->setDuration(2000);
//    animation->setStartValue(0);
//    animation->setEndValue(1);
//    animation->start();

    QMainWindow::showEvent(event);
}

void MainWindow::notificationClicked(const Notification &notification)
{
    Q_UNUSED(notification)
    qDebug() << "Notification clicked";

}

void MainWindow::notificationReplied(const Notification &notification, const QString &reply)
{
    Q_UNUSED(notification)
    Q_UNUSED(reply)
    qDebug() << "Notification replied";
}
