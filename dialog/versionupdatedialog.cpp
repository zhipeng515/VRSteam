#include "versionupdatedialog.h"
#include "ui_versionupdatedialog.h"
#include "QSimpleUpdater.h"

VersionUpdateDialog::VersionUpdateDialog(const QString &url, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VersionUpdateDialog),
    updateUrl(url)
{
    ui->setupUi(this);

    setWindowTitle(tr("New version found"));
    setFixedSize(540, 400);

    QSimpleUpdater * updater = QSimpleUpdater::getInstance();
    ui->changelog->setText(updater->getChangelog(updateUrl));

    QString title = "<h3>"
                    + tr ("Version %1 of %2 has been released!")
                    .arg (updater->getLatestVersion(url)).arg (updater->getModuleName(url))
                    + "</h3>";
    ui->title->setText(title);
}

VersionUpdateDialog::~VersionUpdateDialog()
{
    delete ui;
}

void VersionUpdateDialog::on_download_clicked()
{
    accept();
}

void VersionUpdateDialog::on_cancel_clicked()
{
    reject();
}
