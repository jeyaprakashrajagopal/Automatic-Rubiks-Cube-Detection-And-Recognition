/****************************************************************************
 * File name     : rubikcubedetection.cpp
 * Definition    : To detect the cube and recognize its colors
 * Inputs        : Camera Input
 * Output        : Color values of three faces of the cube
 * Created by    : Jeyaprakash Rajagopal
 * Date          : 15.Jan.2015
 * Place         : Bonn, Germany  
 ***************************************************************************/

#include "rubikcubedetection.h"

// Creating object for the class Rubikscubedetection
Rubikscubedetection rubik;
// Faces of the cube
char pre_key;
// String to store the color values
string c2[POINT_THREE][POINT_THREE];

/****************************************************************************
 * Function name : cubeReconstruction
 * Definition    : Surface rotation and colour detection process
 * Inputs        : P1,P2,P3,P4 of a quadrangle
 * Output        : 3D matrix output
 * Return values : dst matrix
 * Created by    : Jeyaprakash Rajagopal
 * Date          : 15.Jan.2015
 * Place         : Bonn  
 ***************************************************************************/
cv::Mat Rubikscubedetection::cubeRecognition(const cv::Mat& _image,const cv::Point2f& _p1, const cv::Point2f& _p2, const cv::Point2f& _p3
              , const cv::Point2f& _p4)
{
	cv::Point2f source_points[VERTICES];
	cv::Point2f dest_points[VERTICES];
	cv::Mat dst,gray, thresframe, outerBox;
	vector<Mat> hsvarr;
	vector<vector<Point> > contours;

	source_points[POINT_ZERO] = _p1;
	source_points[POINT_ONE] = _p2;
	source_points[POINT_TWO] = _p3;
	source_points[POINT_THREE] = _p4;
	
	dest_points[POINT_ZERO] = Point(RES_HSV_MAX,RES_HSV_MIN);
	dest_points[POINT_ONE] = Point(RES_HSV_MAX,RES_HSV_MAX);
	dest_points[POINT_TWO] = Point(RES_HSV_MIN,RES_HSV_MAX);
	dest_points[POINT_THREE] = Point(RES_HSV_MIN,RES_HSV_MIN);
	
	int erosion_size = ERO_SIZE;  
	Mat element = getStructuringElement(cv::MORPH_RECT,
              cv::Size(POINT_TWO * erosion_size + POINT_ONE, POINT_TWO * erosion_size + POINT_ONE),
              cv::Point(erosion_size, erosion_size) );
	dst = cv::getPerspectiveTransform(source_points, dest_points);
	cv::warpPerspective(_image, dst, dst, Size(RES_HSV_MAX, RES_HSV_MAX));
	erode(dst,dst,element);
	dilate(dst,gray, element);	

	cvtColor(gray, gray, CV_BGR2HSV);
	thresframe = gray.clone();
	inRange(gray, cv::Scalar(32, 150, 50), cv::Scalar(80, 255, 255), thresframe);
	threshold(thresframe, thresframe, 127, 255, 0);
	int numofcontours;
	findContours(thresframe,contours,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE);
	numofcontours = contours.size();
	vector<Rect> boundRect(contours.size());
	vector<vector<Point> > contours_poly(contours.size());
	cv::Mat tmp = cv::Mat::zeros( thresframe.size(), CV_8UC3 );
	cv::Mat cubeout = cv::Mat::zeros( thresframe.size(), CV_8UC3 );
	
	for(int i = POINT_ZERO ; i<numofcontours ; i++)
	{
		cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = cv::boundingRect( cv::Mat(contours_poly[i]) );
		rectangle( tmp, Point(boundRect[i].x, boundRect[i].y), Point(boundRect[i].x + boundRect[i].width, boundRect[i].y + boundRect[i].height) , cv::Scalar(0, 255, 0), 2, 8, 0 ); 
	}

	int matey, inc =POINT_ZERO, calx = 40, caly = 45, H, S, V;
	CvRect roi;
	roi.x = caly;
	roi.y = calx;
	roi.width = ROI_WIDTH;
	roi.height = ROI_HEIGHT;
	string c1[POINT_THREE][POINT_THREE];
	
	Vec3b hsv1[roi.y];
	double H1, S1,V1;
	// creating 3x3 grid of each face of the cube
	for(int j = POINT_ZERO; j<POINT_THREE; j++)
	{
		if(j == POINT_ONE)
		{calx = 40; caly = 140; roi.x = caly; roi.y = calx;}
		else if(j == POINT_TWO)
		{calx = 40; caly = 240; roi.x = caly; roi.y = calx;}

		for(matey=POINT_ZERO;matey<POINT_THREE;matey++)
		{
			for(int y = roi.y; y < roi.y + roi.height; y++)
				for(int x = roi.x; x < roi.x + roi.width; x++)
					hsv1[inc++] = gray.at<Vec3b>(x,y);
				// Calculating the mean of hue,saturation and value
				for(int chk = POINT_ZERO; chk < inc; chk++)
				{
					H1 += hsv1[chk].val[POINT_ZERO];
					S1 += hsv1[chk].val[POINT_ONE];
					V1 += hsv1[chk].val[POINT_TWO];
				}

			H1 = H1/inc; S1 = S1/inc; V1 = V1/inc;
			// Color comparision operation
			if((H1 >= 50.0 && H1 <= 75.0) && (S1 >= 200.0 && S1 <= 255.0))// && (V1 >= 50.0 && V1 <= 255.0))
				c1[j][matey] = "g";
			else if((H1 >= 0.0 && H1 <= 17.0) && (S1 >= 179.0  && S1 <= 255.0))// && (V1 >= 179.0 && V1 <= 255.0))	
				c1[j][matey] = "o";
			else if((H1 >= 20.0 && H1 <= 30.0) && (S1 >= 100.0 && S1 <= 255.0))// && (V1 >=100.0 && V1 <= 255.0))
				c1[j][matey] = "y";
			else if((H1 >= 0.0 && H1 <= 180.0) && (S1 >= 0.0 && S1 <= 170.0))// && (V1 >=100.0 && V1 <= 255.0))
				c1[j][matey] = "w";
			else if((H1 >= 160.0 && H1 <= 179.0) && (S1 >= 200.0 && S1 <= 255.0))// && (V1 >= 50.0 && V1 <= 255.0))
				c1[j][matey] = "r";
			else if((H1 >= 110.0 && H1 <= 120.0) && (S1 >= 150.0 && S1 <= 255.0))// && (V1 >= 50.0 && V1 <= 255.0))
				c1[j][matey] = "b";
				
			calx += 110;
			roi.y += 110;
			inc =0;
			
			if(c1[j][matey] != "\0")
				c2[j][matey] = c1[j][matey];
		}
		
		caly += 110;
		roi.y += 110;
		inc =0;
	}
	int row1 = 20, row2 = 20, rowcol, colcol;
	int col1 = 80, col2 = 80;
			
	// Colors for the grid in an order respectively.  Red, orange, yellow, white, green, blue
	cv::Scalar colval1(0,0,255),colval2(0, 118,238),colval3(0,255,255),colval4(255,255,255),colval5(0,128,0),colval6(255, 0,0), colval7(33, 36,41);
	cv::Scalar colvalchk;

	// To display the faces of the cube
	for( rowcol = 0; rowcol<3; rowcol++)
	{
		if(rowcol == 1)
			{row1 = 20; row2 = 80; col1 = 80; col2 = 140;}
		else if(rowcol == 2)
			{row1 = 20; row2 = 140; col1 = 80; col2 = 200;}

		for(colcol = 0; colcol<3;colcol++)
		{
			if(c2[rowcol][colcol] == "r")
				colvalchk = colval1;
			else if(c2[rowcol][colcol] == "o")
				colvalchk = colval2;
			else if(c2[rowcol][colcol] == "y")
				colvalchk = colval3;
			else if(c2[rowcol][colcol] == "w")
				colvalchk = colval4;
			else if(c2[rowcol][colcol] == "g")
				colvalchk = colval5;
			else if(c2[rowcol][colcol] == "b")
				colvalchk = colval6;
			else
				colvalchk=colval7;

			rectangle(cubeout, Point(row1,row2), Point(col1,col2), colvalchk, -1,8,0);

			row1 += 60;
			col1 += 60;
		}
	}

	imshow("Color of faces", cubeout);

	return dst;
}

/****************************************************************************
 * Function name : main
 * Definition    : Main loop for Rubik's cube detection and recognition
 * Return values : exit
 * Created by    : Jeyaprakash Rajagopal
 * Date          : 15.Jan.2015
 * Place         : Bonn  
 ***************************************************************************/
int main(int argc, char **argv)
{
	VideoCapture cap(CAM_INP);

	if(!cap.isOpened())
	{
		cout << "Can't open the camera" << endl;
		return -1;
	}

	Mat input, hsv, gray, edge, thickedge, thickedge_orig, img_contours;
	Mat cubetop, cubeleft, cuberight;
	vector<vector<Point> > contours;
	int numofcontours;
	vector<Point> singlecontour;
	vector<Point> chosencontour;  bool chosencontourflag;
	vector<Point> appcontour;
	Point singlepoint;
	Point p1,p2,p3,p4,p5,p6,p7,pc; 
	double con_area, con_size, cannythresh = CAN_THR;
	char key;

	while(1)
	{
		cap >> input;
		cvtColor(input, gray, CV_BGR2GRAY);
		blur(gray,gray,Size(GUA_KER,GUA_KER));

		Canny(gray,edge,cannythresh,CANTHR_RATIO*cannythresh,CANTHR_KER,true);

		dilate(edge,thickedge,Mat::ones(DIL_THR,DIL_THR,CV_8UC1));

		thickedge_orig = thickedge.clone();

		findContours(thickedge,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

		numofcontours = contours.size();

		img_contours = input.clone();

		chosencontourflag = false;

		for(int i = POINT_ZERO ; i<numofcontours ; i++)
		{
			singlecontour = contours.at(i);
			con_area = contourArea(singlecontour);
			con_size = arcLength(singlecontour,true);

			if ((con_area > CONAREA_MIN) && (con_area < CONAREA_MAX) && (con_size < CONSIZE_MAX) && (con_size > CONSIZE_MIN))
			{
				chosencontour = singlecontour;
				chosencontourflag = true;
			}
		}

		if(chosencontourflag == true)
		{
			approxPolyDP(chosencontour,appcontour,arcLength(chosencontour,true)*0.02,true);

			if(appcontour.size() == APPCON_SIZE)
			{
				for(unsigned int i = POINT_ZERO ; i < appcontour.size() ; i++ )
				{
					singlepoint = appcontour.at(i);
				}
				
				// Find p1
				p1 = Point(RES_MAX,RES_MIN);
				for(int i = POINT_ZERO ; i < APPCON_SIZE ; i++)
				{
					if(appcontour.at(i).y < p1.y)
					{
						p1 = appcontour.at(i);					
					}
				}

				// Find p4
				p4 = Point(POINT_ZERO,POINT_ZERO);
				for(int i = POINT_ZERO ; i < APPCON_SIZE ; i++)
				{
					if(appcontour.at(i).y > p4.y)
					{
						p4 = appcontour.at(i);
					}
				}

				// Init pc
				pc.x = (int)(p1.x + p4.x)/2;
				pc.y = (int)(((p4.y - p1.y)/2) + p1.y);
		
				// Find p2
				p2 = Point(POINT_ZERO,POINT_ZERO);
				for(int i = POINT_ZERO ; i < APPCON_SIZE ; i++)
				{
					if((appcontour.at(i).x > pc.x) && (appcontour.at(i).y < pc.y))
					{
						p2 = appcontour.at(i);
					}
				}

				// Find p3
				p3 = Point(POINT_ZERO,POINT_ZERO);
				for(int i = POINT_ZERO ; i < APPCON_SIZE ; i++)
				{
					if((appcontour.at(i).x > pc.x) && (appcontour.at(i).y > pc.y))
					{
						p3 = appcontour.at(i);
					}
				}

				// Find p5
				p5 = Point(POINT_ZERO,POINT_ZERO);
				for(int i = POINT_ZERO ; i < APPCON_SIZE ; i++)
				{
					if((appcontour.at(i).x < pc.x) && (appcontour.at(i).y > pc.y))
					{
						p5 = appcontour.at(i);
					}
				}

				// Find p6
				p6 = Point(POINT_ZERO,POINT_ZERO);
				for(int i = POINT_ZERO ; i < APPCON_SIZE ; i++)
				{
					if((appcontour.at(i).x < pc.x) && (appcontour.at(i).y < pc.y))
					{
						p6 = appcontour.at(i);
					}
				}

				// Find p7
				p7 = Point(POINT_ZERO,POINT_ZERO);
				double a,b;
				
				if(((p1.x-p2.x) != 0.0) && ((p1.x-p6.x) != 0.0))
				{
					a = (double)(p1.y-p2.y)/(p1.x-p2.x);
					b = (double)(p1.y-p6.y)/(p1.x-p6.x);
					if((a-b) != 0.0)
					{
						p7.x = (int)(p2.y - (b * p2.x) - p6.y + (a* p6.x))/(a-b);
						p7.y = (int)(p2.y - (b*p2.x) + (b*p7.x));
					}
				}
			/*cout << "Points before "<< p1.x << ": "<<p1.y<<endl;
				
					corners[0] = p1;
					corners[1] = p2;
					corners[2] = p3;
					corners[3] = p4;
					corners[4] = p5;
					corners[5] = p6;
					corners[6] = p7;
				cornerSubPix(thickedge, corners, Size(5,5), Size(-1,-1), TermCriteria( CV_TERMCRIT_ITER , 40, 0 ));

					p1 = corners[0];
					p2 = corners[1];
					p3 = corners[2];
					p4 = corners[3];
					p5 = corners[4];
					p6 = corners[5];
					p7 = corners[6];
			cout << "Points After "<< p1.x << ": "<<p1.y<<endl;*/
			}
		}

		if(((p1.x-p2.x) != 0.0) && ((p1.x-p6.x) != 0.0))
		{
			line(img_contours,p1,p2,Scalar(0,0,255),2.2);
			line(img_contours,p2,p3,Scalar(0,0,255),2.2);
			line(img_contours,p3,p4,Scalar(0,0,255),2.2);
			line(img_contours,p4,p5,Scalar(0,0,255),2.2);
			line(img_contours,p5,p6,Scalar(0,0,255),2.2);
			line(img_contours,p6,p1,Scalar(0,0,255),2.2);
			line(img_contours,p2,p7,Scalar(0,0,255),2.2);
			line(img_contours,p6,p7,Scalar(0,0,255),2.2);
			line(img_contours,p7,p4,Scalar(0,0,255),2.2);
			key = waitKey(120);

			switch(key)
			{
				case 'q':
					exit(1);
					break;
				case 'l':
					pre_key = 'l';
					cubeleft = rubik.cubeRecognition(input,p7,p4,p5,p6); // left face
					break;
				case 't':
					pre_key = 't';
					cubetop = rubik.cubeRecognition(input, p1, p2, p7, p6); //top face
					break;
				case 'r':
					pre_key = 'r';
					cuberight = rubik.cubeRecognition(input,p2,p3,p4,p7); //right face
					break;
				default:
					if(pre_key == 'l')
						cubeleft = rubik.cubeRecognition(input,p7,p4,p5,p6); // left face
					else if(pre_key == 't')
						cubetop = rubik.cubeRecognition(input, p1, p2, p7, p6); //top face
					else if(pre_key == 'r')	
						cuberight = rubik.cubeRecognition(input,p2,p3,p4,p7); //right face
			}
		}
		
		imshow("EdgeDetection", edge);
		//imshow("Dilated", thickedge_orig);
		imshow("Rubik's Cube Detection and Recognition", img_contours);

		if(!cubeleft.empty())
			imshow("Left Face View", cubeleft);
		if(!cuberight.empty())
			imshow("Right Face View", cuberight);
		if(!cubetop.empty())
			imshow("Top Face View", cubetop);
			
		if(waitKey(10) =='q') break;
	}

	imwrite("edge.bmp",edge);
	imwrite("gray.bmp",gray);
	imwrite("contour.bmp",img_contours);
	
    return POINT_ZERO;
}
