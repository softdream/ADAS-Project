#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QUdpSocket>
#include <QImage>

#include <QNetworkInterface>
#include <QBuffer>

#include <QOpenGLWidget>

#include "mypainter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // init the udp server
    bool udpInit();

    void getImageFromArray( const QString &data );

signals:
    void sendSignal();

private slots:
    void udpServerReceiveData();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    QUdpSocket *udp_server;

    QHostAddress client_address;
    quint16 client_port = 0;

    QByteArray datagram;
    int frameSize = 0;
    int recvCount = 0;

    QImage image;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
