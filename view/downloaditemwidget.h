#ifndef DOWNLOADITEMWIDGET_H
#define DOWNLOADITEMWIDGET_H

#include <QWidget>

namespace Ui {
class DownloadItemWidget;
}

class DownloadItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadItemWidget(const int id, QWidget *parent = 0);
    ~DownloadItemWidget();

public slots:
    void appDownloadComplete(const QUrl & url);
    void appDownloadProgress(const QUrl & url, int nPercentage);

private slots:
    void on_operatorButton_clicked();

private:
    Ui::DownloadItemWidget *ui;
    int  appId;
};

#endif // DOWNLOADITEMWIDGET_H
