#include "widget.h"
#include "ui_mainWindow.h"
#include <QFileDialog>
#include "common.h"
#include <QDateTime>

const qint32 Widget::defaultPackageLength = 512;
const qint32 Widget::defaultPortNumber = 0;
const qint32 Widget::defaultSendInterval = 10;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    m_tcpServer(new TcpServer),
    m_listenPort(defaultPortNumber),
    m_packageLength(defaultPackageLength),
    m_sendInterval(defaultSendInterval),
    m_isIntervalSend(false),
    m_isSending(false),
    m_isFileOpened(false),
    m_isRepeatSend(false)

{
    ui->setupUi(this);
    m_fileName = QString();
    ui->btn_startListen->setEnabled(false);
    connect(m_tcpServer, TcpServer::connectedAmount, this, updateConnectedSocketAmount);

    statusChange();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_lineEdit_portNumber_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        ui->btn_startListen->setEnabled(false);
        m_listenPort = defaultPortNumber;
    }
    else
    {
        if(m_listenPort != arg1)
        {
            ui->btn_startListen->setEnabled(true);
        }

        m_listenPort = arg1.toInt();
    }
}

void Widget::on_lineEdit_packageLength_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
       m_packageLength = defaultPackageLength;
    }
    else
    {
        qint32 packageLength = arg1.toInt();
        if(0 != packageLength)
        {
            m_packageLength = packageLength;
        }
    }
}

void Widget::on_lineEdit_sendInterval_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        m_sendInterval =defaultSendInterval;
    }
    else
    {
        qint32 sendInterval = arg1.toInt();
        if(0 != sendInterval)
        {
            m_sendInterval = sendInterval;
        }
    }
}



void Widget::on_btn_startListen_clicked()
{
    m_tcpServer->close();
    m_tcpServer->init(m_listenPort);
    ui->btn_startListen->setEnabled(false);
}

void Widget::on_btn_openFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择要传输的文件"), ".");
    if(fileName.isNull())
    {
        showWarningBox(tr("警告"), tr("没有选择要传输的文件"));
    }
    else
    {
        m_file.close();

        m_file.setFileName(fileName);

        if(!m_file.open(QIODevice::ReadOnly))
        {
            showWarningBox(tr("警告"), tr("打开文件失败"));
        }
        else
        {
            m_isFileOpened = true;
            m_fileName = fileName;
            ui->lineEdit_openedFile->setText(m_fileName);
            m_inStream.setDevice(&m_file);

            createFile();
            statusChange();
        }
    }
}

void Widget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)

    sendFile();
}

void Widget::on_btn_startSend_clicked()
{
    if(m_fileName.isEmpty())
    {
        showWarningBox(tr("警告"), tr("没有要传输的文件"));
        return;
    }
    if(m_isIntervalSend)
    {
        m_timerId = startTimer(m_sendInterval);
    }
    else
    {
        m_timerId = startTimer(1);
    }

    m_isSending = true;

    statusChange();
}

void Widget::on_checkBox_intervalSend_clicked()
{
    m_isIntervalSend = ui->checkBox_intervalSend->isChecked();
}

qint32 Widget::sendFile()
{
    char *ch = new char[m_packageLength];
    qint32 readLength = 0;

   readLength = m_inStream.readRawData(ch, m_packageLength);

   qDebug() << readLength;
   if(-1 == readLength)
   {
       return -1;
   }

   if(0 == readLength)
   {
       if(m_isRepeatSend)
       {
           m_file.seek(0);
           readLength = m_inStream.readRawData(ch, m_packageLength);
       }
       else
       {
           return 0;
       }
   }

   /*
    * 将发送的数据存储到本地
*/
   QDataStream out(&m_fileSendedData);
   out.writeRawData(ch, readLength);
   m_fileSendedData.flush();
   m_tcpServer->sendDataToAllClients(ch, readLength);

   ui->progressBar->setValue(calculatePercentage(m_file));

   delete[] ch;

   return readLength;
}

void Widget::statusChange()
{


    if((!m_isSending) && (m_isFileOpened))
    {
        ui->btn_startSend->setEnabled(true);
    }
    else
    {
        ui->btn_startSend->setEnabled(false);
    }

    if(m_isSending)
    {
        ui->btn_stopSend->setEnabled(true);
    }
    else
    {
        ui->btn_stopSend->setEnabled(false);
    }

    if(m_isSending)
    {
        ui->btn_openFile->setEnabled(false);
    }
    else
    {
        ui->btn_openFile->setEnabled(true);
    }

    if(m_fileName.isEmpty())
    {
        ui->btn_startSend->setEnabled(false);
        ui->btn_stopSend->setEnabled(false);
    }
}

void Widget::createFile()
{
    m_fileSendedData.close();
    QDateTime dateTime = QDateTime::currentDateTime();
    QString fileName = dateTime.toString(QString("yyyyMMddHHmmss"));
    m_fileSendedData.setFileName(fileName+".txt");
    if(!m_fileSendedData.open(QIODevice::WriteOnly))
    {
        showWarningBox(tr("警告"), tr("创建用于存储发送数据的文件失败!"));
    }
}

void Widget::on_btn_stopSend_clicked()
{
    killTimer(m_timerId);
    m_isSending = false;
    statusChange();
}

void Widget::on_checkBox_repeatSend_clicked()
{
    m_isRepeatSend = ui->checkBox_repeatSend->isChecked();
}

/*
 *功能：
 * 更新当前链接的客户端的数量
 *输入参数：
 *输出参数：
 *返回值：
*/
void Widget::updateConnectedSocketAmount(qint32 number)
{
    ui->lineEdit_connectedCounts->setText(QString("%1").arg(number));
}
