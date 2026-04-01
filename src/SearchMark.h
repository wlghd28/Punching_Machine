#pragma once
//opencv header file include
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/types_c.h"
#include "opencv2/imgcodecs.hpp"

class CSearchMark
{
public:
	CSearchMark();
	~CSearchMark();


public:


	int Test();	// 테스트용

	// pDistX, pDistY : 값이 구해질 픽셀거리변수의 포인터
	// imagesrc : 이미지 경로
	// double dbThreshold_dp // dp = 1 >> 입력 이미지와 같은 해상도, dp = 2 >> 입력 이미지의 절반 해상도
	// double dbThreshold_mindist // 검출할 원의 최소거리
	// double dbThreshold_canny // 이미지를 흑백화 할 때 쓰이는 임계 값
	// double dbThreshold_detection // 축적 배열에서 원검출을 위한 임계값 (값이 커질 수록 정확, 작을 수록 모호)
	int GetDistFromCircle
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
	);

	// pDistX, pDistY : 값이 구해질 픽셀거리변수의 포인터
	// imagesrc : 이미지 경로
	// iThresholdBlockSize : 적응형 Threshold에 쓰일 행렬 사이즈
	// dbThresholdOffset : 적응형 Threshold에 적용될 때 평균 값에서 빼줄 값
	// iApproxSize : 검출된 윤곽선들을 근사화 했을 때 객체를 판별하기 위한 최소 꼭지점의 개수
	// dbApproxMinArea : 검출될 객체의 최소 넓이
	// dbApproxMinRatio : 검출될 객체의 넓이와 외곽선 길이의 최소비율 (1에 가까울수록 원)
	int GetDistFromContours
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
	);


	// imagesrc : 이미지 경로
	// iPixel : 화면의 중심으로부터 오른쪽으로 선을 그을 픽셀 수 
	int Calibration
	(
		unsigned char* imagesrc,
		int iImageWidth,
		int iImageHeight,
		int iPixel
	);
};

