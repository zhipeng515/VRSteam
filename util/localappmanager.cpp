#include "localappmanager.h"
#include "models.h"

#include <QFileInfo>
#include <3rd-party/QSimpleUpdater/src/Updater.h>

#include <QDesktopServices>
#include "download/downloadmanager.h"
#include "Preferences.h"
#include <QJsonDocument>
#include <QJsonObject>

LocalAppManager * LocalAppManager::getInstance()
{
    static LocalAppManager localAppManager;
    return &localAppManager;
}

LocalAppManager::LocalAppManager(QObject *parent) : QObject(parent)
{
    connect(DownloadManager::getInstance(), SIGNAL(downloadComplete(const QUrl&)),
            this, SIGNAL(downloadComplete(const QUrl&)));
    connect(DownloadManager::getInstance(), SIGNAL(progress(const QUrl& , int)),
            this, SIGNAL(downloadProgress(const QUrl&, int)));

    // Test code
//    downloadApp(0, "http://www.sina.com.cn", "abc");
//    downloadApp(1, "http://www.baidu.com", "abcdef");
//    downloadApp(2, "http://www.qqq.com", "abcdefhig");

//    AppInfo::saveModels(Preferences::getInstance()->getSettingPath()+"apps.json");
//    AppInfo::removeAllModels();
//    AppInfo::loadModels(Preferences::getInstance()->getSettingPath()+"apps.json");
}

void LocalAppManager::downloadApp(const int id, const QString & downloadUrl, const QString & appName)
{
    AppInfo * appInfo = new AppInfo;
    appInfo->id(id);
    appInfo->downloadUrl(downloadUrl);
    appInfo->name(appName);

    AppInfo::addModel(appInfo->id(), appInfo);

    DownloadManager::getInstance()->download(downloadUrl, Preferences::getInstance()->getSettingPath());
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
        QFileInfo fileInfo(appInfo->uninstallLauncher());
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

bool LocalAppManager::isAppNeedUpdated(const int appId, const QString & latestVersion)
{
    AppInfo * appInfo = AppInfo::getModel(appId);
    if(appInfo->isValid()) {
        return Updater::compare(latestVersion, appInfo->version());
    }
    return false;
}
