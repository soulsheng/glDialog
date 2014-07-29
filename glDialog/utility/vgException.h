/*
-----------------------------------------------------------------------------
  VRES(Virtual Reality GIS Engine System)    
  -------------------------------------------------------------
  License details ... waiting to add...
  -------------------------------------------------------------
  Copyright  (C) 2007 , leven - All Rights Reserved
-----------------------------------------------------------------------------
*/

#ifndef __VGEXCEPTION_H__
#define __VGEXCEPTION_H__

#include "vgPrerequisites.h"


namespace vgFoundationSystem {
	
	/**
		@date 	2007/02/25  14:12	
		@author  leven
	
		@brief 	
	
		@see    
	*/
	class Exception : public std::exception
    {
    protected:
        long line;
		///�쳣������:ExceptionCodes
        int number;		
		String typeName;
        String description;
        String source;
        String file;
		mutable String fullDesc;
    public:
        /** 
		ERR_CANNOT_WRITE_TO_FILE		
		д���ļ�����.

		ERR_INVALID_STATE				
		����ȷ��״̬.(�����Լ�д��һ��String��,���а���char* �ַ���,������ַ���������\0��β��,
		���״̬�ǲ�Ӧ�ó��ֵ�,����˵�Ǵ����״̬.
		�ٱ���һ��Texture����һ��TextureID,�������TextureID ��ֵ��-1,��ʱ,ʵ�ʵ�ֵ���������ֵ,
		��ΪGLuintΪ�Ƿ�������,���״̬��Ȼ�Ǵ����.)

		ERR_INVALIDPARAMS				
		����ȷ�Ĳ���.(����һ���Ի����������������˷�Χ.)

		ERR_RENDERINGAPI_ERROR
		OpenGL API����.(����,��glGenTexture������glBegin��glEnd֮��ʱ,opengl�ᱨ��.��ʱ����Ӧ
		����һ��RENDERINGAPI_ERROR)

		ERR_DUPLICATE_ITEM
		��Ŀ�Ѵ���.

		ERR_ITEM_NOT_FOUND
		�������Ŀû�����������߲�����.(����,texture�������ж�ĳ��texture����ȡ,�����ܱ�֤���е�
		��Ŀ�����������ĵ�.)

		ERR_FILE_NOT_FOUND
		�ļ��Ҳ���.

		ERR_INTERNAL_ERROR
		�ڲ�����.��һ�������Ҳ�����Ӧ��ExceptionCodes,������Ȼ�����ڲ�����ʱ�����Ĵ���,��ʱ��Ϊ�ڲ�����.

		ERR_RT_ASSERTION_FAILED
		����ʧ��.����release�汾�µ�assertʵ��.

		ERR_NOT_IMPLEMENTED
		��δ���.��ģ���ĳһ������ҪΪ�Ժ��ʵ�����ռ�ʱ,���׳�δʵ���쳣.

        */
        enum ExceptionCodes {
            ERR_CANNOT_WRITE_TO_FILE,
            ERR_INVALID_STATE,
            ERR_INVALIDPARAMS,
            ERR_RENDERINGAPI_ERROR,
            ERR_DUPLICATE_ITEM,
            ERR_ITEM_NOT_FOUND,
            ERR_FILE_NOT_FOUND,
            ERR_INTERNAL_ERROR,
            ERR_RT_ASSERTION_FAILED, 
			ERR_NOT_IMPLEMENTED
        };

        /** Default constructor.
        */
        Exception( int number, const String& description, const String& source );

        /** Advanced constructor.
        */
        Exception( int number, const String& description, const String& source, const char* type, const char* file, long line );

        /** Copy constructor.
        */
        Exception(const Exception& rhs);

		/// Needed for  compatibility with std::exception
		~Exception() throw() {}

        /** Assignment operator.
        */
        void operator = (const Exception& rhs);

        /** Returns a string with the full description of this error.
            @remarks
                The description contains the error number, the description
                supplied by the thrower, what routine threw the exception,
                and will also supply extra platform-specific information
                where applicable. For example - in the case of a rendering
                library error, the description of the error will include both
                the place in which OGRE found the problem, and a text
                description from the 3D rendering library, if available.
        */
        virtual const String& getFullDescription(void) const;

        /** Gets the error code.
        */
        virtual int getNumber(void) const throw();

        /** Gets the source function.
        */
        virtual const String &getSource() const { return source; }

        /** Gets source file name.
        */
        virtual const String &getFile() const { return file; }

        /** Gets line number.
        */
        virtual long getLine() const { return line; }

		/** Returns a string with only the 'description' field of this exception. Use 
			getFullDescriptionto get a full description of the error including line number,
			error number and what function threw the exception.
        */
		virtual const String &getDescription(void) const { return description; }

		/// Override std::exception::what
		const char* what() const throw() { return getFullDescription().c_str(); }
        
    };


	/** Template struct which creates a distinct type for each exception code.
	@note
	This is useful because it allows us to create an overloaded method
	for returning different exception types by value without ambiguity. 
	From 'Modern C++ Design' (Alexandrescu 2001).
	*/
	template <int num>
	struct ExceptionCodeType
	{
		enum { number = num };
	};

	// Specialised exceptions allowing each to be caught specifically
	// backwards-compatible since exception codes still used

	class  UnimplementedException : public Exception 
	{
	public:
		UnimplementedException(int number, const String& description, const String& source, const char* file, long line)
			: Exception(number, description, source, "UnimplementedException", file, line) {}
	};
	class  FileNotFoundException : public Exception
	{
	public:
		FileNotFoundException(int number, const String& description, const String& source, const char* file, long line)
			: Exception(number, description, source, "FileNotFoundException", file, line) {}
	};
	class  IOException : public Exception
	{
	public:
		IOException(int number, const String& description, const String& source, const char* file, long line)
			: Exception(number, description, source, "IOException", file, line) {}
	};
	class  InvalidStateException : public Exception
	{
	public:
		InvalidStateException(int number, const String& description, const String& source, const char* file, long line)
			: Exception(number, description, source, "InvalidStateException", file, line) {}
	};
	class  InvalidParametersException : public Exception
	{
	public:
		InvalidParametersException(int number, const String& description, const String& source, const char* file, long line)
			: Exception(number, description, source, "InvalidParametersException", file, line) {}
	};
	class  ItemIdentityException : public Exception
	{
	public:
		ItemIdentityException(int number, const String& description, const String& source, const char* file, long line)
			: Exception(number, description, source, "ItemIdentityException", file, line) {}
	};
	class  InternalErrorException : public Exception
	{
	public:
		InternalErrorException(int number, const String& description, const String& source, const char* file, long line)
			: Exception(number, description, source, "InternalErrorException", file, line) {}
	};
	class  RenderingAPIException : public Exception
	{
	public:
		RenderingAPIException(int number, const String& description, const String& source, const char* file, long line)
			: Exception(number, description, source, "RenderingAPIException", file, line) {}
	};
	class  RuntimeAssertionException : public Exception
	{
	public:
		RuntimeAssertionException(int number, const String& description, const String& source, const char* file, long line)
			: Exception(number, description, source, "RuntimeAssertionException", file, line) {}
	};


	/** Class implementing dispatch methods in order to construct by-value
		exceptions of a derived type based just on an exception code.
	@remarks
		This nicely handles construction of derived Exceptions by value (needed
		for throwing) without suffering from ambiguity - each code is turned into
		a distinct type so that methods can be overloaded. This allows OGRE_EXCEPT
		to stay small in implementation (desirable since it is embedded) whilst
		still performing rich code-to-type mapping. 
	*/
	class ExceptionFactory
	{
	private:
		/// Private constructor, no construction
		ExceptionFactory() {}
	public:
		static UnimplementedException create(
			ExceptionCodeType<Exception::ERR_NOT_IMPLEMENTED> code, 
			const String& desc, 
			const String& src, const char* file, long line)
		{
			return UnimplementedException(code.number, desc, src, file, line);
		}
		static FileNotFoundException create(
			ExceptionCodeType<Exception::ERR_FILE_NOT_FOUND> code, 
			const String& desc, 
			const String& src, const char* file, long line)
		{
			return FileNotFoundException(code.number, desc, src, file, line);
		}
		static IOException create(
			ExceptionCodeType<Exception::ERR_CANNOT_WRITE_TO_FILE> code, 
			const String& desc, 
			const String& src, const char* file, long line)
		{
			return IOException(code.number, desc, src, file, line);
		}
		static InvalidStateException create(
			ExceptionCodeType<Exception::ERR_INVALID_STATE> code, 
			const String& desc, 
			const String& src, const char* file, long line)
		{
			return InvalidStateException(code.number, desc, src, file, line);
		}
		static InvalidParametersException create(
			ExceptionCodeType<Exception::ERR_INVALIDPARAMS> code, 
			const String& desc, 
			const String& src, const char* file, long line)
		{
			return InvalidParametersException(code.number, desc, src, file, line);
		}
		static ItemIdentityException create(
			ExceptionCodeType<Exception::ERR_ITEM_NOT_FOUND> code, 
			const String& desc, 
			const String& src, const char* file, long line)
		{
			return ItemIdentityException(code.number, desc, src, file, line);
		}
		static ItemIdentityException create(
			ExceptionCodeType<Exception::ERR_DUPLICATE_ITEM> code, 
			const String& desc, 
			const String& src, const char* file, long line)
		{
			return ItemIdentityException(code.number, desc, src, file, line);
		}
		static InternalErrorException create(
			ExceptionCodeType<Exception::ERR_INTERNAL_ERROR> code, 
			const String& desc, 
			const String& src, const char* file, long line)
		{
			return InternalErrorException(code.number, desc, src, file, line);
		}
		static RenderingAPIException create(
			ExceptionCodeType<Exception::ERR_RENDERINGAPI_ERROR> code, 
			const String& desc, 
			const String& src, const char* file, long line)
		{
			return RenderingAPIException(code.number, desc, src, file, line);
		}
		static RuntimeAssertionException create(
			ExceptionCodeType<Exception::ERR_RT_ASSERTION_FAILED> code, 
			const String& desc, 
			const String& src, const char* file, long line)
		{
			return RuntimeAssertionException(code.number, desc, src, file, line);
		}

	};
	

	

#define VG_EXCEPT(num, description, src) throw vgFoundationSystem::ExceptionFactory::create( \
	vgFoundationSystem::ExceptionCodeType<num>(), description, src, __FILE__, __LINE__ );

	
	
	
} // end of namespace vgFoundationSystem


#endif // end of __VGEXCEPTION_H__