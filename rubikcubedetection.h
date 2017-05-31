#ifndef __RUBIKS_CUBE_H
#define __RUBIKS_CUBE_H

/****************************************************************************
 * File name     : rubikcubedetection.h
 * Definition    : Header file of the Rubik's cube project
 * Created by    : Jeyaprakash Rajagopal
 * Date          : 15.Jan.2015
 * Place         : Bonn, Germany  
 ***************************************************************************/
 
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/photo/photo.hpp>
#include<iostream>
#include<stdio.h>

// Canny operator parameters
#define CAN_THR 400
#define CANTHR_RATIO 2.5
#define CANTHR_KER 5
#define GUA_KER 3
#define DIL_THR 6

//Defining contour parameters
#define CONAREA_MIN 7000
#define CONAREA_MAX 13000
#define CONSIZE_MIN 285
#define CONSIZE_MAX 485
#define APPCON_SIZE 6

// defining vertices 
#define POINT_ZERO  0
#define VERTICES    4
#define POINT_ONE   1
#define POINT_TWO   2
#define POINT_THREE 3
#define RES_HSV_MIN 0.0
#define RES_HSV_MAX 300.0
#define ERO_SIZE    6
#define ROI_WIDTH   10
#define ROI_HEIGHT  10

//Camera parameters
#define RES_MAX 640
#define RES_MIN 480
#define CAM_INP 1

using namespace std;
using namespace cv;

class Rubikscubedetection
{
	public:
		Rubikscubedetection() {}
		cv::Mat cubeRecognition(const cv::Mat& _image,const cv::Point2f& _p1, const cv::Point2f& _p2, const cv::Point2f& _p3
              , const cv::Point2f& _p4);
};

#endif
