#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <string>

using namespace cv;
using namespace std;

int FindWhite(Mat& img, int startX, int endX, int y, int BW_TRESHOLD){
  int i = 0;

  for(int x = startX; x < endX; x ++)
    {
      if(img.at<unsigned char>(y,x) > BW_TRESHOLD) i++;
    }
  return i;
}

int variance(Mat& img, int startX, int endX, int y,int BW_TRESHOLD){
  int col[endX];
  int mean = 0;
  int var = 0;

  for (int i = 0; i < endX; ++i)
    {
      col[i] = 0;
    }

  for(int x = startX; x < endX; x ++)
    {
      for ( int j = y-3; j < y+4; j++){
      	if((img.at<unsigned char>(j,x)) <= BW_TRESHOLD) continue;
      	else col[x] = col[x] + 1 ;
      }
      // std::cout << col[x] << std::endl;
      mean += col[x];
    }
  //std::cout << mean <<  " " << endX<< std::endl;
  mean = mean/endX;
  //  std::cout << mean <<  " " << endX<< std::endl;
  for (int i = 0; i < endX; ++i)
    {
      var += (mean-col[i])*(mean-col[i]);
    }
  return var;
}

int setHorizonalLine(Mat& half_img, Mat& temp_frame, int BW_TRESHOLD){
  int height = 0;
  int w = half_img.cols;
  int h = half_img.rows;

  std::vector<int> count;
  int most = 0;
  int most_1 = 0;
  int most_2 = 0;

  std::vector<int> heights;

  for(int y = 0; y < h; y ++){
    int size = FindWhite(half_img, 0, w, y, BW_TRESHOLD);
    count.push_back(size);
    if(size > most){
      most = size;
    }
  }

  for(int y = 0; y < h; y ++){
    if((count[y] > most_1) && (count[y] < most)){
      most_1 = count[y];
    }
  }

  for(int y = 0; y < h; y ++){
    if((count[y] > most_2) && (count[y] < most_1)){
      most_2 = count[y];
    }
  }

  for(int y = 0; y < h; y ++){
    if((count[y] == most )|| (count[y] == most_1 )||(count[y] == most_2 ) ){
      heights.push_back(y);
    }
  }

  //cout << "most "<< most << endl;
  //cout << "most_1 "<< most_1 << endl;
  //cout << "most_2 "<< most_2 << endl;

  int count_block[heights.size()];
  int var;

  //cout << "height "<< heights.size() << endl;
  for (int i = 0; i < heights.size(); ++i)
    {
      var = variance(half_img, 0, w, heights[i], BW_TRESHOLD);
      count_block[i] = var;
      //    cout << "var i "<< count_block[i] << " "<< heights[i] << endl;
    }

  int best = 10000000;
  for(int i = 0; i < heights.size(); ++i){
    if(count_block[i] < best){
      best = count_block[i];
      height = heights[i];
      //std::cout << best <<" "<< heights[i] << std::endl;
    }
  }

  height = height + h/2;
  //  std::cout << height <<std::endl;
  line(temp_frame, cvPoint(0,heights[0] + h/2), cvPoint(w,heights[0] + h/2), CV_RGB(0, 0, 0), 2);
  line(temp_frame, cvPoint(0,heights[1] + h/2), cvPoint(w,heights[1] + h/2), CV_RGB(0, 0, 0), 2);
  line(temp_frame, cvPoint(0,heights[2] + h/2), cvPoint(w,heights[2] + h/2), CV_RGB(0, 0, 0), 2);
  line(temp_frame, cvPoint(0,height), cvPoint(w,height), CV_RGB(0, 255, 255), 2);
  line(temp_frame, cvPoint(w/2, 0), cvPoint(w/2, 2*h), CV_RGB(0, 255, 0), 1);

  return height;
}
