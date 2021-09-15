#ifndef MYPAINTER_H
#define MYPAINTER_H

#include <QWidget>

#include <QPainter>
#include <QTime>
#include <QTimer>

#include <QUdpSocket>


class MyPainter : public QWidget, QPainter
{
    Q_OBJECT
public:
    MyPainter( QWidget *parent = nullptr );
    ~MyPainter();

public:
    void drawACar( int x, int y );
    void startUDPReceiver();
    void closeUDPReceiver();

private:
    void paintEvent(QPaintEvent *event);

private:
    QUdpSocket *udp_server;

    QHostAddress client_address;
    quint16 client_port = 0;

private slots:
    void udpServerReceiveData();

};

#endif // MYPAINTER_H
