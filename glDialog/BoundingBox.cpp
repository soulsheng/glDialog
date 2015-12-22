
#include "stdafx.h"
#include "BoundingBox.h"

void BoundingBox::reductionBoundingBox( Point3 *pVertex, int nCountVertex )
{
	Point3 *pVertexItr = pVertex;
	for(int i=0;i<nCountVertex;i++, pVertexItr++)
	{
		if( pVertex[i].x < m_MinMax[0].x )
			m_MinMax[0].x = pVertex[i].x;
		if( pVertex[i].x > m_MinMax[1].x )
			m_MinMax[1].x = pVertex[i].x;

		if( pVertex[i].y < m_MinMax[0].y )
			m_MinMax[0].y = pVertex[i].y;
		if( pVertex[i].y > m_MinMax[1].y )
			m_MinMax[1].y = pVertex[i].y;

		if( pVertex[i].z < m_MinMax[0].z )
			m_MinMax[0].z = pVertex[i].z;
		if( pVertex[i].z > m_MinMax[1].z )
			m_MinMax[1].z = pVertex[i].z;
	}
}

void BoundingBox::write_box( const char*file_name )
{
	
	FILE *fp;
	
	if((fp=fopen(file_name, "wb")) == NULL) {
		printf("请确认文件(%s)是否存在!\n", file_name);
		return;
	}

	double* pValue = (double*)m_MinMax;

	for(int i = 0; i< 6; i++)
		fprintf(fp, "%lf \n", pValue[i]);

	fclose(fp);

}

void BoundingBox::read_box( const char*file_name )
{

	FILE *fp;

	if((fp=fopen(file_name, "rb")) == NULL) {
		printf("请确认文件(%s)是否存在!\n", file_name);
		return;
	}

	double* pValue = (double*)m_MinMax;

	for(int i = 0; i< 6; i++)
		fscanf(fp, "%lf", pValue+i);

	fclose(fp);

}

BoundingBox::BoundingBox()
{
	m_MinMax[0].x = m_MinMax[0].y = m_MinMax[0].z =	 1000000.0f;
	m_MinMax[1].x = m_MinMax[1].y = m_MinMax[1].z =	-1000000.0f;

}

void BoundingBox::print()
{
	printf( "min (%lf, %lf, %lf) , \n", m_MinMax[0].x, m_MinMax[0].y, m_MinMax[0].z ); 
	printf( "max (%lf, %lf, %lf) , \n", m_MinMax[1].x, m_MinMax[1].y, m_MinMax[1].z ); 

}
