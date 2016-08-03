#include "downloadmanagerHTTP.h"

#include <QFileInfo>
#include <QDateTime>
#include <QDebug>


DownloadManagerHTTP::DownloadManagerHTTP(QObject *parent) :
    QObject(parent)
    , _pManager(NULL)
    , _pCurrentReply(NULL)
    , _pFile(NULL)
    , _nDownloadTotal(0)
    , _bAcceptRanges(false)
    , _nDownloadSize(0)
    , _nDownloadSizeAtPause(0)
    , _state(HTTP_DOWNLOAD_UNKNOW)
{
}


DownloadManagerHTTP::~DownloadManagerHTTP()
{
    if (_pCurrentReply != NULL)
    {
        pause();
    }
}


void DownloadManagerHTTP::download(const QUrl & url, const QString & localPath)
{
    qDebug() << "download: URL=" <<url.toString();

    _URL = url;
    {
        QFileInfo fileInfo(url.toString());
        _qsFileName = localPath + fileInfo.fileName();
    }

    _pManager = new QNetworkAccessManager(this);
    requestHead();

    emit downloadBegin(_URL);
}

void DownloadManagerHTTP::requestHead()
{
    _nDownloadSize = 0;
    _nDownloadSizeAtPause = 0;
    _CurrentRequest = QNetworkRequest(_URL);

    _pCurrentReply = _pManager->head(_CurrentRequest);

    _Timer.setInterval(5000);
    _Timer.setSingleShot(true);
    connect(&_Timer, SIGNAL(timeout()), this, SLOT(timeout()));
    _Timer.start();

    connect(_pCurrentReply, SIGNAL(finished()), this, SLOT(finishedHead()));
    connect(_pCurrentReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));

    _state = HTTP_DOWNLOAD_REQUEST_HEAD;
}

void DownloadManagerHTTP::stopRequest()
{
    if (_pCurrentReply == NULL)
    {
        return;
    }
    _Timer.stop();
    disconnect(&_Timer, SIGNAL(timeout()), this, SLOT(timeout()));
    disconnect(_pCurrentReply, SIGNAL(finished()), this, SLOT(finished()));
    disconnect(_pCurrentReply, SIGNAL(finished()), this, SLOT(finishedHead()));
    disconnect(_pCurrentReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
    disconnect(_pCurrentReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));

    _pCurrentReply->abort();
    if(_pFile)
        _pFile->flush();
    _pCurrentReply = 0;
    _nDownloadSizeAtPause = _nDownloadSize;
    _nDownloadSize = 0;

    _state = HTTP_DOWNLOAD_PAUSE;
}

void DownloadManagerHTTP::pause()
{
    qDebug() << "pause() = " << _nDownloadSize;

    stopRequest();

    emit downloadPause(_URL);
}


void DownloadManagerHTTP::resume()
{
    qDebug() << "resume() = " << _nDownloadSizeAtPause;

    if(_pFile == 0 || _nDownloadSize == 0) {
        requestHead();
    }
    else {
        download();
    }
    emit downloadResume(_URL);
}


void DownloadManagerHTTP::download()
{
    qDebug() << "download()";

    if (_bAcceptRanges)
    {
        QByteArray rangeHeaderValue = "bytes=" + QByteArray::number(_nDownloadSizeAtPause) + "-";
        if (_nDownloadTotal > 0)
        {
            rangeHeaderValue += QByteArray::number(_nDownloadTotal);
        }
        _CurrentRequest.setRawHeader("Range", rangeHeaderValue);
    }

    _pCurrentReply = _pManager->get(_CurrentRequest);

    _Timer.setInterval(5000);
    _Timer.setSingleShot(true);
    connect(&_Timer, SIGNAL(timeout()), this, SLOT(timeout()));
    _Timer.start();

    connect(_pCurrentReply, SIGNAL(finished()), this, SLOT(finished()));
    connect(_pCurrentReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
    connect(_pCurrentReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));

    _state = HTTP_DOWNLOADING;
}


void DownloadManagerHTTP::finishedHead()
{
    _Timer.stop();
    _bAcceptRanges = false;

    QList<QByteArray> list = _pCurrentReply->rawHeaderList();
    foreach (QByteArray header, list)
    {
        QString qsLine = QString(header) + " = " + _pCurrentReply->rawHeader(header);
        emit addLine(_URL, qsLine);
    }

    if (_pCurrentReply->hasRawHeader("Accept-Ranges"))
    {
        QString qstrAcceptRanges = _pCurrentReply->rawHeader("Accept-Ranges");
        _bAcceptRanges = (qstrAcceptRanges.compare("bytes", Qt::CaseInsensitive) == 0);
        qDebug() << "Accept-Ranges = " << qstrAcceptRanges << _bAcceptRanges;
    }

    _nDownloadTotal = _pCurrentReply->header(QNetworkRequest::ContentLengthHeader).toInt();

//    _CurrentRequest = QNetworkRequest(url);
    _CurrentRequest.setRawHeader("Connection", "Keep-Alive");
    _CurrentRequest.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    _pFile = new QFile(_qsFileName + ".part");
    if (!_bAcceptRanges)
    {
        _pFile->remove();
    }
    _pFile->open(QIODevice::WriteOnly | QIODevice::Append);

    _nDownloadSizeAtPause = _pFile->size();
    download();
}


void DownloadManagerHTTP::finished()
{
    qDebug() << __FUNCTION__;

    _Timer.stop();
    _pFile->close();
    QFile::remove(_qsFileName);
    _pFile->rename(_qsFileName + ".part", _qsFileName);
    _pFile = NULL;
    _pCurrentReply = 0;

    _state = HTTP_DOWNLOAD_COMPLETE;
    emit downloadComplete(_URL);
}


void DownloadManagerHTTP::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    _Timer.stop();
    _nDownloadSize = _nDownloadSizeAtPause + bytesReceived;
    qDebug() << "Download Progress: Received=" << _nDownloadSize <<": Total=" << _nDownloadSizeAtPause + bytesTotal;

    _pFile->write(_pCurrentReply->readAll());
    int nPercentage = static_cast<int>((static_cast<float>(_nDownloadSizeAtPause + bytesReceived) * 100.0) / static_cast<float>(_nDownloadSizeAtPause + bytesTotal));
    qDebug() << nPercentage;
    emit downloadProgress(_URL, nPercentage);

    _Timer.start(5000);
}

bool DownloadManagerHTTP::isDownloading()
{
    return _Timer.isActive();
}

bool DownloadManagerHTTP::isDownloadError()
{
    return _state == HTTP_DOWNLOAD_ERROR;
}

bool DownloadManagerHTTP::isDownloadTimeout()
{
    return _state == HTTP_DOWNLOAD_TIMEOUT;
}

bool DownloadManagerHTTP::isDownloadPause()
{
    return _state == HTTP_DOWNLOAD_PAUSE;
}

bool DownloadManagerHTTP::isDownloadComplete()
{
    return _state == HTTP_DOWNLOAD_COMPLETE;
}


void DownloadManagerHTTP::error(QNetworkReply::NetworkError code)
{
    qDebug() << __FUNCTION__ << "(" << code << ")";
    stopRequest();
    _state = HTTP_DOWNLOAD_ERROR;
    emit downloadError(_URL, code);
}


void DownloadManagerHTTP::timeout()
{
    qDebug() << __FUNCTION__;
    stopRequest();
    _state = HTTP_DOWNLOAD_TIMEOUT;
    emit downloadTimeout(_URL);
}
