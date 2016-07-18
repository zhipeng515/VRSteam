#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include "util/regexputils.h"
#include "download/downloadmanager.h"
#include "util/Preferences.h"

void MainWindow::linkClicked(const QUrl & url)
{
    qDebug() << __FUNCTION__  + url.toString();

    QString strUrl = url.toString();

    if(RegExpUtils::Instance().isUrl(strUrl)) {
        if(!Preferences::Instance().isInBlacklist(strUrl))
           QDesktopServices::openUrl(url);
    }
    else {
        // Example : vrst://download?http://www.sina.com.cn/vrsteam.exe
        // Example : vrst://open?http://www.sina.com.cn
        QStringList customProtocol = RegExpUtils::Instance().matchCustomUrl(strUrl);
        if(customProtocol.count() > 2) {
            if(customProtocol[0] == "vrst") {
                if(customProtocol[1] == "download" && RegExpUtils::Instance().isUrl(customProtocol[2])) {
                    downloadManager->download(customProtocol[2], Preferences::Instance().getDownloadPath());
                }
                else if(customProtocol[1] == "open" && RegExpUtils::Instance().isUrl(customProtocol[2])) {
                    ui->webView->load(customProtocol[2]);
                }
            }
        }
    }
}

void MainWindow::downloadComplete()
{
    qDebug() << __FUNCTION__;
}

void MainWindow::downloadProgress(int nPercentage)
{
    qDebug() << __FUNCTION__ << "(%" << nPercentage << ")";
}
