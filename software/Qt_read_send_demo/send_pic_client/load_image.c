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

unsigned char *load_image()
{
  int sockfd;
	
  /* // AXI VDMA 1 -> RGB */
  /* unsigned long src_phy_addr1 = 0x1C000000; */
  /* unsigned long src_phy_addr2 = 0x1C12C000; */
  /* unsigned long src_phy_addr3 = 0x1C258000; */

  // AXI VDMA 2 -> RBG
  unsigned long src_phy_addr1 = 0x1C600000;
	
  int fd = open("/dev/mem", O_RDWR);

  volatile unsigned char* virtual_src_addr1;
  unsigned long size = 640*480*4;

  virtual_src_addr1 = (volatile unsigned char*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)src_phy_addr1);
  if(virtual_src_addr1 == MAP_FAILED)
    {
      perror("virtual_addr1 mapping for absolute memory access failed.\n");
      return;
    }
  
  return virtual_src_addr1;
}

