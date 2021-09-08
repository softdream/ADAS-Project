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

void MyPainter::startUDPServer()
{
    qDebug()<<"start a Udp Server ..."<<endl;


}


