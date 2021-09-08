#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --------- Init the dispaly as black background ---------- //
    QPalette label_palette;
    label_palette.setColor(QPalette::Background, QColor(0, 0, 0));
    ui->image_label->setAutoFillBackground(true);
    ui->image_label->setPalette(label_palette);

    // --------- Init Information -------------//
    ui->log->setText("Program Begins ...");

    connect(ui->pushButton, SIGNAL( sendSignal() ), ui->widget, SLOT( ui->widget->startUDPServer()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

// udp server初始化函数
bool MainWindow::udpInit()
{
    quint16 local_port = 2333;
    udp_server = new QUdpSocket(this);

    //int ret = udp_server->bind( local_port, QUdpSocket::ShareAddress );
    int ret = udp_server->bind( QHostAddress("127.0.0.1"), local_port );
    if( ret < 0 ){
        ui->log->setText("Can not bind the IP address ...");
        ui->connection_status->setText("Bind Error: " + QString::number(ret));

        return false;
    }

    ui->log->setText("Bind the IP Address successfully And Connected to the Udp Server ...");
    connect(udp_server, SIGNAL(readyRead()), this, SLOT(udpServerReceiveData()));
    ui->connection_status->setText("Connected: " + QString::number(ret));

    return true;
}

// udp server接收槽函数
void MainWindow::udpServerReceiveData()
{
    QByteArray recvBuff;
    while( udp_server->hasPendingDatagrams() ){
        qDebug()<<"received length: "<<udp_server->pendingDatagramSize()<<endl;

        // Allocate the memory for the coming image
        recvBuff.resize( udp_server->pendingDatagramSize() );

        // Received the Data sent from the client
        udp_server->readDatagram(recvBuff.data(), recvBuff.size(), &client_address, &client_port);

        if( recvBuff[0] == 'L' && recvBuff[1] == 'E' && recvBuff[2] == 'N' && recvBuff[3] == 'G' ){
            memcpy( &frameSize, &recvBuff.data()[4], 4 );
            qDebug()<<"frame size : "<<frameSize<<endl;

            datagram.resize(frameSize);
        }
        else {
            memcpy( &datagram.data()[recvCount * 50000], recvBuff.data(), recvBuff.size() );
            recvCount ++;
            qDebug()<<"recvCount : "<<recvCount<<endl;

            if( recvCount == frameSize / 50000 + 1 ){
                recvCount = 0;
                getImageFromArray(datagram.data());
                ui->image_label->setPixmap(QPixmap::fromImage(this->image).scaled(ui->image_label->size()));
            }
        }
    }
}

void MainWindow::getImageFromArray( const QString &data )
{
    QByteArray imageData = QByteArray::fromBase64(data.toLatin1());

    this->image.loadFromData(imageData);

}


// 点击开始按钮
void MainWindow::on_pushButton_clicked()
{
    // 1. 点击开始按钮之后初始化 Udp Server
    this->udpInit();
}

// 点击停止按钮
void MainWindow::on_pushButton_2_clicked()
{
    // 关闭 Udp Server
    udp_server->close();
}
