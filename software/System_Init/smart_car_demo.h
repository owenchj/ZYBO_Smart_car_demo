//----------------------------------------------------------------
//
// Create Date:         Jan 4, 2015
// Design Name:         ZYBO Camera Demo
// Module Name:         smart_car_demo.h
// Project Name:        ZYBO Camera Demo
// Target Devices:      Zynq-7000 SoC
// Hardware Boards:     ZC701 + OV7725
//
// Tool versions:       Vivado 2015.2
//
// Description:         SMART CAR Frame Buffer Demo
//                      
//                         - 640x480 resolution
//
// Dependencies:
//
// Revision:            Jan 4, 2015: 21:00 Initial version
//
//----------------------------------------------------------------

#ifndef __SMART_CAR_DEMO_H__
#define __SMART_CAR_DEMO_H__

/*
 * Device related constants. Defined in xparameters.h.
 */
#include "xparameters.h"
#include "xbasic_types.h"

#define VDMA_DEVICE_ID        XPAR_AXI_VDMA_0_DEVICE_ID
#define VDMA_BASE_ADDR        XPAR_AXI_VDMA_0_BASEADDR
#define VDMA_READ_NUM_FRAMES  XPAR_AXI_VDMA_0_NUM_FSTORES
#define VDMA_WRITE_NUM_FRAMES XPAR_AXI_VDMA_0_NUM_FSTORES
#define VDMA_MEM_BASE_ADDR    0x1c000000
#define VDMA_MEM_BASE_ADDR_1  0x1c600000


#include "xaxivdma.h"
#include "xsobel.h"

// This structure contains the context for the OV7725 frame buffer design
struct struct_smart_car_demo_t
{

  XSobel SOBEL;
  XAxiVdma vdma_car;
  Xuint32 uBaseAddr_VDMA_FrameBuffer;
  Xuint32 uBaseAddr_VDMA_FrameBuffer_1;
  Xuint32 vdma_resolution;
  Xuint32 bVerbose;
};

typedef struct struct_smart_car_demo_t smart_car_demo_t;

int smart_car_demo_init( smart_car_demo_t *pDemo );
int smart_car_demo_quit( smart_car_demo_t *pDemo );



#endif // __SMART_CAR_DEMO_H__
