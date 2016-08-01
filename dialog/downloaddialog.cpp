#include "downloaddialog.h"
#include "ui_downloaddialog.h"
#include "view/downloaditemwidget.h"
#include "util/models.h"
#include "util/localappmanager.h"
#include <QResizeEvent>
#include <QScrollBar>

const QSize ARROW_SIZE = QSize(20, 10);

DownloadDialog::DownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadDialog)
{
    ui->setupUi(this);

    setStyleSheet ("#DownloadDialog{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 lightgray, stop: 1 gray)};color: rgb(255,255,255);");

    setWindowFlags(Qt::Popup);
    setFixedWidth(320);
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

    QList<AppInfo*> downloadApps = LocalAppManager::getInstance()->getDownloadApps();
    for(auto appInfo = downloadApps.begin(); appInfo != downloadApps.end(); appInfo++) {
        addDownloadItem( *appInfo);
    }

    connect(LocalAppManager::getInstance(), SIGNAL(appDownloadBegin(const QUrl&)),
            this, SLOT(appDownloadBegin(const QUrl&)));
}

void DownloadDialog::resizeEvent(QResizeEvent *event)
{
    clearMask();

    QPainterPath path(QPoint((event->size().width()-ARROW_SIZE.width())/2, ARROW_SIZE.height()));
    path.lineTo(event->size().width()/2, 0);
    path.lineTo((event->size().width()+ARROW_SIZE.width())/2, ARROW_SIZE.height());
    path.addRoundRect(QRectF(0,ARROW_SIZE.height(),event->size().width(),event->size().height()-ARROW_SIZE.height()), 4, 4);
    setMask(path.toFillPolygon().toPolygon());
}

DownloadDialog::~DownloadDialog()
{
    delete ui;
}

void DownloadDialog::addDownloadItem(const AppInfo * appInfo)
{
    QListWidgetItem * item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(QSize(320, 80));
    item->setData(Qt::UserRole, appInfo->id());
    ui->listWidget->addItem(item);
    DownloadItemWidget * downloadItemWidget = new DownloadItemWidget(appInfo->id(), this);
    ui->listWidget->setItemWidget(item, downloadItemWidget);
    int scrollCount = std::min(ui->listWidget->count(), 5);

    int height = scrollCount * downloadItemWidget->geometry().height() + ARROW_SIZE.height() + 5;
    setFixedHeight(height);
}

void DownloadDialog::appDownloadBegin(const QUrl & url)
{
    AppInfo * appInfo = LocalAppManager::getInstance()->getDownloadApp(url);
    if(appInfo) {
        addDownloadItem(appInfo);
    }
}
