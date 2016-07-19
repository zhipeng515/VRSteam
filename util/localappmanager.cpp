#include "localappmanager.h"
#include "models.h"

LocalAppManager::LocalAppManager(QObject *parent) : QObject(parent)
{

}

void LocalAppManager::installApp(const AppInfo & app)
{
    Q_UNUSED(app);
}

void LocalAppManager::uninstallApp(const AppInfo & app)
{
    Q_UNUSED(app);
}

bool LocalAppManager::isAppInstalled(const int appId)
{
    return AppInfo::getModel(appId)->isValid();
}
