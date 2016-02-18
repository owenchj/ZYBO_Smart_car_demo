#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <pthread.h>
#include <thread>

using namespace cv;
using namespace std;


#if 1
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#endif

#include "xvdma_ioctl.h"

extern "C"  void *sender(void *);
extern "C"  void *send_start(void *);
extern "C"  void send_init(char *ip, int id);
extern "C"  volatile int ready;

pthread_mutex_t frameLocker;

// choose rgb or sobel 8bit
int id = 0;

int vdma_fd;
int read_index = 0;
char RI = {0x00};
int write_index = 0;
int already_write_index = 0;
char WI[6] = {0x00, 0x00, 0x00, 0x00, 0x00}; 
int irq_num = 0; 

unsigned int frameAddress;
unsigned int *addr;
 

unsigned int read_buffer(int id, int read_index){

  unsigned long src_phy_addr1 = 0x00000000;
  unsigned long size = 0;

  if(id == 0)
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

  volatile unsigned int  virtual_src_addr1;

  virtual_src_addr1 = (unsigned int)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)src_phy_addr1);

  if(virtual_src_addr1 ==  (unsigned int)MAP_FAILED)
    {
      perror("virtual_addr1 mapping for absolute memory access failed.\n");
      return -1;
    }

  return  virtual_src_addr1;
}


unsigned int write_buffer(int id){
  
  unsigned long src_phy_addr1 = 0x00000000;
  unsigned long size = 0;

  if(id == 0)
    { 
      src_phy_addr1 = 0x1D000000;
      size = 640*480*3;
    }
  else
    {
      src_phy_addr1 = 0x1E000000;
      size = 640*480;
    }

  int fd = open("/dev/mem", O_RDWR);

  volatile unsigned int  virtual_src_addr1;

  virtual_src_addr1 = (unsigned int)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)src_phy_addr1);

  if(virtual_src_addr1 ==  (unsigned int)MAP_FAILED)
    {
      perror("virtual_addr1 mapping for absolute memory access failed.\n");
      return -1;
    }

  return  virtual_src_addr1;
}




void *update(void *arg)
{
  struct timespec tim, tim2;
  tim.tv_sec = 0;
  //  tim.tv_nsec = 500000000; // 0.5s
  tim.tv_nsec = 100000000; // 0.1s
  

  Mat src[3];
  frameAddress = read_buffer(id, 0);
  addr = (unsigned int *) frameAddress;
  
  if(id == 0)
    src[0] =  Mat(480,640, CV_8UC3, addr);
  else
    src[0] =  Mat(480,640, CV_8UC1, addr);
  

  frameAddress = read_buffer(id, 1);
  addr = (unsigned int *) frameAddress;
 
  if(id == 0)
    src[1] =  Mat(480,640, CV_8UC3, addr);
  else
    src[1] =  Mat(480,640, CV_8UC1, addr);

  frameAddress = read_buffer(id, 2);
  addr = (unsigned int *) frameAddress;

  if(id == 0)
    src[2] =  Mat(480,640, CV_8UC3, addr);
  else
    src[2] =  Mat(480,640, CV_8UC1, addr);

  //Mat src =  Mat(480,640, CV_8UC1, addr);

  // write to specified address 0 for rbg; 1 for gray
  frameAddress = write_buffer(id);
  addr = (unsigned int *) frameAddress;
  
  Mat src_des;
  if(id ==0)
    src_des = Mat(480,640, CV_8UC3, addr);
  else
    src_des = Mat(480,640, CV_8UC1, addr);


  while(1){
    read(vdma_fd, WI, 6);
    irq_num &= 0;
    irq_num |= ( WI[0] << 24 );
    irq_num |= ( WI[1] << 16 );
    irq_num |= ( WI[2] <<  8 );
    irq_num |= ( WI[3]       );
    write_index &= 0;
    write_index |= ( WI[4] );
	  
    already_write_index &= 0;
    already_write_index |= ( WI[5] );

    // Process user input from Serial Console
    //if(irq_num && already_write_index != write_index){
    if(irq_num){
	    
  //      read_index =  already_write_index;
      read_index =  write_index;
	    
      RI = read_index & 0xFF;
      //printf("read_index = %d\n", read_index);

      if(ready == 0){
	
	write(vdma_fd, &RI, 1);    
	// cout << src.rows << std::endl ;
	// cout << src.cols << std::endl ;
	
	printf("read_index = %d\n", read_index);
	
	//GaussianBlur( src[read_index], src_des, Size( 3, 3 ), 0, 0 );
	//	cvSmooth(src[read_index], src_des, CV_GAUSSIAN, 5, 5);

	src[read_index].copyTo(src_des);
	
	/*simulation for running time*/
	// if(nanosleep(&tim , &tim2) < 0 ){
	//   printf("Oh la la!\n");
	// }

	// cvtColor( src_color, src_des, CV_BGR2GRAY );
	// imwrite( "magic.bmp", src_des );
	cout << "Finish image" << std::endl ;
	ready = 1;
      }
    }
    
  }


}



int main( int argc, char** argv )
{

  clock_t tStart = clock();
  
  printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
  
  // pthread_mutex_init(&frameLocker,NULL);  
  // pthread_t updateThread;    
  // pthread_create(&updateThread, NULL, update, NULL);
  // while(1){
  //   pthread_mutex_lock(&frameLocker);
  //   send_start();
    
  //   pthread_mutex_unlock(&frameLocker);
  // }

  if(id == 0)
    vdma_fd = open("/dev/my_xvdma_original", O_RDWR);
  else
    vdma_fd = open("/dev/my_xvdma_filter", O_RDWR);

  if (vdma_fd >= 0) 
    {
      if(ioctl(vdma_fd, XVDMA_RESET_IRQ_NUM) == -1)
	printf("Error ioctl.\n");
      else
	printf("XVDMA_RESET_IRQ_NUM\n");
    }	  


  char *ip = (char *)"192.168.1.100";
  // 0 for rbf; 1 for grey
  send_init(ip, id);

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





