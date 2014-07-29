

#ifndef __TVGFILESTREAMREADER_H__
#define __TVGFILESTREAMREADER_H__

#include "vgPrerequisites.h"
#include "tvgStreamReader.h"
#include "tvgStringUtil.h"

namespace vgFoundationSystem {



	class FileStreamReader : public StreamReader
	{
	protected:

#define VGES_FILESTREAM_LINE_TEMP_SIZE 300

		/// Reference to source stream
		std::ifstream* mpStream;
		bool mFreeOnClose;		

	public:

		///���캯��.ע��freeOnClose����;:��������ʱ������ɾ��ifstream
		///������һ��ifstream��ʵ��,��Ҫע�ⲻҪ����2����������.

		FileStreamReader();
		FileStreamReader(const String& filename);

		FileStreamReader(std::ifstream* s, 
			bool freeOnClose = true);

		FileStreamReader(const String& name, 
			std::ifstream* s, 
			bool freeOnClose = true);

		FileStreamReader(const String& name, 
			std::ifstream* s, 
			size_t size, 
			bool freeOnClose = true);

		~FileStreamReader();



		virtual bool create( const String& filename )
		{
			std::ifstream *filestream = 
				new std::ifstream( filename.c_str() , std::ifstream::binary);

#if _MSC_VER >= 1400	
			if ( filestream->fail() )
			{
				filestream->clear();
				// ��ʱʹ��WString���г���
				filestream->open( 
					StringUtil::convertStringToWString(filename).c_str() );

				if ( filestream->fail() )
				{
					delete filestream;
					return false;
				}
			}
#else
	#if _MSC_VER == 1200
				if ( filestream->fail() )
				{
					delete filestream;
					return false;
				}
	#else
		#error IDE is not VC6 or VC8!
	#endif
#endif

			this->create( filestream );
			return true;
		}

		inline void create(std::ifstream* s, bool freeOnClose = true )
		{
			mpStream = s;
			mFreeOnClose = freeOnClose;

			// calculate the size
			mpStream->seekg(0, std::ios_base::end);
			mSize = mpStream->tellg();
			mpStream->seekg(0, std::ios_base::beg);
		}

		inline bool fail()
		{
			return mpStream->fail();
		}

		template<typename T> 
		FileStreamReader& operator >> (T& val)
		{
			/// ע�����ﲻ����String����.��Ȼ��׼��std::fstream����.
			/// template���������ص�.
			(*this->mpStream) >> val;
			return *this;
		}





		virtual size_t read(void* buf, size_t count);

		virtual size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");

		inline String readLine(const String& delim = "\n" )
		{
			char *buf = new char[VGES_FILESTREAM_LINE_TEMP_SIZE];
			memset(buf,0,VGES_FILESTREAM_LINE_TEMP_SIZE);
			readLine( buf , VGES_FILESTREAM_LINE_TEMP_SIZE , delim );

			String res(buf);
			return res;
		}


		void skip(long count);

		void seek( size_t pos );

		size_t tell(void) const;

		bool eof(void) const;

		void close(void);

	};

	
	typedef SharedPtr<FileStreamReader> FileStreamReaderPtr;
	
	
}// end of namespace

#endif // end of __TVGFILESTREAMREADER_H__