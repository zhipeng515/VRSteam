#ifndef APPINFO_H
#define APPINFO_H

#include <QObject>

class AppInfo : public QObject
{
    Q_OBJECT
public:
    explicit AppInfo(QObject *parent = 0);
    AppInfo(const AppInfo & appInfo);

    int id;
    QString name;
    QString installPath;
    QString launcher;
    QString uninstallLauncher;
    QString version;
    int size;
    int installSize;

    bool isValid();
    bool versionCompare(const QString & version);
    inline AppInfo & operator = (const AppInfo & appInfo)
    {
        this->id = appInfo.id;
        this->name = appInfo.name;
        this->installPath = appInfo.installPath;
        this->launcher = appInfo.launcher;
        this->size = appInfo.size;
        this->installSize = appInfo.installSize;
        this->uninstallLauncher = appInfo.uninstallLauncher;

        return *this;
    }

    static std::map<const int, AppInfo> appInfos;
    static AppInfo & getAppInfo(const int id);
    static AppInfo & getAppInfo(const QString & name);
    static void addAppInfo(const int id, AppInfo & appInfo);

    static AppInfo invalidAppInfo;

signals:

public slots:
};

#endif // APPINFO_H
