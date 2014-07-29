
#ifndef __TVGSTRINGUTIL_H__
#define __TVGSTRINGUTIL_H__

#include "vgPrerequisites.h"


namespace vgFoundationSystem {

	class StringUtil
	{
	public:

		inline static StringPtr createStringPtr( const String& str )
		{
			StringPtr pstr ( new String( str ) );
			return pstr;
		}

		/// ���������ҵ�����ո����Ϣɾ��.
		static void trim( String& str, bool left = true, 
			bool right = true , const String& delstr = " \t\r");

		/// ���ַ�����ΪСд.
		static void toLowerCase( String& str );

		 /// ���ַ�����Ϊ��д.
		static void toUpperCase( String& str );

		static WString convertStringToWString( const String& str )
		{
			int wcs_len = str.length() * 2;
			wchar_t *pwcs = new wchar_t[ wcs_len ];

			setlocale(LC_CTYPE, "chs");
			mbstowcs(pwcs, str.c_str(), wcs_len);

			wstring newword( pwcs );
			delete pwcs;

			return newword;
		}



		/** Method for splitting a fully qualified filename into the base name
		and path.
		@remarks
		Path is standardised as in standardisePath
		*/
		static void splitFilename(const String& qualifiedName,
			String& outBasename, String& outPath);
	


		/** Simple pattern-matching routine allowing a wildcard pattern.
		@param str String to test
		@param pattern Pattern to match against; can include simple '*' wildcards
		@param caseSensitive Whether the match is case sensitive or not
		*/
		static bool match(const String& str, const String& pattern,
			bool caseSensitive = true);

	};
	
	
	
}// end of namespace

#endif // end of __TVGSTRINGUTIL_H__