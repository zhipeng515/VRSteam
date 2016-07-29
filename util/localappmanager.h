#ifndef LOCALAPPMANAGER_H
#define LOCALAPPMANAGER_H

#include <QObject>

class AppInfo;
class DownloadManager;

class LocalAppManager : public QObject
{
    Q_OBJECT
public:
    explicit LocalAppManager(QObject *parent = 0);
    static LocalAppManager * getInstance();

private:
    QList<AppInfo*> downloadApps;

signals:
    void appInstallBegin(const AppInfo & app);
    void appInstallProgress(int percentage);
    void appInstallComplete(const AppInfo & app);

    void appDownloadBegin(const QUrl & url);
    void appDownloadComplete(const QUrl & url);
    void appDownloadProgress(const QUrl & url, int nPercentage);

public slots:    
    void downloadApp(const int id, const QString & downloadUrl, const QString & appName);
    void installApp(const int appId);
    void uninstallApp(const int appId);
    bool isAppInstalled(const int appId);
    bool isDownloading(const int appId);
    bool isAppNeedUpdated(const int appId, const QString & latestVersion);

    void downloadComplete(const QUrl & url);
    void downloadProgress(const QUrl & url, int nPercentage);

    AppInfo * getDownloadApp(const QUrl & url);
    QList<AppInfo*>  getDownloadApps(){
        return downloadApps;
    }
};

#endif // LOCALAPPMANAGER_H
