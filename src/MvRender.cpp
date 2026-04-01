#include "pch.h"
#include "MvRender.h"

#include <utility>
#include <windows.h>
/***********************************************************************
*
*                            constant define area
*
************************************************************************/
// defult long¡¢width
const unsigned int DEFAULT_WIDTH = 16;
const unsigned int DEFAULT_HEIGHT = 16;
const unsigned int DEFAULT_DATA_SIZE = DEFAULT_WIDTH * DEFAULT_HEIGHT * 3 / 2;


/***********************************************************************
*
*                            object function definition area
*
************************************************************************/
/// constructor function
CRender::CRender(HANDLE hWnd)
	: m_hWnd(hWnd)
	, m_vrHandle(NULL)
{

}
/// constructor function
CRender::CRender()
	: m_hWnd(NULL)
	, m_vrHandle(NULL)
{

}

/// \set handle
void CRender::setHandle(HANDLE hWnd)
{
	m_hWnd = hWnd;
}

/// constructor function
CRender::~CRender()
{
	close();
}

/// open window
bool CRender::open()
{
	if (m_vrHandle != NULL || m_hWnd == NULL)
	{
		return false;
	}

	memset(&m_vrParams, 0, sizeof(m_vrParams));
	m_vrParams.eVideoRenderMode = VR_MODE_GDI;
	m_vrParams.hWnd = (VR_HWND)m_hWnd;
	m_vrParams.nWidth = DEFAULT_WIDTH;   // When GDI is displayed, the value can be any value other than 0
	m_vrParams.nHeight = DEFAULT_HEIGHT; // When GDI is displayed, the value can be any value other than 0

	VR_ERR_E ret = VR_Open(&m_vrParams, &m_vrHandle);
	if (ret == VR_NOT_SUPPORT)
	{
		TRACE("%s cant't display RGB on this computer", __FUNCTION__);
		return false;
	}

	m_width = 0;
	m_height = 0;
	TRACE("%s open success.\n", __FUNCTION__);
	return true;
}

/// close window
bool CRender::close()
{
	if (m_vrHandle != NULL)
	{
		VR_Close(m_vrHandle);
		m_vrHandle = NULL;
	}
	return true;
}

/// if open
bool CRender::isOpen() const
{
	return NULL != m_vrHandle;
}

/// display image
bool CRender::display(unsigned char* pData, int iWidth, int iHeight, IMV_EPixelType iPixelFormat)
{
	if (pData == NULL || iWidth == 0 || iHeight == 0)
	{
		TRACE("%s image is invalid.", __FUNCTION__);
		return false;
	}

	if (isOpen() &&
		(getWidth() != iWidth || getHeight() != iHeight) &&
		(getWidth() != 0 || getHeight() != 0))
	{
		close();
	}

	if (!isOpen())
	{
		open();
	}

	if (isOpen())
	{
		setWidth(iWidth);
		setHeight(iHeight);

		VR_FRAME_S	renderParam = { 0 };
		renderParam.data[0] = pData;
		renderParam.stride[0] = iWidth;
		renderParam.nWidth = iWidth;
		renderParam.nHeight = iHeight;

		if (iPixelFormat == gvspPixelMono8)
		{
			renderParam.format = VR_PIXEL_FMT_MONO8;
		}
		else
		{
			renderParam.format = VR_PIXEL_FMT_RGB24;
		}

		if (VR_SUCCESS == VR_RenderFrame(m_vrHandle, &renderParam, NULL))
		{
			return true;
		}

		TRACE("%s render failed.", __FUNCTION__);
		return false;

	}
	else
	{
		return false;
	}

	return true;
}
