#ifndef DOWNLOADMANAGERFTP_H
#define DOWNLOADMANAGERFTP_H

#include <QtGlobal>

#if QT_VERSION < 0x050000

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QTimer>
#include <QFtp>


class DownloadManagerFTP : public QObject
{
    Q_OBJECT

public:
    explicit DownloadManagerFTP(QObject *parent = 0);
    virtual ~DownloadManagerFTP();

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
    void finished();
    void error(QNetworkReply::NetworkError code);
    void timeout();
    void stateChanged(int state);
    void commandStarted(int id);
    void commandFinished(int id, bool error);
    void rawCommandReply(int replyCode, const QString & detail);
    void readyRead();

private:
    static QString ftpCmd(QFtp::Command eCmd);
    void ftpFinishedHelp();

    QUrl _URL;
    QString _qsFileName;
    QFtp *_pFTP;
    QFile* _pFile;
    int _nDownloadTotal;
    bool _bAcceptRanges;
    int _nDownloadSize;
    int _nDownloadSizeAtPause;
    QTimer _Timer;
    enum FTP_DOWNLOAD_STATE{
        FTP_DOWNLOAD_UNKNOW = 0,
        FTP_DOWNLOADING,
        FTP_DOWNLOAD_PAUSE,
        FTP_DOWNLOAD_ERROR,
        FTP_DOWNLOAD_TIMEOUT,
        FTP_DOWNLOAD_COMPLETE
    };
    FTP_DOWNLOAD_STATE _state;

};

#endif // QT_VERSION < 0x050000

#endif // DOWNLOADMANAGERFTP_H
