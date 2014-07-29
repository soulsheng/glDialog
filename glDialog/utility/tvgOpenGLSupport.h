/*
-----------------------------------------------------------------------------
  VRES(Virtual Reality GIS Engine System)    
  -------------------------------------------------------------
  License details ... waiting to add...
  -------------------------------------------------------------
  Copyright  (C) 2007 , leven - All Rights Reserved
-----------------------------------------------------------------------------
*/

#ifndef __TVGOPENGLSUPPORT_H__
#define __TVGOPENGLSUPPORT_H__

#include "vgPrerequisites.h"


namespace vgFoundationSystem {


	/**
		@date 	2007/02/25  14:12	
		@author  leven

		@brief 	OpenGL支持类,包含查询目的机器对Opengl的支持情况.

		此类都为静态函数.

		@see    
	*/
	class OpenGLSupport
	{
	private:
		///声明为private不允许创建.
		OpenGLSupport()
		{

		}
		~OpenGLSupport()
		{

		}
	public:

		//----------------------------------------------------------------
		// 查看机器是否支持压缩纹理.
		inline static bool	checkTextureCompresstionSupport()
		{
			/// 检查显卡是否支持.
			if (!GLEW_ARB_texture_compression)
			{
				VG_WARN(Warning::WARN_INVALID_STATE ,
					Warning::BUTTON_OK ,
					"显卡不支持GLEW_ARB_texture_compression" ,
					"OpenGLSupport::checkTextureCompresstionSupport()");
				return false;
			}
			return true;
		}
		//----------------------------------------------------------------
		inline static bool checkCubeMapSupport()
		{
			if (GLEW_EXT_texture_cube_map)
			{
				return true;
			}
			else
			{
				VG_WARN(Warning::WARN_INVALID_STATE ,
					Warning::BUTTON_OK ,
					"显卡不支持GLEW_ARB_CUBE_MAP" ,
					"OpenGLSupport::checkCubeMapSupport()");

				return false;
			}
		}
		//----------------------------------------------------------------
		// 查看机器是否支持各异向性过滤.
		inline static bool  checkTextureFilterAnisotropicSupport()
		{
			if ( !GLEW_EXT_texture_filter_anisotropic )
			{
				return false;
			}
			return true;
		}
		//----------------------------------------------------------------
		inline static bool turnOnLargestTexFilterAnisotropic()
		{
			if ( !checkTextureFilterAnisotropicSupport() )
			{
				VG_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR,
					"显卡不支持各异向性过滤.GLEW_EXT_texture_filter_anisotropic is not support." ,
					"OpenGLSupport::checkTextureFilterAnisotropic()");
				return false;
			}
			float fLargest;
			glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT , &fLargest );
			glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAX_ANISOTROPY_EXT , fLargest );
			return true;
		}

		//----------------------------------------------------------------
		inline static bool checkOpenGLErrorThrowException()
		{
			const GLenum& errorCode = glGetError();
			
			if ( GL_NO_ERROR == errorCode )
			{
				return false;
			}
			else
			{
				// means there is error.
				
				const uchar* str = gluErrorString( errorCode );

				char buffer[200];

				sprintf(buffer, "Error Message : %s\n", str);

				VG_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR,
					"OpenGL API 调用错误:" + String(buffer) +
						"Error Description: " + getOpenGLErrorInfo( errorCode ),
					"OpenGLSupport::checkOpenGLError()");

				return true;
			}
		}
		//----------------------------------------------------------------
		inline static void checkOpenGLErrorThrowException( 
			const String& source , const String& file , const long& line )
		{
			const GLenum& errorCode = glGetError();

			if ( GL_NO_ERROR == errorCode )
			{
				return;
			}
			else
			{
				// means there is error.

				const uchar* str = gluErrorString( errorCode );
				char buffer[200];
				sprintf(buffer, "Error Message : %s\n", str);

				String buf(buffer);
				StringUtil::trim(buf ,false, true);

				std::ostringstream desc;

				desc <<  "- OpenGL Error: " <<  buf
					<< "\n in " << source;

				if( line > 0 )
				{
					desc << " at " << file << " (line " << line << ")\n\n\n";
				}

				desc << "Error Description: " 
					<< getOpenGLErrorInfo( errorCode );


				VG_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR,
					desc.str(),
					"OpenGLSupport::checkOpenGLError()");
			}
		}
		//----------------------------------------------------------------
		inline static bool checkOpenGLErrorWithoutThrow( String& errMsg )
		{
			const GLenum& errorCode = glGetError();
			
			if ( GL_NO_ERROR == errorCode )
			{
				return false;
			}
			else
			{
				// means there is error.

				const uchar* str = gluErrorString( errorCode );

				char buffer[200];

				sprintf(buffer, "Error Message : %s\n", str);

				errMsg = buffer ;
				errMsg += "Error Description: " + getOpenGLErrorInfo( errorCode );
				return true;
			}
		}
		//----------------------------------------------------------------
		inline static void makeErrorStackEmpty()
		{
			GLenum errorCode;

			do 
			{
				errorCode = glGetError();
			} 
			while( GL_NO_ERROR != errorCode );

			return ;
		}
		//----------------------------------------------------------------
//#if CG_PROGRAM_SUPPORT
//
//
//
//#endif // end of CG_PROGRAM_SUPPORT


	private:
		
		static const String getOpenGLErrorInfo( const GLenum& errorCode );
	};
	
	
}

#define VG_CHECK_OPENGL_ERROR() vgFoundationSystem::OpenGLSupport::checkOpenGLErrorThrowException( \
	"", __FILE__, __LINE__ );



#endif // end of __TVGOPENGLSUPPORT_H__