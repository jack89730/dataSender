#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QFile>
void showWarningBox(const QString &caption, const QString &text);
qint32 calculatePercentage(const QFile &fileName);
#endif // COMMON_H
