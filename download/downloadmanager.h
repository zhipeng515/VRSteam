#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QtGlobal>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QTimer>

#include "downloadmanagerHTTP.h"

#if QT_VERSION < 0x050000
#include <QFtp>
#include "downloadmanagerFTP.h"
#endif


class DownloadManager : public QObject
{
    Q_OBJECT

public:
    explicit DownloadManager(QObject *parent = 0);
    ~DownloadManager();
    static DownloadManager* getInstance(){
        static DownloadManager downloadManager;
        return &downloadManager;
    }

signals:
    void addLine(const QUrl & url, const QString & qsLine);
    void progress(const QUrl & url, const int nPercentage);
    void downloadComplete(const QUrl & url);

public slots:
    void download(const QUrl & url, const QString & localPath);
    void pause(const QUrl & url);
    void resume(const QUrl & url);

private slots:
    void localAddLine(const QUrl & url, const QString & qsLine);
    void localProgress(const QUrl & url, const int nPercentage);
    void localDownloadComplete(const QUrl & url);

private:
    QMap<QUrl, DownloadManagerHTTP*> httpDownloads;
#if QT_VERSION < 0x050000
    QList<DownloadManagerFTP*> ftpDownloads;
#endif
};

#endif // DOWNLOADMANAGER_H
