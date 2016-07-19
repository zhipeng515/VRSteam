#ifndef REGEXPUTILS_H
#define REGEXPUTILS_H

#include <QObject>

class RegExpUtils : public QObject
{
    Q_OBJECT
public:
    explicit RegExpUtils(QObject *parent = 0);

    static RegExpUtils & Instance();

signals:

public slots:
    bool isContainUrl(const QString& url);
    bool isUrl(const QString& url);
    QStringList matchCustomUrl(const QString& url);
};

#endif // REGEXPUTILS_H
