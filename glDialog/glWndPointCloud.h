#pragma once

#include "glWndBase.h"

// glWnd

class glWndPointCloud : public glWndBase
{
public:
	glWndPointCloud();
	virtual ~glWndPointCloud();

	virtual	void initialize();
	virtual	void render();

	// add member function and variable
private:
	void cleanup();
	void openTxt( char* filename, float* pBuffer, int width, int height, 
		int depth = 0, float scale = 1.0f );

private:
	float *Ex;
	double *m_pVertex;
	double *m_pColor;

	float	m_widthScene;
};


