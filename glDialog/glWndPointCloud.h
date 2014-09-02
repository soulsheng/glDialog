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
	void cleanup();

};


