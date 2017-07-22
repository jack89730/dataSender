#include "common.h"

#include <QMessageBox>
#include <QObject>

#include <QDebug>

void showWarningBox(const QString &caption, const QString &text)
{
    QMessageBox box(QMessageBox::Warning, caption, text);
    box.setStandardButtons(QMessageBox::Ok);
    box.setButtonText(QMessageBox::Ok, QObject::tr("确定"));
    box.exec();
}

qint32 calculatePercentage(const QFile &fileName)
{
     return (double)fileName.pos() / (fileName.bytesAvailable() + fileName.pos()) * 100;
}
