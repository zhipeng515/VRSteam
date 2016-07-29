#include "networkiconlabel.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>

NetworkIconLabel::NetworkIconLabel(QWidget *parent) : QLabel(parent)
{

}

void NetworkIconLabel::setUrl(const QUrl &url)
{
    QNetworkAccessManager manager;
    QEventLoop loop;

    // qDebug() << "Reading picture form " << url;
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    //请求结束并下载完成后，退出子事件循环
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    //开启子事件循环
    loop.exec();

    QByteArray jpegData = reply->readAll();
    QPixmap pixmap;
    pixmap.loadFromData(jpegData);
    setPixmap(pixmap); // 你在QLabel显示图片
}
