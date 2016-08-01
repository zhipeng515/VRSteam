#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenuBar>
#include <QAction>
#include <QMenu>
#include <QWebChannel>

#include "dialog/AboutDialog.h"
#include "dialog/PreferencesDialog.h"
#include "dialog/downloaddialog.h"
#include "view/webpage.h"

#include "download/downloadmanager.h"
#include "util/localappmanager.h"
#include "QSimpleUpdater.h"
#include "util/regexputils.h"
#include "util/httpservice.h"

void MainWindow::initActions()
{
    QAction *action = ui->webView->pageAction(QWebEnginePage::Copy);
    action->setShortcut(QKeySequence::Copy);
    ui->webView->addAction(action);

    action = ui->webView->pageAction(QWebEnginePage::Cut);
    action->setShortcut(QKeySequence::Cut);
    ui->webView->addAction(action);

    action = ui->webView->pageAction(QWebEnginePage::Paste);
    action->setShortcut(QKeySequence::Paste);
    ui->webView->addAction(action);

    action = ui->webView->pageAction(QWebEnginePage::SelectAll);
    action->setShortcut(QKeySequence::SelectAll);
    ui->webView->addAction(action);

    action = ui->webView->pageAction(QWebEnginePage::Reload);
#ifdef Q_OS_OSX
    action->setShortcut(QKeySequence("Ctrl+R"));
#else
    action->setShortcut(QKeySequence("F5"));
#endif
    ui->webView->addAction(action);
}

void MainWindow::initMenus()
{
    QAction *aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, &QAction::triggered, this, [this]() {
        AboutDialog dialog(this);
        dialog.exec();
    });

    QAction *aboutQtAction = new QAction(tr("About Qt"), this);
    connect(aboutQtAction, &QAction::triggered, qApp, QApplication::aboutQt);

    QAction *preferencesAction = new QAction(tr("Preferences"), this);
    preferencesAction->setShortcut(QKeySequence("Ctrl+,"));

    connect(preferencesAction, &QAction::triggered, this, [this]() {
        PreferencesDialog dialog(this);
        if(dialog.exec() == QDialog::Accepted)
            initNotificationService();
    });

    QMenu *aboutMenu = menuBar()->addMenu(tr("Help"));
    aboutMenu->addAction(aboutAction);
    aboutMenu->addAction(aboutQtAction);
    aboutMenu->addSeparator();
    aboutMenu->addAction(preferencesAction);
}

void MainWindow::initTitleBar()
{
    QPixmap mainPixmap = this->style()->standardPixmap(QStyle::SP_TitleBarMenuButton);
    ui->mainMenu->setIcon(mainPixmap);
//    connect(ui->mainMenu, SIGNAL(clicked()), this, SLOT(showMinimized()));

    QPixmap minPixmap = this->style()->standardPixmap(QStyle::SP_TitleBarMinButton);
    ui->miniWindow->setIcon(minPixmap);
    connect(ui->miniWindow, SIGNAL(clicked()), this, SLOT(showMinimized()));

    QPixmap closePixmap = this->style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    ui->closeWindow->setIcon(closePixmap);
    connect(ui->closeWindow, &QToolButton::clicked, [&]{
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
        connect(animation, SIGNAL(finished()), qApp, SLOT(quit()));
        animation->setDuration(500);
        animation->setStartValue(1);
        animation->setEndValue(0);
        animation->start();
    });

    QPixmap downloadPixmap = this->style()->standardPixmap(QStyle::SP_ArrowDown);
    ui->openDownload->setIcon(downloadPixmap);
    connect(ui->openDownload, &QToolButton::clicked, [&]{
        static DownloadDialog * downloadDialog = new DownloadDialog(this);
        if(!downloadDialog->isHidden()) {
            downloadDialog->close();
        }
        else {
            QPoint showPos = QWidget::mapToGlobal(ui->openDownload->pos());
            showPos.setX(showPos.x() - (downloadDialog->size().width() - ui->openDownload->size().width()) / 2);
            showPos.setY(showPos.y() + ui->openDownload->size().height());
            downloadDialog->move(showPos);
            downloadDialog->show();
        }
    });
}
void MainWindow::initMainWebView()
{
    webViewService = new WebViewService(ui->webView, this);

    connect(webViewService, &WebViewService::titleChanged, this, &MainWindow::webViewTitleChanged);
    connect(webViewService, &WebViewService::loadStarted, this, &MainWindow::webViewLoadStarted);
    connect(webViewService, &WebViewService::loadProgress, this, &MainWindow::webViewLoadProgress);
    connect(webViewService, &WebViewService::loadFinished, this, &MainWindow::webViewLoadFinished);
    connect(ui->webView->page(), SIGNAL(linkClicked(const QUrl&)), this, SLOT(linkClicked(const QUrl&)));

    ui->webView->setUrl(HttpService::getInstance()->getUrl("home"));
}

void MainWindow::initDownloadManager()
{
    DownloadManager::getInstance();
}

void MainWindow::initLocalAppManager()
{
    connect(LocalAppManager::getInstance(), SIGNAL(appInstallBegin(const AppInfo &)),
            this, SLOT(appInstallBegin(const AppInfo &)));
    connect(LocalAppManager::getInstance(), SIGNAL(appInstallProgress(int)),
            this, SLOT(appInstallProgress(int)));
    connect(LocalAppManager::getInstance(), SIGNAL(appInstallComplete(const AppInfo &)),
            this, SLOT(appInstallComplete(const AppInfo &)));
}

void MainWindow::initNotificationService() {
    if(notificationService) notificationService->deleteLater();

    notificationService = NotificationService::getNotificationService(this);
    connect(notificationService, &NotificationService::notificationClicked, this, &MainWindow::notificationClicked);
    connect(notificationService, &NotificationService::notificationReplied, this, &MainWindow::notificationReplied);

    if(notificationWrapper) notificationWrapper->setNotificationService(notificationService);

    notificationWrapper = new JSNotifcationWrapper(notificationService, this);
}

void MainWindow::initWebService()
{
    initNotificationService();
    initDownloadManager();
    initLocalAppManager();

    auto channel = new QWebChannel(this);
    channel->registerObject("notificationService", notificationWrapper);
    channel->registerObject("webViewService", webViewService);

    channel->registerObject("downloadService", DownloadManager::getInstance());
    channel->registerObject("localAppService", LocalAppManager::getInstance());
    channel->registerObject("regExpService", RegExpUtils::getInstance());
    channel->registerObject("updateService", QSimpleUpdater::getInstance());
    channel->registerObject("httpService", HttpService::getInstance());

    ui->webView->page()->setWebChannel(channel);
}

void MainWindow::initHttpService()
{
    HttpService::getInstance();
}
