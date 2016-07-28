#ifndef DOWNLOADITEMWIDGET_H
#define DOWNLOADITEMWIDGET_H

#include <QWidget>

namespace Ui {
class DownloadItemWidget;
}

class DownloadItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadItemWidget(QWidget *parent = 0);
    ~DownloadItemWidget();

private:
    Ui::DownloadItemWidget *ui;
};

#endif // DOWNLOADITEMWIDGET_H
