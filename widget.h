#ifndef WIDGET_H
#define WIDGET_H
#include "tcpserver.h"
#include <QFile>
#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();



private slots:
    void on_lineEdit_portNumber_textChanged(const QString &arg1);

    void on_lineEdit_packageLength_textChanged(const QString &arg1);

    void on_lineEdit_sendInterval_textChanged(const QString &arg1);

    void on_btn_startListen_clicked();

    void on_btn_openFile_clicked();

    void on_btn_startSend_clicked();

private:
    bool openFile(const QString &fileName);

protected:
    virtual void timerEvent(QTimerEvent *event);

private:
    Ui::Widget *ui;

    QString m_fileName;
    TcpServer *m_tcpServer;
    qint32 m_listenPort;
    qint32 m_packageLength;
    qint32 m_sendInterval;
    QFile m_file;
    QDataStream m_inStream;

    static const qint32 defaultPackageLength;
    static const qint32 defaultPortNumber;
    static const qint32 defaultSendInterval;
};

#endif // WIDGET_H
