
#include "StdAfx.h"
#include "tvgArchiveSaver.h"


namespace vgFoundationSystem {
	
	
	
	
	//----------------------------------------------------------------
	bool ArchiveSaver::saveToStream( const MetaSaverListPtr& saver_list )
	{
		if ( saver_list.isNull() )
		{
			return false;
		}

		MetaSaverList::iterator iter = saver_list->begin();
		MetaSaverList::iterator iter_end = saver_list->end();

		for ( ; iter != iter_end ; ++iter )
		{
			assert( !iter->first.isNull() );
			assert( !iter->second.isNull() );


			//////这里要改！！！！！！！！！直接loadFromFileInfo即可！！！

			ImagePtr pimg = ImageFactory::createFromFileFormat( iter->first->filename );

			if( !pimg->loadFromFileInfo( iter->first ) )
			{
				VG_WARN(Warning::WARN_FILE_NOT_FOUND,
					Warning::BUTTON_NONE,
					"读取原文件时失败." ,
					"ArchiveSaver::saveToStream");

				continue;
			}

			if ( !pimg->saveToWriter( iter->second ) )
			{
				VG_WARN(Warning::WARN_CANNOT_WRITE_TO_FILE,
					Warning::BUTTON_NONE,
					"存取文件时失败.原文件:" + iter->first->filename 
					+ " . 目标文件: " + iter->second->aimFilename ,
					"ArchiveSaver::saveToStream");

				continue;
			}

			/*
			ImagePtr pimg = ImageFactory::createFromFileFormat( iter->first->filename );

			if( !pimg->loadFromStream( iter->first->streamReaderPtr ,
				iter->first->filename ) )
			{
				VG_WARN(Warning::WARN_FILE_NOT_FOUND,
					Warning::BUTTON_NONE,
					"读取原文件时失败." ,
					"ArchiveSaver::saveToStream");

				continue;
			}

			if ( !pimg->saveToWriter( iter->second ) )
			{
				VG_WARN(Warning::WARN_CANNOT_WRITE_TO_FILE,
					Warning::BUTTON_NONE,
					"存取文件时失败.原文件:" + iter->first->filename 
					+ " . 目标文件: " + iter->second->aimFilename ,
					"ArchiveSaver::saveToStream");

				continue;
			}
			*/
		}

		return true;
	}
	//----------------------------------------------------------------


}// end of namespace vgFoundationSystem
