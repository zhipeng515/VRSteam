#include "download.h"

Download::Download(int index, QObject *parent)
: QObject(parent), m_Index(index)
{
    m_HaveDoneBytes = 0;
    m_StartPoint = 0;
    m_EndPoint = 0;
    m_File = NULL;
}

void Download::StartDownload(const QUrl &url,
                                 QFile *file,
                                 qint64 startPoint/* =0 */,
                                 qint64 endPoint/* =-1 */)
{
    if( NULL == file )
        return;

    m_HaveDoneBytes = 0;
    m_StartPoint = startPoint;
    m_EndPoint = endPoint;
    m_File = file;

    //根据HTTP协议，写入RANGE头部，说明请求文件的范围
    QNetworkRequest qheader;
    qheader.setUrl(url);
    QString range;
    range.sprintf("Bytes=%lld-%lld", m_StartPoint, m_EndPoint);
    qheader.setRawHeader("Range", range.toLatin1());

    //开始下载
    qDebug() << "Part " << m_Index << " start download";
    m_Reply = m_Qnam.get(QNetworkRequest(qheader));
    connect(m_Reply, SIGNAL(finished()),
        this, SLOT(FinishedSlot()));
    connect(m_Reply, SIGNAL(readyRead()),
        this, SLOT(HttpReadyRead()));
    connect(m_Reply, SIGNAL(downloadProgress(qint64, qint64)),
        this, SLOT(downloadProgress(qint64, qint64)));

//    m_Thread = new QThread;
//    m_Reply->moveToThread(m_Thread);
}

void Download::PauseDownload()
{
}

void Download::StopDownload()
{
    m_File->flush();
    m_Thread->terminate();
    m_Thread->deleteLater();
    m_Reply->abort();
    m_Reply->deleteLater();
    m_Reply = 0;
    m_File = 0;
    qDebug() << "Part " << m_Index << " download finished";
    emit DownloadFinished();
}

//下载结束
void Download::FinishedSlot()
{
    m_File->flush();
    m_Reply->deleteLater();
    m_Reply = 0;
    m_File = 0;
    qDebug() << "Part " << m_Index << " download finished";
    emit DownloadFinished();
}

void Download::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit DownloadProgressUpdated(bytesReceived, bytesTotal);
}

void Download::HttpReadyRead()
{
    if ( !m_File )
        return;

    //将读到的信息写入文件
    QByteArray buffer = m_Reply->readAll();
    m_File->seek( m_StartPoint + m_HaveDoneBytes );
    m_File->write(buffer);
    m_HaveDoneBytes += buffer.size();
}

//用阻塞的方式获取下载文件的长度
qint64 DownloadControl::GetFileSize(QUrl url)
{
    QNetworkAccessManager manager;
    qDebug() << "Getting the file size...";
    QEventLoop loop;
    //发出请求，获取目标地址的头部信息
    QNetworkReply *reply = manager.head(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()), Qt::DirectConnection);
    loop.exec();
    QVariant var = reply->header(QNetworkRequest::ContentLengthHeader);
    delete reply;
    qint64 size = var.toLongLong();
    qDebug() << "The file size is: " << size;
    return size;
}

DownloadControl::DownloadControl(QObject *parent)
: QObject(parent)
{
    m_DownloadCount = 0;
    m_FinishedNum = 0;
    m_FileSize = 0;
    m_ReceivedSize = 0;
    m_File = new QFile;
}

void DownloadControl::StartFileDownload(const QString &url, int count)
{
    m_DownloadCount = count;
    m_FinishedNum = 0;
    m_Url = QUrl(url);
    m_FileSize = GetFileSize(m_Url);
    //先获得文件的名字
    QFileInfo fileInfo(m_Url.path());
    QString fileName = fileInfo.fileName();
    if (fileName.isEmpty())
        fileName = "index.html";

    m_File->setFileName(fileName);
    //打开文件
    m_File->open(QIODevice::WriteOnly);
    Download *tempDownload;

    //将文件分成PointCount段，用异步的方式下载
    qDebug() << "Start download file from " << url;
    for(int i=0; i<m_DownloadCount; i++)
    {
        //先算出每段的开头和结尾（HTTP协议所需要的信息）
        int start = m_FileSize * i / m_DownloadCount;
        int end = m_FileSize * (i+1) / m_DownloadCount;
        if( i != 0 )
            start--;

        //分段下载该文件
        tempDownload = new Download(i+1, this);
        connect(tempDownload, SIGNAL(DownloadFinished()),
            this, SLOT(SubPartFinished()));
        connect(tempDownload, SIGNAL(DownloadProgressUpdated(qint64,qint64)),
            this, SLOT(SubPartDownloadProgressUpdated(qint64, qint64)));
        connect(tempDownload, SIGNAL(DownloadFinished()),
            tempDownload, SLOT(deleteLater()));
        tempDownload->StartDownload(m_Url, m_File, start, end);
    }
}

void DownloadControl::SubPartFinished()
{
    m_FinishedNum++;
    //如果完成数等于文件段数，则说明文件下载完毕，关闭文件，发生信号
    if( m_FinishedNum == m_DownloadCount )
    {
        m_File->close();
        emit FileDownloadFinished();
        qDebug() << "Download finished";
    }
}

void DownloadControl::SubPartDownloadProgressUpdated(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_UNUSED(bytesTotal);

    m_ReceivedSize += bytesReceived;
    emit FileDownloadProgressUpdated(m_ReceivedSize, m_FileSize);

    qDebug() << "Download progress update" << (float)m_ReceivedSize/(float)m_FileSize;
}
