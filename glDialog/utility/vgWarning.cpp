

#include "StdAfx.h"
#include "vgStableHeaders.h"
#include "vgWarning.h"


namespace vgFoundationSystem {

	//----------------------------------------------------------------
	Warning::Warning(int num, const String& desc, const String& src) 
		:line( 0 ),
		number( num ),
		description( desc ),
		source( src )
	{
		// Log this error - not any more, allow catchers to do it
		LogManager::getSingleton().logMessage(this->getFullDescription());
	}
	//----------------------------------------------------------------
	Warning::Warning(int num, const String& desc, const String& src, 
		const char* typ, const char* fil, long lin) :
	line( lin ),
		number( num ),
		typeName(typ),
		description( desc ),
		source( src ),
		file( fil )
	{
		// Log this error, mask it from debug though since it may be caught and ignored
		if(LogManager::getSingletonPtr())
		{
			LogManager::getSingleton().logMessage(
				this->getFullDescription(), 
				LML_CRITICAL, true);
		}

	}
	//----------------------------------------------------------------
	Warning::Warning(const Warning& rhs)
		: line( rhs.line ), number( rhs.number ), description( rhs.description ), source( rhs.source ), file( rhs.file )
	{
	}
	//----------------------------------------------------------------
	void Warning::operator = ( const Warning& rhs )
	{
		description = rhs.description;
		number = rhs.number;
		source = rhs.source;
		file = rhs.file;
		line = rhs.line;
		typeName = rhs.typeName;
	}
	//----------------------------------------------------------------
	const String& Warning::getFullDescription(void) const
	{
		if (fullDesc.empty())
		{

			std::ostringstream desc;

			desc << description ;
			desc << "\n\nDetails:\n" ;
			desc <<  "VGES WARNING(" << getNumberAsString() << ":" << typeName << "): "
				<< " in " << source;

			if( line > 0 )
			{
				desc << " at " << file << " (line " << line << ")";
			}

			fullDesc = desc.str();
		}

		return fullDesc;

	}
	//----------------------------------------------------------------
	int Warning::getNumber(void) const throw()
	{
		return number;
	}
	//----------------------------------------------------------------
	std::string Warning::getNumberAsString( void ) const
	{
		switch( number )
		{
			/// д���ļ�ʧ��.
			/// ��Ӧ: retry : cancel
		case WARN_CANNOT_WRITE_TO_FILE:
			return "WARN_CANNOT_WRITE_TO_FILE";

			/// ���Ƽ���״̬:��Ȼ����ǿ������.
			/// ��Ӧ: ok : cancel
		case WARN_INVALID_STATE:
			return "WARN_INVALID_STATE";

			/// ���Ƽ��Ĵ������:Ҳ���������.
			/// ��Ӧ: ok : cancel
		case WARN_INVALIDPARAMS:
			return "WARN_INVALIDPARAMS";

			/// û�в��ҵ������Դ:ѯ���Ƿ����Ĭ�Ϸ�ʽ������������.
			/// ��Ӧ: retry : ok : cancel
		case WARN_ITEM_NOT_FOUND:
			return "WARN_ITEM_NOT_FOUND";

			/// �ļ�δ������ȡ:ѯ��ȡ�����ǲ���Ĭ�Ͽհ��ļ�.
			/// ��Ӧ: retry : ok : cancel
		case WARN_FILE_NOT_FOUND:
			return "WARN_FILE_NOT_FOUND";

			/// �ڲ�Warning:ͨ������debugģʽ.
			/// ��Ӧ: ok
		case WARN_INTERNAL_WARNING:
			return "WARN_INTERNAL_WARNING";

			/// ʵ�ֲ����:������������:Warningһ��.
			/// ��Ӧ: ok
		case WARN_IMPLEMENT_ILL:	
			return "WARN_IMPLEMENT_ILL";

		default:
			return "WARN_UNKOWN";
		}
	}
	//----------------------------------------------------------------
	int Warning::getWarningReturnFromWin32Btn( const int& val ) const
	{
		switch( val )
		{
		case IDOK:
			return Warning::RETURN_OK;
		case IDCANCEL:
			return Warning::RETURN_CANCEL;
		case IDRETRY:
			return Warning::RETURN_RETRY;
		case IDYES:
			return Warning::RETURN_YES;
		case IDNO:
			return Warning::RETURN_NO;

		case IDABORT:
			return Warning::RETURN_ABORT;

#if _MSC_VER > 1200	
		case IDIGNORE:
			return Warning::RETURN_IGNORE;
		case IDCONTINUE:
			return Warning::RETURN_CONTINUE;
		case IDTRYAGAIN:
			return Warning::RETURN_RETRY;
#endif

		default:
			return Warning::RETURN_CANCEL;
		}
	}
	//----------------------------------------------------------------

}// end of namespace vgFoundationSystem

