/* obstacle detection*/
#include <stdio.h>
// #include "opencv2/imgproc/imgproc.hpp"
// #include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>
#include <string>
#include "little_sobel.h"

// extern void opencv_sobel(Mat &src, Mat &dst);
// extern void cvtcolor_rgb2yuv422(Mat& rgb, Mat& yuv); 

void yuv422rgb(Mat &src, Mat &dst){
	for(int row = 0; row < src.rows; row++)
		for(int col=0;col<src.cols;col+=2){
			Vec2b p0_in = src.at<Vec2b>(row,col);
			Vec2b p1_in = src.at<Vec2b>(row,col+1);
			Vec3b p0_out;
			Vec3b p1_out;
			p0_out.val[0] = p0_in.val[0]+1.370705*(p1_in.val[1]-128);
			p0_out.val[1] = p0_in.val[0]-0.698001*(p1_in.val[1]-128)-0.337633*(p0_in.val[1]-128);
			p0_out.val[2] = p0_in.val[0]+1.732446*(p0_in.val[1]-128);
			p1_out.val[0] = p1_in.val[0]+1.370705*(p1_in.val[1]-128);
			p1_out.val[1] = p1_in.val[0]-0.698001*(p1_in.val[1]-128)-0.337633*(p0_in.val[1]-128);
			p1_out.val[2] = p1_in.val[0]+1.732446*(p0_in.val[1]-128);
			dst.at<Vec3b>(row,col)=p0_out;
			dst.at<Vec3b>(row,col+1)=p1_out;
		}
}

void little_sobel(IplImage* pImg, IplImage* dst)
{
    Mat input = cvarrToMat(pImg);
    Mat src_yuv(input.rows, input.cols, CV_8UC2);
    Mat dst_yuv(input.rows, input.cols, CV_8UC2);
    Mat output(input.rows, input.cols, CV_8UC3);
    cvtcolor_rgb2yuv422(input, src_yuv);
    opencv_sobel(src_yuv, dst_yuv);
   // cvtColor(dst_yuv, output, CV_YUV2RGB_YUYV);
    yuv422rgb(dst_yuv,output);
	Mat edges(input.rows, input.cols, CV_8UC1);
    cvtColor(output, edges, CV_BGR2GRAY);
    *dst = edges;
    // return 0;
}

#undef MIN
#undef MAX
#define MAX(a,b) ((a)<(b)?(b):(a))
#define MIN(a,b) ((a)>(b)?(b):(a))

#define GREEN CV_RGB(0,255,0)
#define RED CV_RGB(255,0,0)
#define BLUE CV_RGB(255,0,255)
#define PURPLE CV_RGB(255,0,255)

//canny parametre
#define CANNY_MIN_TRESHOLD 1	  // edge detector minimum hysteresis threshold
#define	CANNY_MAX_TRESHOLD 100 // edge detector maximum hysteresis threshold

//houghline parametres
#define HOUGH_TRESHOLD 50		// line approval vote threshold
#define HOUGH_MIN_LINE_LENGTH 50	// remove lines shorter than this threshold
#define HOUGH_MAX_LINE_GAP 100  // join lines to one with smaller than this gaps

//setBorder parametre
#define	LINE_REJECT_DEGREES_MIN 10 // in degrees
#define	LINE_REJECT_DEGREES_MAX 80 // in degrees
#define BORDERX 10			  // px, skip this much from left & right borders
#define BW_TRESHOLD 250		  // edge response strength to recognize for 'WHITE'
#define SCAN_STEP 5

CvSize frame_size;

CvPoint2D32f sub(CvPoint2D32f b, CvPoint2D32f a) { return cvPoint2D32f(b.x-a.x, b.y-a.y); }
CvPoint2D32f mul(CvPoint2D32f b, CvPoint2D32f a) { return cvPoint2D32f(b.x*a.x, b.y*a.y); }
CvPoint2D32f add(CvPoint2D32f b, CvPoint2D32f a) { return cvPoint2D32f(b.x+a.x, b.y+a.y); }
CvPoint2D32f mul(CvPoint2D32f b, float t) { return cvPoint2D32f(b.x*t, b.y*t); }
float dot(CvPoint2D32f a, CvPoint2D32f b) { return (b.x*a.x + b.y*a.y); }
float dist(CvPoint2D32f v) { return sqrtf(v.x*v.x + v.y*v.y); }

CvPoint2D32f point_on_segment(CvPoint2D32f line0, CvPoint2D32f line1, CvPoint2D32f pt){
	CvPoint2D32f v = sub(pt, line0);
	CvPoint2D32f dir = sub(line1, line0);
	float len = dist(dir);
	float inv = 1.0f/(len+1e-6f);
	dir.x *= inv;
	dir.y *= inv;

	float t = dot(dir, v);
	if(t >= len) return line1;
	else if(t <= 0) return line0;

	return add(line0, mul(dir,t));
}

float dist2line(CvPoint2D32f line0, CvPoint2D32f line1, CvPoint2D32f pt){
	return dist(sub(point_on_segment(line0, line1, pt), pt));
}


void cutImage(IplImage* src,  IplImage* dest, CvRect rect) {
    cvSetImageROI(src, rect); 
    cvCopy(src, dest); 
    cvResetImageROI(src); 
}

struct Lane {
	Lane(){}
	Lane(CvPoint a, CvPoint b, float angle, float kl, float bl): p0(a),p1(b),angle(angle),
		votes(0),visited(false),found(false),k(kl),b(bl) { }
	CvPoint p0, p1;
	int votes;
	bool visited, found;
	float angle, k, b;
};

// struct Box {
// 	CvPoint bmin, bmax;
// 	int symmetryX;
// 	bool valid;
// 	unsigned int lastUpdate;
// };

// std::vector<Box> boxes;
// void circleDetec();

void boxDetect(IplImage *img,IplImage *temp_frame,  int & height){
	const int row = img->width * img->nChannels;
	unsigned char* ptr = (unsigned char*)img->imageData;
	int y = height - frame_size.height/4;

	int col[img->width];
	for (int i = 0; i < img->width; ++i)
    {
    	col[i] = 0;
    }


    float mean = 0;
    int left = 0, right = 0;
	for(int x = 0 ; x < img->width ; x ++)
    {    	
    	for ( int j = y; j < y+30; j++){
    		if(ptr[row*j + x] <= BW_TRESHOLD) continue; 
    		else col[x] = col[x] + 1 ;
    	}
    	// std::cout << x << " " << col[x] << std::endl;
    	if(x < img->width/2) left += col[x];
    	else right += col[x];
    	mean += col[x];
    }

    mean = mean/img->width;
    std::cout << mean << std::endl;
    for(int x = 0 ; x < img->width ; x ++)
    {  
    	if(col[x] > mean){
    		std::cout << x << " " << col[x] << std::endl;
    		cvLine(temp_frame, cvPoint(x,height), cvPoint(x,height+col[x]), CV_RGB(0, 255, 255), 2);
    	}
	}

	if(right - left > 10 ){ //box is at right, turn left
		cvLine(temp_frame, cvPoint( frame_size.width/2 - 30 ,frame_size.height - 30), cvPoint( frame_size.width/2,frame_size.height - 30), CV_RGB(0, 255, 0), 2);
	} 
	else if(left - right > 10){  //box is at left, turn right
		cvLine(temp_frame, cvPoint( frame_size.width/2 + 30 ,frame_size.height - 30), cvPoint( frame_size.width/2,frame_size.height - 30), CV_RGB(0, 255, 0), 2);
	}
	else{
		cvLine(temp_frame, cvPoint( frame_size.width/2 ,frame_size.height - 30), cvPoint( frame_size.width/2,frame_size.height - 60), CV_RGB(0, 255, 0), 2);
	}



}


float variance(IplImage *img, int startX, int endX, int y){
	const int row = img->width * img->nChannels;
	unsigned char* ptr = (unsigned char*)img->imageData;

    int col[endX];
    float mean = 0;
    float var = 0;
    for (int i = 0; i < endX; ++i)
    {
    	col[i] = 0;
    }

    for(int x = startX; x < endX; x ++)
    {    	
    	for ( int j = y-3; j < y+4; j++){
    		if(ptr[row*j + x] <= BW_TRESHOLD) continue; 
    		else col[x] = col[x] + 1 ;
    	}
    	// std::cout << col[x] << std::endl;
     	mean += col[x];
    }

    // std::cout << mean <<  " " << endX<< std::endl;

    mean = mean/endX;

    // std::cout << mean <<  " " << endX<< std::endl;

    for (int i = 0; i < endX; ++i)
    {
    	var += (mean-col[i])*(mean-col[i]);
    }

    return var;
}

void FindWhite(IplImage *img, int startX, int endX, int y, std::vector<int>& list){
	const int row = y * img->width * img->nChannels;
	unsigned char* ptr = (unsigned char*)img->imageData;

    for(int x = startX; x < endX; x ++)
    {
        if(ptr[row + x] <= BW_TRESHOLD) continue; // skip black: loop until white pixels show up
        else
            list.push_back(x);
    }
}

void setHorizonalLine(IplImage *img,IplImage *temp_frame,  int & height){
	int w = img->width;
	int h = img->height;
	std::vector<int> count;
	// int height = 0;
	int most = 0;
	int most_1 = 0;
	int most_2 = 0;
	int num = 0;

	std::vector<int> heights;

	for(int y = 0; y < h; y ++){
		std::vector<int> rsp;		
		FindWhite(img, 0, w, y, rsp);
		count.push_back(rsp.size());
		if(rsp.size() > most){
			most = rsp.size();
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

	float count_block[heights.size()];
	float var;

	for (int i = 0; i < heights.size(); ++i)
	{	
		var = variance(img,0, w, heights[i]); 
		count_block[i] = var;
	}

	int best = 10000000;
	for(int i = 0; i < heights.size(); ++i){
		if(count_block[i] < best){
			best = count_block[i];
			height = heights[i];
			std::cout << best <<" "<< heights[i] << std::endl;
		}
	}

	height = height + frame_size.height/4;

	std::cout << height <<std::endl;
	cvLine(temp_frame, cvPoint(0,heights[0] + frame_size.height/4), cvPoint(w,heights[0] + frame_size.height/4), CV_RGB(0, 0, 0), 2);
	cvLine(temp_frame, cvPoint(0,heights[1] + frame_size.height/4), cvPoint(w,heights[1] + frame_size.height/4), CV_RGB(0, 0, 0), 2);
	cvLine(temp_frame, cvPoint(0,heights[2] + frame_size.height/4), cvPoint(w,heights[2] + frame_size.height/4), CV_RGB(0, 0, 0), 2);
	cvLine(temp_frame, cvPoint(0,height), cvPoint(w,height), CV_RGB(0, 255, 255), 2);
}


// void setHorizonalLine(IplImage *img,IplImage *temp_frame){
// 	int w = img->width;
// 	int h = img->height;
// 	std::vector<int> count;
// 	int height = 0;
// 	int most = 0;
// 	int num = 0;

// 	for(int y = 0; y < h; y ++){
// 		std::vector<int> rsp;		
// 		FindWhite(img, 0, w, y, rsp);
// 		count.push_back(rsp.size());
// 		if(rsp.size() > most){
// 			most = rsp.size();
// 		}
// 	}

// 	for(int y = 0; y < h; y ++){
// 		// std::cout << count[y] << std::endl;
// 		if(count[y] == most){
// 			height += y;
// 			num ++;
// 			// std::cout << "line" << y << std::endl;
// 		}
// 	}

// 	// height = num * 20 + 10 + frame_size.height/4;
// 	height = height/num + frame_size.height/4;
// 	std::cout << height <<std::endl;
// 	cvLine(temp_frame, cvPoint(0,height), cvPoint(w,height), CV_RGB(0, 255, 255), 2);
// }



void sideLine(std::vector<Lane> lanes, IplImage *temp_frame,bool right, int & best) {
	float k = lanes[0].k;
	// int best = 0;

	for(int i=0; i<lanes.size(); i++){	
		if(right){
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
	
	// cvLine(temp_frame, lanes[best].p0, lanes[best].p1, CV_RGB(255, 0, 255), 2);		
}


void setLines(CvSeq* lines, IplImage* down_frame, IplImage* temp_frame, int & height) {
	// classify lines to left/right side
	std::vector<Lane> left, right;

	for(int i = 0; i < lines->total; i++ )
    {
        CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
        line[0].y += frame_size.height/2;
    	line[1].y += frame_size.height/2;
		int dx = line[1].x - line[0].x;
		int dy = line[1].y - line[0].y;
		float angle = atan2f(dy, dx) * 180/CV_PI;

		if ((fabs(angle) <= LINE_REJECT_DEGREES_MIN) || (fabs(angle) >= LINE_REJECT_DEGREES_MAX) ) { // reject near horizontal lines
			continue;
		}

		// assume that vanishing point is close to the image horizontal center
		// calculate line parameters: y = kx + b;
		dx = (dx == 0) ? 1 : dx; // prevent DIV/0!  
		float k = dy/(float)dx;
		float b = line[0].y - k*line[0].x;

		// assign lane's side based by its k
		if (k<0) {
			left.push_back(Lane(line[0], line[1], angle, k, b));
		} else {
			right.push_back(Lane(line[0], line[1], angle, k, b));
		}
    }

    // show Hough lines
	for	(int i=0; i<right.size(); i++) {
		cvLine(temp_frame, right[i].p0, right[i].p1, CV_RGB(0, 0, 255), 2);
	}
	for	(int i=0; i<left.size(); i++) {
		cvLine(temp_frame, left[i].p0, left[i].p1, CV_RGB(255, 0, 0), 2);
	}

	//set the left line and right line by finding the remote line to center
	int best_left = 0, best_right = 0;
	sideLine(left, temp_frame,false, best_left);
	sideLine(right, temp_frame,true, best_right);

	float left_x = (height - left[best_left].b)/left[best_left].k ;
	float right_x = (height - right[best_right].b)/right[best_right].k ;
	float cross_x = (left[best_left].b - right[best_right].b)/(right[best_right].k - left[best_left].k);

	std::cout << left_x << " "<< right_x<< std::endl;

	if((abs(left_x - right_x) < 20)&&(left_x > 0) && (left_x < frame_size.width) && (right_x > 0) && (right_x < frame_size.width)){
		cvLine(temp_frame, left[best_left].p0, left[best_left].p1, CV_RGB(255, 0, 255), 2);
		cvLine(temp_frame, right[best_right].p0, right[best_right].p1, CV_RGB(255, 0, 255), 2);
		if(cross_x < frame_size.width/2){ //trun left
			cvLine(temp_frame, cvPoint( frame_size.width/2 - 60 , frame_size.height - 60), cvPoint(frame_size.width/2 , frame_size.height - 60), CV_RGB(0, 255, 0), 2);
		}
		else{ //turn right
			cvLine(temp_frame, cvPoint( frame_size.width/2 + 60 , frame_size.height - 60), cvPoint(frame_size.width/2 , frame_size.height - 60), CV_RGB(0, 255, 0), 2);
		}
		
	}
	else{
		cvLine(temp_frame, cvPoint(0, frame_size.height), cvPoint(BORDERX, height), CV_RGB(255, 0, 255), 2);
		cvLine(temp_frame, cvPoint(frame_size.width , frame_size.height), cvPoint(frame_size.width-BORDERX ,height), CV_RGB(255, 0, 255), 2);
		cvLine(temp_frame, cvPoint(frame_size.width/2 , frame_size.height), cvPoint(frame_size.width/2 , frame_size.height - 60), CV_RGB(0, 255, 0), 2);  //keep
 
	}

}


int main(int argc, char** argv){

    const char* filename = argc >= 2 ? argv[1] : "../001.jpg";
    IplImage* frame; 
    frame = cvLoadImage( filename, 1);
    // cout<<"coucou ";
	frame_size = cvGetSize(frame);
	// cout<<"coucou2 ";
	IplImage *temp_frame = cvCreateImage(frame_size, IPL_DEPTH_8U, 3);
	IplImage *grey = cvCreateImage(frame_size, IPL_DEPTH_8U, 1);
	IplImage *edges = cvCreateImage(frame_size, IPL_DEPTH_8U, 1);
	IplImage *half_frame = cvCreateImage(cvSize(frame_size.width, frame_size.height/2), IPL_DEPTH_8U, 1);
	IplImage *down_frame = cvCreateImage(cvSize(frame_size.width, frame_size.height/2), IPL_DEPTH_8U, 1);

	CvMemStorage* houghStorage = cvCreateMemStorage(0);

	cvCopy(frame,temp_frame,NULL); 

	cvCvtColor(frame,grey,CV_BGR2GRAY);// convert to grayscale	

	// Perform a Gaussian blur ( Convolving with 5 X 5 Gaussian) & detect edges
	cvSmooth(grey, grey, CV_GAUSSIAN, 5, 5);
	// cvCanny(grey, edges, CANNY_MIN_TRESHOLD, CANNY_MAX_TRESHOLD);
	
	
	// IplImage *rgb = cvCreateImage(frame_size, IPL_DEPTH_8U, 3);
	// cvCvtColor(grey,rgb,CV_GRAY2RGB);
	IplImage *edges2 = cvCreateImage(frame_size, IPL_DEPTH_8U, 1);
	IplImage *edges3 = cvCreateImage(frame_size, IPL_DEPTH_8U, 1);
	little_sobel(temp_frame, edges2);
	cvDilate(edges2,edges3);
	cvErode(edges3,edges);
	
	// cvSobel(grey, edges2, 1,1,5);

	
	// show middle line
	cvLine(temp_frame, cvPoint(frame_size.width/2,0), 
		cvPoint(frame_size.width/2,frame_size.height), CV_RGB(255, 255, 0), 1);

	//find horizonal line as boundary
	int level = 0;
	cutImage(edges, half_frame, cvRect(0,frame_size.height/4,frame_size.width,frame_size.height/2));
	setHorizonalLine(half_frame,temp_frame, level);

	//find vertical lines 
	cutImage(edges, down_frame, cvRect(0,frame_size.height/2,frame_size.width,frame_size.height/2));
	// do Hough transform to find lanes
	double rho = 1;
	double theta = CV_PI/180;
	CvSeq* lines = cvHoughLines2(down_frame, houghStorage, CV_HOUGH_PROBABILISTIC, 
		rho, theta, HOUGH_TRESHOLD, HOUGH_MIN_LINE_LENGTH, HOUGH_MAX_LINE_GAP);
	//two closest to border
	setLines(lines, down_frame, temp_frame, level);

	//detecte box
	boxDetect(half_frame,temp_frame, level);


	// cvShowImage("Grey", grey);
	// cvShowImage("Edges", edges);
	// cvShowImage("Color", temp_frame);

	std::string name(filename);
	name[3] = 'R';
	name[4] = 'e';
	name[5] = 's';
	name[6] = 'u';
	name[7] = 'l';
	name[8] = 't';
	name[9] = 's';
	cvSaveImage( name.c_str(), edges );
	name +="1";
	name[14] = 'e';
	name[15] = '.';
	name[16] = 'j';
	name[17] = 'p';
	name[18] = 'g';
	cvSaveImage( name.c_str(), temp_frame );

	

	cvMoveWindow("Grey", 0, 0); 
	cvMoveWindow("Edges", 0, frame_size.height+25);
	cvMoveWindow("Color", 0, 2*(frame_size.height+25)); 

	// cvWaitKey(0); 

	cvReleaseMemStorage(&houghStorage);
	cvReleaseImage(&frame);
	cvReleaseImage(&grey);
	cvReleaseImage(&edges);
	cvReleaseImage(&temp_frame);
	cvReleaseImage(&half_frame);
	
}
