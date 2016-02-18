//----------------------------------------------------------------
//
// Create Date:         Jan 4, 2015
// Design Name:         ZYBO Camera Demo
// Module Name:         smart_car_demo.c
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

#include "smart_car_demo.h"
#include "os.h"
#include "xsobel.h"


int smart_car_demo_init( smart_car_demo_t *pDemo )
{
  int ret;

  OS_PRINTF("\n\r");
  OS_PRINTF("------------------------------------------------------\n\r");
  OS_PRINTF("--    Xilinx Zybo smart car demo                    --\n\r");
  OS_PRINTF("--      640*480P60 Real-Time Camera Demonstration   --\n\r");
  OS_PRINTF("------------------------------------------------------\n\r");
  OS_PRINTF("\n\r");

  pDemo->bVerbose = 0;

  OS_PRINTF( "SMART_CAR Initialization ...\n\r" );


#if defined(XPAR_XIMAGE_FILTER_0_S_AXI_CONTROL_BUS_BASEADDR)
  pDemo->SOBEL.IsReady = 1;

  XSobel_Set_rows(&(pDemo->SOBEL), 480);

  XSobel_Set_cols(&(pDemo->SOBEL), 640);

  XSobel_Set_C_XR0C0(&(pDemo->SOBEL), 1);

  XSobel_Set_C_XR0C1(&(pDemo->SOBEL), 0);

  XSobel_Set_C_XR0C2(&(pDemo->SOBEL), -1);
 

  XSobel_Set_C_XR1C0(&(pDemo->SOBEL), 2);


  XSobel_Set_C_XR1C1(&(pDemo->SOBEL), 0);

  XSobel_Set_C_XR1C2(&(pDemo->SOBEL), -2);

  XSobel_Set_C_XR2C0(&(pDemo->SOBEL), 1);

  XSobel_Set_C_XR2C1(&(pDemo->SOBEL), 0);

  XSobel_Set_C_XR2C2(&(pDemo->SOBEL), -1);

  XSobel_Set_C_YR0C0(&(pDemo->SOBEL), 1);

  XSobel_Set_C_YR0C1(&(pDemo->SOBEL), 2);

  XSobel_Set_C_YR0C2(&(pDemo->SOBEL), 1);

  XSobel_Set_C_YR1C0(&(pDemo->SOBEL), 0);

  XSobel_Set_C_YR1C1(&(pDemo->SOBEL), 0);

  XSobel_Set_C_YR1C2(&(pDemo->SOBEL), 0);

  XSobel_Set_C_YR2C0(&(pDemo->SOBEL), -1);

  XSobel_Set_C_YR2C1(&(pDemo->SOBEL), -2);

  XSobel_Set_C_YR2C2(&(pDemo->SOBEL), -1);

  XSobel_Set_c_high_thresh(&(pDemo->SOBEL), 200);

  XSobel_Set_c_low_thresh(&(pDemo->SOBEL), 100);

  XSobel_Set_c_invert(&(pDemo->SOBEL), 1);

  XSobel_Set_threshold(&(pDemo->SOBEL), 75);

  XSobel_EnableAutoRestart(&(pDemo->SOBEL));
  XSobel_InterruptDisable(&(pDemo->SOBEL), 0xFFFFFFFF);
  XSobel_InterruptGlobalDisable(&(pDemo->SOBEL));
  XSobel_Start(&(pDemo->SOBEL));
  //  printf("*******%d\n",XSobel_Get_c_invert(&(pDemo->SOBEL)));

  OS_PRINTF("SOBEL done\r\n");
#endif

  pDemo->vdma_resolution = 0;

  // re-initialization
  vdma_init( pDemo->uBaseAddr_VDMA_FrameBuffer,
  	     pDemo->vdma_resolution,
  	     VDMA_MEM_BASE_ADDR
  	     );

  sleep(2);
  
  vdma_init_sobel( pDemo->uBaseAddr_VDMA_FrameBuffer_1,
  	     pDemo->vdma_resolution,
  	     VDMA_MEM_BASE_ADDR_1
  	     );


  return 0;
}


int smart_car_demo_quit( smart_car_demo_t *pDemo )
{
  return 0;
}
