// ForegroundDetectionTrial1.cpp : Defines the entry point for the console application.
// Written by Jackson Dean Goodwin as part of a research project in the area of biomedical informatics under the direction of the National Science Foundation at the University of Tennessee at Chattanooga.
// Hit the Esc key to exit the program. The focus must be on the image window before you hit Esc.

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "cv_yuv_codebook.h"
using namespace std;
using namespace cv;

// these are for the cvconnectedComponents.
IplImage
	*frame, // A three-channel, 8-bit-per-channel image. Do not modify or release this image. It is used as a buffer by OpenCV. You must make a copy of this image in order to work with it.
	*clonedFrame, // A copy of frame.		
	*frameGray, // A single-channel, 8-bit (grayscale) image that is a grayscale copy of frame.
	*foreground, // A single-channel 8-bit binary image (0 or 255) that shows the foreground as white, and the background as black.
	*foregroundSkin, // A single-channel 8-bit binary image (0 or 255) where the white part is a subset of that in foreground, such that any part of the foreground that is detected as skin remains white.
	*foregroundSkinMask; // A single-channel 8-bit binary image (0 or 255) used to define a small portion of foregroundSkin in which to detect the head.
CvFont font; // A font object used to write text onto the image.
int posture = 0; // Used to represent the posture of the patient: 0 = Lying Down, 1 = Sitting Down, 2 = Standing Up.
int postureCount[] = {0, 0, 0}; // Used to count the number of postures detected within a finite period of time (defined by calibrationCount below) from all ellipses fitted around blobs.

int main()
{
	CvCapture* capture = cvCaptureFromCAM(1);
	if (!capture)
	{
		fprintf(stderr, "ERROR: capture is NULL \n");
		getchar();
		return -1;
	}

	cvNamedWindow("ForegroundDetectionTrial1", CV_WINDOW_AUTOSIZE);

	double
		lowExposure = -9.0, // The lowest exposure that the camera will have. Logitech is -9 to -3; Dell Webcam is -12 to -3
		cameraExposure = lowExposure, // The exposure of the camera.
		previousCameraExposure = lowExposure, // Starts at lowExposure at the beginning of calibration and increases by 1.0 until highExposure is reached.
		highExposure = -3.0; // The highest exposure that the camera will have.
	const int numExposures = 7; // must be manually set to (int)(highExposure) - (int)(lowExposure) + 1;
	int currentExposure = 0, // starts from the lowest cameraExposure (smallest number).
		previousExposure = 0; // Used during calibration.

	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 640); // You may change the width and height of the camera image if you need to, however, larger image sizes significantly slow down the frame rate.
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 480);
	cvSetCaptureProperty(capture, CV_CAP_PROP_EXPOSURE, 0.0);

	return 0;
}

//For connected components:
int CVCONTOUR_APPROX_LEVEL = 2; // Approx.threshold - the bigger it is, the simpler is the boundary **.** it was 2
int CVCLOSE_ITR = 1; // How many iterations of erosion and/or dialation there should be
//Just some convienience macros
#define CV_CVX_WHITE	CV_RGB(0xff,0xff,0xff)
#define CV_CVX_BLACK	CV_RGB(0x00,0x00,0x00)
/*
This cleans up the forground segmentation mask
mask			Is a grayscale (8 bit depth) "raw" mask image which will be cleaned up
OPTIONAL PARAMETERS:
poly1_hull0	If set, approximate connected component by (DEFAULT) polygon, or else convex hull (0)
perimScale 	Len = image (width+height)/perimScale.  If contour len < this, delete that contour (DEFAULT: 4)
num			Maximum number of rectangles and/or centers to return, on return, will contain number filled (DEFAULT: NULL)
bbs			Pointer to bounding box rectangle vector of length num.  (DEFAULT SETTING: NULL)
centers		Pointer to contour centers vectore of length num (DEFULT: NULL)
*/
void cvconnectedComponents(IplImage *mask, int poly1_hull0, float perimScale, int num, CvRect *bbs, CvPoint *centers)
{
	static CvMemStorage* mem_storage = NULL;
	static CvSeq* contours = NULL;
	// Clean up raw mask.
	cvMorphologyEx(mask, mask, NULL, NULL, CV_MOP_OPEN, CVCLOSE_ITR);
	//cvMorphologyEx(mask, mask, NULL, NULL, CV_MOP_OPEN, CVCLOSE_ITR);
	//cvMorphologyEx(mask, mask, NULL, NULL, CV_MOP_CLOSE, CVCLOSE_ITR);
	cvMorphologyEx(mask, mask, NULL, NULL, CV_MOP_CLOSE, CVCLOSE_ITR);

	// Find contours around only bigger regions.
	if(mem_storage == NULL) mem_storage = cvCreateMemStorage(0);
    else cvClearMemStorage(mem_storage);

	CvContourScanner scanner = cvStartFindContours(mask, mem_storage, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	CvSeq* c;
	int numCont = 0;

	while((c = cvFindNextContour(scanner)) != NULL)
	{
		double len = cvContourPerimeter(c);
		double q = (mask->height + mask->width) / perimScale;   // Calculate perimeter len threshold.
		if(len < q) // Get rid of blob if it's perimeter is too small
		{
			cvSubstituteContour(scanner, NULL);
		}
		else // Smooth its edges if it's large enough.
		{
			CvSeq* c_new;
			if(poly1_hull0) // Polygonal approximation of the segmentation.
	            c_new = cvApproxPoly(c, sizeof(CvContour), mem_storage, CV_POLY_APPROX_DP, CVCONTOUR_APPROX_LEVEL, 0);
			else // convex hull of the segmentation
				c_new = cvConvexHull2(c, mem_storage, CV_CLOCKWISE, 1);
            cvSubstituteContour(scanner, c_new);
			numCont++;
        }
	}
	contours = cvEndFindContours(&scanner);

	// Paint the found regions back into the image.
	cvZero(mask);
	IplImage *maskTemp;
	// Calculate the center of mass and/or bounding rectangles.
	if(num != 0)
	{
		int N = num,
			numFilled = 0,
			i = 0,
			count,
			semiMajorAxis,
			semiMinorAxis,
			focus,
			head1X,
			head1Y,
			head2X,
			head2Y,
			headSize = 16;
		CvMoments moments;
		double	M00,
				M01, 
				M10,
				angle;
		maskTemp = cvCloneImage(mask);
		vector<Vec2f> lines;
		CvBox2D32f* box;   
		CvPoint* PointArray;   
		CvPoint2D32f* PointArray2D32f;
		CvScalar average;
		for(i = 0, c=contours; c != NULL; c = c->h_next, i++)
		{
			if(i < N) // Only process up to num of them.
			{
				cvDrawContours(maskTemp, c, CV_CVX_WHITE, CV_CVX_WHITE, -1, CV_FILLED, 8);
				// Find the center of each contour.
				if(centers != NULL)
				{
					cvMoments(maskTemp, &moments, 1);
					M00 = cvGetSpatialMoment(&moments, 0, 0) + 0.001;
					M10 = cvGetSpatialMoment(&moments, 1, 0);
					M01 = cvGetSpatialMoment(&moments, 0, 1);
					centers[i].x = (int)(M10 / M00) + 0.001;
					centers[i].y = (int)(M01 / M00) + 0.001;
				}
				// bounding rectangles around blobs
				if(bbs != NULL)
				{
					bbs[i] = cvBoundingRect(c);
				}
				// Find number of straight lines in the foreground.
				/*
				clonedFrame =  cvCloneImage(frameGray);
				frameEdges = cvCloneImage(frameGray);
				cvCanny(clonedFrame, frameEdges, 10, 100, 3);
				Mat tempMatrixEdges(frameEdges);
				Mat tempMatrixMask(maskTemp);
				Mat tempMatrixCombination(frameEdges);
				bitwise_and(tempMatrixEdges, tempMatrixMask, tempMatrixCombination);
				HoughLines(tempMatrixCombination, lines, 4.0, CV_PI / 10.0, 100, 0, 0);
				*/
				cvZero(maskTemp);
				numFilled++;
			}
			// Draw filled contours into mask.
			if	(true
				//lines.size() < 75
				)
			{
				cvDrawContours(mask, c, CV_CVX_WHITE, CV_CVX_WHITE, -1, CV_FILLED, 8); // Draw to central mask.
				int i; // Indicator of cycle.
				int count = c->total; // This is number of points in the contour.
				CvPoint center;
				CvSize size;

				// Number point must be greater than or equal to 6 (for cvFitEllipse_32f).
				if(count <= 6)
				{
					continue;
				}

				// Alloc memory for contour point set.    
				PointArray = (CvPoint*)malloc(count * sizeof(CvPoint));
				PointArray2D32f= (CvPoint2D32f*)malloc(count * sizeof(CvPoint2D32f));

				// Alloc memory for ellipse data.
				box = (CvBox2D32f*)malloc(sizeof(CvBox2D32f));

				// Get contour point set.
				cvCvtSeqToArray(c, PointArray, CV_WHOLE_SEQ);

				// Convert CvPoint set to CvBox2D32f set.
				for(i = 0; i < count; i++)
				{
					PointArray2D32f[i].x = (float)PointArray[i].x;
					PointArray2D32f[i].y = (float)PointArray[i].y;
				}

				// Fit ellipse to current contour.
				// The output ellipse has the property that box->size.width > box->size.height.
				cvFitEllipse(PointArray2D32f, count, box);
				center.x = cvRound(box->center.x);
				center.y = cvRound(box->center.y);
				size.width = cvRound(box->size.width * 0.5);
				size.height = cvRound(box->size.height * 0.5);
				double aspect = ((double)(size.height) / (double)(size.width)) + 0.001;
				angle = -1.0 * box->angle + 90.0;
				//cout << "angle: " << box->angle << endl;
				//cout << "angle: " << angle << endl;
				//cout << "aspect: " << aspect << endl;
				// Posture detection
				if(size.height > 96)
				{
					if(abs(angle) < 45)
					{
						// Lying Down
						postureCount[0] += 1;
					}
					else if(aspect < 1.8)
					{
						// Sitting Down
						postureCount[1] += 1;
					}
					else
					{
						// Standing Up
						postureCount[2] += 1;
					}
				}
				// Angle is from vertical clockwise in degrees.
				//cvEllipse(mask, center, size, box->angle, 0, 360, CV_RGB(255, 255, 255), 4, 4, 0);
				semiMajorAxis = size.height;
				semiMinorAxis = size.width;
				/*
				focus = (int)(sqrt((double)(abs((semiMajorAxis * semiMajorAxis) - (semiMinorAxis * semiMinorAxis)))));
				head1X = center.x + focus * cos(angle * 3.141592654 / 180.0);
				head1Y = center.y - focus * sin(angle * 3.141592654 / 180.0);
				head2X = center.x - focus * cos(angle * 3.141592654 / 180.0);
				head2Y = center.y + focus * sin(angle * 3.141592654 / 180.0);
				*/
				head1X = center.x + semiMajorAxis * cos(angle * 3.141592654 / 180.0);
				head1Y = center.y - semiMajorAxis * sin(angle * 3.141592654 / 180.0);
				head2X = center.x - semiMajorAxis * cos(angle * 3.141592654 / 180.0);
				head2Y = center.y + semiMajorAxis * sin(angle * 3.141592654 / 180.0);
				/*
				cvZero(foregroundSkinMask);
				cvRectangle(foregroundSkinMask, cvPoint(head1X - headSize, head1Y - headSize), cvPoint(head1X + headSize, head1Y + headSize), CV_RGB(255, 255, 255), CV_FILLED, 4, 0);
				average = cvAvg(foregroundSkin, foregroundSkinMask);
				
				if(*average.val > 16.0)
				{
					cvCircle(mask, cvPoint(head1X, head1Y), 24, CV_RGB(255, 255, 255), -1, 4, 0);
				}
				
				//cout << *average.val << endl;
				cvZero(foregroundSkinMask);
				cvRectangle(foregroundSkinMask, cvPoint(head2X - headSize, head2Y - headSize), cvPoint(head2X + headSize, head2Y + headSize), CV_RGB(255, 255, 255), CV_FILLED, 4, 0);
				average = cvAvg(foregroundSkin, foregroundSkinMask);
				
				if(*average.val > 16.0)
				{
					cvCircle(mask, cvPoint(head2X, head2Y), 24, CV_RGB(255, 255, 255), CV_FILLED, 4, 0);
				}
				*/
				//cout << *average.val << endl;
				free(PointArray);
				free(PointArray2D32f);
				free(box);
			}
		} //end looping over contours
		num = numFilled;
		cvReleaseImage(&maskTemp);
	}
	// Else, draw processed contours into the mask.
	else
	{
		for( c=contours; c != NULL; c = c->h_next )
		{
			cvDrawContours(mask, c, CV_CVX_WHITE, CV_CVX_BLACK, -1, CV_FILLED, 8);
		}
	}
}