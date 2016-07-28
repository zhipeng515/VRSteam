#include "downloaddialog.h"
#include "ui_downloaddialog.h"
#include "dialog/downloaditemwidget.h"

DownloadDialog::DownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadDialog)
{
    ui->setupUi(this);

    setFixedWidth(300);

    QListWidgetItem * item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(QSize(300, 80));
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, new DownloadItemWidget(this));
}

DownloadDialog::~DownloadDialog()
{
    delete ui;
}
