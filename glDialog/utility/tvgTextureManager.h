

#ifndef __TVGTEXTUREMANAGER_H__
#define __TVGTEXTUREMANAGER_H__

//#include "vgPrerequisites.h"
#include "vgSingleton.h"
#include "tvgTexture.h"
#include "tvgTextureBuilder.h"
#include "tvgStringUtil.h"



namespace vgFoundationSystem {

	
	class TextureManager : public Singleton<TextureManager>
	{
	public:

		typedef		std::map< StringPtr , TexturePtr >			TexturePtrMap;
		typedef		TexturePtrMap::value_type					TexturePtrPair;
		typedef		std::pair< TexturePtrMap::iterator , bool>	TextureInsertResult;

		TextureManager()
		{

		}
		~TextureManager()
		{
			this->releaseAll();
		}

		inline bool	isExist( const String& texName ) 
		{
			/// do not use count() for it search the whole Container
			TexturePtrMap::iterator iter = 
				findIteratorByString( texName );
			return ( iter != mTexturePtrMap.end() );
		}

		inline void	releaseAll()
		{
			this->mTexturePtrMap.clear();
		}

		inline bool isEmpty() const
		{
			return this->mTexturePtrMap.empty();
		}

		inline size_t getSize() const
		{
			return this->mTexturePtrMap.size();
		}



		static inline void  unloadTexturePtr( TexturePtr& ptex )
		{
			ptex->makeUnloadedFromLoaded();
		}

		/// reload all Textures that in database.
		/// The Textures that is already unloaded do not change.
		bool  unloadTexturePtr( const String& uniname );


		/// unload all Textures that exist in database.
		void unloadAll();


		/// reload all Textures that in database.
		/// The Textures that is already loaded do not change.
		void reloadAll();

		/// static function that can reload the texture exactly.
		/// the pointer of Texture is needed.
		static bool  reloadTexturePtr( TexturePtr& ptex , 
			const bool& useSelfOption = true ,
			const TextureBuildOptionPtr& pOption = TextureBuildOptionPtr::DEFAULT_OPTION )
		{
			return TextureBuilder::setTexturePtrLoaded( ptex , useSelfOption ,
				IMAGE_FORMAT_UNKNOWN , pOption );
		}

		/// member function that can reload the texture exactly.
		/// the unique name of Texture is needed.
		bool  reloadTexturePtr( const String& uniname )
		{
			TexturePtrMap::iterator iter = 
				findIteratorByString( uniname );

			if ( iter == this->mTexturePtrMap.end() )
			{
				return false;
			}

			return TextureBuilder::setTexturePtrLoaded( iter->second );
		}
		
		/// ʹ���Ѿ������imgPtr��ptex����
		static inline  bool  reloadTexturePtr( TexturePtr& ptex , ImagePtr& imgPtr ,
			const bool& ifSetImgNull = true )
		{
			return TextureBuilder::setTexturePtrLoadedFromImagePtr( ptex , imgPtr ,ifSetImgNull);
		}


		
		/// ֧����uniqueName���ļ�������.
		/// Ϊ�β������غ���?��Ϊ���˵Ļ�String���ͻ�ת����bool����.
		TextureInsertResult	addTexturePtrByName( const String& uniqueName , const String& filename ,
			const bool& autoSearch = true , const bool& emptyTexInsert = true ,
			const TextureBuildOptionPtr& option = TextureBuildOptionPtr::DEFAULT_OPTION ) ;


		/// ֧��ֻ���ļ�������,��ʱ��Ĭ���ļ���ΪuniqueName.
		TextureInsertResult 	addTexturePtr( const String& fileName , 
			const bool& autoSearch = true , const bool& emptyTexInsert = true ,
			const TextureBuildOptionPtr& option = TextureBuildOptionPtr::DEFAULT_OPTION ) 
		{
			return this->addTexturePtrByName( fileName ,fileName ,autoSearch ,
				emptyTexInsert , option );
		}

		/// ע��,������û�б�Ҫ��ɾ����TexturePtr�����ó�Unbinded?
		bool  removeTexturePtr( const String& uniqueName )
		{
			TexturePtrMap::iterator iter = 
				findIteratorByString( uniqueName );
			if ( iter == this->mTexturePtrMap.end() )
			{
				return false;
			}
			this->mTexturePtrMap.erase( iter );
			return true;
		}

		TexturePtr getTexturePtr( const String& uniqueName , 
			const bool& autoLoad = true , const bool& emptyTexInsert = true ,
			const TextureBuildOptionPtr& option = TextureBuildOptionPtr::DEFAULT_OPTION );

	
		/**
			public function. Get the texture that is unloaded( means Unloaded state exactly).

			@var uniName 
			@var filename 
			@var autoSearch 
			@var autoInsert 
			@var forceUnload 
			@var format 
			@var option 
		*/
		TexturePtr getUnloadedTexturePtr(  const String& uniName , 
			const String& filename , const bool& autoSearch = true ,
			const bool& autoInsert = true , const bool& forceUnload = false,
			const ImageFormat& format = IMAGE_FORMAT_UNKNOWN ,
			const TextureBuildOptionPtr& option = TextureBuildOptionPtr::DEFAULT_OPTION );


		/// static functions which can make singleton pattern.
		static TextureManager& getSingleton(void);
		static TextureManager* getSingletonPtr(void);


		/// get the exactly details of the texture database.
		const String  getDetails() const;

		const StringVectorPtr getNamesDetails()
		{
			//TexturePtrMap::iterator iter = this->mTexturePtrMap.begin();
			//TexturePtrMap::iterator end_iter = this->mTexturePtrMap.end();

			StringVectorPtr pResult( new StringVector );

			TexturePtrMap::iterator iter = this->mTexturePtrMap.begin();
			TexturePtrMap::iterator iter_end = this->mTexturePtrMap.end();

			for ( ; iter != iter_end ; ++iter )
			{
				pResult->push_back( *(iter->first) );
			}
			return pResult;
		}

	private:

		inline TextureInsertResult  addTexturePtr( const StringPtr& uniqueName ,
			TexturePtr& texptr , const bool& emptyInsert = true );


		inline TextureInsertResult failedInsertResultReturn()
		{
			return std::make_pair( this->mTexturePtrMap.end() , false );
		}

		inline static TexturePtr failedFindTexptrReturn() 
		{
			return TexturePtr::EmptyTexturePtr;
		}

		inline TextureInsertResult	loadImageAndInsert( const String& uniqueName , 
			const String& filename ,
			const bool& emptyTexInsert = true ,
			const TextureBuildOptionPtr& option = TextureBuildOptionPtr::DEFAULT_OPTION );


		inline TexturePtrMap::iterator findIteratorByString(
			const String& name) 
		{
			return 	this->mTexturePtrMap.find( StringUtil::createStringPtr( name ) );
		}

		inline TextureInsertResult insertBindedTexturePtr( const TexturePtr& ptex )
		{
			assert( !ptex->isEmptyTexture() );
			assert( ptex->getTextureState() == Texture::TEXTURE_STATE_LOADED 
				|| ptex->getTextureState() == Texture::TEXTURE_STATE_UNLOADED );

			return this->mTexturePtrMap.insert( 
				std::make_pair( ptex->getUniqueName(), ptex ) );
		}

		inline TextureInsertResult insertEmptyTexturePtr( const StringPtr& uniName )
		{
			return this->mTexturePtrMap.insert( 
				std::make_pair( uniName, TexturePtr::EmptyTexturePtr ) );
		}

		inline TextureInsertResult insertEmptyTexturePtr( const String& uniName )
		{
			return insertEmptyTexturePtr( StringUtil::createStringPtr( uniName ) );
		}

		static inline bool bindUniqueNameToTexture( const StringPtr& uniname ,
			TexturePtr& texptr )
		{
			if ( texptr->isEmptyTexture() )
			{
				return false;
			}
			texptr->makeLoadedFromUnbinded( uniname );
			return true;
		}

		static inline bool bindUniqueNameToTexture( const String& uniname ,
			TexturePtr& texptr )
		{
			return bindUniqueNameToTexture( StringUtil::createStringPtr( uniname ) , texptr );
		}

	private:

		TexturePtrMap	mTexturePtrMap;

	};

	#include "tvgTextureManager.inl"

	
}// end of namespace vgFoundationSystem

#endif // end of __TVGTEXTUREMANAGER_H__