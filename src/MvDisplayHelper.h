#ifndef __MVDISPLAY_HELPER_H__
#define __MVDISPLAY_HELPER_H__

#include "MvImageConvertHelper.h"
#include "MvRender.h"
#include "CDisplayView.h"

class CDisplayView;
class CMvDisplayHelper
{
public:
	CMvDisplayHelper(IMV_HANDLE devHandle, CDisplayView* ptr);
	~CMvDisplayHelper();

public:
	CMvImageConvertHelper* getCMvImageConvertHelper();

	bool isValid();

	bool startDisplay(void* pHandle, int iDispalyRate);

	bool stopDisplay();

	void displayThreadProc();

	unsigned char* getPtrArrImgSrc();

private:
	CMvImageConvertHelper*			m_pImageConvertHelper;
	HANDLE							m_displayThreadHandle;
	bool							m_bRunning;
	CRender							m_render;
	unsigned int					m_iWidth;
	unsigned int					m_iHeight;
#ifdef COLORMODE
	RGBTRIPLE						m_iArrImgSrc[CAMERA_WIDTH * CAMERA_HEIGHT];
#endif
#ifdef GRAYMODE
	unsigned char					m_iArrImgSrc[CAMERAWIDTH * CAMERAHEIGHT];
#endif

	CDisplayView*					m_pDisplayView;

public:
	int								m_iCheckReverseX;	// 이미지 좌우반전
	int								m_iCheckReverseY;	// 이미지 상하반전
	int								m_iCheckCenterGrid;		// 중앙 십자마크 표시
};

#endif // __MVDISPLAY_HELPER_H__
