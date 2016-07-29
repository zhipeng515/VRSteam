#ifndef NETWORKICONLABEL_H
#define NETWORKICONLABEL_H

#include <QLabel>

class NetworkIconLabel : public QLabel
{
    Q_OBJECT
public:
    explicit NetworkIconLabel(QWidget *parent = 0);

signals:

public slots:
    void setUrl(const QUrl &url);
};

#endif // NETWORKICONLABEL_H
