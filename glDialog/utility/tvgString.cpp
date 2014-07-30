
#include "StdAfx.h"
//#include "../vgStableHeaders.h"
#include "tvgString.h"


namespace vgFoundationSystem {
	
	const StringPtr StringPtr::EmptyStringPtr( new String() );

	const StringPtr StringPtr::EmptyTexStringPtr( new String("EMPTY_TEXTURE") );

	const StringPtr StringPtr::UndefinedTexStringPtr( new String("UNDEFIND_TEXTURE") );

	const StringPtr StringPtr::UnbindedTexStringPtr( new String("UNBINDED_TEXTURE") );
	
	const StringPtr StringPtr::EmptyBillboardStrPtr( new String("EMPTY_BILLBOARD") );
	
}// end of namespace vgFoundationSystem
