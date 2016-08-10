#include "urlservice.h"
#include "download/downloadmanager.h"
#include <QDesktopServices>
#include "view/webview.h"
#include "util/regexputils.h"

URLService * URLService::getInstance()
{
    static URLService urlService(nullptr);
    return &urlService;
}

URLService::URLService(QObject *parent) : QObject(parent)
{

}

void URLService::handleURL(const QUrl &url, WebView *webView)
{
    QString strUrl = url.toString();

    if(RegExpUtils::getInstance()->isUrl(strUrl)) {
        if(!Preferences::getInstance()->isInBlacklist(strUrl))
            webView->load(url);
//            QDesktopServices::openUrl(url);
    }
    else {
        // Example : vrst://download?http://www.sina.com.cn/vrsteam.exe
        // Example : vrst://open?http://www.sina.com.cn
        QStringList customProtocol = RegExpUtils::getInstance()->matchCustomUrl(strUrl);
        if(customProtocol.count() > 2) {
            if(customProtocol[0] == "vrst") {
                if(customProtocol[1] == "download" && RegExpUtils::getInstance()->isUrl(customProtocol[2])) {
                    DownloadManager::getInstance()->download(customProtocol[2], Preferences::getInstance()->getDownloadPath());
                }
                else if(customProtocol[1] == "open" && RegExpUtils::getInstance()->isUrl(customProtocol[2])) {
                    if(webView) {
                        webView->load(customProtocol[2]);
                    }
                    else {
                        QDesktopServices::openUrl(customProtocol[2]);
                    }
                }
            }
        }
    }
}
