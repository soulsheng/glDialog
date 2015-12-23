
#ifndef BOUNDING_BOX_H__
#define BOUNDING_BOX_H__

#include <stdio.h>

struct Point3
{ 
	double x,y,z;
};

class BoundingBox
{
public:

	void reductionBoundingBox( Point3 *pVertex, int nCountVertex );

	void write_box( const char*file_name );

	void read_box( const char*file_name );

	void print( );

	BoundingBox();
	Point3*	getPoints()	{ return m_MinMax; }
protected:
private:
	Point3	m_MinMax[2];
};
#endif //BOUNDING_BOX_H__