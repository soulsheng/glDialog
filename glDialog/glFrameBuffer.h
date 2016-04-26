



#ifndef __VGCAFRAMEBUFFER_H__
#define __VGCAFRAMEBUFFER_H__

#include <gl/GL.h>
#include <gl/GLU.h>

#include <assert.h>

	class FrameBuffer
	{
	public:
		FrameBuffer( int width , int height );
		~FrameBuffer();


		bool create();

		void bind();

		void unbind();

		void destroy()
		{
			assert( isValid() );
			glDeleteTextures( 1, &g_dynamicTextureID );
			glDeleteTextures( 1, &g_dynamicDepthTextureID);
			glDeleteFramebuffersEXT( 1, &g_frameBuffer );
			glDeleteRenderbuffersEXT( 1, &g_depthRenderBuffer );
		}

	private:

		bool check_extension();

		bool isValid()
		{
			return m_IsValid ;
		}

		void check_buffer_status(int no = 0);

		//private:
	public:

		GLuint g_frameBuffer;
		GLuint g_depthRenderBuffer;

		GLuint g_dynamicTextureID;

		GLuint g_dynamicDepthTextureID;

		bool m_IsValid;

		int RENDERBUFFER_WIDTH ;
		int RENDERBUFFER_HEIGHT;
	};




#endif // end of __VGFRAMEBUFFER_H__