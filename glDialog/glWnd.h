#pragma once

#include "glWndBase.h"

// glWnd

class glWnd : public glWndBase
{
public:
	glWnd();
	virtual ~glWnd();

	virtual	void initialize();
	virtual	void render();

	// add member function and variable
	void cleanup();

};


