#include "downloaditemwidget.h"
#include "ui_downloaditemwidget.h"

DownloadItemWidget::DownloadItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadItemWidget)
{
    ui->setupUi(this);

    QPixmap minPixmap = this->style()->standardPixmap(QStyle::SP_MediaPause);
    ui->operatorButton->setIcon(minPixmap);

}

DownloadItemWidget::~DownloadItemWidget()
{
    delete ui;
}
