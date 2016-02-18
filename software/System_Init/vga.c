#include "vga.h"

static int img_cnt = 0;
static int img_cnt_filter = 0;

int vga_command(int read_index)
{
  Xuint32 physicalAddress;
  Xuint32 physicalSize;
  Xuint32 virtualAddress;
  int fd_mmap;
  int fd_bmp;
  //
  int n;
  //
#define CAPTURE_WIDTH_0 640
#define CAPTURE_HEIGHT_0 480
  Xuint32 height = CAPTURE_HEIGHT_0;
  Xuint32 width = CAPTURE_WIDTH_0;

  Xuint32 stride =  width * 3;
  Xuint32 frame_size   = stride * height;

  Xuint32 frameIdx;
  Xuint32 frameAddress;
  Xuint32 lineAddress;
  Xuint32 pixelAddress;
  Xuint32 *p32Frame;
  Xint32 row, col;
  Xuint32 mem32Pixel;
  Xuint8 u;
  Xuint8 y1;
  Xuint8 v;
  Xuint8 y2;
  Xuint8 memRed;
  Xuint8 memGreen;
  Xuint8 memBlue;
  Xuint8 bmp24Line[CAPTURE_WIDTH_0 * 3];
  char bmp24Header[54] = { 0x42,
			   0x4D, // MAGIC ('B' 'M')
			   0x36, 0x10, 0x0E, 0x00, // FILE SIZE ((640*480*3) + 0x36)
			   0x00, 0x00, // RSVD
			   0x00, 0x00, // RSVD
			   0x36, 0x00, 0x00, 0x00, // IMAGE OFFSET (54)
			   0x28, 0x00, 0x00, 0x00, // DIB_HEADER SIZE (40)
			   0x80, 0x02, 0x00, 0x00, // WIDTH (640)
			   0xe0, 0x01, 0x00, 0x00, // HEIGHT (480)
			   0x01, 0x00, // COLOR PLANES (1)
			   0x18, 0x00, // BPP (24)
			   0x00, 0x00, 0x00, 0x00, // COMPRESSION METHOD
			   0x00, 0x10, 0x0E, 0x00, // IMAGE SIZE (640*480*3)
			   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			   0x00, 0x00, 0x00, 0x00, 0x00 };


  physicalAddress = 0x1C000000 + read_index * frame_size;

  physicalSize    = frame_size * 3;

  printf("\tframeadress 0x%08X\n\r", physicalAddress);

  // Map physical frame buffer address to virtual space
  fd_mmap = open("/dev/mem", O_RDWR);
  if ( fd_mmap == -1 )
    {
      //pConsole->io_hprintf( pConsole->io_handle, "\tERROR : failed to open /dev/mem\n");
      return;
    }
  virtualAddress = (Xuint32)mmap(NULL, physicalSize,
				 PROT_READ | PROT_WRITE, MAP_SHARED, fd_mmap, (off_t)physicalAddress );
  if (virtualAddress == (Xuint32)MAP_FAILED)
    {
      //pConsole->io_hprintf( pConsole->io_handle, "\tERROR : mmap failed.\n");
      return;
    }


  frameIdx = img_cnt%3;

  char str[15];
  sprintf(str, "%d", img_cnt);
  printf(  "\tIMG_CNT = %d \n" ,img_cnt);

  {
    frameAddress = virtualAddress;


    // Open BMP file
    fd_bmp = open(str, O_CREAT | O_RDWR);
    if ( fd_bmp == -1 )
      {
	//pConsole->io_hprintf( pConsole->io_handle, "\tERROR : failed to open %s\n", cargv[1] );
	return;
      }

    // Write image to BMP file
    if ( write(fd_bmp, (char *)bmp24Header, sizeof(bmp24Header)) == -1 )
      {
      }
    else
      {

	    if ( write(fd_bmp, frameAddress, frame_size) == -1 )
	      {
	      }

	    //printf( "\theight = %d, width = %d, n = %d\n\r", height, width, n);
      }

  } // frameIdx = ...

  // Close BMP file
  close( fd_bmp );

  // Unmap frame buffer address
  munmap((void *)virtualAddress,physicalSize);
  close(fd_mmap);


  //vdma_status(smart_car_demo.uBaseAddr_VDMA_FrameBuffer);

  // (Re)Start Video Frame Buffer
  /*   vdma_rx_start( smart_car_demo.uBaseAddr_VDMA_FrameBuffer, */
  /* 		   smart_car_demo.vdma_resolution, */
  /* 		   VDMA_MEM_BASE_ADDR */
  /* 		   ); */
  img_cnt++;
    

  return 0;
}


int vga_command_sobel(int read_index)
{
  Xuint32 physicalAddress;
  Xuint32 physicalSize;
  Xuint32 virtualAddress;
  int fd_mmap;
  int fd_bmp;
  //
  int n;
  //
#define CAPTURE_WIDTH_0 640
#define CAPTURE_HEIGHT_0 480
  Xuint32 height = CAPTURE_HEIGHT_0;
  Xuint32 width = CAPTURE_WIDTH_0;

  Xuint32 stride =  width;
  Xuint32 frame_size   = stride * height;
  Xuint32 frameIdx;
  Xuint32 frameAddress;
  Xuint32 lineAddress;
  Xuint32 pixelAddress;
  Xuint32 *p32Frame;
  Xint32 row, col;
  Xuint32 mem32Pixel;
  Xuint8 u;
  Xuint8 y1;
  Xuint8 v;
  Xuint8 y2;
  Xuint8 memRed;
  Xuint8 memGreen;
  Xuint8 memBlue;
  Xuint8 bmp24Line[CAPTURE_WIDTH_0 * 3];
  char bmp24Header[54] = { 0x42,
			   0x4D, // MAGIC ('B' 'M')
			   0x36, 0x10, 0x0E, 0x00, // FILE SIZE ((640*480*3) + 0x36)
			   0x00, 0x00, // RSVD
			   0x00, 0x00, // RSVD
			   0x36, 0x00, 0x00, 0x00, // IMAGE OFFSET (54)
			   0x28, 0x00, 0x00, 0x00, // DIB_HEADER SIZE (40)
			   0x80, 0x02, 0x00, 0x00, // WIDTH (640)
			   0xe0, 0x01, 0x00, 0x00, // HEIGHT (480)
			   0x01, 0x00, // COLOR PLANES (1)
			   0x18, 0x00, // BPP (24)
			   0x00, 0x00, 0x00, 0x00, // COMPRESSION METHOD
			   0x00, 0x10, 0x0E, 0x00, // IMAGE SIZE (640*480*3)
			   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			   0x00, 0x00, 0x00, 0x00, 0x00 };

  // Stop Video Frame Buffer
  //vdma_rx_pause( fmc_imageon_demo.uBaseAddr_VDMA_VitaFrameBuffer );
  physicalAddress = 0x1c600000 + read_index * frame_size;
  physicalSize    = frame_size ;

  printf("\tframeadress_filter 0x%08X\n\r", physicalAddress);

  // Map physical frame buffer address to virtual space
  fd_mmap = open("/dev/mem", O_RDWR);
  if ( fd_mmap == -1 )
    {
      //pConsole->io_hprintf( pConsole->io_handle, "\tERROR : failed to open /dev/mem\n");
      return;
    }
  virtualAddress = (Xuint32)mmap(NULL, physicalSize,
				 PROT_READ | PROT_WRITE, MAP_SHARED, fd_mmap, (off_t)physicalAddress );
  if (virtualAddress == (Xuint32)MAP_FAILED)
    {
      //pConsole->io_hprintf( pConsole->io_handle, "\tERROR : mmap failed.\n");
      return;
    }


  frameIdx = img_cnt_filter%3;

  char str[15];
  sprintf(str, "%d", img_cnt_filter+20);
  printf(  "\tIMG_CNT = %d \n" ,img_cnt_filter);
  
  {
    frameAddress = virtualAddress;


    // Open BMP file
    fd_bmp = open(str, O_CREAT | O_RDWR);
    if ( fd_bmp == -1 )
      {
	//pConsole->io_hprintf( pConsole->io_handle, "\tERROR : failed to open %s\n", cargv[1] );
	return;
      }


    // Write image to BMP file
    if ( write(fd_bmp, (char *)bmp24Header, sizeof(bmp24Header)) == -1 )
      {
	//pConsole->io_hprintf( pConsole->io_handle, "\tERROR : writing header to file %s\n\r", cargv[1]);
      }
    else
      {
	n = 0;

	for (row = 0 ; row <height; row++)
	  {
	    lineAddress = frameAddress + (row * stride);
	    for (col = 0; col < width/4; col++)
	      {
		pixelAddress = lineAddress + (col << 2);
		p32Frame = (Xuint32 *) pixelAddress;
		mem32Pixel = *p32Frame;
    
		bmp24Line[(col*12) + 0] = (mem32Pixel & 0x000000FF) >> 0;
		bmp24Line[(col*12) + 1] = 0;
		bmp24Line[(col*12) + 2] = 0;

		bmp24Line[(col*12) + 3] = (mem32Pixel & 0x0000FF00) >> 8;
		bmp24Line[(col*12) + 4] = 0;
		bmp24Line[(col*12) + 5] = 0;

		bmp24Line[(col*12) + 6] = (mem32Pixel & 0x00FF0000) >> 16;
		bmp24Line[(col*12) + 7] = 0;
		bmp24Line[(col*12) + 8] = 0;

		bmp24Line[(col*12) + 9] = (mem32Pixel & 0xFF000000) >> 24;
		bmp24Line[(col*12) + 10] = 0;
		bmp24Line[(col*12) + 11] = 0;
	      }

	    if ( write(fd_bmp, (char *) bmp24Line, width * 3) == -1 )
	      {
		//pConsole->io_hprintf( pConsole->io_handle, "\tERROR : writing video line to file %s\n\r", cargv[1]);
		break;
	      }
	    n += (width * 3);
	  }
	//	printf( "\theight = %d, width = %d, n = %d\n\r", height, width, n);
      }

  } // frameIdx = ...

  // Close BMP file
  close( fd_bmp );

  // Unmap frame buffer address
  munmap((void *)virtualAddress,physicalSize);
  close(fd_mmap);

  // vdma_status(smart_car_demo.uBaseAddr_VDMA_FrameBuffer_1);

  img_cnt_filter++;

  return 0;
}
