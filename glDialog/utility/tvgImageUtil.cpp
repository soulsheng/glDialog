
#include "StdAfx.h"
//#include "../vgStableHeaders.h"
#include "tvgImageUtil.h"


namespace vgFoundationSystem {
	
	//----------------------------------------------------------------
	ImageFormat ImageUtility::findImageFormatFromExt( const String& ext )
	{
		//assert( ext != "" );
		if ( ext == "" )
		{
			return IMAGE_FORMAT_UNKNOWN;
		}
		ImageFormat type;

		if ( ext == "dds" )
		{
			type = IMAGE_FORMAT_DDS;
			return type;
		}

		if (ext == "bmp")					type = IMAGE_FORMAT_BMP;
#if CXIMAGE_SUPPORT_JPG
		else if (ext=="jpg"||ext=="jpeg")	type = IMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_GIF
		else if (ext == "gif")				type = IMAGE_FORMAT_GIF;
#endif
#if CXIMAGE_SUPPORT_PNG
		else if (ext == "png")				type = IMAGE_FORMAT_PNG;
#endif
#if CXIMAGE_SUPPORT_MNG
		else if (ext=="mng"||ext=="jng")	type = IMAGE_FORMAT_MNG;
#endif
#if CXIMAGE_SUPPORT_ICO
		else if (ext == "ico")				type = IMAGE_FORMAT_ICO;
#endif
#if CXIMAGE_SUPPORT_TIF
		else if (ext=="tiff"||ext=="tif")	type = IMAGE_FORMAT_TIF;
#endif
#if CXIMAGE_SUPPORT_TGA
		else if (ext=="tga")				type = IMAGE_FORMAT_TGA;
#endif
#if CXIMAGE_SUPPORT_PCX
		else if (ext=="pcx")				type = IMAGE_FORMAT_PCX;
#endif
#if CXIMAGE_SUPPORT_WBMP
		else if (ext=="wbmp")				type = IMAGE_FORMAT_WBMP;
#endif
#if CXIMAGE_SUPPORT_WMF
		else if (ext=="wmf"||ext=="emf")	type = IMAGE_FORMAT_WMF;
#endif
#if CXIMAGE_SUPPORT_J2K
		else if (ext=="j2k"||ext=="jp2")	type = IMAGE_FORMAT_J2K;
#endif
#if CXIMAGE_SUPPORT_JBG
		else if (ext=="jbg")				type = IMAGE_FORMAT_JBG;
#endif
#if CXIMAGE_SUPPORT_JP2
		else if (ext=="jp2"||ext=="j2k")	type = IMAGE_FORMAT_JP2;
#endif
#if CXIMAGE_SUPPORT_JPC
		else if (ext=="jpc"||ext=="j2c")	type = IMAGE_FORMAT_JPC;
#endif
#if CXIMAGE_SUPPORT_PGX
		else if (ext=="pgx")				type = IMAGE_FORMAT_PGX;
#endif
#if CXIMAGE_SUPPORT_RAS
		else if (ext=="ras")				type = IMAGE_FORMAT_RAS;
#endif
#if CXIMAGE_SUPPORT_PNM
		else if (ext=="pnm"||ext=="pgm"||ext=="ppm") type = IMAGE_FORMAT_PNM;
#endif
		else type = IMAGE_FORMAT_UNKNOWN;

		return type;
	}
	//----------------------------------------------------------------
	bool ImageUtility::getHeaderInfo( const string& filename , 
		Integer& width , Integer& height , 
		const ImageFormat& imgformat /*= IMAGE_FORMAT_UNKNOWN */ )
	{
		if ( imgformat == IMAGE_FORMAT_UNKNOWN )
		{
			const ImageFormat& iformat = findImageFormat( filename );
			if ( iformat == IMAGE_FORMAT_GIF ||
				iformat == IMAGE_FORMAT_ICO )
			{
				VG_WARN(Warning::WARN_IMPLEMENT_ILL,
					Warning::BUTTON_OK,
					"gif��ʽ����ico��ʽ��û��ʵ�ֶ�ȡͷ�ļ���Ϣ�Ĺ���." ,
					"ImageUtility::getHeaderInfo");
				return false;
			}
			return  CxImage::GetHeaderInfo( filename.c_str() , iformat ,
				width , height );
		}
		else
		{
			if ( imgformat == IMAGE_FORMAT_GIF ||
				imgformat == IMAGE_FORMAT_ICO )
			{
				VG_WARN(Warning::WARN_IMPLEMENT_ILL,
					Warning::BUTTON_OK,
					"gif��ʽ����ico��ʽ��û��ʵ�ֶ�ȡͷ�ļ���Ϣ�Ĺ���." ,
					"ImageUtility::getHeaderInfo");
				return false;
			}
			return  CxImage::GetHeaderInfo( filename.c_str() , imgformat ,
				width , height );
		}
		return false;
	}
	//----------------------------------------------------------------











}// end of namespace vgFoundationSystem
