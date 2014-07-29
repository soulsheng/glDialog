

#ifndef __TVGDDSIMAGE_H__
#define __TVGDDSIMAGE_H__

#include "vgPrerequisites.h"
#include "vgDdsDeclaration.h"
#include "vgSharedPtr.h"
#include "tvgImage.h"
#include "tvgStreamFactory.h"

namespace vgFoundationSystem {


	/**
		@date 	2007/04/28  16:38	
		@author  Leven
			
		@brief 	Ddsͼ��������.һ������dds������.
			DXT1,DXT3,DXT5��ʽ�洢�Ķ�ΪRGBAѹ������.A8R8G8B8Ϊδѹ������.

		@remarks 

			DXTC�����������ڴ����ĵ�50%�������࣬��3��DXTC�ĸ�ʽ�ɹ�ʹ�ã��ֱ���DXT1��DXT3��DXT5��

		����DXT1 ѹ��������1:8 
				ѹ������ߣ���ֻ��1Bit  1555 Alpha��Alphaͨ����Ϣ������ȫɥʧ��
				һ�㽫����Alphaͨ����ͼƬѹ�������ָ�ʽ����WW�õ�����ͼƬ��

		  ��DXT3 ѹ��������1:4
				ʹ����4Bit Alpha��������16��Alphaֵ���ɺܺõ�����alphaͨ���������Ա�ǿ�ҵİ�͸��
				�οղ��ʡ�

			DXT5 ѹ��������1:4 
				ʹ�������β�ֵ��4Bit Alpha���ر��ʺ�Alphaͨ����͵Ĳ��ʣ�����߹�������ʡ�
			
			���3D��������󲿷���Ϸ����dds��ʽ���ֳơ���ͼ�������fansͨ���޸�dds�ļ���������������ʾЧ����

			nVidia�ṩ��PhotoShopʹ��dds�Ĳ����ͨ���ò��Ҳ��������dds�ļ���


		@see    
	*/
	class DdsImage : public Image
	{
	public:

		/**
			���캯��,���а��������ĳ�ʼ��.
		*/
		DdsImage()
		{
			this->setDdsParamsDefault();
		}
		/**
			��������.�������ͷ�����.
		*/
		~DdsImage()
		{
			this->release();
		}

	public:


		//////////////////////////////////////////////////////////////////////////

		/// @copydoc Image::loadFromStream
		virtual bool	loadFromStream( const StreamReaderPtr& pstream , 
			const String& stream_filename ,
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN );

		virtual bool loadFromHeaderInfo( const StreamReaderPtr& pstream ,
			const String& filename , 
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN );

		/// @copydoc Image::saveToFile
		virtual bool	saveToStream( const StreamWriterPtr& pstream ,
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN ,
			const DdsFormat& ddsformat = DDS_FORMAT_DEFAULT )
		{
			if ( pstream.isNull() )
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

			// �������û�ʹ���������롣
			assert( ddsformat == DDS_FORMAT_DEFAULT );
			assert( imgformat == IMAGE_FORMAT_UNKNOWN );
			assert( mpPixelData != NULL );

			pstream->write( mDdsFileCode , 4 );
			pstream->write( (const char*)( &mDdsHeader ) , sizeof( mDdsHeader ) );
			pstream->write( (const char*)mpPixelData , mPixelDataSize );

			return true;
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

			return this->loadFromStream( pfileinfo->streamReaderPtr , 
				pfileinfo->basename, imgformat );
		}


		// ע�⣡�������ָ���WriterInfoPtr
		virtual bool    saveToWriter( WriterInfoPtr& pwriter )
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

			if ( pwriter->aimFilename.empty() == false )
			{
				if( ImageUtility::findImageFileExtension( pwriter->aimFilename )
					!= "dds" )
				{
					VG_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
						"Convert from the original DDS Image to " + pwriter->aimFilename + " is not supported.",
						"DdsImage::saveToWriter");

					return false;
				}
			}

			const String& originfile = this->mFilePath;

			pwriter->streamWriterPtr->seek( pwriter->absoluteBeginPos );

			if ( this->saveToStream(pwriter->streamWriterPtr) == false )
			{
				return false;
			}

			pwriter->absoluteEndPos = pwriter->streamWriterPtr->tell();

			assert( (pwriter->absoluteEndPos - pwriter->absoluteBeginPos) >= 0 );

			pwriter->fileLength = 
				 pwriter->absoluteEndPos - pwriter->absoluteBeginPos;

			pwriter->originalFilename = originfile;
			return true;

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
			const DdsFormat& ddsformat = DDS_FORMAT_DEFAULT )
		{
			return this->saveToStream( StreamFactory::createWriterFromFile( filename ) );
		}

		/// @copydoc Image::createUnbindedTexPtr
		virtual TexturePtr createUnbindedTexPtr(
			const TextureBuildOptionPtr& option = TextureBuildOptionPtr::DEFAULT_OPTION );

		/// @copydoc Image::release
		virtual void release()
		{
			if ( mpPixelData != NULL )
			{
				delete mpPixelData;
				mpPixelData = NULL;
			}
			this->setDdsParamsDefault();
		}

	private:

		/**
			����Ddsͼ���ļ��ĳ�ʼ������.
		*/
		void setDdsParamsDefault();

		/// @copydoc Image::createTextureID
		virtual uint createTextureID( 
			const TextureBuildOptionPtr& option = TextureBuildOptionPtr::DEFAULT_OPTION );


	private:

		//////////////////////////////////////////////////////////////////////////
		//bool loadFromStreamReader( const StreamReaderPtr& pStream ,
		//	const String& ddsfilename );


		//////////////////////////////////////////////////////////////////////////
	private:

		/// ���ɵ�opengl�����ظ�ʽ.
		GLenum   mOpenGLFormat;

		/// mipmap�ĸ���.
		Integer  mNumMipMaps;

		/// �洢��DDS������.
		uchar	 *mpPixelData;

		long	 mPixelDataSize;

		char  mDdsFileCode[4];
		DDSURFACEDESC2 mDdsHeader;

		DdsFormat mDdsFormat;
	};
	
	
	
}// end of namespace vgFoundationSystem

#endif // end of __TVGDDSIMAGE_H__