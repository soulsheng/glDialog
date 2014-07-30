

#include "StdAfx.h"
//#include "../vgStableHeaders.h"
#include "tvgTextureManager.h"


template <> 
vgFoundationSystem::TextureManager* Singleton<vgFoundationSystem::TextureManager>::ms_pSingleton = 0;

namespace vgFoundationSystem {

	//////////////////////////////////////////////////////////////////////////
	///Implement the Singleton pattern begin
	///attention:this must include 3 implements.
	///ע�ⵥ��ģʽÿ��Ҫ��д�������3����ʼ��

	TextureManager* TextureManager::getSingletonPtr(void)
	{
		return ms_pSingleton;
	}
	TextureManager& TextureManager::getSingleton(void)
	{  
		assert( ms_pSingleton );  
		return ( *ms_pSingleton );  
	}
	//----------------------------------------------------------------

	///Implement the Singleton pattern end
	//////////////////////////////////////////////////////////////////////////
	//----------------------------------------------------------------
	const String TextureManager::getDetails() const
	{
		std::stringstream result;
		result << "------------------TextureManager begin size :" << this->getSize() 
			<< "--------------------\n";

		TexturePtrMap::const_iterator iter = this->mTexturePtrMap.begin();
		TexturePtrMap::const_iterator iter_end = this->mTexturePtrMap.end();

		for( ; iter != iter_end; ++ iter)
		{
			result << "TextureUniqueName : " << *iter->first << "  \n--"
				<< iter->second->getDetails() << "\n" ;
		}

		result << "------------------TextureManager details end --------------------\n";

		return result.str();
	}
	//----------------------------------------------------------------
	TexturePtr TextureManager::getUnloadedTexturePtr( 
		const String& uniName , const String& filename ,
		const bool& autoSearch /*= true */,  
		const bool& autoInsert /*= true */, 
		const bool& forceUnload /*= false*/,  
		const ImageFormat& format /*= IMAGE_FORMAT_UNKNOWN */,  
		const TextureBuildOptionPtr& option /*= TextureBuildOptionPtr::DEFAULT_OPTION */ )
	{
		if ( autoSearch == true )
		{
			TexturePtrMap::iterator iter = 
				findIteratorByString( uniName );

			// if found
			if ( iter != this->mTexturePtrMap.end() )
			{
				if ( forceUnload == true )
				{
					iter->second->makeUnloadedFromLoaded();
				}
				return iter->second;
			}
		}// end of autoSearch

		const TexturePtr& ptex =
			TextureBuilder::createUnloadedTexturePtr( uniName ,
			filename , format , option );

		if ( ptex->isEmptyTexture() )
		{
			if ( autoInsert == true )
			{
				insertEmptyTexturePtr( uniName );
			}
			return failedFindTexptrReturn();
		}

		insertBindedTexturePtr( ptex );
		return ptex;
	}
	//----------------------------------------------------------------
	TexturePtr TextureManager::getTexturePtr( 
		const String& uniqueName , const bool& autoLoad /*= true */, 
		const bool& emptyTexInsert /*= true */,  
		const TextureBuildOptionPtr& option /*= TextureBuildOptionPtr::DEFAULT_OPTION */ )
	{
		TexturePtrMap::iterator iter = 
			findIteratorByString( uniqueName );

		if ( autoLoad == false )
		{
			if ( iter == this->mTexturePtrMap.end() )
			{
				return failedFindTexptrReturn();
			}
			return iter->second;
		}

		/// auto load

		/// search found
		if ( iter != this->mTexturePtrMap.end() )
		{
			return iter->second;
		}

		/// not found ,so insert
		TextureInsertResult result = 
			this->addTexturePtr( uniqueName , false , emptyTexInsert , option );

		// failed insert
		if ( result.second == false )
		{
			return failedFindTexptrReturn();
		}
		else
		{
			// insert ok so return first
			return result.first->second;
		}
	}
	//----------------------------------------------------------------
	TextureManager::TextureInsertResult TextureManager::addTexturePtrByName(
		const String& uniqueName , const String& filename ,  
		const bool& autoSearch /*= true */, 
		const bool& emptyTexInsert /*= true */,  
		const TextureBuildOptionPtr& option /*= TextureBuildOptionPtr::DEFAULT_OPTION */ )
	{
		/// ����Ҫ����,���Ѿ�֪�����ǵ�һ������,��������ֱ�Ӳ��뼴��.
		if ( autoSearch == false )
		{
			return loadImageAndInsert(
				uniqueName , filename , emptyTexInsert ,option );
		}

		TexturePtrMap::iterator iter = 
			findIteratorByString( uniqueName );

		// in case mMap already have the tex with the same name.
		if ( iter != this->mTexturePtrMap.end() )
		{
			return failedInsertResultReturn();
		}

		return loadImageAndInsert(
			uniqueName , filename , emptyTexInsert ,option );
	}
	//----------------------------------------------------------------
	bool TextureManager::unloadTexturePtr( const String& uniname )
	{
		TexturePtrMap::iterator iter = 
			findIteratorByString( uniname );

		if ( iter == this->mTexturePtrMap.end() )
		{
			return false;
		}
		unloadTexturePtr( iter->second );
		return true;
	}
	//----------------------------------------------------------------
	void TextureManager::unloadAll()
	{
		TexturePtrMap::iterator iter = mTexturePtrMap.begin() ;
		TexturePtrMap::iterator iter_end = mTexturePtrMap.end() ;
		for( ; iter != iter_end ; ++ iter )
		{
			iter->second->makeUnloadedFromLoaded();
		}
	}
	//----------------------------------------------------------------
	void TextureManager::reloadAll()
	{
		TexturePtrMap::iterator iter = mTexturePtrMap.begin() ;
		TexturePtrMap::iterator iter_end = mTexturePtrMap.end() ;

		for( ; iter != iter_end ; ++ iter )
		{
			TextureBuilder::setTexturePtrLoaded( iter->second );
		}
	}
	//----------------------------------------------------------------
	
	
}// end of namespace vgFoundationSystem
