#include "networkiconlabel.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QNetworkDiskCache>
#include <QStandardPaths>


static QNetworkAccessManager * getCachedNetworkAccessManager()
{
    static QNetworkAccessManager * manager = nullptr;
    if(manager == nullptr) {
        manager = new QNetworkAccessManager;
        QString dataPath =  QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        QNetworkDiskCache * diskCache = new QNetworkDiskCache(manager);
        diskCache->setCacheDirectory(dataPath);
        diskCache->setMaximumCacheSize(50*1024*1024); // 50Mb
        manager->setCache(diskCache);
    }
    return manager;
}
NetworkIconLabel::NetworkIconLabel(QWidget *parent) : QLabel(parent)
{
    setAlignment(Qt::AlignCenter);
}

void NetworkIconLabel::setUrl(const QUrl &url)
{
    qDebug() << "Reading picture form " << url;
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    QNetworkReply *reply = getCachedNetworkAccessManager()->get(QNetworkRequest(url));
    //请求结束并下载完成后，退出子事件循环
    connect(reply, &QNetworkReply::finished, this, [reply, this](){
        QByteArray imageData = reply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(imageData);
        pixmap = pixmap.scaled(size(), Qt::KeepAspectRatioByExpanding);
        setPixmap(pixmap); // 你在QLabel显示图片
    });
}
