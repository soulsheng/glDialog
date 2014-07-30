

#ifndef __TVGNORMALIMAGE_H__
#define __TVGNORMALIMAGE_H__

#include "vgPrerequisites.h"
#include "vgSharedPtr.h"
#include "tvgTexture.h"
#include "tvgImage.h"
#include "vgDdsDeclaration.h"
#include "tvgArchiveManager.h"

#include "tvgArchive.h"
#include "vgfColorValue.h"

//#include <acimg.h>
#include <squish.h>


namespace vgFoundationSystem {


	class NormalImage;

	/**
		@date 	2007/04/28  20:46	
		@author  Leven
			
		@brief 	ʹ������ָ�뽫CxImage��װ,��ֹ��Դй¶.
			
		@see    SharedPtr
	*/
	typedef SharedPtr<CxImage>  CximagePtr;
	
	/**
		@date 	2007/04/28  20:46	
		@author  Leven
			
		@brief 	ʹ������ָ�뽫NormalImage��װ,��ֹ��Դй¶.
			
		@see    SharedPtr
	*/
	typedef SharedPtr<NormalImage>  NormalImagePtr;

	/**
		@date 	2007/04/28  20:46	
		@author  Leven
			
		@brief 	CxImage�İ�װ��.������dds�ļ����������ͼ���ļ��Ķ�ȡ��һ����װ.
			֧�ֵĸ�ʽ�����м���:

			CXIMAGE_FORMAT_BMP
			CXIMAGE_FORMAT_UNKNOWN
			CXIMAGE_FORMAT_JPG
			CXIMAGE_FORMAT_GIF
			CXIMAGE_FORMAT_PNG
			CXIMAGE_FORMAT_ICO
			CXIMAGE_FORMAT_TIF
			CXIMAGE_FORMAT_TGA
			CXIMAGE_FORMAT_PCX
			CXIMAGE_FORMAT_WBMP
			CXIMAGE_FORMAT_WMF
			CXIMAGE_FORMAT_JP2
			CXIMAGE_FORMAT_JPC
			CXIMAGE_FORMAT_PGX
			CXIMAGE_FORMAT_RAS
			CXIMAGE_FORMAT_PNM
			
			����֧��ddsѹ����ʽ�Ĵ洢,����ѹ������ϸ��Ϣ,�μ�DdsImage

		@see    Cximage DdsImage
	*/
	class NormalImage : public Image
	{
	public:

		/// ����ˮ��дbegin
		bool buildTextureByGLEnum( GLenum target );

		/// ����ˮ��дend



		/**
			���캯��.
		*/
		NormalImage()
		{
			this->setParametersDefault();
		}

		/**
			��������.
		*/
		virtual ~NormalImage()
		{
			this->release();

			mCximagePtr.setNull();
		}

		//////////////////////////////////////////////////////////////////////////


		virtual bool loadFromHeaderInfo( const StreamReaderPtr& pstream ,
			const String& filename , 
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN );


		/// @copydoc Image::loadFromStream
		virtual bool	loadFromStream( const StreamReaderPtr& pstream , 
			const String& stream_filename ,
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN )
		{
			assert( !isValid() );	/// ������д,������.

			ImageFormat resFormat = imgformat;

			if ( resFormat == IMAGE_FORMAT_DDS )
			{
				return false;
			}
			if ( resFormat == IMAGE_FORMAT_UNKNOWN )
			{
				resFormat = ImageUtility::findImageFormat( stream_filename );
			}

			if ( resFormat == IMAGE_FORMAT_UNKNOWN )
			{
				return false;
			}

			if ( pstream.isNull() )
			{
				return false;
			}
			/*
			size_t buf_size = pstream->size();

			// ���뻺����
			byte* buf = new byte[buf_size];

			pstream->read( buf , buf_size );

			bool result = this->mCximagePtr->Decode( 
				static_cast<BYTE*>( buf ) , buf_size , imgformat );

			if ( result )
			{
				this->updateSelfInfoFromCximage();
			}

			// �ͷŻ�����
			delete[] buf;
			buf = NULL;
			*/

			//StreamReaderPtr preader( new FileStreamReader( stream_filename ) );

			CxStreamReader cxfile( pstream );

			bool result = this->mCximagePtr->Decode( &cxfile , resFormat );

			if ( result )
			{
				this->updateSelfInfoFromCximage();
				this->mFilePath = stream_filename;
				this->mInitialised = true;
			}

			return result;
		}


		virtual bool	saveToStream( const StreamWriterPtr& pstream ,
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN ,
			const DdsFormat& ddsformat = DDS_FORMAT_DEFAULT )
		{
			assert( isValid() );

			if ( pstream.isNull() )
			{
				return false;
			}

			if ( imgformat == IMAGE_FORMAT_UNKNOWN )
			{
				//iformat = ImageUtility::findImageFormat( this->mFilePath );
				return false;
			}

			if ( imgformat == IMAGE_FORMAT_DDS )
			{
				return this->saveToDdsFromCximage( pstream , ddsformat );
			}

			CxStreamWriter cxfile( pstream );

			const bool result = mCximagePtr->Encode( &cxfile , imgformat );

			return result;
		}



		virtual bool	loadFromFileInfo( const FileInfoPtr& pfileinfo , 
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN ) 
		{
			// check the file pointer
			if ( pfileinfo.isNull() )
			{
				return false;
			}

			// check the stream
			if ( pfileinfo->streamReaderPtr.isNull() )
			{
				return false;
			}

			return loadFromStream( pfileinfo->streamReaderPtr ,
				pfileinfo->basename , imgformat );

			/*

			uchar* buffer = new uchar[ buffersize ];
			pfileinfo->streamReaderPtr->read( buffer , buffersize );

			// copy from loadFromFile() begin

			assert( !isValid() );	/// ������д,������.

			if ( imgformat == IMAGE_FORMAT_DDS )
			{
				return false;
			}

			if ( imgformat == IMAGE_FORMAT_UNKNOWN )
			{
				ImageFormat t = ImageUtility::findImageFormat( filename );

				if ( t == IMAGE_FORMAT_UNKNOWN 
					|| t == IMAGE_FORMAT_DDS )
				{
					return false;
				}

				if( this->mCximagePtr->Decode( buffer , buffersize , t ) )
				{
					this->mFilePath = filename;
					this->updateSelfInfoFromCximage();
					return true;
				}
				return false;
			}
			else
			{
				if( mCximagePtr->Decode( buffer , buffersize , imgformat ) )
				{
					this->mFilePath = filename;
					this->updateSelfInfoFromCximage();
					return true;
				}
				return false;
			}

			// copy from loadFromFile() end

			// ע���ͷ�
			delete[] buffer;
			buffer = NULL;

			return true;
			*/
		}


		// ע�⣡�������ָ���WriterInfoPtr
		virtual bool    saveToWriter( WriterInfoPtr& pwriter )
		{
			assert( isValid() );
			// ��δ�����ļ�����ʱ��
			if ( pwriter->aimFilename.empty() )
			{
				return this->copyOriginalDataToWriter( pwriter );
			}

			const ImageFormat original_format = 
				ImageUtility::findImageFormat( this->mFilePath );

			const ImageFormat aim_format = 
				ImageUtility::findImageFormat( pwriter->aimFilename );

			if ( original_format == aim_format )
			{
				return this->copyOriginalDataToWriter( pwriter );
			}

			pwriter->streamWriterPtr->seek( pwriter->absoluteBeginPos );

			this->saveToStream( pwriter->streamWriterPtr , aim_format , pwriter->ddsFormat );

			pwriter->absoluteEndPos = pwriter->streamWriterPtr->tell();

			assert( pwriter->absoluteEndPos >= pwriter->absoluteBeginPos);

			pwriter->fileLength = 
				pwriter->absoluteEndPos - pwriter->absoluteBeginPos;

			pwriter->originalFilename = this->mFilePath;

			return  true;
		}
		//////////////////////////////////////////////////////////////////////////


		/// @copydoc Image::loadFromFile
		virtual bool	loadFromArchiveFile( const String& filename , 
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN );

		/// @copydoc Image::loadFromHeaderInfo
		virtual bool loadFromHeaderInfo( const String& filename ,
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN );

		/// @copydoc Image::saveToFile
		virtual bool	saveToFile( const String& filename ,
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN ,
			const DdsFormat& ddsformat = DDS_FORMAT_DEFAULT );

		/// @copydoc Image::createUnbindedTexPtr
		virtual TexturePtr createUnbindedTexPtr(
			const TextureBuildOptionPtr& option = TextureBuildOptionPtr::DEFAULT_OPTION );

		/// @copydoc Image::release
		virtual void release()
		{
			this->setParametersDefault();
		}

	protected:

		/// @copydoc Image::createTextureID
		virtual uint createTextureID( 
			const TextureBuildOptionPtr& option = TextureBuildOptionPtr::DEFAULT_OPTION );

	public:

		/// �����ǷǼ̳к���.
		/// not virtual functions

		/**
			����alphaͨ���ļ�.���ļ�Ϊ�ǵ�ͨ��,���Զ�תΪ��ɫ(����ͨ��)
				
			@note	��ʱ������״̬���Ѿ��������ļ�.����ļ�����alphaͨ��,��ʱ�Ḳ��.
				
			@param	alphafile ����alpha���ļ���
				
			@returns	bool �Ƿ�ɹ�������.
		*/
		bool  loadAlphaFile( const String& alphafile );

		/**
			�и�ͼ��.
				
			@note	һ��ִ�е�ͼ�����С����.��Rect�Ĵ�С����ͼ��Ĵ�Сʱ�����ܱ�֤��ȷ.
				����ע��ͼ�������������.
				
			@param	rect �и�ͼ��ĳ����α�ʾ.����ԭͼ����ռ��λ��.

			@param	pDstImg ��ͼ������Ŀ�ĵ�.ע��Ϊ��ʱ,�Ὣ���ݴ��뱾��.

			@param	axisLeftTop �������Ƿ������Ͻ�.
				
			@returns	bool �Ƿ�ɹ����������и�.
		*/
		bool  crop( const ImageRect& rect , NormalImagePtr pDstImg , 
			const bool& axisLeftTop = true );

		/**
			��ͼ���ϻ���������Ϣ.
				
			@note	ע��,��ʱxƫ�ƺ�yƫ�ƶ��Ƕ��������������ϽǶ���.
				
			@param	xoffset xƫ��.
			@param	yoffset yƫ��.
			@param	text Ҫд������.
			@param	color ���ֵ���ɫ.
			@param	font ���ֵ���������.
			@param	fontsize ���ֵĴ�С.
			@param	setAlpha �Ƿ�������Ϊ͸��.
				
			@returns	bool �Ƿ�ɹ���д��������.
		*/
		bool drawString( const Integer& xoffset ,
			const Integer& yoffset , const String& text ,
			const ColorValue& color , const string& font = "" , 
			const Integer& fontsize = 30 , const bool& setAlpha = false );

		/**
			��imgsrc2��ͼ�����ݵ��ڱ�ͼ��֮��.
				
			@note	������Ҳ��������֮��.����ע��,�������ݲ�֧�ֳ���ͼ���С��Rect.
				
			@param	imgsrc2 �ص����ϲ��ͼ��.
			@param	rect ����ڵײ������������.
			@param	axisLeftTop �����Ƿ������Ͻ�.
				
			@returns	void 
		*/
		void overlap( NormalImage &imgsrc2, const ImageRect& rect ,
			const bool& axisLeftTop = true );

		/**
			�Ӹ�����ͼ���С�����հ׵�ͼ��.
				
			@note	dBppָ����bit per pixel, can be 1, 4, 8, 24 
				
			@param	dwWidth ͼ��
			@param	dwHeight ͼ���
			@param	dBpp can be 1, 4, 8, 24 
				
			@returns	bool �Ƿ�ɹ�����.
		*/
		bool createFromSize( const Integer& dwWidth, 
			const Integer& dwHeight , const Integer& dBpp = 24 );

		/**
			�����ɫ.����ͼ���ȫ������ֵ��Ϊ��������ɫ.
				
			@param	bval Ŀ����ɫֵ.
		*/
		void clearColor( const uchar& bval = 0 )
		{
			assert( isValid() );
			mCximagePtr->Clear( bval );
		}

		/**
			�õ�alpha�ļ�·����Ϣ.
		*/
		const String getAlphaFilePath() const
		{
			return this->mAlphaFilePath;
		}

		CximagePtr getCximagePtr()
		{
			return this->mCximagePtr;
		}
		/**
			��������ͼ��Ĵ�С.
		*/
		bool resize( const ImageSize& imgSize );

		/**
			������������ͼ���С��Ŀ���.
		*/
		const NormalImagePtr  resizeCopy( const ImageSize& imgSize ) const;

	private:

		/**
			set all the parameters default.
		*/
		void setParametersDefault();

		/**
			ͨ����ȡCxImage��Ϣ�����������ֵ.
		*/
		void updateSelfInfoFromCximage();

		/**
			����Cximage����Ϣ����dds����.
				
			@note	cximageҪ��֤������.
				
			@param	ddsfile dds�ļ����ļ���.
			@param	dformat dds�ļ���ʽ.����μ�DdsFormat
				
			@returns	bool �Ƿ���ȷ�洢.
		*/
		bool	saveToDdsFromCximage( const String& ddsfile ,
			const DdsFormat& dformat = DDS_FORMAT_DEFAULT );


		bool	saveToDdsFromCximage( const StreamWriterPtr& pstream ,
			const DdsFormat& dformat = DDS_FORMAT_DEFAULT );




#if 0
		/**
			ʹ��A8R8G8B8�ķ�ʽ�洢dds.
				
			@note	���ʱ��dds�ļ�û�о���ѹ���洢,�������Ƽ��ķ�ʽ.�˺����ᱻ
				saveToFile�Զ�����.
				
			@param	ddsfile dds���ļ���.
				
			@returns	bool �Ƿ���ȷ��ȡ.
		*/
		bool  saveDdsUsingA8R8G8B8(const String& ddsfile);

		bool  saveDdsUsingA8R8G8B8( const StreamWriterPtr& pstream );
#endif

	private:

		//////////////////////////////////////////////////////////////////////////


		bool copyOriginalDataToWriter( WriterInfoPtr& pwriter )
		{
			if ( pwriter.isNull() )
			{
				return false;
			}
			if ( pwriter->streamWriterPtr.isNull() )
			{
				return false;
			}

			if ( isValid() == false )
			{
				return false;
			}

			if ( this->mFilePath.empty() )
			{
				return false;
			}

			const String& originfile = this->mFilePath;

			FileInfoPtr pfileinfo = ArchiveManager::getSingleton().getFileInfo( originfile );

			if ( pfileinfo.isNull() )
			{
				VG_EXCEPT(Exception::ERR_FILE_NOT_FOUND,
					"The original Image file :" + originfile + " has been lost.",
					"DdsImage::saveToWriter");

				return false;
			}

			StreamReaderPtr& preader = pfileinfo->streamReaderPtr;

			if ( preader.isNull() )
			{
				VG_EXCEPT(Exception::ERR_FILE_NOT_FOUND,
					"The original Image file :" + originfile + " has been lost.",
					"DdsImage::saveToWriter");

				return false;
			}

			size_t filesize = pfileinfo->uncompressedSize;

			char *buffer = new char[ filesize ];

			assert( buffer != NULL );

			preader->read( buffer , filesize );
			// end to read all data

			pwriter->streamWriterPtr->seek( pwriter->absoluteBeginPos );
			pwriter->streamWriterPtr->write( buffer , filesize );
			pwriter->absoluteEndPos = pwriter->streamWriterPtr->tell();

			pwriter->fileLength = filesize;
			pwriter->originalFilename = originfile;

			delete[] buffer;
			buffer = NULL;

			return true;
		}

		//////////////////////////////////////////////////////////////////////////

	private:
	public:

		/// alpha�ļ����ļ�·��.
		String		mAlphaFilePath;

		/// ָ��Cximage��ָ��.
		CximagePtr	mCximagePtr;
	};
	
	
	
	
}// end of namespace vgFoundationSystem

#endif // end of __TVGNORMALIMAGE_H__