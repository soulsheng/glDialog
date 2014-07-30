// vgstruct1.h: interface for the vgstruct class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VGBOUNDRYBOX1_H__B4BE2247_AC52_43A0_9370_D059B9722EAC__INCLUDED_)
#define AFX_VGBOUNDRYBOX1_H__B4BE2247_AC52_43A0_9370_D059B9722EAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include <cassert>

#include "vgfVector3.h"


namespace vgFoundationSystem{
	
	/////////////////////////////////////////////////////////////////////////////////
	
	class vgBoundryBox
	{
	public:
		vgBoundryBox();
		vgBoundryBox(Vector3 min, Vector3 max);
		void MergeBox(vgBoundryBox box);
	public:
		Vector3 minPoint;
		Vector3 maxPoint;
		Vector3 midPoint;
	};
	
	
	////////////////////////////////////////////////////////////////////////////////
}

#endif // !defined(AFX_VGSTRUCT1_H__B4BE2247_AC52_43A0_9370_D059B9722EAC__INCLUDED_)
