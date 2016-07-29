#ifndef DOWNLOADITEMWIDGET_H
#define DOWNLOADITEMWIDGET_H

#include <QWidget>
#include <QNetworkReply>
#include <QStyle>

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
    void appDownloadError(const QUrl & url, QNetworkReply::NetworkError code);
    void appDownloadPause(const QUrl & url);
    void appDownloadResume(const QUrl & url);
    void appDownloadTimeout(const QUrl & url);

    void updateInfo(const QString &info, QStyle::StandardPixmap pixmap);

private slots:
    void on_operatorButton_clicked();

private:
    Ui::DownloadItemWidget *ui;
    int  appId;
};

#endif // DOWNLOADITEMWIDGET_H
