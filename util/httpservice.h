#ifndef HTTPSERVICE_H
#define HTTPSERVICE_H

#include <QObject>
#include "util/models.h"
#include "3rd-party/http/src/http.h"

class HttpService : public QObject
{
    Q_OBJECT
public:
    explicit HttpService(QObject *parent = 0);
    static HttpService * getInstance();

    bool request(const QString & api, const QMap<QString, QString>& params,
                 const std::function<void(const QByteArray &)> &success,
                 const std::function<void(const QString &)> &failed);

signals:

public slots:
    QUrl getUrl(const QString & api);

private:
    ApiInfos apiInfos;
};

#endif // HTTPSERVICE_H
