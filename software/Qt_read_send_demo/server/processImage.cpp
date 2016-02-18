#include <QtGui>
#include <QtNetwork/QTcpServer>
#include "processImage.h"
#include "videodevice.h"
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
}

static int display = 0;

ProcessImage::ProcessImage(QWidget *parent):QWidget(parent)
{
  image_w = 640;
  image_h = 480;
  
  if(display == 0){
    pp = (unsigned char *)malloc(image_w * image_h/*QWidget::width()*QWidget::height()*/* 3 * sizeof(char));
    rgb_buf[0] = (unsigned char *)malloc(image_w * image_h* 3 * sizeof(char));
    rgb_buf[1] = (unsigned char *)malloc(image_w * image_h* 3 * sizeof(char));
    rgb_buf[2] = (unsigned char *)malloc(image_w * image_h* 3 * sizeof(char));
    frame = new QImage(pp,image_w,image_h, QImage::Format_RGB888);
    //frame = new QPixmap(640,480,);
  }
  else if(display == 1){
    pp = (unsigned char *)malloc(image_w * image_h/*QWidget::width()*QWidget::height()*/* 1 * sizeof(char));
    rgb_buf[0] = (unsigned char *)malloc(image_w * image_h* 1 * sizeof(char));
    rgb_buf[1] = (unsigned char *)malloc(image_w * image_h* 1 * sizeof(char));
    rgb_buf[2] = (unsigned char *)malloc(image_w * image_h* 1 * sizeof(char));
    frame = new QImage(pp,image_w,image_h, QImage::Format_Indexed8);
  }

  c_buf_num = 0;
  dis_num = 2;
  pos = 0;
  painter = new QPainter(this);


  label = new QLabel();

  InitServer();

  timer = new QTimer(this);
  connect(timer,SIGNAL(timeout()),this,SLOT(update()));
  timer->start(30);

  QHBoxLayout *hLayout = new QHBoxLayout();
  hLayout->addWidget(label);
  setLayout(hLayout);
  setWindowTitle(tr("Capture"));
}
void ProcessImage::InitServer()
{
  this->listenSocket =new QTcpServer;
  this->listenSocket->listen(QHostAddress::Any,8181);
  connect(this->listenSocket,SIGNAL(newConnection()),this,SLOT(processConnection()));
}
void ProcessImage::processConnection()
{
  this->readWriteSocket =this->listenSocket->nextPendingConnection();
  connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(processdata()));
}

void ProcessImage::processdata()
{
  int len = this->readWriteSocket->bytesAvailable();
  int left = 0;
  if(display == 0)
    left = image_w*image_h*3 - pos;
  else if(display == 1)	 
    left = image_w*image_h*1 - pos;
  printf("my_len %d\n",len); 

#if 1
  if(left > len)
    {

      this->readWriteSocket->read((char *)(rgb_buf[c_buf_num] + pos),len);
      pos += len;

    }
  else
    {
      printf("c_buf_num is %d\n",c_buf_num);
      this->readWriteSocket->read((char *)(rgb_buf[c_buf_num]+pos),left);
      printf("pic size is %d\n",pos+left);
      pos = 0;

      if(display == 0)
         convert_rgb(rgb_buf[c_buf_num],pp,image_w,image_h);
      else
         pp = rgb_buf[c_buf_num];

      if(c_buf_num != 2)
	c_buf_num++;
      else
	c_buf_num=0;

        QImage q;

      if(display == 0){
         frame->loadFromData((uchar *)pp,/*len*/image_w * image_h * 3 * sizeof(char));
         //QImage q(pp,image_w,image_h,QImage::Format_RGB888);
         q=QImage(pp,image_w,image_h,QImage::Format_RGB888);
      }
       else if(display == 1){
        frame->loadFromData((uchar *)pp,/*len*/image_w * image_h * 1 * sizeof(char));
        //QImage q(pp,image_w,image_h,QImage::Format_Indexed8);
         q=QImage(pp,image_w,image_h,QImage::Format_Indexed8);
       }
      //QPixmap pix(":tiantan.jpg");
      
      QPixmap pix = QPixmap::fromImage(q,Qt::AutoColor);
      
      label->setPixmap(pix);

    }
#endif

}
ProcessImage::~ProcessImage()
{
  //rs = vd->stop_capturing();
  //rs = vd->uninit_device();
  //rs = vd->close_device();
  free(rgb_buf[0]);
  free(rgb_buf[1]);
  free(rgb_buf[2]);
  free(pp);
}

void ProcessImage::paintEvent(QPaintEvent *)
{
  //rs = vd->get_frame((void **)&p,&len);
  //convert_yuv_to_rgb_buffer(p,pp,image_w,image_h/*QWidget::width(),QWidget::height()*/);
  //if(dis_num != c_buf_num){
  /*convert_rgb(rgb_buf[dis_num],pp,image_w,image_h);
    if(dis_num != 2)
    dis_num++;
    else
    dis_num=0;
    printf("paintevent\n");
    frame->loadFromData((uchar *)pp,image_w * image_h * 3 * sizeof(char));
  */
  //label->setPixmap(QPixmap::fromImage(*frame,Qt::AutoColor));
  //rs = vd->unget_frame();
  //}
  //printf("paintevent\n");
}

void ProcessImage::display_error(QString err)
{
  QMessageBox::warning(this,tr("error"), err,QMessageBox::Yes);
}

int ProcessImage::convert_rgb(unsigned char *src, unsigned char *dst, unsigned int width, unsigned int height)
{
  int i,j;

  for(i=0;i<height;i++)
    for(j=0;j<width;j++)
      {
	//printf("data is %d\n",virtual_src_addr[i*width*4+j*4+3]);
	// AXI VDMA 1 RGB
    dst[i*width*3+j*3] =   src[i*width*3+j*3+2];
    dst[i*width*3+j*3+1] = src[i*width*3+j*3+1];
    dst[i*width*3+j*3+2] = src[i*width*3+j*3+0];
      }
  return 0;
}

/*yuv格式转换为rgb格式*/
int ProcessImage::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
  unsigned int in, out = 0;
  unsigned int pixel_16;
  unsigned char pixel_24[3];
  unsigned int pixel32;
  int y0, u, y1, v;
  for(in = 0; in < width * height * 2; in += 4) {
    pixel_16 =
      yuv[in + 3] << 24 |
      yuv[in + 2] << 16 |
      yuv[in + 1] <<  8 |
      yuv[in + 0];
    y0 = (pixel_16 & 0x000000ff);
    u  = (pixel_16 & 0x0000ff00) >>  8;
    y1 = (pixel_16 & 0x00ff0000) >> 16;
    v  = (pixel_16 & 0xff000000) >> 24;
    pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
    pixel_24[0] = (pixel32 & 0x000000ff);
    pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
    pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
    rgb[out++] = pixel_24[0];
    rgb[out++] = pixel_24[1];
    rgb[out++] = pixel_24[2];
    pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
    pixel_24[0] = (pixel32 & 0x000000ff);
    pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
    pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
    rgb[out++] = pixel_24[0];
    rgb[out++] = pixel_24[1];
    rgb[out++] = pixel_24[2];
  }
  return 0;
}

int ProcessImage::convert_yuv_to_rgb_pixel(int y, int u, int v)
{
  unsigned int pixel32 = 0;
  unsigned char *pixel = (unsigned char *)&pixel32;
  int r, g, b;
  r = y + (1.370705 * (v-128));
  g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
  b = y + (1.732446 * (u-128));
  if(r > 255) r = 255;
  if(g > 255) g = 255;
  if(b > 255) b = 255;
  if(r < 0) r = 0;
  if(g < 0) g = 0;
  if(b < 0) b = 0;
  pixel[0] = r * 220 / 256;
  pixel[1] = g * 220 / 256;
  pixel[2] = b * 220 / 256;
  return pixel32;
}
/*yuv格式转换为rgb格式*/
