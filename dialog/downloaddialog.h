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

    virtual void hideEvent(QHideEvent *event);
    virtual void showEvent(QShowEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual bool eventFilter(QObject *obj, QEvent *event);
public slots:
    void appDownloadBegin(const QUrl & url);
    void appDownloadComplete(const QUrl & url);
    void addDownloadItem(const AppInfo * appInfo);
    void resizeDialog();

private:
    Ui::DownloadDialog *ui;
};

#endif // DOWNLOADDIALOG_H
