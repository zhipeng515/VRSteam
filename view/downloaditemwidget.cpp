#include "downloaditemwidget.h"
#include "ui_downloaditemwidget.h"
#include "util/models.h"
#include "util/localappmanager.h"
#include "download/downloadmanager.h"

#include <QUrl>

DownloadItemWidget::DownloadItemWidget(const int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadItemWidget),
    appId(id)
{
    ui->setupUi(this);

    QPixmap minPixmap = this->style()->standardPixmap(QStyle::SP_MediaPause);
    ui->operatorButton->setIcon(minPixmap);

    connect(LocalAppManager::getInstance(), SIGNAL(appDownloadComplete(const QUrl&)),
            this, SLOT(appDownloadComplete(const QUrl&)));
    connect(LocalAppManager::getInstance(), SIGNAL(appDownloadProgress(const QUrl& , int)),
            this, SLOT(appDownloadProgress(const QUrl&, int)));
    connect(LocalAppManager::getInstance(), SIGNAL(appDownloadError(const QUrl&, QNetworkReply::NetworkError)),
            this, SLOT(appDownloadError(const QUrl&, QNetworkReply::NetworkError)));
    connect(LocalAppManager::getInstance(), SIGNAL(appDownloadTimeout(const QUrl&)),
            this, SLOT(appDownloadTimeout(const QUrl&)));

    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid()) {
        ui->nameLabel->setText(appInfo->name());
        ui->iconLabel->setUrl(appInfo->iconUrl());
        updateInfo(tr("Downloading"), QStyle::SP_MediaPause);
    }
}

DownloadItemWidget::~DownloadItemWidget()
{
    delete ui;
}

void DownloadItemWidget::appDownloadComplete(const QUrl & url)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid() && QUrl(appInfo->downloadUrl()) == url)
        deleteLater();
}

void DownloadItemWidget::appDownloadProgress(const QUrl & url, int nPercentage)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid() && QUrl(appInfo->downloadUrl()) == url) {
        ui->progressBar->setValue(nPercentage);
    }
}

void DownloadItemWidget::appDownloadError(const QUrl & url, QNetworkReply::NetworkError code)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid() && QUrl(appInfo->downloadUrl()) == url) {
        ui->progressBar->setVisible(false);
        updateInfo(tr("Error"), QStyle::SP_MediaPlay);
    }
}

void DownloadItemWidget::appDownloadTimeout(const QUrl & url)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid() && QUrl(appInfo->downloadUrl()) == url) {
        ui->progressBar->setVisible(false);
        updateInfo(tr("Timeout"), QStyle::SP_MediaPlay);
    }
}

void DownloadItemWidget::on_operatorButton_clicked()
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid()) {
        if(DownloadManager::getInstance()->isDownloading(appInfo->downloadUrl())) {
            DownloadManager::getInstance()->pause(appInfo->downloadUrl());
            updateInfo(tr("Pause"), QStyle::SP_MediaPlay);
        }
        else {
            DownloadManager::getInstance()->resume(appInfo->downloadUrl());
            updateInfo(tr("Downloading"), QStyle::SP_MediaPause);
        }
    }
}

void DownloadItemWidget::updateInfo(const QString &info, QStyle::StandardPixmap pixmap)
{
    ui->infoLabel->setText(info);
    ui->operatorButton->setIcon(this->style()->standardPixmap(pixmap));
}
