#ifndef LOCALAPPMANAGER_H
#define LOCALAPPMANAGER_H

#include <QObject>
#include <QNetworkReply>

class AppInfo;
class DownloadManager;
class DownloadApps;

class LocalAppManager : public QObject
{
    Q_OBJECT
public:
    explicit LocalAppManager(QObject *parent = 0);
    static LocalAppManager * getInstance();

private:
    DownloadApps * downloadApps;

signals:
    void appInstallBegin(const AppInfo & app);
    void appInstallProgress(int percentage);
    void appInstallComplete(const AppInfo & app);

    void appDownloadBegin(const QUrl & url);
    void appDownloadPause(const QUrl & url);
    void appDownloadResume(const QUrl & url);
    void appDownloadComplete(const QUrl & url);
    void appDownloadProgress(const QUrl & url, int nPercentage);
    void appDownloadError(const QUrl & url, QNetworkReply::NetworkError code);
    void appDownloadTimeout(const QUrl & url);


public slots:    
    void downloadApp(const int id, const QString & downloadUrl, const QString & appName,
                     const QString & iconUrl, const QString & version, const QString & launcher);
    void installApp(const int appId);
    void launchApp(const int appId);
    void uninstallApp(const int appId);

    bool isAppInstalled(const int appId);
    bool isAppDownloading(const int appId);
    bool isAppDownloaded(const int appId);
    bool isAppRunning(const int appId);
    bool isAppNeedUpdated(const int appId, const QString & latestVersion);

    void downloadComplete(const QUrl & url);

    AppInfo * getDownloadApp(const QUrl & url);
    DownloadApps * getDownloadApps(){
        return downloadApps;
    }
};

#endif // LOCALAPPMANAGER_H
