#ifndef DOWNLOAD_H
#define DOWNLOAD_H


#include <QtCore>
#include <QtNetwork>

//多线程下载的线程数
const int PointCount = 5;
//目标文件的地址（千千静听的下载地址，我用来做实验的）
const QString strUrl = "http://ttplayer.qianqian.com/otherdown/alladin/ttpsetup_5713.exe";

//用于下载文件（或文件的一部分）
class Download : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager m_Qnam;
    QNetworkReply *m_Reply;
    QThread *m_Thread;
    QFile *m_File;

    const int m_Index;
    qint64 m_HaveDoneBytes;
    qint64 m_StartPoint;
    qint64 m_EndPoint;

public:
    Download(int index, QObject *parent = 0);

    void StartDownload(const QUrl &url, QFile *file,
        qint64 startPoint=0, qint64 endPoint=-1);
    void PauseDownload();
    void StopDownload();

signals:
    void DownloadFinished();
    void DownloadProgressUpdated(qint64 bytesReceived, qint64 bytesTotal);

public slots:
    void FinishedSlot();
    void HttpReadyRead();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
};

//用于管理文件的下载
class DownloadControl : public QObject
{
    Q_OBJECT
private:
    int m_DownloadCount;
    int m_FinishedNum;
    int m_FileSize;
    int m_ReceivedSize;
    QUrl m_Url;
    QFile *m_File;
public:
    DownloadControl(QObject *parent = 0);
    void StartFileDownload(const QString &url, int count);
    qint64 GetFileSize(QUrl url);
signals:
    void FileDownloadFinished();
    void FileDownloadProgressUpdated(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    void SubPartFinished();
    void SubPartDownloadProgressUpdated(qint64 bytesReceived, qint64 bytesTotal);
};

#endif // DOWNLOAD_H
