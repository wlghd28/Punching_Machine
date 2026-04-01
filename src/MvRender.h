#ifndef __MVRENDER_H__
#define __MVRENDER_H__

#include "MvImageBuf.h"
#include "Media/VideoRender.h"

class CRender
{
public:
	/// construc function
	CRender(HANDLE hWnd);

	/// construc function
	CRender();


	~CRender();

	///set handle
	void setHandle(HANDLE hWnd);

	/// open window
	bool open();

	/// close window
	bool close();

	/// whether open
	bool isOpen() const;

	/// set width
	void setWidth(unsigned int iWidth) { m_width = iWidth; }

	/// set height
	void setHeight(unsigned int iHeight) { m_height = iHeight; }

	/// get width 
	unsigned int getWidth() { return m_width; }

	/// get height
	unsigned int getHeight() { return m_height; }

	///display image
	bool display(unsigned char* pData, int iWidth, int iHeight, IMV_EPixelType iPixelFormat);

private:
	int					m_width;			///< image width
	int					m_height;			///< image height		
	HANDLE				m_hWnd;				///< window handle
	VR_HANDLE			m_vrHandle;			///< draw inco handle
	VR_OPEN_PARAM_S		m_vrParams;			///< display parameters
};

#endif // __MVRENDER_H__
