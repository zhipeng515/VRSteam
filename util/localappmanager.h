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

signals:
    void appInstallBegin(const AppInfo & app);
    void appInstallProgress(int percentage);
    void appInstallComplete(const AppInfo & app);

    void downloadComplete(const QUrl & url);
    void downloadProgress(const QUrl & url, int nPercentage);

public slots:    
    void downloadApp(const int id, const QString & downloadUrl, const QString & appName);
    void installApp(const int appId);
    void uninstallApp(const int appId);
    bool isAppInstalled(const int appId);
    bool isAppNeedUpdated(const int appId, const QString & latestVersion);
};

#endif // LOCALAPPMANAGER_H
