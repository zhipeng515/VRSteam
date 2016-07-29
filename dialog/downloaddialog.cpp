#include "downloaddialog.h"
#include "ui_downloaddialog.h"
#include "view/downloaditemwidget.h"
#include "util/models.h"
#include "util/localappmanager.h"

DownloadDialog::DownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadDialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setFixedWidth(320);

    QList<AppInfo*> downloadApps = LocalAppManager::getInstance()->getDownloadApps();
    for(auto appInfo = downloadApps.begin(); appInfo != downloadApps.end(); appInfo++) {
        addDownloadItem( *appInfo);
    }

    connect(LocalAppManager::getInstance(), SIGNAL(appDownloadBegin(const QUrl&)),
            this, SLOT(appDownloadBegin(const QUrl&)));
}

DownloadDialog::~DownloadDialog()
{
    delete ui;
}

void DownloadDialog::addDownloadItem(const AppInfo * appInfo)
{
    QListWidgetItem * item = new QListWidgetItem(ui->listWidget);
    item->setData(Qt::UserRole, appInfo->id());
    item->setSizeHint(QSize(320, 80));
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, new DownloadItemWidget(appInfo->id(), this));
}

void DownloadDialog::appDownloadBegin(const QUrl & url)
{
    AppInfo * appInfo = LocalAppManager::getInstance()->getDownloadApp(url);
    if(appInfo) {
        addDownloadItem(appInfo);
    }
}
