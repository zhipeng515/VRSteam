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
    bool isDownloadError();
    bool isDownloadTimeout();
    bool isDownloadPause();
    bool isDownloadComplete();

private slots:
    void download();
    void finishedHead();
    void finished();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void error(QNetworkReply::NetworkError code);
    void timeout();
    void requestHead();
    void stopRequest();

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
    enum HTTP_DOWNLOAD_STATE{
        HTTP_DOWNLOAD_UNKNOW = 0,
        HTTP_DOWNLOAD_REQUEST_HEAD,
        HTTP_DOWNLOADING,
        HTTP_DOWNLOAD_PAUSE,
        HTTP_DOWNLOAD_ERROR,
        HTTP_DOWNLOAD_TIMEOUT,
        HTTP_DOWNLOAD_COMPLETE
    };
    HTTP_DOWNLOAD_STATE _state;
};

#endif // DOWNLOADMANAGERHTTP_H
