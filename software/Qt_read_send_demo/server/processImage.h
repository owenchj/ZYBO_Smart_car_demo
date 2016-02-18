#ifndef PROCESSIMAGE_H
#define PROCESSIMAGE_H

#include <QtGui>
#include <QPixmap>
#include <QtWidgets>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include "videodevice.h"

class ProcessImage : public QWidget
{
    Q_OBJECT
public:
    ProcessImage(QWidget *parent=0);
    ~ProcessImage();
    void InitServer();
private:
    QPainter *painter;
    QLabel *label;
    QImage *frame;

    QTimer *timer;
    int rs;
    uchar *pp;
    uchar * p;
    unsigned int len;
    int image_w,image_h;
   int convert_rgb(unsigned char *src, unsigned char *dst, unsigned int width, unsigned int height);
   int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
   int convert_yuv_to_rgb_pixel(int y, int u, int v);
    uchar * rgb_buf[3];
    uchar c_buf_num;
    uchar dis_num;
    int pos;
    QString globalString;
    QTcpServer   *listenSocket;  // 侦听套接字
    QTcpSocket   *readWriteSocket;//读写套接字
private slots:
    void paintEvent(QPaintEvent *);
    void display_error(QString err);
    void  processConnection();
    void  processdata();

};

#endif
