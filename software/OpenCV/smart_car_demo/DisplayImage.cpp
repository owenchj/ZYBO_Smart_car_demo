
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>
//#include <opencv/cxcore.h>
//#include <opencv/highgui.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <sstream>
#include <math.h>

#include "horizonalLine.h"
#include "boundary.h"
#include "obstacle.h"
#include "light.h"

static int BW_TRESHOLD = 50;		  // edge response strength to recognize for 'WHITE'

using namespace cv;
using namespace std;


#if 1
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#endif

#include "xvdma_ioctl.h"

/// opencv mats
Mat src[3];
Mat src_edge[3];
Mat src_result[3];

/// virtual address
unsigned int frameAddress;
unsigned int *addr;
unsigned int img_cnt = 0;

/// image compression
std::vector<uchar > buff[3];//buffer for coding

std::vector<int> params;



/******************************
 * C programm
 * Send picture
 ***************************/
extern "C"  void *sender(void *);
extern "C"  void *send_start(void *);
extern "C"  void send_init(char *ip, int id);
extern "C"  volatile int resultIndex_fini;
extern "C"  volatile int ready;
extern "C"  unsigned char *buffer[3];

/// sychronization show
int resultIndex = 0;

/// control VDMA
int fd_orignal, fd_filter;
int read_index = 0;
char RI = {0x00};
int write_index = 0;
int already_write_index = 0;
char WI[6] = {0x00, 0x00, 0x00, 0x00, 0x00};
int irq_num = 0;


int read_index_filter = 0;
char RI_FILTER = {0x00};
int write_index_filter = 0;
int already_write_index_filter = 0;
char WI_FILTER[6] = {0x00, 0x00, 0x00, 0x00, 0x00};
int irq_num_filter = 0;

/// control ps to pl
int fd_pspl, fd_plps;
char receive[4];
char sends[5] = {0x01, 0x00, 0x05, 0x05, 0x01};



// Read buffer from a specified ram address
unsigned int read_buffer(int num, int read_index){

  unsigned long src_phy_addr1;
  unsigned long size ;

  if(num == 0)
    {
      src_phy_addr1 = 0x1C000000 + read_index * (640*480*3);
      size = 640*480*3;
    }
  else
    {
      src_phy_addr1 = 0x1C600000 + read_index * (640*480);
      size = 640*480;
    }


  int fd = open("/dev/mem", O_RDWR);

  unsigned int  virtual_src_addr1;

  virtual_src_addr1 = (unsigned long)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)src_phy_addr1);

  if(virtual_src_addr1 ==  (unsigned long)MAP_FAILED)
    {
      perror("virtual_addr1 mapping for absolute memory access failed.\n");
      return -1;
    }

  return  virtual_src_addr1;
}


// Write buffer to a specified ram address
unsigned int write_buffer(int id, int write_index){

  unsigned long src_phy_addr1 = 0x00000000;
  unsigned long size = 0;

  if(id == 0)
    {
      src_phy_addr1 = 0x1D000000 + write_index * (640*480*3);
      size = 640*480*3;
    }
  else if(id == 1)
    {
      src_phy_addr1 = 0x1E000000 + write_index * (640*480);
      size = 640*480;
    }


  int fd = open("/dev/mem", O_RDWR);

  unsigned int  virtual_src_addr1;

  virtual_src_addr1 = (unsigned int)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)src_phy_addr1);

  if(virtual_src_addr1 ==  (unsigned int)MAP_FAILED)
    {
      perror("virtual_addr1 mapping for absolute memory access failed.\n");
      return -1;
    }

  return  virtual_src_addr1;
}


void direction(Mat& edge, Mat& temp_frame){
  //void direction(Mat& edge, Mat& edge_sobel, Mat& edge_hough, Mat& temp_frame){
  clock_t tStart = clock();

  int WIDTH = temp_frame.cols;
  int HEIGHT = temp_frame.rows;

  int horizonalLine = HEIGHT/4;
  int dir_line = 1;  
  int dir_obstacle = 1;
  bool dir_light = 1;

  Rect rect_half(0, HEIGHT/4, WIDTH, HEIGHT/2);
  Rect rect_up(0, 0, WIDTH, HEIGHT/2);
  Rect rect_down(0, HEIGHT/2, WIDTH, HEIGHT/2);

 
  Mat half_img (edge, rect_half);
  Mat up_img (edge, rect_up);
  Mat down_img  (edge, rect_down);
  
  // watch time
  printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
  horizonalLine = setHorizonalLine(half_img, temp_frame, BW_TRESHOLD);
  printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
  //cout << "horizonalLine" << horizonalLine << endl;

  dir_line = setBoundaries(down_img, temp_frame, horizonalLine);
  printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
  
  dir_obstacle = obsDetect(half_img,temp_frame, horizonalLine, BW_TRESHOLD);
  printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

  //  dir_light = light(up_img, temp_frame);
  //printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
  //  cout<<" dir_line "<< dir_line<<" dir_obstacle "<<dir_obstacle<<" dir_light "<< dir_light<<endl;

  if(dir_line == 2){
    sends[2] = 0x09;
    sends[3] = 0x00;
  } else if(dir_line == 1){
 
   if(dir_obstacle == 2){
      sends[2] = 0x09;
      sends[3] = 0x00;
      
    } else if(dir_obstacle == 1){
      sends[2] = 0x09;
      sends[3] = 0x09;
    } else if(dir_obstacle == 0){
      sends[2] = 0x00;
      sends[3] = 0x09;
    }
    
  } else if(dir_line == 0){
    sends[2] = 0x00;
    sends[3] = 0x09;
  }
  
  
  write(fd_pspl, sends, 5);
  
}



void *update(void *arg)
{

  while(1){
    clock_t tStart = clock();
    // read from the VDMA original
    read(fd_orignal, WI, 6);
    irq_num &= 0;
    irq_num |= ( WI[0] << 24 );
    irq_num |= ( WI[1] << 16 );
    irq_num |= ( WI[2] <<  8 );
    irq_num |= ( WI[3]       );
    write_index &= 0;
    write_index |= ( WI[4] );

    // read from the VDMA filter
    read(fd_filter, WI_FILTER, 6);
    irq_num_filter &= 0;
    irq_num_filter |= ( WI_FILTER[0] << 24 );
    irq_num_filter |= ( WI_FILTER[1] << 16 );
    irq_num_filter |= ( WI_FILTER[2] <<  8 );
    irq_num_filter |= ( WI_FILTER[3]       );
    write_index_filter &= 0;
    write_index_filter |= ( WI_FILTER[4] );
    
    if(irq_num && irq_num_filter){
      read_index =  write_index;
      RI = read_index & 0xFF;
      
      read_index_filter =  write_index_filter;
      RI_FILTER = read_index_filter & 0xFF;
      
      
      // Write to VDMA to lock the space
      write(fd_orignal, &RI, 1);    
      write(fd_filter, &RI_FILTER, 1);    

      resultIndex_fini = resultIndex;
      resultIndex = img_cnt;
	
      cout << resultIndex << endl;

      // imwrite("a.bmp", src_edge[read_index_filter]);
      //imwrite("b.bmp", src[read_index]);


      direction(src_edge[read_index_filter], src[read_index]);
      
      /// save to specified address in ram
      src[read_index].copyTo(src_result[resultIndex]);
      //imwrite("c.bmp", src[read_index]);

      /// compression image
      // printf("Compression Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);  
      // cv::imencode(".jpg",  src[read_index], buff[resultIndex], params);
      // cout << buff[resultIndex].size() << endl;
      // //      buffer[resultIndex] = & buff[resultIndex][0];
      // buffer[resultIndex] = (unsigned char *)addr;
      // printf("Compression Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

      img_cnt = (img_cnt+1) % 3;      	
      cout << "Finish image" << std::endl ;
      
      ready = 1;
    }

  }



}

void initAddr(){

  // initalize the read address 
  for(int i = 0; i < 3; i++){
    frameAddress = read_buffer(0, i);
    addr = (unsigned int *) frameAddress;
    src[i] =  Mat(480,640, CV_8UC3, addr);
  }

  for(int i = 0; i < 3; i++){
    frameAddress = read_buffer(1, i);
    addr = (unsigned int *) frameAddress;
    src_edge[i] =  Mat(480,640, CV_8UC1, addr);
  }

  for(int i = 0; i < 3; i++){
    frameAddress = write_buffer(0, i);
    addr = (unsigned int *) frameAddress;
    src_result[i] =  Mat(480,640, CV_8UC3, addr);
  }

  /// image compression
  params.push_back(CV_IMWRITE_JPEG_QUALITY);
  params.push_back(40);   // that's percent, so 100 == no compression, 1 == full 
    
}

int main( int argc, char** argv )
{

  clock_t tStart = clock();

  printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

  // initial the devices
  fd_orignal = open("/dev/my_xvdma_original", O_RDWR);

  fd_filter = open("/dev/my_xvdma_filter", O_RDWR);

  fd_pspl = open("/dev/my_ps2pl", O_WRONLY);

  if (fd_pspl >= 0)
    printf("Ready to send\n");

  if (fd_orignal >= 0)
    {
      if(ioctl(fd_orignal, XVDMA_RESET_IRQ_NUM) == -1)
	printf("Error ioctl.\n");
      else
	printf("XVDMA_RESET_IRQ_NUM_ORIGINAL\n");
    }

  if (fd_filter >= 0)
    {
      if(ioctl(fd_filter, XVDMA_RESET_IRQ_NUM) == -1)
	printf("Error ioctl.\n");
      else
	printf("XVDMA_RESET_IRQ_NUM_FILTER\n");
    }

  char *ip = (char *)"192.168.1.100";
  send_init(ip, 0);

  initAddr();

  pthread_t thread1, thread2;
  int i1,i2;
  i1 = pthread_create( &thread1, NULL, update, (void*) "thread 1");
  sleep(1);
  i2 = pthread_create( &thread2, NULL, send_start, (void*) "thread 2");

  cout << i1 << i2 <<std::endl ;

  pthread_join(thread1,NULL);
  pthread_join(thread2,NULL);

  return 0;

}
