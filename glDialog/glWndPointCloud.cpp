// glWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "glWndPointCloud.h"

#include <iostream>
#include <fstream>
using namespace std;

static const int    N                   = 64;
#define CAMERA_CONFIG_ONCE	1

// glWnd

glWndPointCloud::glWndPointCloud()
{
	Ex=(float*)calloc(N*N,sizeof(float));
}

glWndPointCloud::~glWndPointCloud()
{
	cleanup();
}


void glWndPointCloud::cleanup()
{
	free( Ex );	
}

void glWndPointCloud::openTxt( char* filename, float* pBuffer, int width, int height, int depth, float scale )
{
	ifstream inputFile( filename );
	if (!inputFile)
	{
		cout << "can not open file: " << filename << endl ;
	}
	for ( int i=0; i<height; i++ )
		for ( int j=0; j<width; j++ )
		{
			inputFile >> pBuffer[ i*N + j ];
			pBuffer[ i*N + j ] *= scale ;
		}

	inputFile.close();
}

void glWndPointCloud::initialize()
{
	openTxt("../Data/Ex.txt", Ex, N, N, 0, 50000.0f );

#if CAMERA_CONFIG_ONCE
	// set camera property by configuration file
	std::ifstream file;
	file.open("cameraPointCloud.txt");
	if (!file)
	{
		MessageBox("can not open file: ", "cameraPointCloud.txt", MB_OK );
	}
	file >> m_camera.g_eye[0]
	>> m_camera.g_eye[1]
	>> m_camera.g_eye[2]
	>> m_camera.g_Angle
		>> m_camera.g_dir[1] ;
	file.close();
#endif
}

void glWndPointCloud::render()
{
	float color3d[][3] = {
		{ 1.0f, 0.0f, 0.0f },	// red
		{ 0.0f, 1.0f, 0.0f },	// green
		{ 0.0f, 0.0f, 1.0f },	// blue
		{ 1.0f, 0.0f, 1.0f }	// purple
	};

	glPushMatrix();

	glBegin( GL_POINTS );
	for ( int i=0;i<N;i++ )
		for ( int j=0;j<N;j++ )
		{
			glColor3fv( color3d[i%4] );
			glVertex3f( i, Ex[i*N+j], j );
		}
	glEnd( );

	glPopMatrix();
}
