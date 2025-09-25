#pragma once

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/types_c.h"
#include "opencv2/imgcodecs.hpp"

class CCalculateAngle
{
public:
	CCalculateAngle();
	~CCalculateAngle();

public:

	double CalAngle
	(
		unsigned char* imagesrc1,
		int iImageWidth1,
		int iImageHeight1,
		double dbDp1,
		double dbMindist1,
		double dbThreshold_canny1,
		double dbThreshold_detection1,
		double dbminRadius1,
		double dbmaxRadius1,
		unsigned char* imagesrc2, 
		int iImageWidth2,
		int iImageHeight2,
		double dbDp2,
		double dbMindist2,
		double dbThreshold_canny2,
		double dbThreshold_detection2,
		double dbminRadius2,
		double dbmaxRadius2
	);

};

