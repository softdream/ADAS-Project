#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <vector>

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

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
@   初始化接收图像数据的UDP
@
*/
bool MainWindow::udpInit()
{
    quint16 local_port = 2333;
    udp_server = new QUdpSocket(this);

    //int ret = udp_server->bind( local_port, QUdpSocket::ShareAddress );
    int ret = udp_server->bind( QHostAddress("192.168.22.69"), local_port );
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

/*
@   初始化接收检测结果的UDP
@
*/
bool MainWindow::resultsUDPInit()
{
    quint16 local_port = 2334;
    udp_server2 = new QUdpSocket(this);

    //int ret = udp_server->bind( local_port, QUdpSocket::ShareAddress );
    int ret = udp_server2->bind( QHostAddress("192.168.22.69"), local_port );
    if( ret < 0 ){
        qDebug()<<"Can not init the results udp ..."<<endl;
        return false;
    }

    ui->log->insertPlainText("Init the UDP Results ...  ");
    connect(udp_server2, SIGNAL(readyRead()), this, SLOT(udpReceiveResults()));

    return true;
}

/*
@   接收检测结果数据
@
*/
void MainWindow::udpReceiveResults()
{
    QByteArray recvBuff;
    while( udp_server2->hasPendingDatagrams() ){
        qDebug()<<"received length: "<<udp_server2->pendingDatagramSize()<<endl;

        // Allocate the memory for the coming image
        recvBuff.resize( udp_server2->pendingDatagramSize() );

        // Received the Data sent from the client
        udp_server2->readDatagram(recvBuff.data(), recvBuff.size(), &client_address2, &client_port2);

        // 1. 收到了 ObjectTrackEventResult 信息 ---- 障碍物信息，包括交通标志和车辆行人
        if( recvBuff[0] == 'a' && recvBuff[1] == 'a' && recvBuff[2] == 'a' && recvBuff[3] == 'a' ){
            qDebug()<<"Objects Track Event Result: "<<endl;
            ObjectTrackEventResult objectsResults;
            memcpy( &objectsResults, &recvBuff.data()[4], sizeof( ObjectTrackEventResult ) );

            objectRects.clear();
            objectsTypes.clear();
            objectsTypesPoses.clear();

            for( int i = 0; i < objectsResults.nObjectNumber; i ++ ){
                cv::Rect rect( objectsResults.objInfo[i].nLeft, objectsResults.objInfo[i].nTop,
                               objectsResults.objInfo[i].nRight - objectsResults.objInfo[i].nLeft, objectsResults.objInfo[i].nBottom - objectsResults.objInfo[i].nTop);

                ui->log->setText("Object Type: " + QString::number(objectsResults.objInfo[i].nDetectType) + "\r\n"
                                 "\tnLeft:   " + QString::number(objectsResults.objInfo[i].nLeft) +
                                 "\tnTop:    " + QString::number(objectsResults.objInfo[i].nTop)+
                                 "\tnRight:  " + QString::number(objectsResults.objInfo[i].nRight) +
                                 "\tnBottom: " + QString::number(objectsResults.objInfo[i].nBottom));

                objectRects.push_back(rect);
                objectsTypes.push_back( objectsResults.objInfo[i].nDetectType );

                cv::Point infoPose( objectsResults.objInfo[i].nLeft, objectsResults.objInfo[i].nTop - 10 );
                objectsTypesPoses.push_back(infoPose);
            }
        }
        // 2. 收到了 DrawPointInfo 信息 ---- 车道线信息
        else if( recvBuff[0] == 'b' && recvBuff[1] == 'b' && recvBuff[2] == 'b' && recvBuff[3] == 'b' ){
            qDebug()<<"Lanes Points Info: "<<endl;
            DrawPointInfo pointsInfo;
            memcpy( &pointsInfo, &recvBuff.data()[4], sizeof( DrawPointInfo ) );

            linePoints.clear();

            for( uchar i = 0; i < 4; i ++ ){ // 一共 4 条车道线
                std::vector<cv::Point2f> points;
                for( unsigned int j = 0; j < pointsInfo.nPointCounters[i]; j ++ ){ // 遍历每个车道上的点
                    points.push_back( cv::Point2f( pointsInfo.pSrcPointX[i][j], pointsInfo.pSrcPointY[i][j] ) );
                }
                linePoints.push_back(points);
            }
        }
        // 3. Other
        else {
            qDebug()<<"UnKnow Info: "<<endl;

        }
    }
}

// udp server接收槽函数
/*
@   接收YUV420p格式的图像数据
@
*/
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
            memcpy( &datagram.data()[recvCount * 60000], recvBuff.data(), recvBuff.size() );
            recvCount ++;
            qDebug()<<"recvCount : "<<recvCount<<endl;

            if( recvCount == frameSize / 60000 + 1 ){
                recvCount = 0;
                getImageFromArray(datagram.data());
                ui->image_label->setPixmap(QPixmap::fromImage(this->image).scaled(ui->image_label->size()));
            }
        }
    }
}

/*
@   设置接收图像的大小
@
*/
void MainWindow::setRecvImgSize( int width, int height )
{
    this->recvImgWidth = width;
    this->recvImgHeight = height;
}

/*
@   将接收到的图像数据数组转成jpg格式显示
@
*/
void MainWindow::getImageFromArray( const QString &data )
{
    QByteArray imageData = QByteArray::fromBase64(data.toLatin1());
    qDebug()<<"imageData.size = "<<imageData.size()<<endl;

    qDebug()<<"image data: "<<endl;
    //for( int i = 0; i < 50; i ++ ){
    //    qDebug()<<"imageData()["<<i<<"] = " <<imageData.data()[i]<<endl;
    //}

    cv::Mat src_yuv;
    //src_yuv.create( recvImgHeight, recvImgWidth, CV_8UC3); // height, width
    src_yuv.create( 720 * 3 / 2, 1280, CV_8UC1); // height, width
    memcpy(src_yuv.data, imageData.data(), imageData.size() );

    cv::Mat src_jpg;
    //cv::cvtColor(src_yuv, src_jpg, cv::COLOR_YUV2BGR);
    cv::cvtColor(src_yuv, src_jpg, cv::COLOR_YUV2BGR_I420);
    //cv::imshow("test", src_jpg);

    // -------------- 在检测上的图像上画出检测结果 -------------- //
    // 1. 画检测框
    if( !objectRects.empty() ){
        for( auto it : objectRects ){
            cv::rectangle(src_jpg, it, cv::Scalar( 0, 255, 0 ), 3);
        }
    }

    // 2. 画目标检测信息
    if( !objectsTypes.empty() ){
        for( size_t i = 0; i < objectsTypes.size(); i ++ ){
            switch (objectsTypes[i]) {
                case NONE_TYPE: cv::putText(src_jpg, "UnKnow", objectsTypesPoses[i], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2); break;
                case PEDESTRIAN_TYPE: cv::putText(src_jpg, "Pedestrian", objectsTypesPoses[i], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2); break;
                case CAR_TYPE: cv::putText(src_jpg, "Car", objectsTypesPoses[i], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2); break;
                case BUS_TYPE: cv::putText(src_jpg, "Bus", objectsTypesPoses[i], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2); break;
                case TRUCK_TYPE: cv::putText(src_jpg, "Truck", objectsTypesPoses[i], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2); break;
                case MIDBUS_TYPE: cv::putText(src_jpg, "MidBus", objectsTypesPoses[i], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2); break;
                case MOTO_TYPE: cv::putText(src_jpg, "Moto", objectsTypesPoses[i], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2); break;
                case NOMOTO_TYPE: cv::putText(src_jpg, "NoMoto", objectsTypesPoses[i], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2); break;
                case TRAFFIC_SIGN_TYPE: cv::putText(src_jpg, "Traffic Sign", objectsTypesPoses[i], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2); break;
            default:break;
            }
        }
    }

    // 3. 画车道线
    if( !linePoints.empty() ){
        for( auto it : linePoints ){
            if( !it.empty() ){
                for( size_t i = 0; i < it.size() - 1; i ++ ){
                    cv::line( src_jpg, it[i], it[i + 1], cv::Scalar( 0, 0, 255 ), 5 );
                }
            }
        }
    }

    // ----------------------- END ------------------------- //

    // Copy input Mat
    const uchar *pSrc = (const uchar*)src_jpg.data;
    // Create QImage with same dimensions as input Mat
    QImage image1(pSrc, src_jpg.cols, src_jpg.rows, src_jpg.step, QImage::Format_RGB888);
    this->image = image1.rgbSwapped();
}


// 点击开始按钮
void MainWindow::on_pushButton_clicked()
{
    // 1. 点击开始按钮之后初始化 Udp Server
    this->udpInit();

    // 2. 初始化接收 图片检测结果的 UDP Server
    this->resultsUDPInit();
}

// 点击停止按钮
void MainWindow::on_pushButton_2_clicked()
{
    // 关闭 Udp Server
    udp_server->close();

    udp_server2->close();

    ui->connection_status->setText("DisConnected");

    ui->log->setText("DisConnected from the UDP Client ...\r\nUDP Results Receiver Closed ...");
}
