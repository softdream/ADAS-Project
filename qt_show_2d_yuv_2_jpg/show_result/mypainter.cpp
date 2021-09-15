#include "mypainter.h"
#include <QDebug>

MyPainter::MyPainter(QWidget *parent) : QWidget(parent)
{
    QPalette pal( this->palette() );
    pal.setColor(QPalette::Background, Qt::black);

    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

MyPainter::~MyPainter()
{

}

void MyPainter::paintEvent(QPaintEvent *event)
{
    QPainter paint( this );

    paint.setPen(Qt::red);
    paint.setBrush(Qt::green);

    paint.drawLine(150, 200, 150, 600);
    paint.drawLine(220, 200, 220, 600);
    paint.drawLine(290, 200, 290, 600);
    paint.drawLine(360, 200, 360, 600);

    paint.drawRect( 167, 300, 35, 45 );
    paint.drawRect( 307, 400, 35, 45 );
    paint.drawRect( 237, 280, 35, 45 );
}

void MyPainter::drawACar(int x, int y)
{
    QPainter paint( this );
    paint.drawRect(x, y, 35, 45);
}

void MyPainter::startUDPReceiver()
{
    qDebug()<<"start a Udp Server ..."<<endl;

    quint16 local_port = 2334;
    udp_server = new QUdpSocket(this);

    //int ret = udp_server->bind( local_port, QUdpSocket::ShareAddress );
    int ret = udp_server->bind( QHostAddress("192.168.22.69"), local_port );
    if( ret < 0 ){
        qDebug() <<"start the udp server failed ..."<<endl;
    }

    connect(udp_server, SIGNAL(readyRead()), this, SLOT(udpServerReceiveData()));
}

void MyPainter::closeUDPReceiver()
{
    this->udp_server->close();
    qDebug() <<"close the udp server ..."<<endl;
}

void MyPainter::udpServerReceiveData()
{
    QByteArray recvBuff;
    while( udp_server->hasPendingDatagrams() ){
        qDebug()<<"received length: "<<udp_server->pendingDatagramSize()<<endl;

        // Allocate the memory for the coming image
        recvBuff.resize( udp_server->pendingDatagramSize() );

        // Received the Data sent from the client
        udp_server->readDatagram(recvBuff.data(), recvBuff.size(), &client_address, &client_port);


    }
}
