

#ifndef __TVGIMAGEFACTORY_H__
#define __TVGIMAGEFACTORY_H__

#include "vgPrerequisites.h"
#include "tvgDdsImage.h"
#include "tvgNormalImage.h"
#include "tvgArchiveManager.h"


namespace vgFoundationSystem {

	/**
		@date 	2007/04/06  13:10	
		@author  Leven
	
		@brief 	Image�Ĺ�����.���ฺ��Image�Ĳ���.

		@note ���캯������private,��������.
	
		@see    Image NormalImage DdsImage
	*/
	class ImageFactory
	{
	private:

		/**
			���캯��,��������.
		*/
		ImageFactory()
		{

		}
		/**
			��������.
		*/
		~ImageFactory()
		{

		}

	public:


		//////////////////////////////////////////////////////////////////////////
		
		static ImagePtr createFromArchiveFile( const String& filename ,
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN )
		{
			FileInfoPtr pfileinfo = ArchiveManager::getSingleton().getFileInfo( filename );

			if ( pfileinfo.isNull() )
			{
				return emptyImagePtrReturn( filename );
			}

			ImagePtr  pimg = createFromFileFormat( filename , imgformat );

			if ( pimg.isNull() )
			{
				return emptyImagePtrReturn( filename );
			}

			if( pimg->loadFromFileInfo( pfileinfo , imgformat ) )
			{
				return pimg;
			}

			return emptyImagePtrReturn( filename );
		};

		static ImagePtr createFromLoadingStream( const StreamReaderPtr& pstream ,
			const String& filename , const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN )
		{
			ImagePtr  pimg = createFromFileFormat( filename , imgformat );

			if ( pimg.isNull() )
			{
				return emptyImagePtrReturn( filename );
			}

			if( pimg->loadFromStream( pstream , filename , imgformat ) )
			{
				return pimg;
			}

			return emptyImagePtrReturn( filename );
		}


		//////////////////////////////////////////////////////////////////////////
		/**
			ͨ���ļ���ʽ������Ҫ��Image.
				
			@note	����Cximage�������dds�����ͼ��,DdsImage����ddsͼ��,Ϊ��
				����ÿ��ʹ��ʱ�ж�ͼ���ʽ,���ṩ�˽ӿ�.
				
			@param	filename ������ļ���.
			@param	imgformat �ļ���ʽ,Ĭ��ΪIMAGE_FORMAT_UNKNOWN,��ʱ����filename
				���Զ�����format
				
			@returns	ImagePtr 
		*/
		static ImagePtr createFromFileFormat( const String& filename ,
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN );

	
		/**
			ͨ����ȡͷ�ļ���Ϣ�����������ݵķ�ʽ����Image.
				
			@note	����Cximage�������dds�����ͼ��,DdsImage����ddsͼ��,Ϊ��
				����ÿ��ʹ��ʱ�ж�ͼ���ʽ,���ṩ�˽ӿ�.
				
			@param	filename ������ļ���.
			@param	imgformat �ļ���ʽ,Ĭ��ΪIMAGE_FORMAT_UNKNOWN,��ʱ����filename
				���Զ�����format
				
			@returns	ImagePtr 
		*/
		static ImagePtr createFromLoadingHeader( const String& filename , 
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN );

		/**
			ͨ�������ļ����е���Ϣ��������Ҫ��Image.
				
			@note	�������ִ��2��:1.�����ļ���ʽѡ���Ҫ��Image
				2.�����filenameִ��Image��loadFromFile���ļ�����.
				
			@param	filename ������ļ���.
			@param	imgformat �ļ���ʽ,Ĭ��ΪIMAGE_FORMAT_UNKNOWN,��ʱ����filename
				���Զ�����format
				
			@returns	ImagePtr 
		*/
		static ImagePtr createFromLoadingFile( const String& filename , 
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN );


		/**
			ͨ��һ��TexturePtr������Image.
				
			@param	ptex �����TexturePtr
			@param	checkLoaded �Ƿ���TexturePtr��״̬������.
				
			@returns	ImagePtr 
		*/
		static  ImagePtr createFromTexturePtr( const TexturePtr& ptex ,
			const bool& checkLoaded = true );

		/**
			����NormalImage
				
			@note	�˷������������dds�ļ������ͼ���ļ�.����dds�ѱ�ѹ��,
				��û�д���Ĺ���.
				
			@returns	NormalImagePtr 
		*/
		static NormalImagePtr createNormalImagePtr();
	
	private:

		/**
			˽��.���ļ��Ҳ���ʱ���ؿյ�ImagePtrָ��.
		*/
		inline static ImagePtr emptyImagePtrReturn( const String& filename )
		{
			return ImagePtr();
		}
	};
	
	
	
	
}// end of namespace vgFoundationSystem

#endif // end of __TVGIMAGEFACTORY_H__