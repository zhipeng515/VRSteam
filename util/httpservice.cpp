#include "httpservice.h"
#include "util/models.h"

HttpService * HttpService::getInstance()
{
    static HttpService httpService(nullptr);
    return &httpService;
}


HttpService::HttpService(QObject *parent) : QObject(parent)
{
    QFile file(":/config/api.json");
    file.open(QIODevice::ReadOnly);
    QByteArray bytes = file.readAll();
    apiInfos.importFromJson(bytes);
}
QUrl HttpService::getUrl(const QString & api)
{
    ApiInfo * apiInfo = ApiInfo::getModel(api);
    if(!apiInfo->isValid())
        return QUrl();

    return apiInfos.host() + apiInfo->path();
}

bool HttpService::request(const QString & api, const QMap<QString, QString>& params,
                          const std::function<void(const QByteArray &)> &success,
                          const std::function<void(const QString &)> &failed)
{
    ApiInfo * apiInfo = ApiInfo::getModel(api);
    if(!apiInfo->isValid())
        return false;

    QObject * reply;
    QUrl url = apiInfos.host() + apiInfo->path();
    if(apiInfo->operation() == "GET")
        reply = Http::instance().get(url);
    else if(apiInfo->operation() == "POST")
        reply = Http::instance().post(url, params);

    connect(static_cast<HttpReply*>(reply), &HttpReply::data, success);
    connect(static_cast<HttpReply*>(reply), &HttpReply::error, failed);

    return true;
}
