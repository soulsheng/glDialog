
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


			//////����Ҫ�ģ�����������������ֱ��loadFromFileInfo���ɣ�����

			ImagePtr pimg = ImageFactory::createFromFileFormat( iter->first->filename );

			if( !pimg->loadFromFileInfo( iter->first ) )
			{
				VG_WARN(Warning::WARN_FILE_NOT_FOUND,
					Warning::BUTTON_NONE,
					"��ȡԭ�ļ�ʱʧ��." ,
					"ArchiveSaver::saveToStream");

				continue;
			}

			if ( !pimg->saveToWriter( iter->second ) )
			{
				VG_WARN(Warning::WARN_CANNOT_WRITE_TO_FILE,
					Warning::BUTTON_NONE,
					"��ȡ�ļ�ʱʧ��.ԭ�ļ�:" + iter->first->filename 
					+ " . Ŀ���ļ�: " + iter->second->aimFilename ,
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
					"��ȡԭ�ļ�ʱʧ��." ,
					"ArchiveSaver::saveToStream");

				continue;
			}

			if ( !pimg->saveToWriter( iter->second ) )
			{
				VG_WARN(Warning::WARN_CANNOT_WRITE_TO_FILE,
					Warning::BUTTON_NONE,
					"��ȡ�ļ�ʱʧ��.ԭ�ļ�:" + iter->first->filename 
					+ " . Ŀ���ļ�: " + iter->second->aimFilename ,
					"ArchiveSaver::saveToStream");

				continue;
			}
			*/
		}

		return true;
	}
	//----------------------------------------------------------------


}// end of namespace vgFoundationSystem
