#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QUdpSocket>
#include <QImage>

#include <QNetworkInterface>
#include <QBuffer>

#include <QOpenGLWidget>

#include "mypainter.h"

#include <opencv2/opencv.hpp>

#include "dataType.h"

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

    bool resultsUDPInit();

    bool resultsUpdInit();

    void getImageFromArray( const QString &data );

    void setRecvImgSize( int width, int height );

private slots:
    void udpServerReceiveData();
    void udpReceiveResults();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    QUdpSocket *udp_server;
    QUdpSocket *udp_server2;

    QHostAddress client_address;
    quint16 client_port = 0;

    QHostAddress client_address2;
    quint16 client_port2 = 0;


    // --------- 接收检测图片的相关参数 ------ //
    QByteArray datagram;
    int frameSize = 0;
    int recvCount = 0;

    QImage image;

    int recvImgHeight = 720;
    int recvImgWidth = 1280;

    // ---------- 检测结果相关参数 ---------- //
    std::vector<cv::Rect> objectRects;
    std::vector<std::vector<cv::Point2f>> linePoints;

    std::vector<cv::Point> objectsTypesPoses;
    std::vector<ObjectDetectType> objectsTypes;;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
