/*
-----------------------------------------------------------------------------
  VRES(Virtual Reality GIS Engine System)    
  -------------------------------------------------------------
  License details ... waiting to add...
  -------------------------------------------------------------
  Copyright  (C) 2007 , leven - All Rights Reserved
-----------------------------------------------------------------------------
*/

#include "StdAfx.h"
#include "vgException.h"



namespace vgFoundationSystem {
	
	Exception::Exception(int num, const String& desc, const String& src) :
		line( 0 ),
		number( num ),
		description( desc ),
		source( src )
    {
        // Log this error - not any more, allow catchers to do it
        //LogManager::getSingleton().logMessage(this->getFullDescription());
    }

    Exception::Exception(int num, const String& desc, const String& src, 
		const char* typ, const char* fil, long lin) :
        line( lin ),
        number( num ),
		typeName(typ),
        description( desc ),
        source( src ),
        file( fil )
    {
       

    }

    Exception::Exception(const Exception& rhs)
        : line( rhs.line ), number( rhs.number ), description( rhs.description ), source( rhs.source ), file( rhs.file )
    {
    }

    void Exception::operator = ( const Exception& rhs )
    {
        description = rhs.description;
        number = rhs.number;
        source = rhs.source;
        file = rhs.file;
        line = rhs.line;
		typeName = rhs.typeName;
    }

    const String& Exception::getFullDescription(void) const
    {
		if (fullDesc.empty())
		{

			std::ostringstream desc;

			desc <<  "VGES EXCEPTION(" << number << ":" << typeName << "): "
				<< description 
				<< " in " << source;

			if( line > 0 )
			{
				desc << " at " << file << " (line " << line << ")";
			}

			fullDesc = desc.str();
		}

		return fullDesc;
    }

    int Exception::getNumber(void) const throw()
    {
        return number;
    }

	
} // end of vgFoundationSystem