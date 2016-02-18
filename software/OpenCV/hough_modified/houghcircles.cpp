//#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/cv.h>
#include <highgui.h>
#include <math.h>
#include <iostream>
#include <cv.h>

using namespace cv;
using namespace std;

int judgeColor(IplImage*);

static void help()
{
    cout << "\nThis program demonstrates circle finding with the Hough transform.\n"
            "Usage:\n"
            "./houghcircles <image_name>, Default is ../data/board.jpg\n" << endl;
}


int main(int argc, char** argv)
{
    const char* filename = argc >= 2 ? argv[1] : "../data/board.jpg";

    Mat img = imread(filename, 0);
    if(img.empty())
    {
        help();
        cout << "can not open " << filename << endl;
        return -1;
    }

    Mat cimg;
    medianBlur(img, img, 5);
    imwrite("gray.jpg", img);

    cvtColor(img, cimg, COLOR_GRAY2BGR);

    
    vector<Vec3f> circles;
    HoughCircles(img, circles, CV_HOUGH_GRADIENT, 1, 10,
		 //    HoughCircles(img, circles, HOUGH_GRADIENT, 1, 10,
                 100, 30, 5, 30 // change the last two parameters
                                // (min_radius & max_radius) to detect larger circles
                 );
    IplImage* pImg = cvLoadImage(filename,1);
    IplImage *ROI;
    CvMat *square;
    int color;
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        circle( cimg, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, CV_AA);
	// circle( cimg, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA);
        // circle( cimg, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, LINE_AA);
        std::cout<<"radius"<<c[2]<<endl;
        std::cout<<"center"<<c[0]<< " "<<c[1] <<endl;
        square = cvGetSubRect(pImg,cvCreateMatHeader(1.4*c[2],1.4*c[2],CV_8UC1),cvRect(c[0]-0.7*c[2],c[1]-0.7*c[2],1.4*c[2],1.4*c[2]));
        ROI = cvGetImage(square,cvCreateImageHeader(cvSize(1.4*c[2],1.4*c[2]),8,1));
        color = judgeColor(ROI);
        if(color == 0) cout<<"off !"<<endl;
        if(color == 1) cout<<"red ！"<<endl;
        if(color == 2) cout<<"green ！"<<endl;
        if(color == 3) cout<<"yellow ！"<<endl;
        // cvNamedWindow( "window", 1 );
        // cvShowImage( "window", ROI );
    }

//imshow("detected circles", cimg);
    imwrite("r.jpg", cimg);
    waitKey();

    return 0;
}

int judgeColor(IplImage* ROI){
    IplImage *RChannel,*GChannel,*BChannel,*tempImg,*middleImg;
    CvSize Size1;//OpenCV的基本数据类型之一。表示矩阵框大小，以像素为精度。与CvPoint结构类似，但数据成员是integer类型的width和height。
    Size1 = cvGetSize(ROI);//OpenCV提供的一种操作矩阵图像的函数。得到二维的数组的尺寸，以CvSize返回。
    RChannel = cvCreateImage(Size1, IPL_DEPTH_8U, 1);//创建头并分配数据,IPL_DEPTH_8U - 无符号8位整型
    GChannel = cvCreateImage(Size1, IPL_DEPTH_8U, 1);
    BChannel = cvCreateImage(Size1, IPL_DEPTH_8U, 1);
    cvSplit(ROI, BChannel, GChannel, RChannel, NULL);//分割多通道数组成几个单通道数组或者从数组中提取一个通道
    CvMat mat = cvMat(Size1.height,Size1.width,IPL_DEPTH_8U,NULL);//CvMat 多通道矩阵 
    CvMat mat2 = cvMat(Size1.height,Size1.width,IPL_DEPTH_8U,NULL);
    CvMat mat3 = cvMat(Size1.height,Size1.width,IPL_DEPTH_8U,NULL);
    CvMat* Rmat;//指针
    CvMat* Gmat;
    CvMat* Bmat;
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


    Rmat = cvGetMat(RChannel, &mat, 0, 0);//转换成向量
    Gmat = cvGetMat(GChannel, &mat2, 0, 0);
    Bmat = cvGetMat(BChannel, &mat3, 0, 0);
    for (int i = 0;i<ROI->height;i++)
    {
        for (int j = 0;j<ROI->width;j++)
        {   
        
            rData += (int)CV_MAT_ELEM(*Rmat, uchar, i, j);//opencv中用来访问矩阵每个元素的宏，这个宏只对单通道矩阵有效，多通道会报错
            gData += (int)CV_MAT_ELEM(*Gmat, uchar, i, j);
            bData += (int)CV_MAT_ELEM(*Bmat, uchar, i, j);
        }
    }

    rData = rData/(ROI->height * ROI->width);
    gData = gData/(ROI->height * ROI->width);
    bData = bData/(ROI->height * ROI->width);

    // float sum =rData+gData+bData;

    rij = rData /255;
    gij = gData /255;
    bij = bData /255;

    rg=rData/gData;
    rb=rData/bData;

    gr=gData/rData;
    gb=gData/bData;

    
    cout << rg<< " "<<rb<<" " << gr <<" " << gb<<endl;
    
    if(rg>=2 && rb >=2 && gr <= 1) {
      // cout << rij<< " "<<gij<<" " << bij<<endl;
        return 1;
    }
    else if(gr>=2 && gb >=2 && rg <= 1) {
      
      return 2;
    }
    else if(rij <= 0.3 && gij <= 0.3 && bij <= 0.3){
      return 0;
    }
    else{
      return 3;
    }
}
