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

            connect(pFTP, SIGNAL(addLine(const QUrl&, const QString&)), this, SLOT(localAddLine(const QUrl&, const QString&)));
            connect(pFTP, SIGNAL(progress(const QUrl&,int)), this, SLOT(localProgress(const QUrl&, int)));
            connect(pFTP, SIGNAL(complete(const QUrl&)), this, SLOT(localDownloadComplete(const QUrl&,)));
            connect(pFTP, SIGNAL(error(const QUrl&, QNetworkReply::NetworkError)), this, SIGNAL(error(const QUrl&, QNetworkReply::NetworkError)));
            connect(pFTP, SIGNAL(timeout(const QUrl&)), this, SIGNAL(timeout(const QUrl&)));

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

            connect(pHTTP, SIGNAL(addLine(const QUrl&, const QString&)), this, SLOT(localAddLine(const QUrl&, const QString&)));
            connect(pHTTP, SIGNAL(progress(const QUrl&,int)), this, SLOT(localProgress(const QUrl&, int)));
            connect(pHTTP, SIGNAL(complete(const QUrl&)), this, SLOT(localDownloadComplete(const QUrl&,)));
            connect(pHTTP, SIGNAL(error(const QUrl&, QNetworkReply::NetworkError)), this, SIGNAL(error(const QUrl&, QNetworkReply::NetworkError)));
            connect(pHTTP, SIGNAL(timeout(const QUrl&)), this, SIGNAL(timeout(const QUrl&)));

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
        if (pFTP == NULL) {
            pFTP->pause();
        }
    }
    else
#endif
    {
        DownloadManagerHTTP * pHTTP = httpDownloads[url];
        if (pHTTP == NULL) {
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
        if (pFTP == NULL) {
            pFTP->resume();
        }
    }
    else
#endif
    {
        DownloadManagerHTTP * pHTTP = httpDownloads[url];
        if (pHTTP == NULL) {
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
        if (pFTP == NULL) {
            return pFTP->isDownloading();
        }
    }
    else
#endif
    {
        DownloadManagerHTTP * pHTTP = httpDownloads[url];
        if (pHTTP == NULL) {
            return pHTTP->isDownloading();
        }
    }
    return false;
}


void DownloadManager::localAddLine(const QUrl& url, const QString & qsLine)
{
    emit addLine(url, qsLine);
}

void DownloadManager::localProgress(const QUrl& url, const int nPercentage)
{
    emit progress(url, nPercentage);
}


void DownloadManager::localDownloadComplete(const QUrl& url)
{
    emit complete(url);
}
