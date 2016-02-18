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


extern volatile int ready = 0;

int sockfd = 0;
unsigned char *buffer;
unsigned long size = 0;
unsigned char * image_addr;

extern void *sender(void *argu){

}


extern void send_init(char *ip, int id){

  //  int sockfd;
	
  unsigned long vdma_apb_addr = 0x43000000;
  unsigned long src_phy_addr1 = 0x00000000;
  /* unsigned char *buffer; */
  /* unsigned long size = 0; */
  if(id == 0)
    {
      // AXI VDMA 1 -> RGB
      src_phy_addr1 = 0x1D000000;
      buffer = (unsigned char *)malloc(640*480*3);
      printf("2\n");
      size = 640*480*3;  
    } else if(id ==1)
    {
      // AXI VDMA 2 -> GRAY
      src_phy_addr1 = 0x1E000000;
      buffer = (unsigned char *)malloc(640*480);
      size = 640*480;
    }


  // sockfd = Socket_Init(ip,"8181");	
  sockfd = Socket_Init(ip,"9090");	

  
  if(NULL == buffer)
    printf("malloc error\n");
  int fd = open("/dev/mem", O_RDWR);

  volatile unsigned char* virtual_src_addr1;


  virtual_src_addr1 = (volatile unsigned char*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)src_phy_addr1);
  if(virtual_src_addr1 == MAP_FAILED)
    {
      perror("virtual_addr1 mapping for absolute memory access failed.\n");
      return;
    }

  unsigned char *virtual_apb_addr;
  virtual_apb_addr = (unsigned char*)mmap(NULL, 32, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)vdma_apb_addr);
  if(virtual_apb_addr == MAP_FAILED)
    {
      perror("virtual_apb_addr mapping for absolute memory access failed.\n");
      return;
    }


  volatile int *vdma_reg = (volatile int *)virtual_apb_addr;
  printf("vdma_reg is %x\n",*vdma_reg);

  //  unsigned char * image_addr;
  image_addr = virtual_src_addr1;

  /* while(1) */
  /*   { */
  /* 	*vdma_reg = 0x02; */
  /* 	memcpy(buffer,image_addr,size); */

  /* 	Socket_Send(sockfd,buffer,size); */
  /* 	break; */
  /*   } */

}


extern void *send_start(void *arg){
  while(1){
    memcpy(buffer,image_addr,size);


    if(ready == 1){
    printf("%d ", (int)buffer[0]);
    printf("%d ", (int)buffer[1]);
    printf("%d ", (int)buffer[640*480 - 2]);
    printf("%d ", (int)buffer[640*480 - 1]);
    printf("\n ");

      Socket_Send(sockfd,buffer,size);
      ready = 0;
      printf("Write a image\n");
    }
   
  }
}
