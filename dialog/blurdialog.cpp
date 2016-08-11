#include "blurdialog.h"
#include <QScreen>
#include <QGraphicsBlurEffect>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

QPixmap BlurAPixmap(const QPixmap& inPixmap, qreal blurRadius)
{
    QGraphicsScene scene;

    QPixmap output(inPixmap.width(), inPixmap.height());
    output.fill(Qt::transparent);
    QPainter painter(&output);

    QGraphicsPixmapItem blurItem;
    blurItem.setPixmap(inPixmap);
    QGraphicsBlurEffect blurEffect;
    blurEffect.setBlurHints(QGraphicsBlurEffect::QualityHint);
    blurEffect.setBlurRadius(blurRadius);
    blurItem.setGraphicsEffect(&blurEffect);
    scene.addItem(&blurItem);
    scene.render(&painter);
    scene.removeItem(&blurItem);

    QGraphicsPixmapItem colorizeItem;
    colorizeItem.setPixmap(output);
    QGraphicsColorizeEffect colorizeEffect;
    colorizeEffect.setColor(Qt::darkGray);
    colorizeEffect.setStrength(0.5);
    colorizeItem.setGraphicsEffect(&colorizeEffect);
    scene.addItem(&colorizeItem);
    scene.render(&painter);
    scene.removeItem(&colorizeItem);

    return output;
}

BlurDialog::BlurDialog(QWidget *parent) :
    QDialog(parent),
    timer(new QTimer(this))
{
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    // 平台注意事项：
    // X11（linux）：此特性依赖于能提供支持ARGB视觉效果和复合式视窗管理的X服务的功能开启。
    // Windows：此控件需要设置窗口标志Qt::FramelessWindowHint才能开启透明功能。
    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);
}

BlurDialog::~BlurDialog()
{
}

void BlurDialog::grabBackground()
{
    QPoint screenPos = QWidget::mapToGlobal(pos());
    QScreen * primaryScreen = QGuiApplication::primaryScreen();
    QPixmap pixmap = primaryScreen->grabWindow(QApplication::desktop()->winId(),
                                                                  screenPos.x()/primaryScreen->devicePixelRatio(),
                                                                  screenPos.y()/primaryScreen->devicePixelRatio(),
                                                                  size().width()/primaryScreen->devicePixelRatio(),
                                                                  size().height()/primaryScreen->devicePixelRatio()); // 獲取整個屏幕窗口
    qDebug() << "1111" << pixmap.save("c:\\1.png", "PNG");
    backgroundPixmap = BlurAPixmap(pixmap, 10);
    qDebug() << "2222" << backgroundPixmap.save("c:\\2.png", "PNG");
}

void BlurDialog::showEvent(QShowEvent *event)
{
    grabBackground();

    QDialog::showEvent(event);
}

void BlurDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawPixmap(0, 0, backgroundPixmap);

    QDialog::paintEvent(event);
}
