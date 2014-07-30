
#include "StdAfx.h"
#include "tvgArchiveManager.h"


template <> 
vgFoundationSystem::ArchiveManager* Singleton<vgFoundationSystem::ArchiveManager>::ms_pSingleton = 0;


namespace vgFoundationSystem {
	
	//----------------------------------------------------------------
	ArchiveManager* ArchiveManager::getSingletonPtr(void)
	{
		return ms_pSingleton;
	}
	ArchiveManager& ArchiveManager::getSingleton(void)
	{  
		assert( ms_pSingleton );  
		return ( *ms_pSingleton );  
	}
	//----------------------------------------------------------------
	
}// end of namespace vgFoundationSystem
