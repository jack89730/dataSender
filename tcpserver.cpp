#include "tcpserver.h"
#include "common.h"
#include <QTcpSocket>
#include <QNetworkProxyFactory>

TcpServer::TcpServer()
    :m_connectedCounts(0)
{
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    connect(this, newConnection, this, processNewConnection);
}

TcpServer::~TcpServer()
{
    m_connectedSockets.clear();
}

bool TcpServer::init(qint32 v_portNumber)
{
    if(!listen(QHostAddress::Any, v_portNumber))
    {
        showWarningBox(tr("警告"), tr("监听端口失败"));
        return false;
    }

    return true;
}

void TcpServer::processNewConnection()
{
    if(hasPendingConnections())
    {
        QTcpSocket *tcpSocket = nextPendingConnection();
        if(tcpSocket->isValid())
        {
            m_connectedSockets.push_back(tcpSocket);
        }
        else
        {

        }
    }
}

void TcpServer::sendDataToAllClients(const char *v_data, qint32 v_dataLength)
{
    for(QTcpSocket *tcpSocket : m_connectedSockets)
    {
        tcpSocket->write(v_data, v_dataLength);
    }
}
