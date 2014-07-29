
#include "StdAfx.h"
#include "../vgStableHeaders.h"
#include "tvgDdsImage.h"
#include "../tvgOpenGLSupport.h"

namespace vgFoundationSystem {
	
	//----------------------------------------------------------------
	bool	DdsImage::loadFromArchiveFile( const String& filename , 
		const ImageFormat& imgformat /*= IMAGE_FORMAT_UNKNOWN*/ )
	{
		return this->loadFromStream( 
			ArchiveManager::getSingleton().open( filename ) ,filename , imgformat );
	}
	//----------------------------------------------------------------
	uint DdsImage::createTextureID( 
		const TextureBuildOptionPtr& option /*= TextureBuildOptionPtr::DEFAULT_OPTION  */)
	{
		assert( GL_ARB_texture_compression );
		assert( mInitialised );

		assert( !option.isNull() );

		TextureBuildOptionPtr opt = option;

		if ( option == TextureBuildOptionPtr::DEFAULT_OPTION )
		{
			opt = TextureBuildOptionPtr::DEFAULT_DXT_OPTION;
		}

		GLuint ntexid;

		const TexturePixelFormat&  nFormat = this->mTexPixelFormat;
		const TextureFlag&		nFlag = this->mTextureFlag;


		// make sure there is no error msg
		OpenGLSupport::makeErrorStackEmpty();
		String errMessage;

		glGenTextures( 1, &ntexid );

		if( OpenGLSupport::checkOpenGLErrorWithoutThrow( errMessage ) )
		{
			VG_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR ,
				"glGenTextures failed! Error Code: " + errMessage ,
				"DdsImage::createTextureID()");

			return -1;
		}

		glBindTexture( GL_TEXTURE_2D, ntexid );

		 //this can be changed later.
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );



		OpenGLSupport::checkOpenGLErrorThrowException();
		
		// now begin loading mip maps.
		int nSize = 0;
		int nOffset = 0;
		unsigned int maxNumMipmaps = 0 ;
		unsigned int correctmip = opt->mMiplevel;
		Integer nNumMipMaps = this->mNumMipMaps;
		uint nWidth = this->mWidth;
		uint nHeight = this->mHeight;


		/// when mip map specified.
		if ( opt->mMiplevel != -1 )
		{
			/*

			// ���³���Ϊ��������ĳ���㡣

			maxNumMipmaps = ImageUtility::getMaxMipmapNum( nWidth , nHeight );

			// ��ʱӦ���.�������,���ʾDDS�ļ��к��в���ȫmip.
			assert( maxNumMipmaps == nNumMipMaps );
			if ( correctmip > maxNumMipmaps -1 )
			{
				correctmip = maxNumMipmaps -1 ;
			}

			/// ����ƫ����.
			for( uint i = 0; i < correctmip ; ++i )
			{ 
				if( nWidth  == 0 ) nWidth  = 1;
				if( nHeight == 0 ) nHeight = 1;

				nSize = ((nWidth+3)/4) * ((nHeight+3)/4) * nBlockSize;

				nOffset += nSize;

				// Half the image size for the next mip-map level...
				nWidth  = (nWidth  / 2);
				nHeight = (nHeight / 2);
			}

			nSize = ((nWidth+3)/4) * ((nHeight+3)/4) * nBlockSize;

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

			/// dealing with border situation.
			if ( opt->mIfHasBorder == true )
			{
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S ,GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T ,GL_CLAMP_TO_EDGE);
			}

			const bool &iborder = opt->mIfHasBorder;
			assert( iborder == 1 || iborder == 0 );

			glCompressedTexImage2DARB( GL_TEXTURE_2D,
				0,			// ʹ֮��Ϊ0��.
				this->mOpenGLFormat,
				nWidth,
				nHeight,
				iborder,
				nSize,
				this->mpPixelData + nOffset );

			OpenGLSupport::checkOpenGLErrorThrowException();

			*/

			maxNumMipmaps = ImageUtility::getMaxMipmapNum( nWidth , nHeight );

			// ��ʱӦ���.�������,���ʾDDS�ļ��к��в���ȫmip.
			assert( maxNumMipmaps == nNumMipMaps );
			if ( correctmip > maxNumMipmaps -1 )
			{
				correctmip = maxNumMipmaps -1 ;
			}

			/// ����ƫ����.

			uint i = 0;
			for( i = 0 ; i < correctmip ; ++i )
			{ 
				if( nWidth  == 0 ) nWidth  = 1;
				if( nHeight == 0 ) nHeight = 1;

				nOffset += ImageUtility::getDdsImageStorageRequirements( 
					nWidth , nHeight , this->mDdsFormat , false );

				nWidth  /= 2;
				nHeight /= 2;
			}

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

			/// ����borderʱ�����.
			if ( opt->mIfHasBorder == true )
			{
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S ,GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T ,GL_CLAMP_TO_EDGE);

				OpenGLSupport::checkOpenGLErrorThrowException();
			}

			for( i = 0; i < maxNumMipmaps - correctmip ; ++ i )
			{
				if( nWidth  == 0 ) nWidth  = 1;
				if( nHeight == 0 ) nHeight = 1;

				nSize = ImageUtility::getDdsImageStorageRequirements( 
					nWidth , nHeight , this->mDdsFormat , false );

				glCompressedTexImage2DARB( GL_TEXTURE_2D,
					i,			// ʹ֮��Ϊ0��.
					this->mOpenGLFormat,
					nWidth ,nHeight,
					opt->mIfHasBorder, nSize,
					this->mpPixelData + nOffset );

				nOffset += nSize;

				OpenGLSupport::checkOpenGLErrorThrowException();

				nWidth  /= 2;
				nHeight /= 2;
			}

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
				GL_LINEAR_MIPMAP_LINEAR);

			OpenGLSupport::checkOpenGLErrorThrowException();


		}// end of if ( texOption.miplevel != -1 )
		else 
		{
			/// load All mipmaps situation.

			///�����������Թ���.
			if ( opt->mIfTurnOnFilterAnisotropic )
			{

				if ( OpenGLSupport::checkTextureFilterAnisotropicSupport() )
				{
					OpenGLSupport::turnOnLargestTexFilterAnisotropic();
				}

				OpenGLSupport::checkOpenGLErrorThrowException();
			}

			
			/// ����borderʱ�����.
			if ( opt->mIfHasBorder == true )
			{
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S ,GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T ,GL_CLAMP_TO_EDGE);
				
				OpenGLSupport::checkOpenGLErrorThrowException();
			}


			for( int i = 0; i < nNumMipMaps; ++i )
			{
				if( nWidth  == 0 ) nWidth  = 1;
				if( nHeight == 0 ) nHeight = 1;

				OpenGLSupport::checkOpenGLErrorThrowException();

				nSize = ImageUtility::getDdsImageStorageRequirements( 
					nWidth , nHeight , this->mDdsFormat , false );

				glCompressedTexImage2DARB( GL_TEXTURE_2D,
					i,			// ʹ֮��Ϊ0��.
					this->mOpenGLFormat,
					nWidth ,nHeight,
					/*opt->mIfHasBorder*/0, nSize,
					this->mpPixelData + nOffset );

				nOffset += nSize;

				OpenGLSupport::checkOpenGLErrorThrowException();

				nWidth  /= 2;
				nHeight /= 2;
			}// end of for




			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
				GL_LINEAR_MIPMAP_LINEAR);

			OpenGLSupport::checkOpenGLErrorThrowException();
		}

		return ntexid;
	}
	//----------------------------------------------------------------
	bool DdsImage::loadFromHeaderInfo( const String& filename ,
		const ImageFormat& imgformat /*= IMAGE_FORMAT_UNKNOWN*/ )
	{
		return this->loadFromHeaderInfo( 
			ArchiveManager::getSingleton().open( filename ) ,filename , imgformat );
	}
	//----------------------------------------------------------------
	bool DdsImage::loadFromHeaderInfo( const StreamReaderPtr& pstream , 
		const String& filename ,
		const ImageFormat& imgformat /*= IMAGE_FORMAT_UNKNOWN */ )
	{
		const String& ddsfilename = filename ;

		if ( pstream.isNull() )
		{
			VG_WARN( Warning::WARN_FILE_NOT_FOUND ,
				Warning::BUTTON_NONE ,
				"Stream is empty.",
				"DdsImage::loadFromHeaderInfo");

			return false;
		}


		pstream->read( this->mDdsFileCode , 4 );

		if ( strncmp( this->mDdsFileCode , "DDS ", 4 ) != 0  )
		{
			VG_WARN(Warning::WARN_FILE_NOT_FOUND ,
				Warning::BUTTON_NONE ,
				"The file " + ddsfilename + " doesn't appear to be a valid .dds file!",
				"loadDdsImageData");

			return false;
		}

		// Get the surface descriptor
		pstream->read( &this->mDdsHeader , sizeof( this->mDdsHeader ) );

		switch( this->mDdsHeader.ddpfPixelFormat.dwFourCC )
		{
		case FOURCC_DXT1:
			{
				mTexPixelFormat = TEXTURE_PIXFORMAT_DXT1;
				mTextureFlag	= TEXTURE_FLAG_COMPRESSED;
				break;
			}
		case FOURCC_DXT3:
			{
				mTexPixelFormat = TEXTURE_PIXFORMAT_DXT3;
				mTextureFlag	= TEXTURE_FLAG_COMPRESSED;
				break;
			}
		case FOURCC_DXT5:
			{
				mTexPixelFormat = TEXTURE_PIXFORMAT_DXT5;
				mTextureFlag	= TEXTURE_FLAG_COMPRESSED;
				break;
			}
			break;

		default:

			VG_WARN(Warning::WARN_IMPLEMENT_ILL ,
				Warning::BUTTON_NONE ,
				"����DDDS��ʽ��ʱ��û��ʵ�ֶ�ȡ.The dds file  doesn't appear to be compressed using DXT1, DXT3, or DXT5!" ,
				"loadDdsImageData()");

			return false;
		}

		mFilePath = ddsfilename;
		mWidth = this->mDdsHeader.dwWidth;
		mHeight = this->mDdsHeader.dwHeight;

		return true;
	}
	//----------------------------------------------------------------
	TexturePtr DdsImage::createUnbindedTexPtr(
		const TextureBuildOptionPtr& option /*= TextureBuildOptionPtr::DEFAULT_OPTION */ )
	{
		assert( mInitialised );

		assert( !option.isNull() );

		bool useDXToption = false;

		if ( option == TextureBuildOptionPtr::DEFAULT_OPTION )
		{
			useDXToption = true;
		}

		uint id;

		if ( useDXToption == true )
		{
			id = createTextureID( TextureBuildOptionPtr::DEFAULT_DXT_OPTION );
		}
		else
		{
			id = createTextureID( option );
		}

		if ( id == -1 )
		{
			return TexturePtr::EmptyTexturePtr;
		}

		// id is correct , so create texture.
		TexturePtr ptex( new Texture() );

		ptex->makeUnbindedFromUndefined( id , 
			this->mFilePath , this->mWidth , this->mHeight , 
			this->mTexPixelFormat , this->mTextureFlag , option );

		return ptex;
	}
	//----------------------------------------------------------------
	void DdsImage::setDdsParamsDefault()
	{
		this->mWidth = 0;
		this->mHeight = 0;
		this->mOpenGLFormat = 0;
		this->mNumMipMaps = 0;
		this->mpPixelData = NULL;
		this->mInitialised = false;
		this->mFilePath = "";

		this->mTextureFlag = TEXTURE_FLAG_UNINITIALISED;
		this->mTexPixelFormat = TEXTURE_PIXFORMAT_UNKOWN;
	}
	//----------------------------------------------------------------
	bool DdsImage::loadFromStream( const StreamReaderPtr& pstream ,
		const String& stream_filename ,
		const ImageFormat& imgformat /*= IMAGE_FORMAT_UNKNOWN */ )
	{
		if ( pstream.isNull() )
		{
			return false;
		}

		pstream->read( mDdsFileCode , 4 );

		if ( strncmp( mDdsFileCode , "DDS ", 4 ) != 0  )
		{
			VG_WARN(Warning::WARN_FILE_NOT_FOUND ,
				Warning::BUTTON_NONE ,
				"When reading DDS stream ,error occurs.",
				"DdsImage::loadFromStream");

			return false;
		}

		// Get the surface descriptor
		pstream->operator >>( this->mDdsHeader );

		switch( this->mDdsHeader.ddpfPixelFormat.dwFourCC )
		{
		case FOURCC_DXT1:
			{
				// DXT1's compression ratio is 8:1
				this->mOpenGLFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				//factor = 2;
				mDdsFormat = DDS_FORMAT_DXT1;
				break;
			}
		case FOURCC_DXT3:
			{
				// DXT3's compression ratio is 4:1
				this->mOpenGLFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				//factor = 4;
				mDdsFormat = DDS_FORMAT_DXT3;
				break;
			}
		case FOURCC_DXT5:
			{
				// DXT5's compression ratio is 4:1
				this->mOpenGLFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				//factor = 4;
				mDdsFormat = DDS_FORMAT_DXT5;
				break;
			}

		default:
			{
				VG_WARN(Warning::WARN_IMPLEMENT_ILL ,
					Warning::BUTTON_NONE ,
					"����DDDS��ʽ��ʱ��û��ʵ�ֶ�ȡ.The dds file  doesn't appear to be compressed using DXT1, DXT3, or DXT5!" ,
					"loadDdsImageData()");

				return false;
			}
		}

		// ����dwLinearSizeΪ0�������
		if( this->mDdsHeader.dwLinearSize == 0 )
		{
			VG_WARN(Warning::WARN_INVALID_STATE,
				Warning::BUTTON_NONE,
				"��ȡDDS�ļ���ͷ�ļ�ʱ����Ϣ�����ϱ�׼��",
				"loadDDSTextureFile  loadDdsImageData()");

			this->mDdsHeader.dwLinearSize =
				ImageUtility::getDdsImageStorageRequirements(
				this->mDdsHeader.dwWidth , this->mDdsHeader.dwHeight , 
				this->mDdsFormat , false );
		}

		long bufferSize = ImageUtility::getDdsImageStorageRequirements(
			this->mDdsHeader.dwWidth , this->mDdsHeader.dwHeight , 
			this->mDdsFormat , true );

		if ( bufferSize == 0 )
		{
			return false;
		}

		this->mPixelDataSize = bufferSize;
		this->mpPixelData = new uchar[ bufferSize ];

		const size_t& readcount = pstream->read( this->mpPixelData , bufferSize );

		assert( readcount == bufferSize );

		switch( mOpenGLFormat )
		{
		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
			{
				mTexPixelFormat = TEXTURE_PIXFORMAT_DXT1;
				mTextureFlag	= TEXTURE_FLAG_COMPRESSED;
				break;
			}
		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
			{
				mTexPixelFormat = TEXTURE_PIXFORMAT_DXT3;
				mTextureFlag	= TEXTURE_FLAG_COMPRESSED;
				break;
			}
		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
			{
				mTexPixelFormat = TEXTURE_PIXFORMAT_DXT5;
				mTextureFlag	= TEXTURE_FLAG_COMPRESSED;
				break;
			}
		default:
			mTexPixelFormat = TEXTURE_PIXFORMAT_UNKOWN;
			assert( 0 && "δ֪��DDS��ʽ!");
			return false;
		}

		this->mFilePath	  = stream_filename;
		this->mWidth      = this->mDdsHeader.dwWidth;
		this->mHeight     = this->mDdsHeader.dwHeight;
		this->mNumMipMaps = this->mDdsHeader.dwMipMapCount;
		this->mInitialised = true;

		return true;
	}
	//----------------------------------------------------------------
}// end of namespace vgFoundationSystem
