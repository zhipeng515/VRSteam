#include "activitydialog.h"
#include "ui_activitydialog.h"
#include "view/webpage.h"
#include "util/urlservice.h"
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>

ActivityDialog::ActivityDialog(QWidget *parent) :
    BlurDialog(parent),
    ui(new Ui::ActivityDialog),
    currentPage(0)
{
    ui->setupUi(this);
    ui->frame->setAttribute(Qt::WA_TransparentForMouseEvents,true);

    setWindowTitle(tr("Activity"));
    setFixedSize(400, 600);

    setAttribute(Qt::WA_DeleteOnClose);
//    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    connect(static_cast<WebPage*>(ui->webView->page()), &WebPage::linkClicked, [&](const QUrl&url){
        URLService::getInstance()->handleURL(url, ui->webView);
    });

    QDesktopWidget* desktopWidget = QApplication::desktop();
    int x = (desktopWidget->availableGeometry().size().width() - width()) / 2;
    int y = (desktopWidget->availableGeometry().size().height() - height()) / 2;
    move(x , y);
}

ActivityDialog::~ActivityDialog()
{
    delete ui;
}

void ActivityDialog::setActivityUrls(const QStringList & urls)
{
    currentPage = 0;
    activityUrls = urls;
    ui->webView->load(activityUrls[currentPage]);
    updateButtonState();
}

void ActivityDialog::on_close_clicked()
{
    accept();
}

void ActivityDialog::on_prevPage_clicked()
{
    if(currentPage > 0)
        currentPage--;
    ui->webView->load(activityUrls[currentPage]);
    updateButtonState();
}

void ActivityDialog::on_nextPage_clicked()
{
    if(currentPage < activityUrls.count()-1)
        currentPage++;
    ui->webView->load(activityUrls[currentPage]);
    updateButtonState();
}

void ActivityDialog::updateButtonState()
{
    ui->prevPage->setEnabled(currentPage != 0);
    ui->nextPage->setEnabled(currentPage != activityUrls.count()-1);
}
