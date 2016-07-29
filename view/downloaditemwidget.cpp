#include "downloaditemwidget.h"
#include "ui_downloaditemwidget.h"
#include "util/models.h"
#include "util/localappmanager.h"

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

    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid()) {
        ui->nameLabel->setText(appInfo->name());
        ui->iconLabel->setUrl(appInfo->iconUrl());
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
    if(appInfo->isValid() && QUrl(appInfo->downloadUrl()) == url)
        ui->progressBar->setValue(nPercentage);
}

void DownloadItemWidget::on_operatorButton_clicked()
{

}
