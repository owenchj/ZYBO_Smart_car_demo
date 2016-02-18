#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <string>

using namespace cv;
using namespace std;

int obsDetect(Mat& half_img,Mat& temp_frame, int height, int BW_TRESHOLD){
  int dir_obstacle = 1;

  int WIDTH =temp_frame.cols;
  int HEIGHT = temp_frame.rows;

  int y = height - HEIGHT/4;
  int col[WIDTH];
  for (int i = 0; i < WIDTH; ++i)
    {
      col[i] = 0;
    }

  float mean = 0;
  int left = 0, right = 0, middle = 0;
  for(int x = 0 ; x < WIDTH ; x ++)
    {    	
      for ( int j = y; j < y+30; j++){
	if((half_img.at<unsigned char>(j,x)) <= BW_TRESHOLD) continue; 
	else col[x] = col[x] + 1 ;
      }
      // std::cout << x << " " << col[x] << std::endl;
      if(x < WIDTH/2) left += col[x];
      else right += col[x];

      if(WIDTH/2 - 50 < x < WIDTH/2 + 50) middle += col[x];


      mean += col[x];
    }
  mean = mean/WIDTH;
  // std::cout << mean << std::endl;

  for(int x = 0 ; x < WIDTH ; x ++)
    {  
      if(col[x] > mean){
	// std::cout << x << " " << col[x] << std::endl;
	line(temp_frame, cvPoint(x,height), cvPoint(x,height+col[x]), CV_RGB(0, 255, 255), 2);
      }
    }

  if(middle/100 > mean){
    if(right - left >= 50 ){ //box is at right, turn left
      dir_obstacle = 0;
      line(temp_frame, cvPoint( WIDTH/2 - 30 ,height), cvPoint( WIDTH/2,height), CV_RGB(255, 0, 0), 3);
    } 
    else if(left - right >= 50){  //box is at left, turn right
      dir_obstacle = 2;
      line(temp_frame, cvPoint( WIDTH/2 + 30 ,height), cvPoint( WIDTH/2,height), CV_RGB(255, 0, 0), 3);
    }
  }
  else {
    dir_obstacle = 1;
    line(temp_frame, cvPoint( WIDTH/2 ,height), cvPoint( WIDTH/2,height - 30), CV_RGB(255, 0, 0), 3);
  }

  return dir_obstacle;
}
