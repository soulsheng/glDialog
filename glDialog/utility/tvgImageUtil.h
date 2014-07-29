/*
-----------------------------------------------------------------------------
  VRES(Virtual Reality GIS Engine System)    
  -------------------------------------------------------------
  License details ... waiting to add...
  -------------------------------------------------------------
  Copyright  (C) 2007 , leven - All Rights Reserved
-----------------------------------------------------------------------------
*/

#ifndef __TVGIMAGEUTIL_H__
#define __TVGIMAGEUTIL_H__

#include "vgPrerequisites.h"
#include "tvgImagePos.h"
#include "tvgImageRect.h"
#include <ximage.h>
#include <squish.h>



namespace vgFoundationSystem {


	enum  ImageFormat 
	{
		IMAGE_FORMAT_UNKNOWN	= CXIMAGE_FORMAT_UNKNOWN ,
		IMAGE_FORMAT_BMP		= CXIMAGE_FORMAT_BMP ,
		IMAGE_FORMAT_JPG		= CXIMAGE_FORMAT_JPG ,
		IMAGE_FORMAT_GIF		= 3,//CXIMAGE_FORMAT_GIF ,
		IMAGE_FORMAT_PNG		= CXIMAGE_FORMAT_PNG ,
		IMAGE_FORMAT_ICO		= 5,//CXIMAGE_FORMAT_ICO ,
		IMAGE_FORMAT_TIF		= 6,//CXIMAGE_FORMAT_TIF ,
		IMAGE_FORMAT_TGA		= CXIMAGE_FORMAT_TGA ,
		IMAGE_FORMAT_PCX		= 8,//CXIMAGE_FORMAT_PCX ,
		IMAGE_FORMAT_WBMP		= 9,//CXIMAGE_FORMAT_WBMP ,
		IMAGE_FORMAT_WMF		= 10,//CXIMAGE_FORMAT_WMF ,
		IMAGE_FORMAT_JP2		= CXIMAGE_FORMAT_JP2 ,
		IMAGE_FORMAT_JPC		= 12,//CXIMAGE_FORMAT_JPC ,
		IMAGE_FORMAT_PGX		= 13,//CXIMAGE_FORMAT_PGX ,
		IMAGE_FORMAT_RAS		= 14,//CXIMAGE_FORMAT_RAS ,
		IMAGE_FORMAT_PNM		= 15,//CXIMAGE_FORMAT_PNM ,
		IMAGE_FORMAT_DDS		= 188/// DDS 文件格式.
	};

	enum  DdsFormat
	{
		DDS_FORMAT_DXT1		= 1,
		DDS_FORMAT_DXT3		= 2,
		DDS_FORMAT_DXT5		= 3,
		DDS_FORMAT_A8R8G8B8	= 4,
		DDS_FORMAT_DEFAULT	= DDS_FORMAT_DXT3
	};

	/**
		@date 	2007/05/19  9:04	
		@author  Leven
			
		@brief 	
			 Utility for ImagePos.
			 This is just a package of static functions only.
			 目前的函数很少.以后可以扩展.比如,要对一个ImagePos做绕某点旋转操作.
			 再比如,随机产生某点等等.
		@see    
	*/
	class ImageUtility
	{
	public:

		static inline ImagePos getMaximalPos( const ImagePos& p1 , const ImagePos& p2 )
		{
			Integer m1 = p1.x > p2.x ? p1.x : p2.x ;
			Integer m2 = p1.y > p2.y ? p1.y : p2.y ;
			return ImagePos( m1 , m2 );
		}

		static inline ImagePos getMinimalPos( const ImagePos& p1 , const ImagePos& p2 )
		{
			Integer m1 = p1.x < p2.x ? p1.x : p2.x ;
			Integer m2 = p1.y < p2.y ? p1.y : p2.y ;
			return ImagePos( m1 , m2 );
		}

		// 检验高度和宽度是否是2的n次方.
		static inline bool checkImageSizeIsPowerOfTwo(const uint& width,const uint& height)
		{
			unsigned int w = width;
			unsigned int h = height;
			while ( (w & 0x0001) == 0 && w != 0 )
			{
				w = w >> 1;
			}
			while ( (h & 0x0001) == 0 && h != 0 )
			{
				h = h >> 1;
			}
			if ( w == 1 && h == 1)
			{
				return true;
			}
			return false;
		}

		static inline bool checkImageSizeIsPowerOfTwo(const ImageSize& imgsize)
		{
			return checkImageSizeIsPowerOfTwo(imgsize.getWidth() ,imgsize.getHeight());
		}

		/// 得到mipmap的最大可能值.
		static inline uint getMaxMipmapNum(const uint& width,const uint& height)
		{
			assert( width != -1 && height != -1 );

			unsigned int w = width;
			unsigned int h = height;
			unsigned int result = 0;
			while ( w!=0 || h!=0 )
			{
				result++;
				w /=2;
				h /=2;
			}
			return result;
		}
		//----------------------------------------------------------------
		static inline string getStringPostfixFromNum( const Integer& num_x , const Integer& num_y ,
			const bool& reverseSequence = false )
		{
			std::ostringstream o;
			o << "_";
			if ( reverseSequence == false )
			{
				o <<  num_x << "_" << num_y << ".";
			}
			else
			{
				o <<  num_y << "_" << num_x << ".";
			}
			return o.str();
		}

		static inline Vector2 getVector2FromImagePos( const ImagePos& pos , const Real& ratio )
		{
			return Vector2( (Real)pos.x * ratio , (Real)pos.y * ratio );
		}

		//----------------------------------------------------------------
		static inline ImageFormat  findImageFormat( const String& filename )
		{
			const String& ext = findImageFileExtension( filename );
			return findImageFormatFromExt( ext );
		}
		

		/// !!!!!!!!!!!要添加caseSensity!~~!!!!!!!!!!!!!!!!!!!!!!
		static ImageFormat  findImageFormatFromExt( const String& ext );
		

		static inline String findImageFileNameWithoutExt( const String& filename )
		{
			std::string::size_type pos = filename.rfind('.');
			std::string ret = filename.substr( 0  , pos);

#if _DEBUG
			std::string::size_type ppp = ret.find_first_of("/\\");
			assert( ppp != std::string::npos );	
#endif

			return ret;	
		}

		static inline String findImageFileExtension( const String& filename )
		{
			std::string::size_type pos = filename.rfind('.');
			std::string ext = filename.substr(pos == std::string::npos ? filename.length() : pos + 1);
			transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

			ext.erase(ext.find_last_not_of(' ') + 1);
			return ext;
		}

		//----------------------------------------------------------------
		static bool getHeaderInfo(const string& filename , 
			Integer& width , Integer& height ,
			const ImageFormat& imgformat = IMAGE_FORMAT_UNKNOWN );

		//----------------------------------------------------------------
		static long getDdsImageStorageRequirements( const long& width ,
			const long& height , const DdsFormat& dds_format , 
			const bool& calculate_mipmap )
		{
			int squish_method;

			switch( dds_format )
			{
			case DDS_FORMAT_DXT1:
				{	
					squish_method = squish::kDxt1;
					break;
				}
			case DDS_FORMAT_DXT3:
				{	
					squish_method = squish::kDxt3;
					break;
				}
			case DDS_FORMAT_DXT5:
				{	
					squish_method = squish::kDxt5;
					break;
				}
			case DDS_FORMAT_A8R8G8B8:
				{
					return 0;
					break;
				}
			default:
				{
					return 0;
				}
			}

			if ( calculate_mipmap == false )
			{
				return squish::GetStorageRequirements( width , height , squish_method );
			}

			assert( calculate_mipmap == true );

			size_t result_size = 0;

			int w = width;
			int h = height;

			int nNumMipMaps = getMaxMipmapNum( w , h );

			for( int i = 0 ; i < nNumMipMaps ; ++ i )
			{
				if( w == 0 ) 
				{
					w = 1;
				}
				if( h == 0 ) 
				{
					h = 1;
				}

				result_size += squish::GetStorageRequirements( w , h , squish_method );
	
				w /= 2;
				h /= 2;
			}
			return result_size;
		}
		//----------------------------------------------------------------

	};

}

#endif // end of __TVGIMAGEUTIL_H__