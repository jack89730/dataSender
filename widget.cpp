#include "widget.h"
#include "ui_mainWindow.h"
#include <QFileDialog>
#include "common.h"

const qint32 Widget::defaultPackageLength = 512;
const qint32 Widget::defaultPortNumber = 0;
const qint32 Widget::defaultSendInterval = 10;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    m_tcpServer(new TcpServer),
    m_listenPort(defaultPortNumber),
    m_packageLength(defaultPackageLength),
    m_sendInterval(defaultSendInterval)
{
    ui->setupUi(this);
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
        ui->btn_startListen->setEnabled(true);
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
    m_tcpServer->init(m_listenPort);
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
        m_fileName = fileName;
        ui->lineEdit_openedFile->setText(m_fileName);
        m_file.setFileName(m_fileName);

        if(!m_file.open(QIODevice::ReadOnly))
        {
            showWarningBox(tr("警告"), tr("打开文件失败"));
        }
        else
        {6+
            m_inStream.setDevice(&m_file);
        }
    }
}

void Widget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
}

void Widget::on_btn_startSend_clicked()
{
    char *ch = new char[5];
    qint32 readLength = 0;

    while((readLength = m_inStream.readRawData(ch, 4)))
    {
        m_tcpServer->sendDataToAllClients(ch, readLength);
    }

}
