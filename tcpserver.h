#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QVector>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer();
    ~TcpServer();

    bool init(qint32 v_portNumber);
    void sendDataToAllClients(const char *v_data, qint32 v_dataLength);

private:
    void processNewConnection();

private:
    qint32 m_connectedCounts;
    QVector<QTcpSocket *> m_connectedSockets;
};

#endif // TCPSERVER_H
