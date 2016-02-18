/*-------------------------------------------------------------------------
This confidential and proprietary software may be only used as authorized
by a licensing agreement from amfpga.
(C COPYRIGHT 2013.www.amfpga.com ALL RIGHTS RESERVED
Filename			:		sdram_ov7670_vga.v
Author				:		Amfpga
Data				:		2013-02-1
Version				:		1.0
Description			:		sdram vga controller with ov7670 display.
Modification History	:
Data			By			Version			Change Description
===========================================================================
13/02/1
--------------------------------------------------------------------------*/

`timescale 1ns/1ns
module	I2C_OV7725_RGB565_Config
(
input		[7:0]	LUT_INDEX,
output	reg	[15:0]	LUT_DATA)
;


parameter	Read_DATA	=	0;			//Read data LUT Address
parameter	SET_OV7725	=	0;			//SET_OV LUT Adderss
/////////////////////	Config Data LUT	  //////////////////////////	
always@(*)
begin
	case(LUT_INDEX)
//RGB
//    SET_OV7725 +  1  :LUT_DATA = 16'h1100; /*clock config*/
//    SET_OV7725 +  2  :LUT_DATA = 16'h1206; /*VGA RGB565*/  

// RAW 8  
    SET_OV7725 +  1  :LUT_DATA = 16'h1101; /*clock config */
    SET_OV7725 +  2  :LUT_DATA = 16'h1203; /*VGA Raw Bayer */  
    SET_OV7725 +  3  :LUT_DATA = 16'h6702; /*RAW 8 */ 
 
    SET_OV7725 +  4  :LUT_DATA = 16'h1502; /*vsnyc negative*/ 
 
	default		 :	LUT_DATA	=	0;
	endcase
end

endmodule
