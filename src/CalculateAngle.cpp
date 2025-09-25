#include "pch.h"
#include "CalculateAngle.h"


#define _USE_MATH_DEFINES
#include <math.h>

using namespace cv;
using namespace std;

CCalculateAngle::CCalculateAngle()
{
}
CCalculateAngle::~CCalculateAngle()
{

}



double CCalculateAngle::CalAngle
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
)
{
	// Parameter �迭ȭ
	unsigned char* Arrimagesrc[2] = { imagesrc1, imagesrc2 };
	int iArrImageWidth[2] = { iImageWidth1 , iImageWidth2 };
	int iArrImageHeight[2] = { iImageHeight1, iImageHeight2 };
	double dbArrDp[2] = { dbDp1, dbDp2 };
	double dbArrMindist[2] = { dbMindist1, dbMindist2 };
	double dbArrThreshold_canny[2] = { dbThreshold_canny1, dbThreshold_canny2 };
	double dbArrThreshold_detection[2] = { dbThreshold_detection1, dbThreshold_detection2 };
	double dbArrminRadius[2] = { dbminRadius1, dbminRadius2 };
	double dbArrmaxRadius[2] = { dbmaxRadius1, dbmaxRadius2};

	double dbDPI = (double)CAMERA_DPI;	// Huaray ī�޶��� �ػ� = 2540 DPI
	int iArrminRadiusPixel[2] = { 0, };
	int iArrmaxRadiusPixel[2] = { 0, };

	Mat srcImage[2];
	Mat srcImage_blurred[2];
	cv::Point PCenterOfScreen[2];
	vector <Vec3f> circles[2];
	int iCircleSize = 0;
	int r = 0;
	Vec3f params_circle[2];
	cv::Point PCenterOfCircle[2];

	Mat dstImageCircle[2];

	int iArrDistX[2] = { 0, };
	int iArrDistY[2] = { 0, };

	double local_dbThreshold_canny = 0;
	int iboundary = 5;

	for (int i = 0; i < 2; i++)
	{
		// ����� ���� �ּ� ������, �ִ� ������ mm to pixel
		iArrminRadiusPixel[i] = (int)((dbminRadius1 * dbDPI) / 25.4);
		iArrmaxRadiusPixel[i] = (int)((dbmaxRadius1 * dbDPI) / 25.4);

		// �̹����ҽ� �ҷ��´�.
		srcImage[i] = Mat(iArrImageHeight[i], iArrImageWidth[i], CV_8UC1, Arrimagesrc[i]);
		if (srcImage[i].empty()) return -1;

		// �̹��� Blur ó��
		GaussianBlur(srcImage[i], srcImage_blurred[i], cv::Size(3, 3), 0);

		// �̹��� �߽� ���Ѵ�.
		PCenterOfScreen[i].x = (int)(srcImage[i].cols / 2);
		PCenterOfScreen[i].y = (int)(srcImage[i].rows / 2);


		// �̹����ҽ� �÷�ȭ
		dstImageCircle[i] = Mat(srcImage[i].size(), CV_8UC3);
		cvtColor(srcImage[i], dstImageCircle[i], COLOR_GRAY2BGR);
		line(dstImageCircle[i], PCenterOfScreen[i], PCenterOfScreen[i], Scalar::all(0), 2);

		// �� ����
	
		// ���� ����� ������ canny �� �����鼭 �ݺ�����
		local_dbThreshold_canny = dbArrThreshold_canny[i];
		while (local_dbThreshold_canny > iboundary)
		{
			HoughCircles
			(
				srcImage_blurred[i], 
				circles[i], 
				HOUGH_GRADIENT, 
				dbArrDp[i], 
				dbArrMindist[i], 
				local_dbThreshold_canny, 
				dbArrThreshold_detection[i], 
				iArrminRadiusPixel[i], 
				iArrmaxRadiusPixel[i]
			);
			if (circles[i].size() > 0) break;
			local_dbThreshold_canny -= 5;
		}

		if (local_dbThreshold_canny <= iboundary) return -1;


		iCircleSize = circles[i].size();
		//cout << "circles.size()=" << iCircleSize << endl;
		r = 0;

		for (int j = 0; j < iCircleSize; j++)
		{
			params_circle[i] = circles[i][j];
			PCenterOfCircle[i].x = cvRound(params_circle[i][0]);
			PCenterOfCircle[i].y = cvRound(params_circle[i][1]);
			r = cvRound(params_circle[i][2]);

			// �� �׸��� (�׽�Ʈ��)
			circle(dstImageCircle[i], PCenterOfCircle[i], r, Scalar(0, 0, 255), 2);
			line(dstImageCircle[i], PCenterOfCircle[i], PCenterOfCircle[i], Scalar(0, 0, 255), 2);
			
			// ȭ����� �߽����κ��� ����� ���� �߽ɻ����� �Ÿ� ���ϱ� (X, Y) �ȼ�����
			// ����� �� �߽� - ȭ����� �߽�
			iArrDistX[i] = (int)(PCenterOfCircle[i].x - PCenterOfScreen[i].x);
			iArrDistY[i] = (int)(PCenterOfCircle[i].y - PCenterOfScreen[i].y);

			printf("Distance from Circle : %d, %d\n", iArrDistX[i], iArrDistY[i]);
		}

	}

	// �� ī�޶��� ���� ��� 
	double dbRadian = 0;
	double dbDegree = 0;
	double dbDistXOfCamera_mm = 70; // mm ����
	double dbDistXOfCamera_pixel = (double)((dbDistXOfCamera_mm * dbDPI) / 25.4); // pixel ����
	double dbDistYOfCamera_pixel = (double)iArrDistY[0] - (double)iArrDistY[1];
	if (dbDistYOfCamera_pixel < 0) dbDistYOfCamera_pixel *= -1;
	dbRadian = atan2(dbDistYOfCamera_pixel, dbDistXOfCamera_pixel);
	dbDegree = dbRadian * 180 / M_PI;		// Radian to Degree (���� >> ����)

	printf("Degree : %lf\n", dbDegree);


	// �̹��� ��� (�׽�Ʈ��)
	char ctest_fn[100] = { 0, };
	char ctest_screentitle[100] = { 0, };
	for (int i = 0; i < 2; i++)
	{
		// ȭ�� �߽ɰ� ���� �߽ɻ��� ������ �ߴ´�.
		line(dstImageCircle[i], PCenterOfScreen[i], PCenterOfCircle[i], Scalar::all(0), 2);
		sprintf_s(ctest_screentitle, "GetAngleTest%d", i + 1);
		sprintf_s(ctest_fn, "test%d.jpg", i + 1);
		imshow(ctest_screentitle, dstImageCircle[i]);
		//imwrite(ctest_fn, dstImageCircle[i]);
	}

	// �ƹ�Ű�� ������ ������ ���
	waitKey();
	
	return dbDegree;
}