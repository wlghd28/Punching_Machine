#pragma once

// CPictureControlEx

class CPictureControlEx : public CStatic
{
	DECLARE_DYNAMIC(CPictureControlEx)
public:
	COLORREF Transparent;

public:
	CPictureControlEx();
	virtual ~CPictureControlEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};