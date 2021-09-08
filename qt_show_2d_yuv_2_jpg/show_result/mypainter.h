#ifndef MYPAINTER_H
#define MYPAINTER_H

#include <QWidget>

#include <QPainter>
#include <QTime>
#include <QTimer>


class MyPainter : public QWidget, QPainter
{
    Q_OBJECT
public:
    MyPainter( QWidget *parent = nullptr );
    ~MyPainter();

public:
    void drawACar( int x, int y );

private:
    void paintEvent(QPaintEvent *event);

private slots:
    void startUDPServer();

};

#endif // MYPAINTER_H
