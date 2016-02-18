/***************************************************************************

*   Â© Copyright 2013 Xilinx, Inc. All rights reserved.

*   This file contains confidential and proprietary information of Xilinx,
*   Inc. and is protected under U.S. and international copyright and other
*   intellectual property laws.

*   DISCLAIMER
*   This disclaimer is not a license and does not grant any rights to the
*   materials distributed herewith. Except as otherwise provided in a valid
*   license issued to you by Xilinx, and to the maximum extent permitted by
*   applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH
*   ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS,
*   EXPRESS, IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES
*   OF MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR
*   PURPOSE; and (2) Xilinx shall not be liable (whether in contract or
*   tort, including negligence, or under any other theory of liability)
*   for any loss or damage of any kind or nature related to, arising under
*   or in connection with these materials, including for any direct, or any
*   indirect, special, incidental, or consequential loss or damage (including
*   loss of data, profits, goodwill, or any type of loss or damage suffered
*   as a result of any action brought by a third party) even if such damage
*   or loss was reasonably foreseeable or Xilinx had been advised of the
*   possibility of the same.

*   CRITICAL APPLICATIONS
*   Xilinx products are not designed or intended to be fail-safe, or for use
*   in any application requiring fail-safe performance, such as life-support
*   or safety devices or systems, Class III medical devices, nuclear facilities,
*   applications related to the deployment of airbags, or any other applications
*   that could lead to death, personal injury, or severe property or environmental
*   damage (individually and collectively, "Critical Applications"). Customer
*   assumes the sole risk and liability of any use of Xilinx products in Critical
*   Applications, subject only to applicable laws and regulations governing
*   limitations on product liability.

*   THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT
*   ALL TIMES.

***************************************************************************/

#include "top.h"

RGB_PIXEL cfa_boudary(Y_WINDOW *window, int bayerPhase, int row, int col, int rows, int cols)
{
  unsigned char r,g,b ;
  RGB_PIXEL pixel;

  int phase = bayerPhase & 0x00000003 ;

  if( row == 1 && col == 1)
    {
      if(phase == 0){
        r = window->getval(0,0);
        g =(window->getval(0,1) + window->getval(1,0))>> 1;
        b = window->getval(1,1);
      }
      else if(phase == 1){
        r = window->getval(0,1);
        g =(window->getval(0,0) + window->getval(1,1))>> 1;
        b = window->getval(1,0);
      }
       else if(phase == 2){
        r = window->getval(1,0);
        g =(window->getval(0,0) + window->getval(1,1))>> 1;
        b = window->getval(0,1);
      }
       else if(phase == 3){
        r =  window->getval(1,1);
        g =(window->getval(0,1) + window->getval(1,0))>> 1;
        b =  window->getval(0,0);
      }
    }
    else if( row == 1 && col == cols ){
      if(phase == 0){
        r = window->getval(0,1);
        g =(window->getval(0,2) + window->getval(1,1))>> 1;
        b = window->getval(1,2);
      }
      else if(phase == 1){
        r = window->getval(0,2);
        g =(window->getval(0,1) + window->getval(1,2))>> 1;
        b = window->getval(1,1);
      }
       else if(phase == 2){
        r = window->getval(1,1);
        g =(window->getval(0,1) + window->getval(1,2))>> 1;
        b = window->getval(0,2);
      }
       else if(phase == 3){
        r =  window->getval(1,2);
        g =(window->getval(0,2) + window->getval(1,1))>> 1;
        b =  window->getval(0,1);
      }
  }
   else if( row == rows && col == 1 ){
     if(phase == 0){
        r = window->getval(1,0);
        g =(window->getval(1,1) + window->getval(2,0))>> 1;
        b = window->getval(2,1);
      }
      else if(phase == 1){
        r = window->getval(1,1);
        g =(window->getval(1,0) + window->getval(2,1))>> 1;
        b = window->getval(2,0);
      }
       else if(phase == 2){
        r = window->getval(2,0);
        g =(window->getval(1,0) + window->getval(2,1))>> 1;
        b = window->getval(1,1);
      }
       else if(phase == 3){
        r =  window->getval(2,1);
        g =(window->getval(1,1) + window->getval(2,0))>> 1;
        b =  window->getval(1,0);
      }
  }
  else if( row == rows && col == cols ){
     if(phase == 0){
        r = window->getval(1,1);
        g =(window->getval(1,2) + window->getval(2,1))>> 1;
        b = window->getval(2,2);
      }
      else if(phase == 1){
        r = window->getval(1,2);
        g =(window->getval(1,1) + window->getval(2,2))>> 1;
        b = window->getval(2,1);
      }
       else if(phase == 2){
        r = window->getval(2,1);
        g =(window->getval(1,1) + window->getval(2,2))>> 1;
        b = window->getval(1,2);
      }
       else if(phase == 3){
        r =  window->getval(2,2);
        g =(window->getval(1,2) + window->getval(2,1))>> 1;
        b =  window->getval(1,1);
      }
  }
  else if( row == 1 && (col & 0x00000001 == 0)){
        if(phase == 0){
          r = window->getval(0,1);
          g =(window->getval(0,0) + window->getval(1,1) + window->getval(0,2))/3;
          b =(window->getval(1,0) + window->getval(1,2))>> 1;
        }
        else if(phase == 1){
          r =(window->getval(0,0) + window->getval(0,2))>> 1;
          g =(window->getval(0,1) + window->getval(1,0) + window->getval(1,2))/3;
          b = window->getval(1,1);
        }
         else if(phase == 2){
          r = window->getval(1,1);
          g =(window->getval(0,1) + window->getval(1,0) + window->getval(1,2))/3;
          b = (window->getval(0,0) + window->getval(0,2))>> 1;
        }
         else if(phase == 3){
          r =(window->getval(1,0) + window->getval(1,2))>> 1;
          g =(window->getval(0,0) + window->getval(1,1) + window->getval(0,2))/3;
          b = window->getval(0,1);
        }
    }
     else if( row == 1 && (col & 0x00000001 == 1)){
        if(phase == 0){
          r = (window->getval(0,0) + window->getval(0,2))>> 1;
          g = (window->getval(0,1) + window->getval(1,0) + window->getval(1,2))/3;
          b = window->getval(1,1);
        }
        else if(phase == 1){
          r = window->getval(0,1);
          g =(window->getval(0,0) + window->getval(1,1) + window->getval(0,2))/3;
          b =(window->getval(1,0) + window->getval(1,2))>> 1;
        }
         else if(phase == 2){
          r = (window->getval(1,0) + window->getval(1,2))>> 1;
          g =(window->getval(0,0) + window->getval(1,1) + window->getval(0,2))/3;
          b = window->getval(0,1);
        }
         else if(phase == 3){
          r = window->getval(1,1);
          g =(window->getval(0,1) + window->getval(1,0) + window->getval(1,2))/3;
          b = (window->getval(0,0) + window->getval(0,2))>> 1;
        }
    }
    else if( (row & 0x00000001 == 0) && col == 1 ){
        if(phase == 0){
          r = window->getval(1,0);
          g =(window->getval(0,0) + window->getval(1,1) + window->getval(2,0))/3;
          b =(window->getval(0,1) + window->getval(2,1))>> 1;
        }
        else if(phase == 1){
          r = window->getval(1,1);
          g =(window->getval(0,1) + window->getval(1,0) + window->getval(2,1))/3;
          b =(window->getval(0,0) + window->getval(2,0))>>2;
        }
         else if(phase == 2){
          r =(window->getval(0,0) + window->getval(2,0))>>2;
          g =(window->getval(0,1) + window->getval(1,0) + window->getval(2,1))/3;
          b = window->getval(1,1);
        }
         else if(phase == 3){
          r =(window->getval(0,1) + window->getval(2,1))>> 1;
          g =(window->getval(0,0) + window->getval(1,1) + window->getval(2,0))/3;
          b = window->getval(1,0);
        }
    }
    else if( (row & 0x00000001 == 1) && col == 1 ){
        if(phase == 0){
          r = window->getval(2,0);
          g =(window->getval(1,0) + window->getval(2,1) + window->getval(0,0))/3;
          b =(window->getval(1,1) + window->getval(0,1))>> 1;
        }
        else if(phase == 1){
          r = window->getval(2,1);
          g =(window->getval(1,1) + window->getval(2,0) + window->getval(0,1))/3;
          b =(window->getval(1,0) + window->getval(0,0))>>2;
        }
         else if(phase == 2){
          r =(window->getval(1,0) + window->getval(0,0))>>2;
          g =(window->getval(1,1) + window->getval(2,0) + window->getval(0,1))/3;
          b = window->getval(2,1);
        }
         else if(phase == 3){
          r =(window->getval(1,1) + window->getval(0,1))>> 1;
          g =(window->getval(1,0) + window->getval(2,1) + window->getval(0,0))/3;
          b = window->getval(2,0);
        }
    }
     else if( row == rows && (col & 0x00000001 == 0)){
        if(phase == 0){
          r = window->getval(1,1);
          g =(window->getval(1,0) + window->getval(2,1) + window->getval(1,2))/3;
          b =(window->getval(2,0) + window->getval(2,2))>> 1;
        }
        else if(phase == 1){
          r =(window->getval(1,0) + window->getval(1,2))>> 1;
          g =(window->getval(1,1) + window->getval(2,0) + window->getval(2,2))/3;
          b = window->getval(2,1);
        }
         else if(phase == 2){
          r = window->getval(2,1);
          g =(window->getval(1,1) + window->getval(2,0) + window->getval(2,2))/3;
          b = (window->getval(1,0) + window->getval(1,2))>> 1;
        }
         else if(phase == 3){
          r =(window->getval(2,0) + window->getval(2,2))>> 1;
          g =(window->getval(1,0) + window->getval(2,1) + window->getval(1,2))/3;
          b = window->getval(1,1);
        }
    }
     else if( row == rows && (col & 0x00000001 == 1)){
        if(phase == 0){
          r = (window->getval(1,0) + window->getval(1,2))>> 1;
          g = (window->getval(1,1) + window->getval(2,0) + window->getval(2,2))/3;
          b = window->getval(2,1);
        }
        else if(phase == 1){
          r = window->getval(1,1);
          g =(window->getval(1,0) + window->getval(2,1) + window->getval(1,2))/3;
          b =(window->getval(2,0) + window->getval(2,2))>> 1;
        }
         else if(phase == 2){
          r = (window->getval(2,0) + window->getval(2,2))>> 1;
          g =(window->getval(1,0) + window->getval(2,1) + window->getval(1,2))/3;
          b = window->getval(1,1);
        }
         else if(phase == 3){
          r = window->getval(2,1);
          g =(window->getval(1,1) + window->getval(2,0) + window->getval(2,2))/3;
          b = (window->getval(1,0) + window->getval(1,2))>> 1;
        }
    }
     else if( (row & 0x00000001 == 0) && col == cols ){
        if(phase == 0){
          r = window->getval(1,1);
          g =(window->getval(0,1) + window->getval(1,2) + window->getval(2,1))/3;
          b =(window->getval(0,2) + window->getval(2,2))>> 1;
        }
        else if(phase == 1){
          r = window->getval(1,2);
          g =(window->getval(0,2) + window->getval(1,1) + window->getval(2,2))/3;
          b =(window->getval(0,1) + window->getval(2,1))>>2;
        }
         else if(phase == 2){
          r =(window->getval(0,1) + window->getval(2,1))>>2;
          g =(window->getval(0,2) + window->getval(1,1) + window->getval(2,2))/3;
          b = window->getval(1,2);
        }
         else if(phase == 3){
          r =(window->getval(0,2) + window->getval(2,2))>> 1;
          g =(window->getval(0,1) + window->getval(1,2) + window->getval(2,1))/3;
          b = window->getval(1,1);
        }
    }
    else if( (row & 0x00000001 == 1) && col == cols ){
        if(phase == 0){
          r = window->getval(2,1);
          g =(window->getval(1,1) + window->getval(2,2) + window->getval(0,1))/3;
          b =(window->getval(1,2) + window->getval(0,2))>> 1;
        }
        else if(phase == 1){
          r = window->getval(2,2);
          g =(window->getval(1,2) + window->getval(2,1) + window->getval(0,2))/3;
          b =(window->getval(1,1) + window->getval(0,1))>>2;
        }
         else if(phase == 2){
          r =(window->getval(1,1) + window->getval(0,1))>>2;
          g =(window->getval(1,2) + window->getval(2,1) + window->getval(0,2))/3;
          b = window->getval(2,2);
        }
         else if(phase == 3){
          r =(window->getval(1,2) + window->getval(0,2))>> 1;
          g =(window->getval(1,1) + window->getval(2,2) + window->getval(0,1))/3;
          b = window->getval(2,1);
        }
    }

  else{
    r = 0;
    g = 0;
    b = 0;
  }

  pixel.val[0] = r;
  pixel.val[1] = g;
  pixel.val[2] = b;

  return pixel;
}


//Sobel Computation using a 3x3 neighborhood
RGB_PIXEL cfa_operator(Y_WINDOW *window, int bayerPhase, int row, int col)
{
  unsigned char r,g,b ;
  RGB_PIXEL pixel;

  int i = row & 0x00000001 ;
  int j = col & 0x00000001 ;
  int phase = bayerPhase & 0x00000003 ;

  if(phase == 1){
      j = 1 - j;
  }
  else if(phase == 2){
	  i = 1- i;
  }
  else if(phase == 3){
	  i = 1 - i;
	  j = 1 - j;
  }

  if( i == 0 && j ==0)
    {
      r = (window->getval(0,0) + window->getval(0,2) + window->getval(2,0) + window->getval(2,2)) >> 2;
      g = (window->getval(0,1) + window->getval(1,0) + window->getval(1,2) + window->getval(2,1)) >> 2;
      b = window->getval(1,1);
    }
  else if( i == 0 && j ==1){
    r = (window->getval(0,1) + window->getval(2,1)) >> 1;
    g =  window->getval(1,1);
    b = (window->getval(1,0) + window->getval(1,2)) >> 1;
  }
  else if( i == 1 && j ==0){
    r = (window->getval(1,0) + window->getval(1,2)) >> 1;
    g =  window->getval(1,1);
    b = (window->getval(0,1) + window->getval(2,1)) >> 1;
    
  }
  else if( i == 1 && j ==1){
    r = window->getval(1,1); 
    g = (window->getval(0,1) + window->getval(1,0) + window->getval(1,2) + window->getval(2,1)) >> 2;
    b = (window->getval(0,0) + window->getval(0,2) + window->getval(2,0) + window->getval(2,2)) >> 2;
  }

  pixel.val[0] = r;
  pixel.val[1] = g;
  pixel.val[2] = b;

  return pixel;
}

void cfa_core(Y_IMAGE& src, RGB_IMAGE& dst, int rows, int cols, int c_bayerPhase)
{
  Y_BUFFER buff_A;
  Y_WINDOW buff_C;

  for(int row = 0; row < rows+1; row++){
#pragma HLS LOOP_TRIPCOUNT MAX = 1081
    for(int col = 0; col < cols+1; col++){
#pragma HLS LOOP_TRIPCOUNT MAX = 1921
#pragma HLS loop_flatten off
#pragma HLS dependence variable=&buff_A false
#pragma HLS PIPELINE II = 1

      // Temp values are used to reduce the number of memory reads
      unsigned char temp;
      Y_PIXEL tempx;

      //Line Buffer fill
      if(col < cols){
          buff_A.shift_down(col);
          temp = buff_A.getval(0,col);
      }

      //There is an offset to accomodate the active pixel region
      //There are only MAX_WIDTH and MAX_HEIGHT valid pixels in the image
      if(col < cols && row < rows){
          Y_PIXEL new_pix;
          src >> new_pix;
          tempx = new_pix;
          buff_A.insert_bottom(tempx.val[0],col);
      }

      //Shift the processing window to make room for the new column
      buff_C.shift_right();

      //The Sobel processing window only needs to store luminance values
      //rgb2y function computes the luminance from the color pixel
      if(col < cols){
          buff_C.insert(buff_A.getval(2,col),2,0);
          buff_C.insert(temp,1,0);
          buff_C.insert(tempx.val[0],0,0);
      }

      RGB_PIXEL edge;

      //The sobel operator only works on the inner part of the image
      //This design assumes there are no edges on the boundary of the image
//      if( row <= 1 || col <= 1 || row > (rows-1) || col > (cols-1)){
//          edge.val[0] = 0;
//	      edge.val[1] = 0;
//	      edge.val[2] = 0;
//      } else {
          //CFA operation on the inner portion of the image
      //edge = cfa_operator(&buff_C, c_bayerPhase, row, col );
      //}

      if( row == 0 || col == 0){
	edge.val[0] = 0;
	edge.val[1] = 0;
	edge.val[2] = 0;
      }
      else if( row == 1 || col == 1 || row == rows || col == cols){
    edge = cfa_boudary(&buff_C, c_bayerPhase, row, col, rows, cols);
  } else {
    edge = cfa_operator(&buff_C, c_bayerPhase, row, col );
  }
      //The output image is offset from the input to account for the line buffer
      if(row > 0 && col > 0) {
          dst << edge;
      }
    }
  }
}

void cfa(Y_STREAM& video_in, AXI_STREAM& video_out, int rows, int cols, int c_bayerPhase){
    //Create AXI streaming interfaces for the core
#pragma HLS INTERFACE axis port=video_in bundle=INPUT_STREAM
#pragma HLS INTERFACE axis port=video_out bundle=OUTPUT_STREAM

#pragma HLS INTERFACE s_axilite port=rows bundle=CONTROL_BUS
#pragma HLS INTERFACE s_axilite port=cols bundle=CONTROL_BUS
#pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS

#pragma HLS INTERFACE s_axilite port= c_bayerPhase bundle=CONTROL_BUS

    Y_IMAGE img_0(rows, cols);
    RGB_IMAGE img_1(rows, cols);

#pragma HLS dataflow
    hls::AXIvideo2Mat(video_in, img_0);
    cfa_core(img_0, img_1, rows, cols, c_bayerPhase);
    hls::Mat2AXIvideo(img_1, video_out);
}
