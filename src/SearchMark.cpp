#include "pch.h"
#include "SearchMark.h"

using namespace cv;
using namespace std;


CSearchMark::CSearchMark()
{
}
CSearchMark::~CSearchMark()
{

}


int CSearchMark::Test()
{
	// 이미지소스 불러온다.
	Mat srcImage = imread("sample01.jpg", IMREAD_GRAYSCALE);
	if (srcImage.empty()) return -1;

	// 이미지 중심 구한다.
	//printf("이미지 중심 x,y : %d, %d\n", (int)(srcImage.cols / 2), (int)(srcImage.rows / 2));

	// HoughLines() 매개변수 설명
	// src : 입력할 이미지 변수, Edge detect 된 이미지를 입력해야 함
	// dst : 허프변환 직선 검출 정보를 저장할 Array
	// rho : 계산할 픽셀(매개 변수)의 해상도, 그냥 1을 사용하면 됨. (변환된 그래프에서, 선에서 원점까지의 수직 거리)
	// theta : 계산할 각도(라디안, 매개변수)의 해상도, 선 회전 각도. (모든 방향에서 직선을 검출하려면 PI / 180 을 사용하면 된다.)
	// threshold : 허프 변환된 그래프에서 라인을 검출하기 위한 최소 교차 수

	int iThreshold1_canny = 0;
	int iThreshold2_canny = 300;
	int iThershold_lines = 50; // 허프 변환된 그래프에서 라인을 검출하기 위한 최소 교차 수 (값이 클수록 정확해지고, 작을수록 모호해짐)

	Mat dstImage(srcImage.size(), CV_8UC3);
	cvtColor(srcImage, dstImage, COLOR_GRAY2BGR);

	// 엣지 검출 (직선 검출)
	Mat edges;
	Canny(dstImage, edges, iThreshold1_canny, iThreshold2_canny);

	// Mat lines;
	vector<Vec2f> lines;
	HoughLines(edges, lines, 1, CV_PI / 180.0, iThershold_lines);
	//HoughLines(srcImage, lines, 1, CV_PI / 180.0, iThershold_lines);
	cout << "lines.size()=" << lines.size() << endl;	// 검출된 직선의 개수

	// 검출한 직선 그리기
	Mat edges_lane;
	threshold(edges, edges_lane, iThreshold1_canny, iThreshold2_canny, THRESH_MASK);

	Vec2f params;
	float fRho = 0, fTheta = 0;
	float c = 0, s = 0;
	float fX0 = 0, fY0 = 0;

	for (int i = 0; i < lines.size(); i++)
	{
		params = lines[i];
		fRho = params[0];
		fTheta = params[1];

		c = cos(fTheta);
		s = sin(fTheta);
		fX0 = fRho * c;
		fY0 = fRho * s;

		cv::Point pt1, pt2;
		pt1.x = cvRound(fX0 + 1000 * (-s));
		pt1.y = cvRound(fY0 + 1000 * (c));
		pt2.x = cvRound(fX0 - 1000 * (-s));
		pt2.y = cvRound(fY0 - 1000 * (c));
		line(dstImage, pt1, pt2, Scalar(0, 0, 255), 2);
		line(edges_lane, pt1, pt2, Scalar::all(255), 2);

		printf("line distance : %d %d\n", pt1.x - pt2.x, pt1.y - pt2.y);

	}


	// 원 검출
	double dThreshold1_dp = 1; // dp = 1 >> 입력 이미지와 같은 해상도, dp = 2 >> 입력 이미지의 절반 해상도
	double dThreshold2_mindist = 1;	// 검출할 원의 최소거리
	double dParam2 = 30; // 축적 배열에서 원검출을 위한 임계값

	//Mat circles;
	vector <Vec3f> circles;
	HoughCircles(srcImage, circles, HOUGH_GRADIENT, dThreshold1_dp, dThreshold2_mindist, iThreshold2_canny, dParam2);
	cout << "circles.size()=" << circles.size() << endl;

	Mat srcImage_color(srcImage.size(), CV_8UC3);
	cvtColor(srcImage, srcImage_color, COLOR_GRAY2BGR);

	Vec3f params_circle;
	int cx = 0, cy = 0, r = 0;
	cv::Point pCenter;

	for (int i = 0; i < circles.size(); i++)
	{
		params_circle = circles[i];
		cx = cvRound(params_circle[0]);
		cy = cvRound(params_circle[1]);
		r = cvRound(params_circle[2]);
		printf("circles[%2d]:(cx, cy)=(%d, %d), r=%d\n", i, cx, cy, r);

		pCenter.x = cx;
		pCenter.y = cy;

		// 원 그리기
		circle(srcImage_color, pCenter, r, Scalar(0, 0, 255), 2);
	}


	// 이미지 출력
	//imshow("org", srcImage);
	imshow("edges", edges);
	//imshow("dstImage", dstImage);
	imshow("laneImage", edges_lane);
	//imshow("Circlemage", srcImage_color);


	// 아무키가 눌리기 전까지 대기
	waitKey();

	return 0;
}

// 화면상의 중심과 검출된 원의 중심 사이의 X, Y 픽셀거리를 구해준다.
// 상(-) ~ 하(+), 좌(-) ~ 우(+)
// pDistX, pDistY : 값이 구해질 픽셀거리변수
// imagesrc : 이미지 경로
//double dbThreshold_dp = 1; // dp = 1 >> 입력 이미지와 같은 해상도, dp = 2 >> 입력 이미지의 절반 해상도
//double dbThreshold_mindist = 2;	// 검출할 원의 최소거리
//double dbThreshold_canny = 500;	// 이미지 엣지화 하는데 쓰이는 경계값
//double dbThreshold_detection = 50; // 축적 배열에서 원검출을 위한 임계값
// 이 함수는 검출된 원이 1개일 경우에만 정상작동한다.
int CSearchMark::GetDistFromCircle
(
	unsigned char* imagesrc,
	int iImageWidth,
	int iImageHeight,
	int* pDistX,
	int* pDistY,
	double dbDp,
	double dbMindist,
	double dbThreshold_max_canny,
	double dbThreshold_min_canny,
	double dbThreshold_detection,
	double dbminRadius,
	double dbmaxRadius,
	int iCheckImageCut,
	int iEngineerMode,
	int iCameraIndex
)
{
	// 이미지소스 불러온다.
	//Mat srcImage = imread((const char*)imagesrc, IMREAD_GRAYSCALE);
	Mat srcImage = Mat(iImageHeight, iImageWidth, CV_8UC1, imagesrc);
	if (srcImage.empty()) return -1;
	
	// 이미지 자르기 ( 4방향 테두리쪽 100 pixel )
	if (iCheckImageCut == 1)
	{
		int iPixelBorder = 100;
		srcImage = srcImage(cv::Rect(iPixelBorder, iPixelBorder, CAMERA_WIDTH - iPixelBorder * 2, CAMERA_HEIGHT - iPixelBorder * 2));
	}

	// 이미지 blur 처리
	Mat srcImage_blurred;
	GaussianBlur(srcImage, srcImage_blurred, cv::Size(7, 7), 1.5, 1.5, BORDER_DEFAULT);


	// 이미지 중심 구한다.
	cv::Point PCenterOfScreen;
	PCenterOfScreen.x = (int)(srcImage.cols / 2);
	PCenterOfScreen.y = (int)(srcImage.rows / 2);
	//printf("이미지 중심 x,y : %d, %d\n", PCenterOfScreen.x, PCenterOfScreen.y);

	// 이미지소스 컬러화
	Mat srcImage_color;
	srcImage_color = Mat(srcImage.size(), CV_8UC3);
	cvtColor(srcImage, srcImage_color, COLOR_GRAY2BGR);
	line(srcImage_color, PCenterOfScreen, PCenterOfScreen, Scalar::all(0), 2);
	

	// 검출될 원의 최소 반지름, 최대 반지름 mm to pixel
	double dbDPI = (double)CAMERA_DPI;	// Huaray 카메라의 해상도 = 2540 DPI
	int iminRadiusPixel = (int)((dbminRadius * dbDPI) / 25.4);
	int imaxRadiusPixel = (int)((dbmaxRadius * dbDPI) / 25.4);

	//printf("%d, %d\n", iminRadiusPixel, imaxRadiusPixel);

	// 원 검출
	//Mat circles;
	vector <Vec3f> circles;

	// 원이 검출될 때까지 canny 값 내리면서 반복수행
	double local_dbThreshold_canny = dbThreshold_max_canny;
	int local_dbThreshold_min_canny = dbThreshold_min_canny;
	while (local_dbThreshold_canny > local_dbThreshold_min_canny)
	{
		HoughCircles(srcImage_blurred, circles, HOUGH_GRADIENT, dbDp, dbMindist, local_dbThreshold_canny, dbThreshold_detection, iminRadiusPixel, imaxRadiusPixel);
		if (circles.size() > 0) break;
		local_dbThreshold_canny -= 10;
	}

	if (local_dbThreshold_canny <= local_dbThreshold_min_canny)
	{
		//CTime CurTime = CTime::GetCurrentTime();

		//cv::String strBuf = cv::format("NG_Image\\%04d%02d%02d_%02d%02d%02d_Camera%d.jpg",
		//	CurTime.GetYear(),
		//	CurTime.GetMonth(),
		//	CurTime.GetDay(),
		//	CurTime.GetHour(),
		//	CurTime.GetMinute(),
		//	CurTime.GetSecond(),
		//	iCameraIndex
		//);

		//imwrite(strBuf, srcImage_color);
		return -1;
	}
	
	// 원이 검출될 때까지 detection 값 내리면서 반복수행
	//double local_dbThreshold_detection = dbThreshold_detection;
	//while (local_dbThreshold_detection > 0)
	//{
	//	HoughCircles(srcImage_blurred, circles, HOUGH_GRADIENT, dbDp, dbMindist, dbThreshold_canny, local_dbThreshold_detection, iminRadiusPixel, imaxRadiusPixel);
	//	if (circles.size() > 0)
	//		break;
	//	local_dbThreshold_detection -= 5;
	//}

	int iCircleSize = circles.size();
	//cout << "circles.size()=" << iCircleSize << endl;

	Vec3f params_circle;
	cv::Point PCenterOfCircle;
	int r = 0;

	for (int i = 0; i < iCircleSize; i++)
	{
		params_circle = circles[i];
		PCenterOfCircle.x = cvRound(params_circle[0]);
		PCenterOfCircle.y = cvRound(params_circle[1]);
		r = cvRound(params_circle[2]);
		//printf("circles[%2d]:(cx, cy)=(%d, %d), r = %d\n", i, PCenterOfCircle.x, PCenterOfCircle.y, r);

		if (iEngineerMode == 1)
		{
			// 원 그리기 (테스트용)
			circle(srcImage_color, PCenterOfCircle, r, Scalar(0, 0, 255), 2);
			//line(srcImage_color, PCenterOfCircle, PCenterOfCircle, Scalar(0, 0, 255), 2);
			line(srcImage_color, cv::Point2f(PCenterOfCircle.x - r, PCenterOfCircle.y), cv::Point2f(PCenterOfCircle.x + r, PCenterOfCircle.y), cv::Scalar(0, 0, 255), 2);
			line(srcImage_color, cv::Point2f(PCenterOfCircle.x, PCenterOfCircle.y - r), cv::Point2f(PCenterOfCircle.x, PCenterOfCircle.y + r), cv::Scalar(0, 0, 255), 2);
		}
		// 화면상의 중심으로부터 검출된 원의 중심사이의 거리 구하기 (X, Y) 픽셀단위
		// 검출된 원 중심 - 화면상의 중심
		(*pDistX) = (int)(PCenterOfCircle.x - PCenterOfScreen.x);
		(*pDistY) = (int)(PCenterOfCircle.y - PCenterOfScreen.y);

		printf("Distance from Circle : %d, %d\n", (*pDistX), (*pDistY));
		//printf("Distance from Circle : %d, %d\n", PCenterOfCircle.x, PCenterOfCircle.y);
	}



	// 이미지 출력 (테스트용)
	if (iEngineerMode == 1)
	{
		cv::String strBuf = cv::format("Camera%d.jpg", iCameraIndex);

		// 화면 중심과 원의 중심사이 직선을 긋는다. (테스트용)
		line(srcImage_color, PCenterOfScreen, PCenterOfCircle, Scalar::all(0), 2);

		//imshow("orgsrc", srcImage);
		//imshow(strBuf, srcImage_color);
		imwrite(strBuf, srcImage_color);

		// 아무키가 눌리기 전까지 대기
		//waitKey();
	}

	return 0;
}


// pDistX, pDistY : 값이 구해질 픽셀거리변수의 포인터
// imagesrc : 이미지 경로
// iThresholdBlockSize : 적응형 Threshold에 쓰일 행렬 사이즈
// dbThresholdOffset : 적응형 Threshold에 적용될 때 평균 값에서 빼줄 값
// iApproxSize : 검출된 윤곽선들을 근사화 했을 때 객체를 판별하기 위한 최소 꼭지점의 개수
// dbApproxMinArea : 검출될 객체의 최소 넓이
// dbApproxMinRatio : 검출될 객체의 넓이와 외곽선 길이의 최소비율 (1에 가까울수록 원)
int CSearchMark::GetDistFromContours
(
	unsigned char* imagesrc,
	int iImageWidth,
	int iImageHeight,
	int* pDistX,
	int* pDistY,
	int iThresholdBlockSize,
	double dbThresholdMaxOffset,
	double dbThresholdMinOffset,
	int iApproxSize,
	double dbApproxMinArea,
	double dbApproxMinRatio,
	int iCheckImageCut,
	int iEngineerMode,
	int iCameraIndex
)
{
	if (iThresholdBlockSize < 3)
		iThresholdBlockSize = 3;

	if (iThresholdBlockSize % 2 == 0)
		iThresholdBlockSize--;

	// 이미지소스 불러온다.
	//Mat srcImage = imread((const char*)imagesrc, IMREAD_GRAYSCALE);
	Mat srcImage = Mat(iImageHeight, iImageWidth, CV_8UC1, imagesrc);
	if (srcImage.empty()) return -1;


	// 이미지 자르기 ( 4방향 테두리쪽 100 pixel )
	if (iCheckImageCut == 1)
	{
		int iPixelBorder = 100;
		srcImage = srcImage(cv::Rect(iPixelBorder, iPixelBorder, CAMERA_WIDTH - iPixelBorder * 2, CAMERA_HEIGHT - iPixelBorder * 2));
	}

	// 이미지 blur 처리
	Mat srcImage_blurred;
	bilateralFilter(srcImage, srcImage_blurred, -1, 10, 5, BORDER_DEFAULT);


	// 이미지 중심 구한다.
	cv::Point PCenterOfScreen;
	PCenterOfScreen.x = (int)(srcImage.cols / 2);
	PCenterOfScreen.y = (int)(srcImage.rows / 2);
	//printf("이미지 중심 x,y : %d, %d\n", PCenterOfScreen.x, PCenterOfScreen.y);

	// 이미지소스 컬러화
	Mat srcImage_color;
	srcImage_color = Mat(srcImage.size(), CV_8UC3);
	cvtColor(srcImage, srcImage_color, COLOR_GRAY2BGR);
	line(srcImage_color, PCenterOfScreen, PCenterOfScreen, Scalar::all(0), 2);
	

	Mat srcImage_threshold;
	vector <vector<cv::Point>> contours;
	bool bCheckDetection = false;
	int ivtc = 0;
	double dblen = 0;
	double dbarea = 0;
	double dbratio = 0;
	double dbmin_area = (double)dbApproxMinArea;
	double dbmin_ratio = (double)dbApproxMinRatio * 0.1;
	//Rect rc;
	cv::Point2f pCenterOfContours;
	Moments moment;
	float fradius = 150;
	vector<cv::Point> approx;

	double dbLoacl_ThrehsoldMaxOffset = dbThresholdMaxOffset;
	double dbLoacl_ThrehsoldMinOffset = dbThresholdMinOffset;

	while (dbLoacl_ThrehsoldMaxOffset >= dbLoacl_ThrehsoldMinOffset)
	{

		// 적응형 Threshold 적용
		//threshold(srcImage_blurred, srcImage_threshold, 169, 255, THRESH_BINARY_INV);
		adaptiveThreshold(srcImage_blurred, srcImage_threshold, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, iThresholdBlockSize, dbLoacl_ThrehsoldMaxOffset);

		// 히스토그램 평활화
		equalizeHist(srcImage_threshold, srcImage_threshold);

		//imshow("threshold", srcImage_blurred);

		// 등고선 찾기 (findContours) 
		findContours(srcImage_threshold, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE/*CHAIN_APPROX_SIMPLE*/, cv::Point(0, 0));

		// 찾은 등고선 그리기
		//drawContours(srcImage_color, contours, -1, (255, 0, 0), 2);

		// Approximate contours to polygons + get bounding rects and circles

		for (vector<cv::Point>& pts : contours)
		{
			approxPolyDP(pts, approx, arcLength(pts, true) * 0.02, true);

			ivtc = (int)approx.size();
			if (iApproxSize < ivtc)
			{
				dblen = arcLength(pts, true);
				dbarea = contourArea(pts);
				dbratio = 4.0 * CV_PI * dbarea / (dblen * dblen);
				if (dbmin_ratio < dbratio && dbmin_area < dbarea)
				{
					moment = moments(pts, false);
					pCenterOfContours.x = moment.m10 / moment.m00;
					pCenterOfContours.y = moment.m01 / moment.m00;

					//rc = boundingRect(pts);
					//rectangle(srcImage_color, rc, cv::Scalar(0, 0, 255), iEngineerMode);
					//minEnclosingCircle(pts, pCenterOfContours, fradius);

					if (iEngineerMode == 1)
					{
						circle(srcImage_color, pCenterOfContours, (int)fradius, cv::Scalar(0, 0, 255), 2);
						line(srcImage_color, cv::Point2f(pCenterOfContours.x - fradius, pCenterOfContours.y), cv::Point2f(pCenterOfContours.x + fradius, pCenterOfContours.y), cv::Scalar(0, 0, 255), 2);
						line(srcImage_color, cv::Point2f(pCenterOfContours.x, pCenterOfContours.y - fradius), cv::Point2f(pCenterOfContours.x, pCenterOfContours.y + fradius), cv::Scalar(0, 0, 255), 2);
					}

					// 검출된 원 중심 - 화면상의 중심
					(*pDistX) = (int)(pCenterOfContours.x - PCenterOfScreen.x);
					(*pDistY) = (int)(pCenterOfContours.y - PCenterOfScreen.y);

					printf("Distance from Circle : %d, %d\n", (*pDistX), (*pDistY));
					bCheckDetection = true;
				}
			}
		}
		if (bCheckDetection) break;
		dbLoacl_ThrehsoldMaxOffset--;
	}

	if (!bCheckDetection)
	{
		//CTime CurTime = CTime::GetCurrentTime();

		//cv::String strBuf = cv::format("NG_Image\\%04d%02d%02d_%02d%02d%02d_Camera%d.jpg",
		//	CurTime.GetYear(),
		//	CurTime.GetMonth(),
		//	CurTime.GetDay(),
		//	CurTime.GetHour(),
		//	CurTime.GetMinute(),
		//	CurTime.GetSecond(),
		//	iCameraIndex
		//);	
		//
		//imwrite(strBuf, srcImage_color);

		return -1;
	}

	// 이미지 출력 (테스트용)
	if (iEngineerMode == 1)
	{
		cv::String strBuf = cv::format("Camera%d.jpg", iCameraIndex);

		// 화면 중심과 원의 중심사이 직선을 긋는다. (테스트용)
		line(srcImage_color, PCenterOfScreen, pCenterOfContours, Scalar::all(0), 2);

		//imshow(strBuf, srcImage_color);
		imwrite(strBuf, srcImage_color);

		// 아무키가 눌리기 전까지 대기
		//waitKey();
	}

	return 0;
}


int CSearchMark::Calibration
(
	unsigned char* imagesrc,
	int iImageWidth,
	int iImageHeight,
	int iPixel
)
{
	// 이미지소스 불러온다.
	Mat srcImage = Mat(iImageHeight, iImageWidth, CV_8UC1, imagesrc);
	if (srcImage.empty()) return -1;

	// 이미지 중심 구한다.
	cv::Point PCenterOfScreen;
	PCenterOfScreen.x = (int)(srcImage.cols / 2);
	PCenterOfScreen.y = (int)(srcImage.rows / 2);
	//printf("이미지 중심 x,y : %d, %d\n", PCenterOfScreen.x, PCenterOfScreen.y);

	// 그어질 직선의 오른쪽 점을 구한다.
	cv::Point PRightOfScreen;
	PRightOfScreen.x = PCenterOfScreen.x + iPixel;
	PRightOfScreen.y = PCenterOfScreen.y;


	// 화면 중심으로부터 오른쪽으로 iPixel 만큼 직선을 긋는다 (테스트용)
	line(srcImage, PCenterOfScreen, PRightOfScreen, Scalar::all(255), 2);

	//imshow("orgsrc", srcImage);
	imshow("Calibration", srcImage);

	// 아무키가 눌리기 전까지 대기
	waitKey();

	return 0;
}