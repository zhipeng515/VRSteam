#include "downloaditemwidget.h"
#include "ui_downloaditemwidget.h"
#include "util/models.h"
#include "util/localappmanager.h"
#include "download/downloadmanager.h"
#include "util/qgraphicsgloweffect.h"

#include <QUrl>

DownloadItemWidget::DownloadItemWidget(const int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadItemWidget),
    appId(id)
{
    ui->setupUi(this);

    QGraphicsGlowEffect * glowEffect = nullptr;
    glowEffect = new QGraphicsGlowEffect(this);
    glowEffect->setStrength(4);
    glowEffect->setBlurRadius(7);
    ui->nameLabel->setGraphicsEffect(glowEffect);

    glowEffect = new QGraphicsGlowEffect(this);
    glowEffect->setStrength(4);
    glowEffect->setBlurRadius(7);
    ui->infoLabel->setGraphicsEffect(glowEffect);

    glowEffect = new QGraphicsGlowEffect(this);
    glowEffect->setStrength(4);
    glowEffect->setBlurRadius(7);
    ui->operatorButton->setGraphicsEffect(glowEffect);

//    ui->progressBar->setStyleSheet("QProgressBar"
//                                   "{"
//                                        "border-radius: 5px;"
//                                        "background-color: #202020;"
//                                        "text-align: center;"
//                                   "}"
//                                   "QProgressBar::chunk"
//                                   "{"
//                                        "background-color: #05B8CC;"
//                                   "}"
//                                   );

    QPixmap minPixmap = this->style()->standardPixmap(QStyle::SP_MediaPause);
    ui->operatorButton->setIcon(minPixmap);

    connect(LocalAppManager::getInstance(), SIGNAL(appDownloadProgress(const QUrl& , int)),
            this, SLOT(appDownloadProgress(const QUrl&, int)));
    connect(LocalAppManager::getInstance(), SIGNAL(appDownloadError(const QUrl&, QNetworkReply::NetworkError)),
            this, SLOT(appDownloadError(const QUrl&, QNetworkReply::NetworkError)));
    connect(LocalAppManager::getInstance(), SIGNAL(appDownloadTimeout(const QUrl&)),
            this, SLOT(appDownloadTimeout(const QUrl&)));
    connect(LocalAppManager::getInstance(), SIGNAL(appDownloadPause(const QUrl&)),
            this, SLOT(appDownloadPause(const QUrl&)));
    connect(LocalAppManager::getInstance(), SIGNAL(appDownloadResume(const QUrl&)),
            this, SLOT(appDownloadResume(const QUrl&)));

    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid()) {
        ui->nameLabel->setText(appInfo->name());
        ui->iconLabel->setUrl(appInfo->iconUrl());
        ui->progressBar->setValue(0);
        if(DownloadManager::getInstance()->isDownloading(appInfo->downloadUrl()))
            updateInfo(tr("Downloading"), QStyle::SP_MediaPause, true);
        else if(DownloadManager::getInstance()->isDownloadPause(appInfo->downloadUrl()))
            updateInfo(tr("Pause"), QStyle::SP_MediaPlay, false);
        else if(DownloadManager::getInstance()->isDownloadError(appInfo->downloadUrl()))
            updateInfo(tr("Error"), QStyle::SP_MediaPlay, false);
        else if(DownloadManager::getInstance()->isDownloadTimeout(appInfo->downloadUrl()))
            updateInfo(tr("Timeout"), QStyle::SP_MediaPlay, false);
    }
}

DownloadItemWidget::~DownloadItemWidget()
{
    delete ui;
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
        updateInfo(tr("Error"), QStyle::SP_MediaPlay, false);
    }
}

void DownloadItemWidget::appDownloadTimeout(const QUrl & url)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid() && QUrl(appInfo->downloadUrl()) == url) {
        ui->progressBar->setVisible(false);
        updateInfo(tr("Timeout"), QStyle::SP_MediaPlay, false);
    }
}

void DownloadItemWidget::appDownloadPause(const QUrl & url)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid() && QUrl(appInfo->downloadUrl()) == url) {
        updateInfo(tr("Pause"), QStyle::SP_MediaPlay, true);
    }
}

void DownloadItemWidget::appDownloadResume(const QUrl & url)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid() && QUrl(appInfo->downloadUrl()) == url) {
        ui->progressBar->setVisible(false);
        updateInfo(tr("Downloading"), QStyle::SP_MediaPause, true);
    }
}

void DownloadItemWidget::on_operatorButton_clicked()
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid()) {
        if(DownloadManager::getInstance()->isDownloading(appInfo->downloadUrl())) {
            DownloadManager::getInstance()->pause(appInfo->downloadUrl());
        }
        else {
            DownloadManager::getInstance()->resume(appInfo->downloadUrl());
        }
    }
}

void DownloadItemWidget::updateInfo(const QString &info, QStyle::StandardPixmap pixmap, bool showProgressBar)
{
    ui->progressBar->setVisible(showProgressBar);
    ui->infoLabel->setText(info);
    ui->operatorButton->setIcon(this->style()->standardPixmap(pixmap));
}
