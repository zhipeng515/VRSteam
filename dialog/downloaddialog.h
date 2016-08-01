#ifndef DOWNLOADDIALOG_H
#define DOWNLOADDIALOG_H

#include <QDialog>

namespace Ui {
class DownloadDialog;
}

class AppInfo;
class DownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DownloadDialog(QWidget *parent = 0);
    ~DownloadDialog();

    void resizeEvent(QResizeEvent *event);

public slots:
    void appDownloadBegin(const QUrl & url);
    void addDownloadItem(const AppInfo * appInfo);

private:
    Ui::DownloadDialog *ui;
};

#endif // DOWNLOADDIALOG_H
