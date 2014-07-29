

#ifndef __TVGSTREAMREADERFACTORY_H__
#define __TVGSTREAMREADERFACTORY_H__

#include "vgPrerequisites.h"
#include "tvgStreamReader.h"
#include "tvgFileStreamReader.h"
#include "tvgStreamWriter.h"

namespace vgFoundationSystem {


	class StreamFactory
	{
	public:

		static StreamWriterPtr createWriterFromFile( const String& filename )
		{
			StreamWriterPtr ppstream( new FileStreamWriter( filename ) );

			return ppstream;
		}

		static StreamReaderPtr createReaderFromFile( const String& filename )
		{
			//StreamReaderPtr p( new FileStreamReader );

			FileStreamReader *p = new FileStreamReader();

			assert( p != NULL );

			if( p->create( filename ) == true )
			{
				return StreamReaderPtr(p);
			}

			VG_WARN(Warning::WARN_FILE_NOT_FOUND ,
				Warning::BUTTON_NONE ,
				filename + " is not found , create StreamReaderPtr failed.",
				"StreamFactory::createReaderFromFile");
			
			//p.setNull();

			// return null smart pointer
			return StreamReaderPtr();
		}

		static FileStreamReaderPtr createFileStreamReader( const String& filename )
		{
			FileStreamReaderPtr p( new FileStreamReader );

			if( p->create( filename ) == true )
			{
				return p;
			}

			VG_WARN(Warning::WARN_FILE_NOT_FOUND ,
				Warning::BUTTON_NONE ,
				filename + " is not found , create StreamReaderPtr failed.",
				"StreamFactory::createReaderFromFile");

			p.setNull();
			return p;
		}
	
	};
	
	
}// end of namespace vgFoundationSystem

#endif // end of __TVGSTREAMREADERFACTORY_H__