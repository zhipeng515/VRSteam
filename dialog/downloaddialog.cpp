#include "downloaddialog.h"
#include "ui_downloaddialog.h"
#include "view/downloaditemwidget.h"
#include "util/models.h"
#include "util/localappmanager.h"
#include <QResizeEvent>
#include <QScrollBar>
#include <QPainter>

const QSize ARROW_SIZE = QSize(20, 10);

DownloadDialog::DownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadDialog)
{
    ui->setupUi(this);

//    installEventFilter(this);

    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setFixedWidth(320);
    // 平台注意事项：
    // X11（linux）：此特性依赖于能提供支持ARGB视觉效果和复合式视窗管理的X服务的功能开启。
    // Windows：此控件需要设置窗口标志Qt::FramelessWindowHint才能开启透明功能。
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);

//    setStyleSheet ("#DownloadDialog{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 lightgray, stop: 1 gray)};color: rgb(255,255,255);");

    ui->listWidget->setFrameShape(QListWidget::NoFrame);
    ui->listWidget->setStyleSheet("background:  transparent;}");
    ui->listWidget->verticalScrollBar()->setStyleSheet("QScrollBar:vertical"
                                                       "{"
                                                       "width:5px;"
                                                       "background:rgba(0,0,0,0%);"
                                                       "margin:0px,0px,0px,0px;"
                                                       "padding-top:2px;"
                                                       "padding-bottom:2px;"
                                                       "}"
                                                       "QScrollBar::handle:vertical"
                                                       "{"
                                                       "width:5px;"
                                                       "background:rgba(0,0,0,25%);"
                                                       "border-radius:2px;"
                                                       "min-height:40;"
                                                       "}"
                                                       );

    connect(LocalAppManager::getInstance(), SIGNAL(appDownloadComplete(const QUrl&)),
            this, SLOT(appDownloadComplete(const QUrl&)));
    connect(LocalAppManager::getInstance(), SIGNAL(appDownloadBegin(const QUrl&)),
            this, SLOT(appDownloadBegin(const QUrl&)));
}

void DownloadDialog::showEvent(QShowEvent *event)
{
    QList<AppInfo*> downloadApps = LocalAppManager::getInstance()->getDownloadApps();
    for(auto appInfo = downloadApps.begin(); appInfo != downloadApps.end(); appInfo++) {
        addDownloadItem( *appInfo);
    }
    QDialog::showEvent(event);
}

void DownloadDialog::hideEvent(QHideEvent *event)
{
    ui->listWidget->clear();
    QDialog::hideEvent(event);
}

void DownloadDialog::paintEvent(QPaintEvent *event)
{
    QRect rect = this->rect();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QLinearGradient linearGradient(rect.width()/2,0,rect.width()/2,rect.height());
    linearGradient.setColorAt(0, Qt::lightGray);
    linearGradient.setColorAt(1, Qt::darkGray);
    painter.setBrush(QBrush(linearGradient));
    painter.setPen(Qt::NoPen);

    QPainterPath path(QPoint((rect.width()-ARROW_SIZE.width())/2, ARROW_SIZE.height()));
    path.lineTo(rect.width()/2, 0);
    path.lineTo((rect.width()+ARROW_SIZE.width())/2, ARROW_SIZE.height());
    painter.drawPath(path);

    rect.setY(ARROW_SIZE.height());
    painter.drawRoundedRect(rect, 5, 5);


    QDialog::paintEvent(event);
}

void DownloadDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
}

bool DownloadDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FocusOut ||
        event->type() == QEvent::KeyPress ||
        event->type() == QEvent::MouseButtonPress)
    {
        close();
    }
    return QDialog::eventFilter(obj, event);
}
DownloadDialog::~DownloadDialog()
{
    delete ui;
}

void DownloadDialog::resizeDialog()
{
    int scrollCount = std::min(ui->listWidget->count(), 5);
    int height = scrollCount * 80 + ARROW_SIZE.height() + 5;
    setFixedHeight(height);
}

void DownloadDialog::addDownloadItem(const AppInfo * appInfo)
{
    QListWidgetItem * item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(QSize(320, 80));
    item->setData(Qt::UserRole, appInfo->id());
    ui->listWidget->addItem(item);
    DownloadItemWidget * downloadItemWidget = new DownloadItemWidget(appInfo->id(), this);
    ui->listWidget->setItemWidget(item, downloadItemWidget);

    resizeDialog();
}

void DownloadDialog::appDownloadBegin(const QUrl & url)
{
    AppInfo * appInfo = LocalAppManager::getInstance()->getDownloadApp(url);
    if(appInfo) {
        addDownloadItem(appInfo);
    }
}

void DownloadDialog::appDownloadComplete(const QUrl & url)
{
    AppInfo * appInfo = LocalAppManager::getInstance()->getDownloadApp(url);
    if(appInfo && appInfo->isValid()) {
        for(int i = 0; i < ui->listWidget->count(); i++) {
            QListWidgetItem * item = ui->listWidget->takeItem(i);
            int appId = item->data(Qt::UserRole).toInt();
            if(appId == appInfo->id()) {
                ui->listWidget->removeItemWidget(item);
                delete item;
                resizeDialog();
                break;
            }
        }
    }
}

