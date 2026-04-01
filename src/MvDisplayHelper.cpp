#include "pch.h"

#include "MvDisplayHelper.h"

// 鞫刻窟넋
// display thread
static unsigned int __stdcall displayThread(void* pUser)
{
	CMvDisplayHelper* pCMvDisplayHelper = (CMvDisplayHelper*)pUser;
	if (!pCMvDisplayHelper)
	{
		TRACE("pCMvDisplayHelper is NULL!\n");
		return -1;
	}

	pCMvDisplayHelper->displayThreadProc();

	return 0;
}

CMvDisplayHelper::CMvDisplayHelper(IMV_HANDLE devHandle, CDisplayView* ptr)
	: m_pImageConvertHelper(NULL)
	, m_displayThreadHandle(NULL)
	, m_bRunning(false)
	, m_iWidth(0)
	, m_iHeight(0)
	, m_iCheckReverseX(0)
	, m_iCheckReverseY(0)
	, m_iCheckCenterGrid(0)
{
	if (!m_pImageConvertHelper)
	{
		m_pImageConvertHelper = new CMvImageConvertHelper(devHandle);
	}

	if (!m_pImageConvertHelper)
	{
		TRACE("m_pImageConvertHelper is NULL!\n");
	}

	int64_t iWidthVal = 0;
	IMV_GetIntFeatureValue(devHandle, "Width", &iWidthVal);
	m_iWidth = (unsigned int)iWidthVal;

	int64_t iHeightVal = 0;
	IMV_GetIntFeatureValue(devHandle, "Height", &iHeightVal);
	m_iWidth = (unsigned int)iWidthVal;
	m_iHeight = (unsigned int)iHeightVal;


	m_pDisplayView = ptr;
	m_pDisplayView->SetPtrImgBuf((unsigned char*)m_iArrImgSrc);
	
}

CMvDisplayHelper::~CMvDisplayHelper()
{
	stopDisplay();

	if (m_pImageConvertHelper)
	{
		delete m_pImageConvertHelper;
		m_pImageConvertHelper = NULL;
	}
}

bool CMvDisplayHelper::isValid()
{
	return (NULL == m_pImageConvertHelper) ? false : true;
}

bool CMvDisplayHelper::startDisplay(void* pHandle, int iDispalyRate)
{
	bool bOpenRet = m_pImageConvertHelper->startConvert(iDispalyRate);
	if (!bOpenRet)
	{
		return false;
	}

	m_render.setHandle(pHandle);

	bOpenRet = m_render.open();
	if (!bOpenRet)
	{
		m_pImageConvertHelper->stopConvert();

		return false;
	}

	m_bRunning = true;

	m_displayThreadHandle = (HANDLE)_beginthreadex(NULL,
		0,
		displayThread,
		this,
		CREATE_SUSPENDED,
		NULL);

	if (!m_displayThreadHandle)
	{
		TRACE("Failed to create display thread!\n");
		return false;
	}
	else
	{
		ResumeThread(m_displayThreadHandle);
	}

	return true;
}

bool CMvDisplayHelper::stopDisplay()
{
	m_bRunning = false;

	if (NULL != m_pImageConvertHelper)
	{
		m_pImageConvertHelper->stopConvert();
	}

	if (m_displayThreadHandle)
	{
		WaitForSingleObject(m_displayThreadHandle, INFINITE);
		CloseHandle(m_displayThreadHandle);
		m_displayThreadHandle = NULL;
	}

	m_render.close();

	return true;
}

void CMvDisplayHelper::displayThreadProc()
{
	while (m_bRunning)
	{
		if (NULL == m_pImageConvertHelper)
		{
			Sleep(1);
			continue;
		}

		CMvImageBuf* pConvertedImage = m_pImageConvertHelper->getConvertedImage();

		if (NULL != pConvertedImage)
		{
			unsigned char* cImageBuf = pConvertedImage->bufPtr();
			int iOrgWidth = pConvertedImage->imageWidth();
			int iOrgHeight = pConvertedImage->imageHeight();
			int iDataSize = pConvertedImage->dataSize();

			if (iOrgWidth < CAMERA_CENTERLINE_THICKNESS || iOrgHeight < CAMERA_CENTERLINE_THICKNESS)
			{
				delete pConvertedImage;
				pConvertedImage = NULL;
				cImageBuf = NULL;
				return;
			}

#ifdef GRAYMODE
			memcpy(m_iArrImgSrc, cImageBuf, sizeof(unsigned char) * iDataSize);

			if (m_iCheckReverseX == 1)
			{
				for (int i = 0; i < iOrgHeight; i++)
				{
					for (int j = 0; j < iOrgWidth; j++)
					{
						cImageBuf[i * iOrgWidth + j] = m_iArrImgSrc[i * iOrgWidth + (iOrgWidth - j - 1)];
					}
				}

				memcpy(m_iArrImgSrc, cImageBuf, sizeof(unsigned char) * iDataSize);
			}

			if (m_iCheckReverseY == 1)
			{
				for (int i = 0; i < iOrgHeight; i++)
				{
					for (int j = 0; j < iOrgWidth; j++)
					{
						cImageBuf[i * iOrgWidth + j] = m_iArrImgSrc[(iOrgHeight - i - 1) * iOrgWidth + j];
					}
				}

				memcpy(m_iArrImgSrc, cImageBuf, sizeof(unsigned char) * iDataSize);
			}

			if (m_iCheckCenterGrid == 1)
			{
				for (int i = 0; i < iOrgHeight; i++)
				{
					m_iArrImgSrc[i * iOrgWidth + (iOrgWidth / 2)] = 0;
				}

				for (int i = 0; i < iOrgWidth; i++)
				{
					m_iArrImgSrc[(iOrgHeight / 2) * iOrgWidth + i] = 0;
				}

			}
#endif
#ifdef COLORMODE
			for (int i = 0; i < iDataSize; i++)
			{
				m_iArrImgSrc[i].rgbtBlue = cImageBuf[i];
				m_iArrImgSrc[i].rgbtGreen = cImageBuf[i];
				m_iArrImgSrc[i].rgbtRed = cImageBuf[i];
			}

			if (m_iCheckReverseX == 1)
			{
				for (int i = 0; i < iOrgHeight; i++)
				{
					for (int j = 0; j < iOrgWidth; j++)
					{
						// RGB 중 하나만 연산 (GRAY를 COLOR로 변환하면 RGB값이 모두 동일하기 때문)
						cImageBuf[i * iOrgWidth + j] = m_iArrImgSrc[i * iOrgWidth + (iOrgWidth - j - 1)].rgbtBlue;
					}
				}

				for (int i = 0; i < iDataSize; i++)
				{
					m_iArrImgSrc[i].rgbtBlue = cImageBuf[i];
					m_iArrImgSrc[i].rgbtGreen = cImageBuf[i];
					m_iArrImgSrc[i].rgbtRed = cImageBuf[i];
				}
			}

			if (m_iCheckReverseY == 1)
			{
				for (int i = 0; i < iOrgHeight; i++)
				{
					for (int j = 0; j < iOrgWidth; j++)
					{
						// RGB 중 하나만 연산 (GRAY를 COLOR로 변환하면 RGB값이 모두 동일하기 때문)
						cImageBuf[i * iOrgWidth + j] = m_iArrImgSrc[(iOrgHeight - i - 1) * iOrgWidth + j].rgbtBlue;
					}
				}

				for (int i = 0; i < iDataSize; i++)
				{
					m_iArrImgSrc[i].rgbtBlue = cImageBuf[i];
					m_iArrImgSrc[i].rgbtGreen = cImageBuf[i];
					m_iArrImgSrc[i].rgbtRed = cImageBuf[i];
				}
			}

			if (m_iCheckCenterGrid == 1)
			{
				for (int i = 0; i < iOrgHeight; i++)
				{
					for (int j = 0; j < (CAMERA_CENTERLINE_THICKNESS >> 1); j++)
					{
						m_iArrImgSrc[i * iOrgWidth + (iOrgWidth >> 1) + j].rgbtBlue = 0;
						m_iArrImgSrc[i * iOrgWidth + (iOrgWidth >> 1) + j].rgbtGreen = 0;
						m_iArrImgSrc[i * iOrgWidth + (iOrgWidth >> 1) + j].rgbtRed = 255;
						m_iArrImgSrc[i * iOrgWidth + (iOrgWidth >> 1) - (j + 1)].rgbtBlue = 0;
						m_iArrImgSrc[i * iOrgWidth + (iOrgWidth >> 1) - (j + 1)].rgbtGreen = 0;
						m_iArrImgSrc[i * iOrgWidth + (iOrgWidth >> 1) - (j + 1)].rgbtRed = 255;

					}
				}

				for (int i = 0; i < iOrgWidth; i++)
				{
					for (int j = 0; j < (CAMERA_CENTERLINE_THICKNESS >> 1); j++)
					{
						m_iArrImgSrc[((iOrgHeight >> 1) + j) * iOrgWidth + i].rgbtBlue = 0;
						m_iArrImgSrc[((iOrgHeight >> 1) + j) * iOrgWidth + i].rgbtGreen = 0;
						m_iArrImgSrc[((iOrgHeight >> 1) + j) * iOrgWidth + i].rgbtRed = 255;
						m_iArrImgSrc[((iOrgHeight >> 1) - (j + 1)) * iOrgWidth + i].rgbtBlue = 0;
						m_iArrImgSrc[((iOrgHeight >> 1) - (j + 1)) * iOrgWidth + i].rgbtGreen = 0;
						m_iArrImgSrc[((iOrgHeight >> 1) - (j + 1)) * iOrgWidth + i].rgbtRed = 255;
					}
				}
			}
#endif
			/*m_render.display
			(
				m_iArrImgSrc,
				(int)iWidth,
				(int)iHeight,
				pConvertedImage->imagePixelFormat()
			);*/

			m_pDisplayView->Invalidate(FALSE);
			//m_pDisplayView->RedrawWindow();

			delete pConvertedImage;
			pConvertedImage = NULL;
			cImageBuf = NULL;
		}
	}
	m_pImageConvertHelper->clearConvertedImage();
}


CMvImageConvertHelper* CMvDisplayHelper::getCMvImageConvertHelper()
{
	return m_pImageConvertHelper;
}

unsigned char* CMvDisplayHelper::getPtrArrImgSrc()
{
	return (unsigned char*)m_iArrImgSrc;
}