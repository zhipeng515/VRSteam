#ifndef VERSIONUPDATEDIALOG_H
#define VERSIONUPDATEDIALOG_H

#include <QDialog>

namespace Ui {
class VersionUpdateDialog;
}

class VersionUpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VersionUpdateDialog(const QString &url, QWidget *parent = 0);
    ~VersionUpdateDialog();

private slots:
    void on_download_clicked();

    void on_cancel_clicked();

private:
    Ui::VersionUpdateDialog *ui;
    const QString updateUrl;
};

#endif // VERSIONUPDATEDIALOG_H
