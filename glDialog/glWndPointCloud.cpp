// glWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "glWndPointCloud.h"
#include "read_xyzrgb.h"

#include <iostream>
#include <fstream>
using namespace std;

static const int    N                   = 64;
static const int	L					= 172666;//136710;//102824;
#define CAMERA_CONFIG_ONCE	1


#define FILE_NAME_POSITION	"../Data/1_w.txt"
#define FILE_NAME_COLOR		"../Data/1_rgb.txt"
#define FILE_NAME_BOX		"../Data/BoundingBox.txt"

// glWnd

glWndPointCloud::glWndPointCloud()
{
	Ex=(float*)calloc(N*N,sizeof(float));

	m_pVertex = new double[ L*3 ];
	m_pColor = new double[ L*3 ];
}

glWndPointCloud::~glWndPointCloud()
{
	cleanup();
}


void glWndPointCloud::cleanup()
{
	free( Ex );	
	delete[] m_pVertex;
	delete[] m_pColor;
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

	m_widthScene = N;
}

void glWndPointCloud::initialize()
{
	openTxt("../Data/Ex.txt", Ex, N, N, 0, 50000.0f );

	read_xyzrgb( FILE_NAME_POSITION, m_pVertex, L*3 );
	read_xyzrgb( FILE_NAME_COLOR, m_pColor, L*3, 0.005 );

	// upside down 
	for ( int i=0;i<L;i++ )
		m_pVertex[3*i+1] *= -1.0f ;

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
	m_box.reductionBoundingBox( (Point3*)m_pVertex, L );
	//m_box.read_box(FILE_NAME_BOX);
	m_camera.setBox( &m_box );
	m_camera.setSpeed( m_widthScene * 0.0001f );
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

#if 0
	for ( int i=0;i<N;i++ )
		for ( int j=0;j<N;j++ )
		{
			glColor3fv( color3d[i%4] );
			glVertex3f( i, Ex[i*N+j], j );
		}
#else
	for ( int i=0;i<L;i++ )
	{
		glNormal3f( 0.0f, 0.0f, -1.0f );
		glColor3dv( m_pColor+3*i );
		glVertex3dv( m_pVertex+3*i );
	}
#endif

	glEnd( );

	renderBox();

	glPopMatrix();
}

void glWndPointCloud::renderBox()
{

	Point3* pPoints = m_box.getPoints() ;

	Point3& pMin = pPoints[0];
	Point3& pMax = pPoints[1];
	
	glColor3f( 1.0f, 0.0f, 0.0f );
	glBegin( GL_LINE_LOOP );

	glVertex3f( pMin.x, pMin.y, pMin.z );
	glVertex3f( pMax.x, pMin.y, pMin.z );
	glVertex3f( pMax.x, pMin.y, pMax.z );
	glVertex3f( pMin.x, pMin.y, pMax.z );

	glEnd();

	glColor3f( 0.0f,1.0f, 0.0f );
	glBegin( GL_LINE_LOOP );

	glVertex3f( pMin.x, pMax.y, pMin.z );
	glVertex3f( pMax.x, pMax.y, pMin.z );
	glVertex3f( pMax.x, pMax.y, pMax.z );
	glVertex3f( pMin.x, pMax.y, pMax.z );

	glEnd();

	glColor3f( 1.0f,1.0f, 1.0f );

}
