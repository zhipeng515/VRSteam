#ifndef ACTIVITYDIALOG_H
#define ACTIVITYDIALOG_H

#include "dialog/blurdialog.h"
#include <QStringList>

namespace Ui {
class ActivityDialog;
}

class ActivityDialog : public BlurDialog
{
    Q_OBJECT

public:
    explicit ActivityDialog(QWidget *parent = 0);
    ~ActivityDialog();

public slots:
    void setActivityUrls(const QStringList & urls);

private slots:
    void on_close_clicked();

    void on_prevPage_clicked();

    void on_nextPage_clicked();

    void updateButtonState();

private:
    Ui::ActivityDialog *ui;

    QStringList activityUrls;
    int currentPage;
};

#endif // ACTIVITYDIALOG_H
