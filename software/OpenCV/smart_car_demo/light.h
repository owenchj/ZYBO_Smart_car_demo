#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <sstream>

using namespace cv;
using namespace std;

int judgeColor(Mat& ROI){
  int WIDTH = ROI.cols;
  int HEIGHT = ROI.rows;

  std::vector<Mat> rgb;
  Mat Rmat, Gmat, Bmat;
    
  split(ROI,rgb);
  Rmat = rgb.at(2);
  Gmat = rgb.at(1);
  Bmat = rgb.at(0);
   
  float rData = 0;
  float gData = 0;
  float bData = 0;
  float rij = 0;
  float gij = 0;
  float bij = 0;

  float rg = 0;
  float rb = 0;
  float gr = 0;
  float gb = 0;

   
  for (int i = 0;i<HEIGHT;i++)
    {
      for (int j = 0;j<WIDTH;j++)
        {   
	  rData += (int)Rmat.at<unsigned char>(i,j);//opencv中用来访问矩阵每个元素的宏，这个宏只对单通道矩阵有效，多通道会报错
	  gData += (int)Gmat.at<unsigned char>(i,j);
	  bData += (int)Bmat.at<unsigned char>(i,j);
        }
    }

  rData = rData/(HEIGHT * WIDTH);
  gData = gData/(HEIGHT * WIDTH);
  bData = bData/(HEIGHT * WIDTH);

  rij = rData /255;
  gij = gData /255;
  bij = bData /255;

  rg=rData/gData;
  rb=rData/bData;

  gr=gData/rData;
  gb=gData/bData;

  // cout << rg<< " "<<rb<<" " << gr <<" " << gb<<endl;
    
  if(rg>=2 && rb >=2 && gr <= 1) { //red
    // cout << rij<< " "<<gij<<" " << bij<<endl;
    return 1;
  }
  else if(gr>=2 && gb >=2 && rg <= 1) { //green
    return 2;
  }
  else if(rij <= 0.3 && gij <= 0.3 && bij <= 0.3){ //off
    return 0;
  }
  else{ //yellow
    return 3;
  }
}


bool light(Mat& up_img, Mat& temp_frame){
  vector<Vec3f> circles;
  // change the last two parameters (min_radius & max_radius) to detect larger circles
  HoughCircles(up_img, circles, CV_HOUGH_GRADIENT, 1, 10, 100, 30, 5, 35 );
  Mat ROI;

  int color;
  for( size_t i = 0; i < circles.size(); i++ )
    {
      Vec3i c = circles[i];
      circle(temp_frame, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, CV_AA);
      std::cout<<"radius: "<< c[2] << endl;
      std::cout<<"center: "<< c[0] << " "<< c[1] <<endl;
      Rect square(c[0]-0.7*c[2],c[1]-0.7*c[2],1.4*c[2],1.4*c[2]);
      ROI = temp_frame(square);
      // temp_frame(square).copyTo(ROI);
      color = judgeColor(ROI);
      if(color == 0) {
	cout <<"off !"<<endl;
      } 
      if(color == 1) {
	cout<<"red ！"<<endl;
	return 0;
      }
      if(color == 2) {
	cout<<"green ！"<<endl;
      }
      if(color == 3) {
	cout<<"yellow ！"<<endl;
	return 0;
      }
    }
  return 1;
}
