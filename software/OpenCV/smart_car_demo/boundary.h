#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <string>

//setBorder parametre
#define RHO 1
//#define THETA CV_PI/180
#define THETA CV_PI/90
#define LINE_REJECT_DEGREES_MIN 5 // 10 degrees
#define LINE_REJECT_DEGREES_MAX 85 // 80 degrees
#define BORDERX 50        // px, skip this much from left & right borders

#define HOUGH_TRESHOLD 40
#define HOUGH_MIN_LINE_LENGTH 100
#define HOUGH_MAX_LINE_GAP 8

using namespace cv;
using namespace std;

struct Lane {
  Lane(){}
Lane(CvPoint a, CvPoint b, float angle, float kl, float bl): p0(a),p1(b),angle(angle),k(kl),b(bl) { }
  CvPoint p0, p1;
  float angle, k, b;
};


int sideLine(std::vector<Lane> lanes, Mat& temp_frame,bool right) {
  int best = 0;
  float k ;

  for(int i=0; i<lanes.size(); i++){
    k +=  lanes[i].k;
  }
  k /= lanes.size();

  for(int i=0; i<lanes.size(); i++){
    if(right){
      // std::cout << "right" << lanes[i].k << std::endl;
      // > inside else < outside
      if (lanes[i].k < k)
	{
	  k = lanes[i].k;
	  best = i;
	}
    }
    else{
      if (lanes[i].k > k)
	{
	  k = lanes[i].k;
	  best = i;
	}
    }
  }
  return best;
}


int setLines(vector<Vec4i>& lines, Mat& down_frame, Mat& temp_frame, int height) {
  int dir_line = 1;
  // classify lines to left/right side
  std::vector<Lane> left, right;
  int WIDTH = temp_frame.cols;
  int HEIGHT = temp_frame.rows;

  for(int i = 0; i < lines.size(); i++ )
    {
      int dx = lines[i][2] - lines[i][0];
      int dy = lines[i][3] - lines[i][1];
      float angle = atan2f(dy, dx) * 180/CV_PI;
      if ((fabs(angle) <= LINE_REJECT_DEGREES_MIN) || (fabs(angle) >= LINE_REJECT_DEGREES_MAX) ) { // reject near horizontal lines
        // cout << "coucou" <<endl;
        continue;
      }

      // // assume that vanishing point is close to the image horizontal center
      // // calculate line parameters: y = kx + b;
      dx = (dx == 0) ? 1 : dx; // prevent DIV/0!
      float k = dy/(float)dx;
      // cout << "k "<<k;
      float b = lines[i][1] + HEIGHT/2 - k*lines[i][0];

      
      // // assign lane's side based by its k
      if (k<0 && lines[i][0] < WIDTH/2 ) {
        left.push_back(Lane(Point(lines[i][0], lines[i][1]+HEIGHT/2), Point(lines[i][2], lines[i][3]+HEIGHT/2), angle, k, b));
      }
      else if (k > 0 && lines[i][0] > WIDTH/2 ) {
        right.push_back(Lane(Point(lines[i][0], lines[i][1]+HEIGHT/2), Point(lines[i][2], lines[i][3]+HEIGHT/2), angle, k, b));
      }
    }

  //set the left line and right line by finding the nearest line to center
  int best_left = -1, best_right = -1;
  if(left.size()){
    for (int i=0; i<left.size(); i++) {
      line(temp_frame, left[i].p0, left[i].p1, CV_RGB(255, 0, 0), 2);
    }
    best_left = sideLine(left, temp_frame,false);
    float left_x = (height - left.at(best_left).b)/left.at(best_left).k ;
    line(temp_frame, left[best_left].p0, left[best_left].p1, CV_RGB(255, 0, 255), 2);
  }
  else{
    line(temp_frame, cvPoint(0, HEIGHT), cvPoint(BORDERX, height), CV_RGB(255, 0, 255), 2);
  }

  if(right.size()){
    for (int i=0; i<right.size(); i++) {
      line(temp_frame, right[i].p0, right[i].p1, CV_RGB(0, 0, 255), 2);
    }
    best_right = sideLine(right, temp_frame,true);
    float right_x = (height - right[best_right].b)/right[best_right].k ;
    line(temp_frame, right[best_right].p0, right[best_right].p1, CV_RGB(255, 0, 255), 2);
  }
  else{
    line(temp_frame, cvPoint(WIDTH , HEIGHT), cvPoint(WIDTH-BORDERX ,height), CV_RGB(255, 0, 255), 2);
  }


  if((best_left == -1) && (best_right == -1)){ //keep along

    dir_line = 1;
    line(temp_frame, cvPoint(WIDTH/2 , HEIGHT), cvPoint(WIDTH/2 , HEIGHT - 60), CV_RGB(0, 255, 0), 2);  //keep

  }else if((best_left == -1) && (best_right != -1)){ //trun left
    //    std::cout << "*********left " <<endl;
    dir_line = 0;
    line(temp_frame, cvPoint( WIDTH/2 - 60 , HEIGHT - 60), cvPoint(WIDTH/2 , HEIGHT - 60), CV_RGB(0, 255, 0), 2);

  }else if((best_left != -1) && (best_right == -1)){ //turn right

    //std::cout << "***********right " << endl;
    dir_line = 2;
    line(temp_frame, cvPoint( WIDTH/2 + 60 , HEIGHT - 60), cvPoint(WIDTH/2 , HEIGHT - 60), CV_RGB(0, 255, 0), 2);

  }else{

    float cross_x = (left[best_left].b - right[best_right].b)/(right[best_right].k - left[best_left].k);
    if(abs(cross_x - WIDTH/2) < BORDERX){ //keep along
      dir_line = 1;
      line(temp_frame, cvPoint(WIDTH/2 , HEIGHT), cvPoint(WIDTH/2 , HEIGHT - 60), CV_RGB(0, 255, 0), 2);  //keep

    }
    else if(cross_x < WIDTH/2 - 100){ //trun left

      //std::cout << "*********left " << cross_x << WIDTH/2 << std::endl;
      dir_line = 0;
      line(temp_frame, cvPoint( WIDTH/2 - 60 , HEIGHT - 60), cvPoint(WIDTH/2 , HEIGHT - 60), CV_RGB(0, 255, 0), 2);

    }
    else  if(cross_x > WIDTH/2 + 100){ //turn right
      // std::cout << "***********right " << cross_x << WIDTH/2 << std::endl;
      dir_line = 2;
      line(temp_frame, cvPoint( WIDTH/2 + 60 , HEIGHT - 60), cvPoint(WIDTH/2 , HEIGHT - 60), CV_RGB(0, 255, 0), 2);
    }
  }
  return dir_line;
}

int setBoundaries(Mat& down_frame, Mat& temp_frame, int horizonalLine){
  int dir_line = 1;
  vector<Vec4i> lines;

  clock_t tStart = clock();

  HoughLinesP(down_frame, lines, RHO, THETA, HOUGH_TRESHOLD, HOUGH_MIN_LINE_LENGTH, HOUGH_MAX_LINE_GAP);

  printf("HoTime taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

  cout << " lines " <<lines.size()<<endl;
  dir_line = setLines(lines, down_frame, temp_frame, horizonalLine);

  return dir_line;
}
