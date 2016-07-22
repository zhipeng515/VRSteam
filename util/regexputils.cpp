#include "regexputils.h"
#include <QDebug>

RegExpUtils * RegExpUtils::getInstance()
{
    static RegExpUtils regExpUtils;
    return &regExpUtils;
}

RegExpUtils::RegExpUtils(QObject *parent) : QObject(parent)
{

}

bool RegExpUtils::isContainUrl(const QString &strText)
{
    bool bResult = false;
    QString strTempText = strText;
    QString strUrlExp = "((http|https|ftp)://|(www)\\.)(\\w+)(\\.?[\\.a-z0-9/:?%&=\\-_+#;]*)";
    QRegExp urlRegExp(strUrlExp,Qt::CaseInsensitive); //Url正则表达式，不区分大小写
    while(urlRegExp.indexIn(strTempText) != -1)
    {
        bResult = true;
        QString strWebUrl = urlRegExp.cap(0);//匹配到的url

        qDebug() << strWebUrl; //输出url
        int nIndex = strTempText.indexOf(strWebUrl); //索引位置
        strTempText.remove(0,nIndex+strWebUrl.size()); //删除已遍历过的内容
    }
    return bResult; //返回是否包含url
}

bool RegExpUtils::isUrl(const QString &strText)
{
    bool bResult = false;
    QString strTempText = strText;
    QString strUrlExp = "((http|https|ftp)://|(www)\\.)(\\w+)(\\.?[\\.a-z0-9/:?%&=\\-_+#;]*)";
    QRegExp urlRegExp(strUrlExp,Qt::CaseInsensitive); //Url正则表达式，不区分大小写
    if(urlRegExp.exactMatch(strTempText))
    {
        bResult = true;
        QString strWebUrl = urlRegExp.cap(0);//匹配到的url

        qDebug() << strWebUrl; //输出url
    }
    return bResult; //返回是否包含url
}

QStringList RegExpUtils::matchCustomUrl(const QString& url)
{
    QStringList strList;
    if(!url.isEmpty())
    {
        QString strTempUrl = url; //需要进行正则匹配的内容
        QRegExp customProtoclExp("([A-Za-z]{1,10})://([A-Za-z0-9]{1,20})\\?(.+$)");
        customProtoclExp.setMinimal(true); //最小匹配模式

        if(customProtoclExp.exactMatch(strTempUrl))
        {
            if(customProtoclExp.captureCount() == 3)
            {
                QString strProtocol = customProtoclExp.cap(1);
                QString strOperator = customProtoclExp.cap(2);
                QString strOperatorParam = customProtoclExp.cap(3);

                qDebug() << "Protocol : " << strProtocol << "Operator : " << strOperator << "OperatorParam : " << strOperatorParam;

                strList.append(strProtocol);
                strList.append(strOperator);
                strList.append(strOperatorParam);

                QString strCustomUrl = customProtoclExp.cap(0);//匹配到的url
                qDebug() << strCustomUrl; //输出url
            }
        }
    }

    return strList;
}
