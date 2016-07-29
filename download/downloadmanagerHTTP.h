#ifndef DOWNLOADMANAGERHTTP_H
#define DOWNLOADMANAGERHTTP_H

#include <QtGlobal>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QTimer>


class DownloadManagerHTTP : public QObject
{
    Q_OBJECT

public:
    explicit DownloadManagerHTTP(QObject *parent = 0);
    virtual ~DownloadManagerHTTP();

signals:
    void addLine(const QUrl& url, const QString & qsLine);
    void downloadProgress(const QUrl & url, const int nPercentage);
    void downloadBegin(const QUrl & url);
    void downloadPause(const QUrl & url);
    void downloadResume(const QUrl & url);
    void downloadComplete(const QUrl & url);
    void downloadError(const QUrl& url, QNetworkReply::NetworkError code);
    void downloadTimeout(const QUrl& url);

public slots:
    void download(const QUrl & url, const QString & localPath);
    void pause();
    void resume();
    bool isDownloading();

private slots:
    void download();
    void finishedHead();
    void finished();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void error(QNetworkReply::NetworkError code);
    void timeout();

private:
    QUrl _URL;
    QString _qsFileName;
    QNetworkAccessManager* _pManager;
    QNetworkRequest _CurrentRequest;
    QNetworkReply* _pCurrentReply;
    QFile* _pFile;
    int _nDownloadTotal;
    bool _bAcceptRanges;
    int _nDownloadSize;
    int _nDownloadSizeAtPause;
    QTimer _Timer;
};

#endif // DOWNLOADMANAGERHTTP_H
