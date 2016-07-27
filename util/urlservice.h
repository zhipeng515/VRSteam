#ifndef URLSERVICE_H
#define URLSERVICE_H

#include <QObject>
#include <QUrl>

class WebView;
class URLService : public QObject
{
    Q_OBJECT
public:
    explicit URLService(QObject *parent = 0);
    static URLService* getInstance();

signals:

public slots:
    void handleURL(const QUrl & url, WebView * webView = nullptr);
};

#endif // URLSERVICE_H
