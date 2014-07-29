

#ifndef __TVGIMAGE_H__
#define __TVGIMAGE_H__

#include "vgPrerequisites.h"
#include "vgSharedPtr.h"
#include "tvgImageUtil.h"
#include "tvgTexture.h"

#include "tvgArchive.h"
#include "tvgStreamWriter.h"

namespace vgFoundationSystem {

	/**
		@date 	2007/04/28  15:23	
		@author  Leven
			
		@brief 	ͼ����Ļ���,�������ͼ������.

		ͼ���ʽһ���Ϊ2��:Cximage��ʽ�Ļ���dds��ʽ��,��Ϊÿ�ָ�ʽ����ķ�ʽ
		��ͬ,������õķ������Ǵ���һ������,Ȼ����ImageFactory��һϵ��create����
		����ImagePtr.
			
		@see DdsImage NormalImage ImageFactory
	*/
	class Image
	{
	public:

		/// ���캯��
		Image()
		{

		}

		/// ��������,����Ϊvirtual,���ڼ̳�.
		virtual ~Image()
		{

		}
	

		/**
			�麯����ͨ��ͼ����ļ������ļ���ʽ�����ļ���

			@note	ֱ�Ӵ���DdsImage,NormalImageʵ����loadFromFile�ķ����ǲ��Ƽ���,
			�Ƽ���ImageFactory������

			@param	filename ������ļ���

			@param	imgformat ������ļ���ʽ,��ΪIMAGE_FORMAT_UNKNOWNʱ�Զ�ͨ��
			�ļ�����ȡ��ʽ��IMAGE_FORMAT_UNKNOWNΪĬ���βΡ�

			@returns	bool �Ƿ�ɹ����������ļ���
		*/
		virtual bool	loadFromArchiveFile( const String& filename , 
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN ) = 0;



		//////////////////////////////////////////////////////////////////////////

		/// @copydoc Image::loadFromStream
		virtual bool	loadFromStream( const StreamReaderPtr& pstream , 
			const String& stream_filename ,
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN ) = 0 ;


		virtual bool loadFromHeaderInfo( const StreamReaderPtr& pstream ,
			const String& filename , 
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN ) = 0 ;



		virtual bool	saveToStream( const StreamWriterPtr& pstream ,
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN ,
			const DdsFormat& ddsformat = DDS_FORMAT_DEFAULT ) = 0;

		virtual bool	loadFromFileInfo( const FileInfoPtr& pfileinfo , 
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN ) = 0 ;

		// ע�⣡�������ָ���WriterInfoPtr
		virtual bool    saveToWriter( WriterInfoPtr& pwriter ) = 0;

		//////////////////////////////////////////////////////////////////////////


		/**
			�麯����ͨ��ͼ����ļ������ļ���ʽ�����ļ�ͷ��Ϣ��

			@note	�����������ͼ���ļ�����ǰ��顣�鿴һ���ļ��Ƿ���ڣ��Լ��Ƿ�
			��ʽ��ȷ��ע�⣬���ﲢ��ֱ������������Ϣ��

			@param	filename ������ļ���

			@param	imgformat ������ļ���ʽ,��ΪIMAGE_FORMAT_UNKNOWNʱ�Զ�ͨ��
			�ļ�����ȡ��ʽ��IMAGE_FORMAT_UNKNOWNΪĬ���βΡ�

			@returns	bool �Ƿ�ɹ����������ļ�ͷ��Ϣ��
		*/
		virtual bool loadFromHeaderInfo( const String& filename ,
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN ) = 0 ;


		/**
			�麯���������ļ����洢��Ӳ�̡�
			@param	filename �ļ���.
			@param	imgformat �ļ��ĸ�ʽ���μ�ImageFormat
			@param	ddsformat �ر�Ϊdds��ʽ���衣��Ϊ��dds��ʽʱ���˲�����Ч��
			@returns	bool �Ƿ�ɹ��Ĵ洢��
		*/
		virtual bool	saveToFile( const String& filename ,
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN ,
			const DdsFormat& ddsformat = DDS_FORMAT_DEFAULT ) = 0 ;


		/**
			��������֮��,����Unbinded״̬��TexturePtr.
			@note	
			@param	option 
			@returns	vgFoundationSystem::TexturePtr 
		*/
		virtual TexturePtr createUnbindedTexPtr(
			const TextureBuildOptionPtr& option = TextureBuildOptionPtr::DEFAULT_OPTION ) = 0 ;


		/**
			�ͷ���Դ
			release the image data
			@note	
			@returns	void 
		*/
		virtual void release() = 0;

		/**
			�Ƿ��Ѿ���ʼ��
		*/
		virtual bool isValid() const
		{
			return mInitialised;
		}

		/**
			�õ��ļ��ĳ���
		*/
		virtual Integer getWidth() const
		{
			return this->mWidth;
		}
	
		/**
			�õ��ļ��Ŀ��.
		*/
		virtual Integer getHeight() const
		{
			return this->mHeight;
		}

		/**
			�õ�ͼ���ļ��Ĵ�С.
			@returns	ImageSize  ͼ��Ĵ�С.
		*/
		virtual ImageSize getImageSize() const
		{
			return ImageSize( mWidth , mHeight );
		}

		/**
			�õ�Texture�����ظ�ʽ��Ϣ
		*/
		const TexturePixelFormat		getTexPixelFormat() const 
		{ 
			return this->mTexPixelFormat; 
		}

		/**
			�õ�Texture��flag��Ϣ
		*/
		const TextureFlag		getTextureFlag() const 
		{ 
			return this->mTextureFlag; 
		}

		/**
			�õ��ļ�·��.
		*/
		const String		getFilePath() const;

		/**
			�õ���Image�ľ�����Ϣ.
		*/
		virtual const String getDetails() const;

	protected:

		/// ͨ������friend TextureBuilder,���ƴ���Texture��Ȩ������.
		friend class TextureBuilder;

		/**
			����Ĭ�ϲ����ĳ�ʼֵ.
			set all the parameters default.
		*/
		//virtual void setParametersDefault() = 0 ;

		/**
			����Texture��idֵ
			for TextureBuilder use.
			@note	ע��,�˷���ֻ�����ض�����ʹ��.
			@param	option �����ѡ��.
			@returns	uint ������opengl������id
		*/
		virtual uint createTextureID( 
			const TextureBuildOptionPtr& option = TextureBuildOptionPtr::DEFAULT_OPTION ) = 0 ;

	
	protected:

		/// Ĭ�ϵ�ͼ���С,Ϊ����.
		static const Integer DEFAULT_IMAGE_WIDTH;
		static const Integer DEFAULT_IMAGE_HEIGHT;

		/// ͼ����ļ�·��.
		String		mFilePath;

		/// ͼ��ĳ���.
		Integer		mWidth;
		Integer		mHeight;

		/// �Ƿ��Ѿ���ʼ���ı�־.
		bool		mInitialised;

		/// texture�ı�־λ.
		TextureFlag mTextureFlag;
		TexturePixelFormat mTexPixelFormat;
	
	};


	/**
		ʹ������ָ���ض���ͼ����.
	*/
	typedef SharedPtr<Image>  ImagePtr;
	
	
}

#endif // end of __TVGIMAGE_H__