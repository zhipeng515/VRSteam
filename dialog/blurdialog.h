#ifndef BLURDIALOG_H
#define BLURDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QTimer>

class BlurDialog : public QDialog
{
public:
    explicit BlurDialog(QWidget *parent = 0);
    ~BlurDialog();

    virtual void showEvent(QShowEvent *event);
    virtual void paintEvent(QPaintEvent *event);

    void grabBackground();

private:
    QPixmap backgroundPixmap;
    QTimer * timer;
};

#endif // BLURDIALOG_H
