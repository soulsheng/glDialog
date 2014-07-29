

#ifndef __TVGTEXTURE_H__
#define __TVGTEXTURE_H__

#include "vgPrerequisites.h"
#include "vgSharedPtr.h"
#include "tvgString.h"

namespace vgFoundationSystem {

	enum  TexturePixelFormat
	{
		TEXTURE_PIXFORMAT_UNKOWN = 0,
		TEXTURE_PIXFORMAT_RGB	,		// 注意此时R为低位.
		TEXTURE_PIXFORMAT_RGBA	,		// 注意此时A为最高位.
		TEXTURE_PIXFORMAT_DXT1	,
		TEXTURE_PIXFORMAT_DXT3	,
		TEXTURE_PIXFORMAT_DXT5	,
	};

	enum TextureFlag 
	{
		TEXTURE_FLAG_UNINITIALISED = 0,
		TEXTURE_FLAG_TEXTURE_2D = 1 << 1,
		TEXTURE_FLAG_COMPRESSED = 1 << 2,
	};

	/// this file include classes:
	class Texture;
	class TextureBuildOption;
	class TextureBuildOptionPtr;
	class TextureUtility;


	class TextureUtility
	{
	public:

		static const String getPixelFormatDescription( 
			const TexturePixelFormat& format );

		static const String getTexFlagDescription( const TextureFlag& flag );

	};


	class TextureBuildOption 
	{
	public:
		//// the unique name. If null , then auto generate names.
		//String  mUniqueName;

		// for pixel format.
		TexturePixelFormat  mTexPixelFormat;

		// for generating huge map flag 
		bool	mIfHasBorder;

		// for auto generating mipmap flag
		bool	mIfGenMipmap;

		// for loading all DDS mipmap flag 
		bool	mIfLoadMipmap;

		// for texture FilterAnisotropic
		bool	mIfTurnOnFilterAnisotropic;

		// for DDS only : select an exact mip to load , -1 means load all mipmaps.
		Integer	mMiplevel;


		TextureBuildOption()
		{
			this->setParametersDefault();
		}

		TextureBuildOption( 
			const String&  uniquename ,
			const TexturePixelFormat& format ,
			const bool&	 border , const bool& genMip ,
			const bool& loadMip , const uint& miplevel = -1 ,
			const bool& turnOnAnisotropic = true )
		{
			this->setParameters( uniquename , format , border ,
				genMip , loadMip , miplevel ,turnOnAnisotropic);
		}

		~TextureBuildOption()
		{

		}

		inline void setParameters( 
			const String&  uniquename ,
			const TexturePixelFormat& format ,
			const bool&	 border , const bool& genMip ,
			const bool& loadMip , const uint& miplevel ,
			const bool& turnOnAnisotropic )
		{
			//this->mUniqueName			= uniquename;
			this->mTexPixelFormat		= format;
			this->mIfHasBorder			= border;
			this->mIfGenMipmap			= genMip;
			this->mIfLoadMipmap			= loadMip;
			this->mMiplevel				= miplevel;
			this->mIfTurnOnFilterAnisotropic = turnOnAnisotropic;
		}

		//inline bool isDefaultOption() const
		//{
		//	return (*this) == DEFAULT_OPTION ;
		//}

		const String getDetails() const;

		bool operator == ( const TextureBuildOption& op2 ) const;

		bool operator != ( const TextureBuildOption& op2 ) const
		{
			return !( (*this) == op2 );
		}

	private:

		void setParametersDefault();

	};


	/**
		@date 	2007/05/14  15:21	
		@author  Leven
			
		@brief 	
			the pointer of TextureBuildOption.
			
		@see    
	*/
	class TextureBuildOptionPtr : public SharedPtr<TextureBuildOption>
	{
	public:
		TextureBuildOptionPtr() : SharedPtr<TextureBuildOption>() {}

		explicit TextureBuildOptionPtr(TextureBuildOption* rep)
			: SharedPtr<TextureBuildOption>(rep) {}

		TextureBuildOptionPtr(const TextureBuildOptionPtr& r)
			: SharedPtr<TextureBuildOption>(r) {} 

		static const TextureBuildOptionPtr  DEFAULT_OPTION;
		static const TextureBuildOptionPtr  DEFAULT_OPTION_RGB;
		static const TextureBuildOptionPtr  DEFAULT_DXT_OPTION;

		static const TextureBuildOptionPtr  DEFAULT_DXT_LEVEL0;
		static const TextureBuildOptionPtr  DEFAULT_DXT_LEVEL1;
		static const TextureBuildOptionPtr  DEFAULT_DXT_LEVEL2;
		static const TextureBuildOptionPtr  DEFAULT_DXT_LEVEL3;
		static const TextureBuildOptionPtr  DEFAULT_DXT_LEVEL4;
		static const TextureBuildOptionPtr  DEFAULT_DXT_LEVEL5;


		static const TextureBuildOptionPtr  DEFAULT_HUGE_MAP_DXT_OPTION;
	};


	/**
		@date 	2007/05/14  15:21	
		@author  Leven
			
		@brief 	纹理类，核心。
			
		@see    
	*/
	class Texture
	{
		friend class DdsImage;
		friend class NormalImage;
		friend class TextureBuilder;
		friend class TextureManager;

	public:
		
		static const uint	 DEFAULT_EMPTY_TEXTURE_ID;
		static const Integer DEFAULT_EMPTY_TEXTURE_WIDTH;
		static const Integer DEFAULT_EMPTY_TEXTURE_HEIGHT;

		static const uint	 DEFAULT_UNDEFINED_TEXTURE_ID;
		static const Integer DEFAULT_UNDEFINED_TEXTURE_WIDTH;
		static const Integer DEFAULT_UNDEFINED_TEXTURE_HEIGHT;

		/**
			TEXTURE_STATE_UNDEFINED	: 最初的状态.
			TEXTURE_STATE_UNBINDED : 由builder创建的没有和uniquename绑定的状态
			TEXTURE_STATE_LOADED : 完全加载,可以使用的状态.
			TEXTURE_STATE_UNLOADED : 卸载了纹理内存,但含有文件信息,可以加载的状态.
		*/
		enum TextureState
		{
			TEXTURE_STATE_EMPTY			= -1 ,
			TEXTURE_STATE_UNDEFINED		= 0 ,
			TEXTURE_STATE_UNBINDED ,
			TEXTURE_STATE_LOADED ,
			TEXTURE_STATE_UNLOADED 
		};

		Texture();

		/// used for empty texture only.
		Texture( TextureState empty );

		~Texture();

	public:

		void	release();

		inline void	useBind();

		inline bool isInitialised() const;

		inline bool isBinded() const;

		inline bool isEmptyTexture() const;

		inline bool isNotEmptyTexture() const
		{
			return !isEmptyTexture();
		}

		String getDetails() const;

	private:

		void  makeLoadedFromUnloaded( const uint& texid );

		void  makeUnloadedFromLoaded();


		void  makeUnbindedFromUndefined( const uint& texid , const String& filePath ,
			const Integer& width , const Integer& height , 
			const TexturePixelFormat& pixformat , const TextureFlag& flag ,
			const TextureBuildOptionPtr& optionPtr );


		void  makeLoadedFromUnbinded( const StringPtr& uniName );


		void  makeUnloadedFromUndefined(const StringPtr& uniName ,
			const String& filePath ,
			const Integer& width , const Integer& height , 
			const TexturePixelFormat& pixformat , const TextureFlag& flag ,
			const TextureBuildOptionPtr& optionPtr );

		void  deleteTexture();

		void  setParametersDefault();

		void setEmptyTextureParameters();


		// used for getDetails public function.
		static String getStateDescription( const TextureState& state );


	public:


		inline const TextureState getTextureState() const
		{
			return this->mTexState;
		}

		const StringPtr		getUniqueName() const 
		{ 
			return this->mpUniqueName; 
		}

		inline const String		getFilePath() const 
		{ 
			return this->mFilePath; 
		}

		inline const Integer		getWidth() const 
		{ 
			return this->mWidth; 
		}

		inline const Integer		getHeight() const 
		{ 
			return this->mHeight; 
		}

		inline const uint		getTextureID() const 
		{ 
			return this->mTextureID; 
		}

		inline const TextureFlag		getTexFlag() const 
		{ 
			return this->mTexFlag; 
		}

		inline const TexturePixelFormat		getTexPixelFormat() const 
		{ 
			return this->mTexPixelFormat; 
		}

	private:

		void		setUniqueName( const StringPtr& val ) 
		{ 
			this->mpUniqueName = val;
		}

		void		setUniqueName( String* const val ) 
		{ 
			assert( this->mpUniqueName.isEmpty() );

			this->mpUniqueName.setNull();
			this->mpUniqueName.bind( val );
		}

	private:
		
		StringPtr	mpUniqueName;
		String		mFilePath;
		Integer		mWidth;
		Integer		mHeight;
		uint		mTextureID;

		TextureBuildOptionPtr mTexBuildOptionPtr;

		TexturePixelFormat mTexPixelFormat;

		TextureFlag		 mTexFlag;
		TextureState	mTexState;
	
	};

	#include "tvgTexture.inl"


	/// the pointer of Texture.
	class TexturePtr : public SharedPtr<Texture>
	{
	public:
		TexturePtr() : SharedPtr<Texture>() {}
		explicit TexturePtr(Texture* rep) : SharedPtr<Texture>(rep) {}
		TexturePtr(const TexturePtr& r) : SharedPtr<Texture>(r) {} 
		
		inline bool isEmpty() const
		{
			if ( SharedPtr<Texture>::isNull() )
			{
				return true;
			}
			if ( SharedPtr<Texture>::operator ->()->isEmptyTexture() )
			{
				return true;
			}
			return false;
		}

		static const TexturePtr EmptyTexturePtr; 
	};
	
	
}// end of namespace 

#endif // end of __TVGTEXTURE_H__