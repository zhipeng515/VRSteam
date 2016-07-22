#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include "util/regexputils.h"
#include "download/downloadmanager.h"
#include "util/Preferences.h"
#include "util/models.h"

void MainWindow::linkClicked(const QUrl & url)
{
    qDebug() << __FUNCTION__  + url.toString();

    QString strUrl = url.toString();

    if(RegExpUtils::getInstance()->isUrl(strUrl)) {
        if(!Preferences::getInstance()->isInBlacklist(strUrl))
           QDesktopServices::openUrl(url);
    }
    else {
        // Example : vrst://download?http://www.sina.com.cn/vrsteam.exe
        // Example : vrst://open?http://www.sina.com.cn
        QStringList customProtocol = RegExpUtils::getInstance()->matchCustomUrl(strUrl);
        if(customProtocol.count() > 2) {
            if(customProtocol[0] == "vrst") {
                if(customProtocol[1] == "download" && RegExpUtils::getInstance()->isUrl(customProtocol[2])) {
                    DownloadManager::getInstance()->download(customProtocol[2], Preferences::getInstance()->getDownloadPath());
                }
                else if(customProtocol[1] == "open" && RegExpUtils::getInstance()->isUrl(customProtocol[2])) {
                    ui->webView->load(customProtocol[2]);
                }
            }
        }
    }
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
