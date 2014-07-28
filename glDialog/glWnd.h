#pragma once


// glWnd

class glWnd : public CWnd
{
	DECLARE_DYNAMIC(glWnd)

public:
	glWnd();
	virtual ~glWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};


