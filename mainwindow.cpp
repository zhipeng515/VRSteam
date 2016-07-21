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
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QSimpleUpdater.h"


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
    checkSelfUpdate();

    initActions();
    initMenus();
    initTitleBar();

    initMainWebView();
    initWebService();

    readSettings();

    nativeSetup();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::webViewTitleChanged(const QString &title)
{
    qDebug() << "Title changed to: " << title;

    QString badge = "";
    QRegExp regex("\\(([0-9]+)\\) VRSteam");

    if(regex.indexIn(title) > -1) {
        badge = regex.cap(1);
    }

    if(badge.isEmpty())
        setWindowTitle("VRSteam");
    else
        setWindowTitle((badge.toInt() == 1 ? tr("%1 unread message") : tr("%1 unread messages")).arg(badge) + " - VRSteam");

    notificationService->setApplicationBadge(badge);
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
                                "alex-spataru/QSimpleUpdater/master/tutorial/"
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

void MainWindow::updateCheckingFinished(QString url)
{
    QSimpleUpdater * updater = QSimpleUpdater::getInstance();
    if(!updater->getNotifyOnUpdate(url) && updater->getUpdateAvailable(url)) {
        qDebug() << __FUNCTION__ << updater->getChangelog(url);
        updater->startDownload(url);
    }
}

void MainWindow::updateDownloadFinished(QString url)
{
    qDebug() << __FUNCTION__ << url;
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
