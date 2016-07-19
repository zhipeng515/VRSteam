1.C++暴露给JS的对象在mainwindow.h文件中

依赖QWebChannel.js

JSNotifcationWrapper *notificationWrapper;
WebViewService  * webViewService;
DownloadManager * downloadManager;
LocalAppManager * localAppManager;

使用以下代码注册给JS
auto channel = new QWebChannel(this);
channel->registerObject("notificationService", notificationWrapper);
channel->registerObject("downloadService", downloadManager);
channel->registerObject("localAppService", localAppManager);
channel->registerObject("webViewService", webViewService);
ui->webView->page()->setWebChannel(channel);


2.JS文件使用下面代码接受C++对象
new QWebChannel(qt.webChannelTransport, function(channel) {
    var notificationService = channel.objects.notificationService;
    var downloadService = channel.objects.downloadService;
    var localAppService = channel.objects.localAppService;
    var webViewService = channel.objects.webViewService;
}

3.JS可以调用的C++对象方法及信号处理在对应的.h中声明

slots关键字之后的方法是可以直接调用的
webViewService.load("http://vr.sina.com.cn”);

如果方法带有返回值使用回调方式获得返回值并处理
localAppService.isAppInstalled(0, function(isInstalled){
    if(isInstalled)
        alert("a");
    else
        alert("b");
});

signal关键字对应的处理方法
对象.signal.connect
downloadService.progress.connect(function(progress) {
    console.log("js log: %" + progress);
});

