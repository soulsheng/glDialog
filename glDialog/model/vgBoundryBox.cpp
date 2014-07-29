// vgstruct1.cpp: implementation of the vgstruct class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "vgBoundryBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace vgFoundationSystem{
	
	//----------------------------------------------------------------
	vgBoundryBox::vgBoundryBox()
	{
		minPoint=0;
		maxPoint=0;
		midPoint=0;
	}
	vgBoundryBox::vgBoundryBox(Vector3 min, Vector3 max)
	{
		minPoint=min;
		maxPoint=max;
		/*midPoint=Vector3((max.x + min.x) * 0.5,
		(max.y + min.y) * 0.5,
		(max.z + min.z) * 0.5);*/
		midPoint.x=(max.x + min.x) * 0.5;
		midPoint.y=(max.y + min.y) * 0.5;
		midPoint.z=(max.z + min.z) * 0.5;
	}
	
	void vgBoundryBox::MergeBox( vgBoundryBox box )
	{
		if(box.maxPoint.x > maxPoint.x || box.maxPoint.y > maxPoint.y || box.maxPoint.z > maxPoint.z)
		{
			if(box.maxPoint.x > maxPoint.x)
				maxPoint.x=box.maxPoint.x;
			if(box.maxPoint.y > maxPoint.y)
				maxPoint.y=box.maxPoint.y;
			if(box.maxPoint.z > maxPoint.z)
				maxPoint.z=box.maxPoint.z;
		}
		if(box.minPoint.x < minPoint.x || box.minPoint.x < minPoint.x || box.minPoint.x < minPoint.x)
		{
			if(box.minPoint.x < minPoint.x)
				minPoint.x=box.minPoint.x;
			if(box.minPoint.y < minPoint.y)
				minPoint.y=box.minPoint.y;
			if(box.minPoint.z < minPoint.z)
				minPoint.z=box.minPoint.z;
			
		}
		midPoint=Vector3((maxPoint.x + minPoint.x) * 0.5,
			(maxPoint.y + minPoint.y) * 0.5,
			(maxPoint.z + minPoint.z) * 0.5);
	}
}