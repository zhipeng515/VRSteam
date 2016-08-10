#include "localappmanager.h"
#include "models.h"

#include <QFileInfo>
#include <3rd-party/QSimpleUpdater/src/Updater.h>

#include <QDesktopServices>
#include "download/downloadmanager.h"
#include "Preferences.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileInfo>

LocalAppManager * LocalAppManager::getInstance()
{
    static LocalAppManager localAppManager;
    return &localAppManager;
}

LocalAppManager::LocalAppManager(QObject *parent) : QObject(parent)
{
    connect(DownloadManager::getInstance(), SIGNAL(downloadComplete(const QUrl&)),
            this, SLOT(downloadComplete(const QUrl&)));
    connect(DownloadManager::getInstance(), SIGNAL(downloadError(const QUrl&, QNetworkReply::NetworkError)),
            this, SIGNAL(appDownloadError(const QUrl&, QNetworkReply::NetworkError)));
    connect(DownloadManager::getInstance(), SIGNAL(downloadTimeout(const QUrl&)),
            this, SIGNAL(appDownloadTimeout(const QUrl&)));
    connect(DownloadManager::getInstance(), SIGNAL(downloadPause(const QUrl&)),
            this, SIGNAL(appDownloadPause(const QUrl&)));
    connect(DownloadManager::getInstance(), SIGNAL(downloadResume(const QUrl&)),
            this, SIGNAL(appDownloadResume(const QUrl&)));
    connect(DownloadManager::getInstance(), SIGNAL(downloadBegin(const QUrl&)),
            this, SIGNAL(appDownloadBegin(const QUrl&)));
    connect(DownloadManager::getInstance(), SIGNAL(downloadProgress(const QUrl &, const int)),
            this, SIGNAL(appDownloadProgress(const QUrl &, const int)));

    AppInfo::loadModels(Preferences::getInstance()->getSettingPath()+"apps.json");

    DownloadApps::loadModels(Preferences::getInstance()->getSettingPath()+"downloadapps.json");
    downloadApps = DownloadApps::getModel(1);
    if(downloadApps == nullptr)
    {
        downloadApps = new DownloadApps;
        downloadApps->id(1);
        downloadApps->name("downloadApps");
        DownloadApps::addModel(1, downloadApps);
        DownloadApps::saveModels(Preferences::getInstance()->getSettingPath()+"downloadapps.json");
    }

    // Test code
//    downloadApp(0, "http://www.sina.com.cn", "abc", "http://e.hiphotos.baidu.com/zhidao/pic/item/cefc1e178a82b9014e150b23718da9773912ef62.jpg", "1.0", "");
//    downloadApp(1, "http://www.baidu.com", "abcdef", "http://e.hiphotos.baidu.com/zhidao/pic/item/cefc1e178a82b9014e150b23718da9773912ef62.jpg", "1.0", "");
//    downloadApp(2, "http://www.qqq.com", "abcdefhig", "http://e.hiphotos.baidu.com/zhidao/pic/item/cefc1e178a82b9014e150b23718da9773912ef62.jpg", "1.0", "");
//    downloadApp(3, "http://www.sinaa.com.cn", "abc", "http://e.hiphotos.baidu.com/zhidao/pic/item/cefc1e178a82b9014e150b23718da9773912ef62.jpg", "1.0", "");
//    downloadApp(4, "http://www.baidua.com", "abcdef", "http://e.hiphotos.baidu.com/zhidao/pic/item/cefc1e178a82b9014e150b23718da9773912ef62.jpg", "1.0", "");
//    downloadApp(5, "http://www.qqqa.com", "abcdefhig", "http://e.hiphotos.baidu.com/zhidao/pic/item/cefc1e178a82b9014e150b23718da9773912ef62.jpg", "1.0", "");
}

void LocalAppManager::downloadApp(const int id, const QString & downloadUrl, const QString & appName,
                                  const QString & iconUrl, const QString & version, const QString & launcher)
{
    AppInfo * appInfo = new AppInfo;
    appInfo->id(id);
    appInfo->downloadUrl(downloadUrl);
    appInfo->name(appName);
    appInfo->iconUrl(iconUrl);
    appInfo->version(version);
    appInfo->launcher(launcher);

    QString downloadPath = Preferences::getInstance()->getDownloadPath();
    QFileInfo localFile(downloadUrl);
    appInfo->localInstallFile(downloadPath + localFile.fileName());

    AppInfo::addModel(appInfo->id(), appInfo);
    downloadApps->appendAppInfo(appInfo);

    AppInfo::saveModels(Preferences::getInstance()->getSettingPath()+"apps.json");
    DownloadApps::saveModels(Preferences::getInstance()->getSettingPath()+"downloadapps.json");

    DownloadManager::getInstance()->download(downloadUrl, downloadPath);
}

void LocalAppManager::installApp(const int appId)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid()) {
        QFileInfo fileInfo(appInfo->localInstallFile());
        if(fileInfo.isExecutable())
        {
            emit appInstallBegin(*appInfo);
            QDesktopServices::openUrl(fileInfo.absoluteFilePath());
            emit appInstallComplete(*appInfo);
        }
    }
}

void LocalAppManager::uninstallApp(const int appId)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid()) {
        QFileInfo fileInfo(appInfo->installPath() + appInfo->uninstallLauncher());
        if(fileInfo.isExecutable())
            QDesktopServices::openUrl(fileInfo.absoluteFilePath());
    }
}

void LocalAppManager::launchApp(const int appId)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid()) {
        QFileInfo fileInfo(appInfo->launcher());
        if(fileInfo.isExecutable())
            QDesktopServices::openUrl(fileInfo.absoluteFilePath());
    }
}

bool LocalAppManager::isAppInstalled(const int appId)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid()) {
        QFileInfo fileInfo(appInfo->launcher());
        return fileInfo.isExecutable();
    }
    return false;
}

bool LocalAppManager::isAppDownloaded(const int appId)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid()) {
        QFileInfo fileInfo(appInfo->localInstallFile());
        return fileInfo.isExecutable();
    }
    return false;
}

bool LocalAppManager::isAppDownloading(const int appId)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid()) {
        return downloadApps->itemAppInfoExist(appInfo);
    }
    return false;
}

bool LocalAppManager::isAppRunning(const int appId)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid()) {
    }
    return false;
}

bool LocalAppManager::isAppNeedUpdated(const int appId, const QString & latestVersion)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid()) {
        return Updater::compare(latestVersion, appInfo->version());
    }
    return false;
}

void LocalAppManager::downloadComplete(const QUrl & url)
{
    qDebug() << __FUNCTION__ << url;
    AppInfo * appInfo = getDownloadApp(url);
    if(appInfo) {
        emit appDownloadComplete(url);
        downloadApps->removeAppInfo(appInfo);
        DownloadApps::saveModels(Preferences::getInstance()->getSettingPath()+"downloadapps.json");
    }
}

AppInfo * LocalAppManager::getDownloadApp(const QUrl & url)
{
    for(int i = 0; i< downloadApps->countAppInfo(); i++) {
        AppInfo * appInfo = downloadApps->itemAppInfoAt(i);
        if(QUrl(appInfo->downloadUrl()) == url) {
            return appInfo;
        }
    }
    return nullptr;
}

