#ifndef LOCALAPPMANAGER_H
#define LOCALAPPMANAGER_H

#include <QObject>

class AppInfo;

class LocalAppManager : public QObject
{
    Q_OBJECT
public:
    explicit LocalAppManager(QObject *parent = 0);

signals:
    void appInstallBegin(const AppInfo & app);
    void appInstallProgress(int percentage);
    void appInstallComplete(const AppInfo & app);

public slots:
    void installApp(const AppInfo & app);
    void uninstallApp(const AppInfo & app);
    bool isAppInstalled(const AppInfo & app);
};

#endif // LOCALAPPMANAGER_H
