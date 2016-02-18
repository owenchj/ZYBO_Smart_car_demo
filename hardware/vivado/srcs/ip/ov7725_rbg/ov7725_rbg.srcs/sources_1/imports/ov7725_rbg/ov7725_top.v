`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2014/05/23 16:24:31
// Design Name: 
// Module Name: ov7725_top
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module ov7725_top(
		  input 	clkInit,
		  output 	OV7725_SIOC,
		  inout 	OV7725_SIOD,
		  input 	OV7725_VSYNC,
		  input 	OV7725_HREF,
		  input 	OV7725_PCLK,
		  output 	OV7725_XCLK,
		  input [7:0] 	OV7725_D,

		  output [3:0] 	LED,
		  //		  output [7:0] rbg,
		  output [23:0] rbg,
		  output 	vga_hsync,
		  output 	vga_vsync,
		  output 	valid,
		  output 	clk25out,
		  input 	btn
		  );

   wire 			config_finished;  
   wire 			clk25; 
   wire 			resend;        
   
   reg [15:0] 			data;    
   reg [1:0] 			wr_hold;    
   reg [15:0] 			d_latch;
   reg [2:0] 			hsync_delay;
   reg [2:0] 			vsync_delay;
   
   assign LED = {3'b0,config_finished};
   
   // RAW
   // assign vga_hsync = OV7725_HREF;
   // assign vga_vsync = ~OV7725_VSYNC;
   // assign valid     = OV7725_HREF;
   // assign bgr       = OV7725_D;

   // RBG
   //   assign vga_hsync = hsync_delay[2];
   //   assign vga_vsync = ~vsync_delay[2];
   
   //   assign bgr[7:0]       = {data[10:5],2'b00};
   //   assign bgr[15:8]      = {data[4:0], 3'b000};
   //   assign bgr[23:16]     = {data[15:11], 3'b000};
   //   assign valid     = hsync_delay[2];
   


   //   initial wr_hold = 2'b0;
   //   initial d_latch = 16'b0;
   //   initial data = 16'b0;
   //   initial hsync_delay = 3'b0;
   //   initial vsync_delay = 3'b0;

   
   //   always@(posedge OV7725_PCLK)
   //     begin 
   //      if( OV7725_VSYNC == 1)
   //	begin
   //           wr_hold <=  2'b0;
   //        end
   //      else 
   //	begin
   //	   wr_hold <= {wr_hold[0] , (OV7725_HREF &&( ! wr_hold[0])) };
   //           d_latch <= {d_latch[7:0] , OV7725_D};
   
   //	   if (wr_hold[1] ==1 )
   //	     begin
   //		data  <= {d_latch[15:11] , d_latch[10:5] , d_latch[4:0] };
   //	     end
   //	end
   //     end
   
   //   always@(posedge OV7725_PCLK)
   //     begin 
   //	hsync_delay[0] <= OV7725_HREF;
   // 	hsync_delay[1] <= hsync_delay[0];
   //	hsync_delay[2] <= hsync_delay[1];
   //     end

   //   always@(posedge OV7725_PCLK)
   //     begin 
   //	vsync_delay[0] <= OV7725_VSYNC;
   // 	vsync_delay[1] <= vsync_delay[0];
   //	vsync_delay[2] <= vsync_delay[1];
   //     end


   // Raw to RBG
   assign valid     = vga_hsync;


   Video_Image_Processor	
     #(
       .IMG_HDISP	(640),	//640*480
       .IMG_VDISP	(480)
       )
   u_Video_Image_Processor
     (
      //global clock
      .clk					(OV7725_PCLK),  			//cmos video pixel clock
      .rst_n					(1),			//global reset

      //Image data prepred to be processd
      .per_frame_vsync		(OV7725_VSYNC),		//Prepared Image data vsync valid signal
      .per_frame_href		(OV7725_HREF),		//Prepared Image data href vaild  signal
      .per_img_RAW		(OV7725_D),			//Prepared Image data 8 Bit RAW Data


      //Image data has been processd
      .post_frame_vsync		(vga_vsync),		//Processed Image data vsync valid signal
      .post_frame_href		(vga_hsync),		//Processed Image data href vaild  signal
      .post_img_red			(rbg[23:16]),			//Processed Image Red output
      .post_img_green			(rbg[15:8]),		//Processed Image Green output
      .post_img_blue			(rbg[7:0])		//Processed Image Blue output
      
      );	



   debounce   btn_debounce(
			   .clk(clk50),
			   .i(btn),
			   .o(resend)
			   );
   

   
   I2C_AV_Config IIC(
		     .iCLK   ( clk25),    
		     .iRST_N (! resend),    
		     .Config_Done ( config_finished),
		     .I2C_SDAT  (OV7725_SIOD),    
		     .I2C_SCLK  (OV7725_SIOC),
		     .LUT_INDEX (),
		     .I2C_RDATA (),
		     .xclk   (OV7725_XCLK)
		     ); 
   
   clk_gen u_clock(
                   .clk_in1 (clkInit),
                   .clk_out1  (clk50),
                   .clk_out2 (clk25),
                   .clk_out3 (clk25out)
		   );



endmodule
