////////////////////////////////////////////////////////////////////////
//
// opencv_hello_world.c
//
// This is a simple, introductory OpenCV program. The program reads an
// image from a file, inverts it, and saves the result.
//
////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>



extern volatile int resultIndex_fini = 0;
extern volatile int ready = 0;
extern unsigned char *buffer[3] = {NULL,NULL,NULL};

int sockfd = 0;
unsigned long size = 0;
unsigned char* virtual_src_addr[3];


extern void *sender(void *argu){

}


extern void send_init(char *ip, int id){

  //  int sockfd;
  unsigned long vdma_apb_addr = 0x43000000;
  unsigned long src_phy_addr[3] = {0x1F000000, 0x1F000000, 0x1F000000};
  int i = 0;
  int fd = open("/dev/mem", O_RDWR);
  
  
  if(id == 0)
    {
      // AXI VDMA 1 -> RGB
      size = 640*480*3;  

      for(i = 0; i < 3; i++)
	{	
	  src_phy_addr[i] = 0x1D000000 + i * (640*480*3);
    
	  virtual_src_addr[i] = (unsigned char*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)src_phy_addr[i]);
	  if(virtual_src_addr[i] == MAP_FAILED)
	    {
	      perror("virtual_addr1 mapping for absolute memory access failed.\n");
	      return;
	    }
	}    
  
    } 
  else if(id ==1)
    {
      // AXI VDMA 2 -> GRAY
      size = 640*480;
      
      for( i = 0; i < 3; i++)
	{
	  src_phy_addr[i] = 0x1E000000 + i * (640*480);
	  
      
	  virtual_src_addr[i] = (unsigned char*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)src_phy_addr[i]);
	  if(virtual_src_addr[i] == MAP_FAILED)
	    {
	      perror("virtual_addr1 mapping for absolute memory access failed.\n");
	      return;
	    }
	}      
    }
  
  
  // sockfd = Socket_Init(ip,"8181");	
    sockfd = Socket_Init(ip,"9090");	
}



extern void *send_start(void *arg){

  while(1){
    if(ready == 1){ 
      int showIndex = resultIndex_fini;
      printf("showIndex = %d\n", showIndex);
      /* printf("0 = %x\n",  virtual_src_addr[0]); */
      /* printf("1 = %x\n",  virtual_src_addr[1]); */
      /* printf("2 = %x\n",  virtual_src_addr[2]); */
      
      Socket_Send(sockfd, virtual_src_addr[showIndex], size);
      //Socket_Send(sockfd, buffer[showIndex], 25000);
      printf("Write a image\n");
      // 0.1s/pic
      // usleep(100000);
      ready = 0;
    }
  }

}
