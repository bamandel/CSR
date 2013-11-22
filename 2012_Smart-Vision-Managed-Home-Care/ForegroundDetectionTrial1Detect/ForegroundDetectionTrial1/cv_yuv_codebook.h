////////YUV CODEBOOK ////////////////////////////////////////////////////
// Gary Bradski, a pre-vacation doodle July 14, 2005
// Note that this is a YUV pixel model, must have one for each YUV pixel that you care about

// Accumulate average and ~std deviation
#ifndef CVYUV_CB
#define CVYUV_CB


#include <cv.h>				// define all of the opencv classes etc.
#include <highgui.h>
#include <cxcore.h>

#define CHANNELS 3

void cvconnectedComponents(IplImage *mask, int poly1_hull0=1, float perimScale=4.0, int num=0, CvRect *bbs=NULL, CvPoint *centers=NULL);

#endif

