#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "util/Preferences.h"
#include "util/models.h"
#include "util/urlservice.h"

void MainWindow::webViewTitleChanged(const QString &title)
{
    qDebug() << "Title changed to: " << title;

//    QString badge = "";
//    QRegExp regex("\\(([0-9]+)\\) VRSteam");

//    if(regex.indexIn(title) > -1) {
//        badge = regex.cap(1);
//    }

//    if(badge.isEmpty())
//        setWindowTitle("VRSteam");
//    else
//        setWindowTitle((badge.toInt() == 1 ? tr("%1 unread message") : tr("%1 unread messages")).arg(badge) + " - VRSteam");

//    notificationService->setApplicationBadge(badge);

    ui->title->setText(title);
}

void MainWindow::webViewUrlChanged(const QUrl & url)
{
    qDebug() << __FUNCTION__  + url.toString();
}

void MainWindow::linkClicked(const QUrl & url)
{
    qDebug() << __FUNCTION__  + url.toString();

    URLService::getInstance()->handleURL(url, ui->webView);
}

void MainWindow::downloadComplete(const QUrl & url)
{
    qDebug() << __FUNCTION__ << url;
}

void MainWindow::downloadProgress(const QUrl & url, int nPercentage)
{
    qDebug() << __FUNCTION__ << url << "(%" << nPercentage << ")";
}

void MainWindow::appInstallBegin(const AppInfo & appInfo)
{
    qDebug() << __FUNCTION__ << "(%" << appInfo.name() << ")";
}

void MainWindow::appInstallProgress(int percentage)
{
    qDebug() << __FUNCTION__ << "(%" << percentage << ")";
}

void MainWindow::appInstallComplete(const AppInfo & appInfo)
{
    qDebug() << __FUNCTION__ << "(%" << appInfo.name() << ")";
}

void MainWindow::webViewLoadStarted()
{
    qDebug() << __FUNCTION__;
}

void MainWindow::webViewLoadProgress(int progress)
{
    qDebug() << __FUNCTION__ << "(%" << progress << ")";
}

void MainWindow::webViewLoadFinished(bool)
{
    qDebug() << __FUNCTION__;
}
