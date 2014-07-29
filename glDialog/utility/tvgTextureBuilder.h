
#ifndef __TVGTEXTUREBUILDER_H__
#define __TVGTEXTUREBUILDER_H__

#include "vgPrerequisites.h"
#include "tvgImageUtil.h"
#include "tvgImageFactory.h"
#include "tvgTexture.h"


namespace vgFoundationSystem {


	/**
		@date 	2007/04/29  9:45	
		@author  Leven
			
		@brief 	����Texture����.����Ĵ���������Ĺ�����Ӧ�ֿ�,���Ǵ��������ԭ��.
			

		@note ��Ҫʱ�������ΪSingleton,����ͳ��֮��,����ͳ��create�Ĵ���,ͳ���ڴ�����ȵ�.
			����,�����ṩ�Ľӿ���ʱֻ��TextureManager��ʹ��.
			
		@see    TextureManager
	*/
	class TextureBuilder
	{
	private:

		/// Ŀǰ����Ľӿ�ֻ��TextureManager��ʹ��.
		friend class TextureManager;

		TextureBuilder()
		{

		}
		~TextureBuilder()
		{

		}

	public:

		/**
			ȡ�ÿ�����ָ��.��ʱ�������൱�ڽ�������Factoryģʽ.
		*/
		static inline  TexturePtr getEmptyTexturePtr()
		{
			return TexturePtr::EmptyTexturePtr;
		}

	private:

		/**
			����Unbinded״̬��Texture.
				
			@param	filename ������ļ���.
			@param	format ͼ���ļ���ʽ.
			@param	option ���������ѡ��.
				
			@returns	TexturePtr ����ָ��.��������ʧ��ʱ,�᷵��ָ��������ָ��,ע�ⲻ��
				��ָ��.
		*/
		static  TexturePtr createUnbindedTexturePtr( 
			const String& filename , const ImageFormat& format ,
			const TextureBuildOptionPtr& option = TextureBuildOptionPtr::DEFAULT_OPTION );

		/**
			����Unbinded״̬��Texture.
				
			@note	Ϊ�˲����ϵķ���.ʡȥ����ĸ�ʽ.
				
			@param	filename ������ļ���.
			@param	option ���������ѡ��.
				
			@returns	TexturePtr ����ָ��.��������ʧ��ʱ,�᷵��ָ��������ָ��,ע�ⲻ��
				��ָ��.
		*/
		static  TexturePtr createUnbindedTexturePtr( const String& filename , 
			const TextureBuildOptionPtr& option  = TextureBuildOptionPtr::DEFAULT_OPTION );

		/**
			����Unbinded״̬��Texture.
				
			@note	use for huge cutted images.
				
			@param	filename ������ļ���.
			@param	option ����ѡ��.Ĭ��ΪDEFAULT_HUGE_MAP_DXT_OPTION
				
			@returns	vgFoundationSystem::TexturePtr 
		*/
		static  TexturePtr createFromHugeCuttedDdsImage( const String& filename ,
			const TextureBuildOptionPtr& option = TextureBuildOptionPtr::DEFAULT_HUGE_MAP_DXT_OPTION );

		/**
			����Texture��״̬Ϊloaded.
				
			@note	��ʱ��ͨ��TexturePtrָ����ļ���,�ṩ�Ĳ����������������ļ�.
				
			@param	ptex ����ָ��.
			@param	format ����ĸ�ʽ.
			@param	option �����ѡ��.
				
			@returns	bool �Ƿ�ɹ�����������.�ļ����Ҳ��������ڶ���Ĺ����г����˴���
				�ͻ᷵��false.
		*/
		static bool setTexturePtrLoaded( TexturePtr& ptex ,
			const bool& useSelfOption = true,
			const ImageFormat& format = IMAGE_FORMAT_UNKNOWN ,
			const TextureBuildOptionPtr& option = TextureBuildOptionPtr::DEFAULT_OPTION );
		

		/**
			����Unloaded״̬��texture.
				
			@note	��ʱ��Ҫ�ṩΨһ��id����.����Texture��״̬����,unloaded״̬����һ��Ψһid.
				
			@param	uniname Ψһ��id.
			@param	filename �ļ���.
			@param	format �ļ��ĸ�ʽ.
			@param	option �����ѡ��.
				
			@returns	TexturePtr ����ָ��.��������ʧ��ʱ,�᷵��ָ��������ָ��,ע�ⲻ��
				��ָ��.
		*/
		static  TexturePtr createUnloadedTexturePtr( const String& uniname , 
			const String& filename , const ImageFormat& format = IMAGE_FORMAT_UNKNOWN,
			const TextureBuildOptionPtr& option = TextureBuildOptionPtr::DEFAULT_OPTION );


		/**
			ʹ���Ѿ������imgPtr��ptex����
				
			@param	ptex �����ָ��.
			@param	imgPtr ͼ�����ݵ�ָ��.
				
			@returns	bool �Ƿ�ɹ���texture����.
		*/
		static  bool  setTexturePtrLoadedFromImagePtr( TexturePtr& ptex , 
			ImagePtr& imgPtr , const bool& ifSetImgNull );

	private:


		/**
			@note	used for empty texture pointer return , when error occurs.
				
			@param	file 
				
			@returns	vgFoundationSystem::TexturePtr 
		*/
		static inline  TexturePtr emptyTextureReturn( const String& file = "" )
		{
			createFileNotFoundWarning( file );
			return getEmptyTexturePtr();
		}

		/**
			@note	used for emptyTextureReturn()
				
			@param	file 
				
			@returns	void 
		*/
		static inline  void createFileNotFoundWarning( const String& file )
		{
			VG_WARN(Warning::WARN_FILE_NOT_FOUND ,
				Warning::BUTTON_NONE,
				"Image file : " + file + " is not found. ",
				"TextureBuilder::createFileNotFoundWarning()");
		}

	};
	
	
	
}// end of vgFoundationSystem

#endif // end of __TVGTEXTUREBUILDER_H__