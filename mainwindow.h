/*
 * This file is part of WhatsQt.
 *
 * WhatsQt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WhatsQt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WhatsQt.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>


#include "notification/notificationservice.h"
#include "view/webview.h"

namespace Ui {
class MainWindow;
}

class WebViewService : public QObject
{
    Q_OBJECT

public:
    explicit WebViewService(WebView * view, QWidget* parent = Q_NULLPTR) : QObject(parent), webView(view)
    {
        connect(webView, &WebView::loadStarted, this, &WebViewService::loadStarted);
        connect(webView, &WebView::loadProgress, this, &WebViewService::loadProgress);
        connect(webView, &WebView::loadFinished, this, &WebViewService::loadFinished);
        connect(webView, &WebView::titleChanged, this, &WebViewService::titleChanged);
        connect(webView, &WebView::urlChanged, this, &WebViewService::urlChanged);
    }

public slots:
    void load(const QUrl& url) { webView->load(url); }
    void stop() { webView->stop(); }
    void back() { webView->back(); }
    void forward() { webView->forward(); }
    void reload() { webView->reload(); }

signals:
    void loadStarted();
    void loadProgress(int progress);
    void loadFinished(bool);
    void titleChanged(const QString& title);
    void urlChanged(const QUrl& url);

private:
    WebView * webView;
};

class JSNotifcationWrapper : public QObject {

    Q_OBJECT

public:
    JSNotifcationWrapper(NotificationService *service, QObject *parent = 0);

    void setNotificationService(NotificationService *notificationService);
    Q_INVOKABLE void deliverNotification(const QString &title, const QMap<QString, QVariant> &options);

private:
    NotificationService *notificationService;

private slots:
    void onNotificationClicked(const Notification &notification);
    void onNotificationReplied(const Notification &notification, const QString &reply);

signals:
    void notificationClicked(const QString &title, const QMap<QString, QVariant> &options);
    void notificationReplied(const QString &title, const QMap<QString, QVariant> &options, const QString &reply);
};

class DownloadManager;
class LocalAppManager;
class AppInfo;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);

protected:
    bool event(QEvent *event);

    virtual void showEvent(QShowEvent *event);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);


private:
    Ui::MainWindow *ui;
    NotificationService *notificationService;
    JSNotifcationWrapper *notificationWrapper;

    WebViewService  * webViewService;

    bool beginDrag;
    QPoint dragPosition;

    void saveSettings();
    void readSettings();

    void initActions();
    void initMenus();
    void initTitleBar();

    void initDownloadManager();
    void initLocalAppManager();
    void initNotificationService();

    void initMainWebView();
    void initWebService();
    void initHttpService();

#ifdef Q_OS_OSX
    void nativeSetup();
#else
    void nativeSetup() {}
#endif

public slots:
    void checkSelfUpdate();
    void checkActivitys();

private slots:
    void downloadComplete(const QUrl & url);
    void downloadProgress(const QUrl & url, int nPercentage);

    void webViewLoadStarted();
    void webViewLoadProgress(int progress);
    void webViewLoadFinished(bool);


    void appInstallBegin(const AppInfo & appInfo);
    void appInstallProgress(int percentage);
    void appInstallComplete(const AppInfo & appInfo);

    void linkClicked(const QUrl & url);
    void webViewTitleChanged(const QString &title);
    void webViewUrlChanged(const QUrl & url);

    void notificationClicked(const Notification &notification);
    void notificationReplied(const Notification &notification, const QString &reply);

    void updateCheckingFinished(QString);
    void updateDownloadFinished(QString, QString);
};

#endif // MAINWINDOW_H
