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
            updateConnectedCounts(COUNTS_ADD);

            connect(tcpSocket, QTcpSocket::stateChanged, this, deleteSocket);
        }
        else
        {

        }
    }
}
/*
 *功能：
 * 当客户端断开链接时，删除对应的套接字
 *输入参数：
 *输出参数：
 *返回值：
*/
void TcpServer::deleteSocket()
{

    for(auto s : m_connectedSockets)
    {
        if(QTcpSocket::UnconnectedState == s->state())
        {
            m_connectedSockets.removeOne(s);
            updateConnectedCounts(COUNTS_SUB);
        }
    }

}
/*
 *功能：
 * 根据参数对客户端计数进行加一或减一操作
 *输入参数：
 *输出参数：
 *返回值：
*/
void TcpServer::updateConnectedCounts(TcpServer::CountsOperation operation)
{
    if(COUNTS_ADD == operation)
    {
        ++m_connectedCounts;
    }

    if(COUNTS_SUB == operation)
    {
        --m_connectedCounts;
    }

    emit connectedAmount(m_connectedCounts);
}

void TcpServer::sendDataToAllClients(const char *v_data, qint32 v_dataLength)
{
   // qint32 retVal = 0;
    for(QTcpSocket *tcpSocket : m_connectedSockets)
    {
        tcpSocket->write(v_data, v_dataLength);

    }
}

qint32 TcpServer::getConnectedAmount()
{
    return m_connectedCounts;
}
