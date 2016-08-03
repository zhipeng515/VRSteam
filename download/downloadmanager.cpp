#include "downloadmanager.h"

#include <QFileInfo>
#include <QDateTime>
#include <QDebug>


DownloadManager::DownloadManager(QObject *parent) :
    QObject(parent)
{
}

DownloadManager::~DownloadManager()
{
#if QT_VERSION < 0x050000
    qDeleteAll(ftpDownloads);
    ftpDownloads.clear();
#endif
    qDeleteAll(httpDownloads);
    httpDownloads.clear();
}

void DownloadManager::download(const QUrl & url, const QString & localPath)
{
    qDebug() << __FUNCTION__ << "(" << url.toString() << ")";

#if QT_VERSION < 0x050000
    if (_URL.scheme().toLower() == "ftp")
    {
        DownloadManagerFTP * pFTP = ftpDownloads[url];
        if (pFTP == NULL) {

            pFTP = new DownloadManagerFTP(this);

            connect(pFTP, SIGNAL(addLine(const QUrl&, const QString&)), this, SIGNAL(addLine(const QUrl&, const QString&)));
            connect(pFTP, SIGNAL(downloadProgress(const QUrl&,int)), this, SIGNAL(downloadProgress(const QUrl&, int)));
            connect(pFTP, SIGNAL(downloadBegin(const QUrl&)), this, SIGNAL(downloadBegin(const QUrl&,)));
            connect(pFTP, SIGNAL(downloadPause(const QUrl&)), this, SIGNAL(downloadPause(const QUrl&,)));
            connect(pFTP, SIGNAL(downloadResume(const QUrl&)), this, SIGNAL(downloadResume(const QUrl&,)));
            connect(pFTP, SIGNAL(downloadComplete(const QUrl&)), this, SIGNAL(downloadComplete(const QUrl&,)));
            connect(pFTP, SIGNAL(downloadError(const QUrl&, QNetworkReply::NetworkError)), this, SIGNAL(downloadError(const QUrl&, QNetworkReply::NetworkError)));
            connect(pFTP, SIGNAL(downloadTimeout(const QUrl&)), this, SIGNAL(downloadTimeout(const QUrl&)));

            ftpDownloads.insert(url, pFTP);
        }
        pFTP->download(url, localPath);
    }
    else
#endif
    {
        DownloadManagerHTTP * pHTTP = httpDownloads[url];
        if (pHTTP == NULL) {
            pHTTP = new DownloadManagerHTTP(this);

            connect(pHTTP, SIGNAL(addLine(const QUrl&, const QString&)), this, SIGNAL(addLine(const QUrl&, const QString&)));
            connect(pHTTP, SIGNAL(downloadProgress(const QUrl&,int)), this, SIGNAL(downloadProgress(const QUrl&, int)));
            connect(pHTTP, SIGNAL(downloadBegin(const QUrl&)), this, SIGNAL(downloadBegin(const QUrl&)));
            connect(pHTTP, SIGNAL(downloadPause(const QUrl&)), this, SIGNAL(downloadPause(const QUrl&)));
            connect(pHTTP, SIGNAL(downloadResume(const QUrl&)), this, SIGNAL(downloadResume(const QUrl&)));
            connect(pHTTP, SIGNAL(downloadComplete(const QUrl&)), this, SIGNAL(downloadComplete(const QUrl&)));
            connect(pHTTP, SIGNAL(downloadError(const QUrl&, QNetworkReply::NetworkError)), this, SIGNAL(downloadError(const QUrl&, QNetworkReply::NetworkError)));
            connect(pHTTP, SIGNAL(downloadTimeout(const QUrl&)), this, SIGNAL(downloadTimeout(const QUrl&)));

            httpDownloads.insert(url, pHTTP);
        }
        pHTTP->download(url, localPath);
    }
}


void DownloadManager::pause(const QUrl & url)
{
    qDebug() << __FUNCTION__;

#if QT_VERSION < 0x050000
    if (_URL.scheme().toLower() == "ftp")
    {
        DownloadManagerFTP * pFTP = ftpDownloads[url];
        if (pFTP != NULL) {
            pFTP->pause();
        }
    }
    else
#endif
    {
        DownloadManagerHTTP * pHTTP = httpDownloads[url];
        if (pHTTP != NULL) {
            pHTTP->pause();
        }
    }
}


void DownloadManager::resume(const QUrl & url)
{
    qDebug() << __FUNCTION__;

#if QT_VERSION < 0x050000
    if (_URL.scheme().toLower() == "ftp")
    {
        DownloadManagerFTP * pFTP = ftpDownloads[url];
        if (pFTP != NULL) {
            pFTP->resume();
        }
    }
    else
#endif
    {
        DownloadManagerHTTP * pHTTP = httpDownloads[url];
        if (pHTTP != NULL) {
            pHTTP->resume();
        }
    }
}

bool DownloadManager::isDownloading(const QUrl & url)
{
#if QT_VERSION < 0x050000
    if (_URL.scheme().toLower() == "ftp")
    {
        DownloadManagerFTP * pFTP = ftpDownloads[url];
        if (pFTP != NULL) {
            return pFTP->isDownloading();
        }
    }
    else
#endif
    {
        DownloadManagerHTTP * pHTTP = httpDownloads[url];
        if (pHTTP != NULL) {
            return pHTTP->isDownloading();
        }
    }
    return false;
}

bool DownloadManager::isDownloadError(const QUrl & url)
{
#if QT_VERSION < 0x050000
    if (_URL.scheme().toLower() == "ftp")
    {
        DownloadManagerFTP * pFTP = ftpDownloads[url];
        if (pFTP != NULL) {
            return pFTP->isDownloadError();
        }
    }
    else
#endif
    {
        DownloadManagerHTTP * pHTTP = httpDownloads[url];
        if (pHTTP != NULL) {
            return pHTTP->isDownloadError();
        }
    }
    return false;
}

bool DownloadManager::isDownloadTimeout(const QUrl & url)
{
#if QT_VERSION < 0x050000
    if (_URL.scheme().toLower() == "ftp")
    {
        DownloadManagerFTP * pFTP = ftpDownloads[url];
        if (pFTP != NULL) {
            return pFTP->isDownloadTimeout();
        }
    }
    else
#endif
    {
        DownloadManagerHTTP * pHTTP = httpDownloads[url];
        if (pHTTP != NULL) {
            return pHTTP->isDownloadTimeout();
        }
    }
    return false;
}

bool DownloadManager::isDownloadPause(const QUrl & url)
{
#if QT_VERSION < 0x050000
    if (_URL.scheme().toLower() == "ftp")
    {
        DownloadManagerFTP * pFTP = ftpDownloads[url];
        if (pFTP != NULL) {
            return pFTP->isDownloadPause();
        }
    }
    else
#endif
    {
        DownloadManagerHTTP * pHTTP = httpDownloads[url];
        if (pHTTP != NULL) {
            return pHTTP->isDownloadPause();
        }
    }
    return false;
}

bool DownloadManager::isDownloadComplete(const QUrl & url)
{
#if QT_VERSION < 0x050000
    if (_URL.scheme().toLower() == "ftp")
    {
        DownloadManagerFTP * pFTP = ftpDownloads[url];
        if (pFTP != NULL) {
            return pFTP->isDownloadComplete();
        }
    }
    else
#endif
    {
        DownloadManagerHTTP * pHTTP = httpDownloads[url];
        if (pHTTP != NULL) {
            return pHTTP->isDownloadComplete();
        }
    }
    return false;
}
