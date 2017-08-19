#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QVector>

class TcpServer : public QTcpServer
{
    Q_OBJECT

    enum CountsOperation{
        COUNTS_ADD,
        COUNTS_SUB
    };
public:
    TcpServer();
    ~TcpServer();

    bool init(qint32 v_portNumber);
    void sendDataToAllClients(const char *v_data, qint32 v_dataLength);
    qint32 getConnectedAmount();

signals:
    void connectedAmount(qint32 count);

private slots:
    void processNewConnection();
    void deleteSocket();

private:
    qint32 m_connectedCounts;
    QVector<QTcpSocket *> m_connectedSockets;           //保存创建的链接
    void updateConnectedCounts(CountsOperation operation);
};

#endif // TCPSERVER_H
