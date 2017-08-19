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

    void on_checkBox_intervalSend_clicked();

    void on_btn_stopSend_clicked();

    void on_checkBox_repeatSend_clicked();

    void updateConnectedSocketAmount(qint32 number);

private:
    bool openFile(const QString &fileName);
    qint32 sendFile();
    void statusChange();
    void createFile();

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
    qint32 m_timerId;
    bool  m_isIntervalSend;
    bool m_isSending;
    bool m_isFileOpened;
    bool m_isRepeatSend;
    QFile m_fileSendedData;

    static const qint32 defaultPackageLength;
    static const qint32 defaultPortNumber;
    static const qint32 defaultSendInterval;
};

#endif // WIDGET_H
