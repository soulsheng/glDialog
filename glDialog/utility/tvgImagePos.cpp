
#include "StdAfx.h"
//#include "../vgStableHeaders.h"
#include "tvgImagePos.h"


namespace vgFoundationSystem {
	
// 在mfc中可能会有默认的max定义宏.
#undef  max
#undef  min
/*
	const ImagePos ImagePos::ZERO (0);

	const ImagePos ImagePos::MAX (
		std::numeric_limits<Integer>::max() ,
		std::numeric_limits<Integer>::max() );

	const ImagePos ImagePos::MIN (
		std::numeric_limits<Integer>::min() ,
		std::numeric_limits<Integer>::min() );
	*/
	
	//----------------------------------------------------------------
	String ImagePos::getDetails() const
	{
		std::ostringstream o;
		o << "ImagePos( " << x <<  " , " << y << " )";

		return o.str();
	}
	//----------------------------------------------------------------


}// end of namespace vgFoundationSystem
