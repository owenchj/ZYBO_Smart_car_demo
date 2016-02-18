//----------------------------------------------------------------
//
// Create Date:         Feb 14, 2012
// Design Name:         ZVIK Camera Demo
// Module Name:         main.c
// Project Name:        ZVIK Camera Demo
//
// Tool versions:       ISE 14.4
//
// Description:         ZVIK Camera Demo
//
// Dependencies:
//
// Revision:            Feb 14, 2012: 1.01 Initial version
//                      Dec 15, 2012: 1.03 Updated to use 14.4 cores
//                                         and Xilinx tools
//
//----------------------------------------------------------------

// ---------------------------------------------------------------------------
// System Includes.
#include <stdio.h>

#include "smart_car_demo.h"
smart_car_demo_t smart_car_demo;
#include "vga.h"
#include "xvdma_ioctl.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#define VIDEO_MMAP_SIZE	0x338 + sizeof(Xuint32)




// forward declarations
int init_base_addresses( smart_car_demo_t *pDemo, int bVerbose );

// Main entry point.
//
//


int main()
{
  // Specify Base Addresses of all PCOREs
  init_base_addresses( &smart_car_demo, 1/*bVerbose*/ );

  // Initialize FMC-IMAGEON Demo
  smart_car_demo_init( &smart_car_demo );


  // Shutdown the FMC-IMAGEON Demo
  smart_car_demo_quit( &smart_car_demo );

  return 0;
}

int init_base_addresses( smart_car_demo_t *pDemo, int bVerbose )
{
  int fd;
  Xuint32 map_CoreAddress;


  fd = open("/dev/mem", O_RDWR);

  //
  // Specify Base Addresses for FMC-IMAGEON demo
  //
  if ( bVerbose ) printf( "Specifying Base Addresses for SMART_CAR demo ...\n\r" );


  // AXI_VDMA 0
  map_CoreAddress = (Xuint32)mmap(NULL, VIDEO_MMAP_SIZE,
				  PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)XPAR_AXI_VDMA_1_BASEADDR );
  if (map_CoreAddress == (Xuint32)MAP_FAILED)
    {
      printf("MMap failed to map VDMA peripheral\n");
      return 0;
    }
  pDemo->uBaseAddr_VDMA_FrameBuffer = map_CoreAddress;
  if ( bVerbose ) printf("\tpDemo->uBaseAddr_VDMA_VitaFrameBuffer = 0x%08X\r\n", pDemo->uBaseAddr_VDMA_FrameBuffer);



  // AXI_VDMA 1
  map_CoreAddress = (Xuint32)mmap(NULL, VIDEO_MMAP_SIZE,
				  PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0x43010000 );
  if (map_CoreAddress == (Xuint32)MAP_FAILED)
    {
      printf("MMap failed to map VDMA peripheral\n");
      return 0;
    }
  pDemo->uBaseAddr_VDMA_FrameBuffer_1 = map_CoreAddress;
  if ( bVerbose ) printf("\tpDemo->uBaseAddr_VDMA_FrameBuffer = 0x%08X\r\n", pDemo->uBaseAddr_VDMA_FrameBuffer_1);




  // SOBEL
#if defined(XPAR_XIMAGE_FILTER_0_S_AXI_CONTROL_BUS_BASEADDR)
  map_CoreAddress = (Xuint32)mmap(NULL, VIDEO_MMAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0x43c00000 );
  if (map_CoreAddress == (Xuint32)MAP_FAILED)
    {
      printf("MMap failed to map IMGFILTER peripheral\n");
      return 0;
    }
  pDemo->SOBEL.Control_bus_BaseAddress = map_CoreAddress;
  pDemo->SOBEL.IsReady = 1;
#endif

  return 0;
}
