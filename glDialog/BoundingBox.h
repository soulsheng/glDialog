
#ifndef BOUNDING_BOX_H__
#define BOUNDING_BOX_H__

#include <stdio.h>

struct Point3
{ 
	double x,y,z;

	Point3& operator *= (float scale)
	{
		x*=scale; y*=scale; z*=scale;
		return *this;
	}

	friend Point3 operator + (Point3& left, Point3& right)
	{
		Point3 tmp;
		tmp.x = left.x+right.x; tmp.y = left.y+right.y; tmp.z = left.z+right.z;
		return tmp;
	}

	friend Point3 operator * (Point3& left,float scale)
	{
		Point3 tmp;
		tmp.x=left.x*scale; tmp.y=left.y*scale; tmp.z=left.z*scale;
		return tmp;
	}

	friend Point3 operator - (Point3& left, Point3& right)
	{
		Point3 tmp;
		tmp.x = left.x-right.x; tmp.y = left.y-right.y; tmp.z = left.z-right.z;
		return tmp;
	}
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