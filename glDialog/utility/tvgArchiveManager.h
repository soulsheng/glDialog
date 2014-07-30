


#ifndef __TVGARCHIVEMANAGER_H__
#define __TVGARCHIVEMANAGER_H__

#include "vgPrerequisites.h"
#include "vgSingleton.h"
#include "tvgArchiveFactory.h"

namespace vgFoundationSystem {

	/**
		@date 	2007/06/11  8:46	
		@author  Leven
	
		@brief 	
	
		@see    
	*/
	class ArchiveManager : public Singleton<ArchiveManager>
	{
	public:

#define  DEFAULT_FILESYSTEM_WEIGHT  100

		//typedef std::vector<ArchivePtr>  ArchivePtrList;
		typedef std::map< int  , ArchivePtr > ArchivePtrMap;

		typedef std::pair< ArchivePtrMap::iterator , bool > ArchivePtrMapInsertResult;

		typedef std::map< String , StringVectorPtr > ArchiveListResult;

	public:

		ArchiveManager( bool createCurrentFileSys = true )
		{
			if ( createCurrentFileSys )
			{
				char buf[1024];
				GetCurrentDirectory( 1024 , buf );

				String currentDir(buf);

				// 考虑这里是否加入容错机制。
				FileSystemArchive* pCurrentFileSys = 
						new FileSystemArchive( currentDir ,"FileSystem" );

				this->addArchive( DEFAULT_FILESYSTEM_WEIGHT , ArchivePtr( pCurrentFileSys ) );
				
			
			}

		}
		~ArchiveManager()
		{
		}
		//----------------------------------------------------------------
		bool addArchive( const int& search_weight , const ArchivePtr& pArchive )
		{
			if ( pArchive.isNull() )
			{
				return false;
			}

			ArchivePtrMapInsertResult res = 
				this->mArchivePtrMap.insert( std::make_pair( search_weight , pArchive ) );

			return res.second;
		}
		//----------------------------------------------------------------
		bool addArchive( const int& search_weight , const String& archive_name )
		{
			ArchivePtr parchive = ArchiveFactory::createFromName( archive_name );
			if ( parchive.isNull() )
			{
				return false;
			}

			ArchivePtrMapInsertResult res = 
				this->mArchivePtrMap.insert( std::make_pair( search_weight , parchive ) );

			return res.second;
		}
		//----------------------------------------------------------------
		void clear()
		{
			this->mArchivePtrMap.clear();
		}
		//----------------------------------------------------------------
		FileInfoPtr getFileInfo(const String& filename) const
		{
			ArchivePtrMap::const_reverse_iterator iter = mArchivePtrMap.rbegin();
			ArchivePtrMap::const_reverse_iterator iter_end = mArchivePtrMap.rend();

			FileInfoPtr search_result;
			for ( ; iter != iter_end ; ++ iter )
			{
				assert( !iter->second.isNull() );
				search_result = iter->second->getFileInfo( filename );

				// when searched the file
				if ( search_result.isNull() == false )
				{
					break;
				}
			}

			return search_result;
		}
		//----------------------------------------------------------------
		ArchiveListResult listAll(bool recursive = true, bool dirs = false)
		{
			ArchivePtrMap::reverse_iterator iter = mArchivePtrMap.rbegin();
			ArchivePtrMap::reverse_iterator iter_end = mArchivePtrMap.rend();

			ArchiveListResult result;

			for ( ; iter != iter_end ; ++ iter )
			{
				assert( !iter->second.isNull() );
				StringVectorPtr tmp = iter->second->list( recursive , dirs );

				result.insert( std::make_pair( iter->second->getName() , tmp ) );
			}

			return result;
		}
		//----------------------------------------------------------------
		void loadAll()
		{
			ArchivePtrMap::reverse_iterator iter = mArchivePtrMap.rbegin();
			ArchivePtrMap::reverse_iterator iter_end = mArchivePtrMap.rend();

			for ( ; iter != iter_end ; ++ iter )
			{
				assert( !iter->second.isNull() );
				iter->second->load();
			}
			return;
		}
		//----------------------------------------------------------------
		void unloadAll()
		{
			ArchivePtrMap::reverse_iterator iter = mArchivePtrMap.rbegin();
			ArchivePtrMap::reverse_iterator iter_end = mArchivePtrMap.rend();

			for ( ; iter != iter_end ; ++ iter )
			{
				assert( !iter->second.isNull() );
				iter->second->unload();
			}
			return;
		}
		//----------------------------------------------------------------
		bool exists(const String& filename)
		{
			ArchivePtrMap::reverse_iterator iter = mArchivePtrMap.rbegin();
			ArchivePtrMap::reverse_iterator iter_end = mArchivePtrMap.rend();

			for ( ; iter != iter_end ; ++ iter )
			{
				assert( !iter->second.isNull() );
				if ( iter->second->exists( filename ) )
				{
					return true;
				}
			}
			return false;
		}
		//----------------------------------------------------------------
		StreamReaderPtr open(const String& filename) const
		{
			ArchivePtrMap::const_reverse_iterator iter = mArchivePtrMap.rbegin();
			ArchivePtrMap::const_reverse_iterator iter_end = mArchivePtrMap.rend();

			StreamReaderPtr result;
			for ( ; iter != iter_end ; ++ iter )
			{
				assert( !iter->second.isNull() );
				result = iter->second->open( filename );

				if ( !result.isNull() )
				{
					break;
				}
			}
			return result;
		}
		//----------------------------------------------------------------
		bool addLoadedZipArchiveFromFile( const String& zip_filename )
		{
			assert(0);
		}
		//----------------------------------------------------------------
		String getDetails()
		{
			std::ostringstream o;
			o << "===================== ArchiveManager Details begin =========================\n";

			if ( mArchivePtrMap.empty() )
			{
				o << "\t\tcontains nothing!!!\n";
			}

			ArchivePtrMap::reverse_iterator iter = mArchivePtrMap.rbegin();
			ArchivePtrMap::reverse_iterator iter_end = mArchivePtrMap.rend();

			for ( ; iter != iter_end ; ++ iter )
			{
				assert( !iter->second.isNull() );
				o << "### Archive Weight: " << iter->first << "\n" 
					<< iter->second->getDetails() << "\n";
			}

			o << "===================== ArchiveManager Details end =========================\n";
			return o.str();
		}

		/// static functions which can make singleton pattern.
		static ArchiveManager& getSingleton(void);
		static ArchiveManager* getSingletonPtr(void);
	
	
	private:
	
		ArchivePtrMap mArchivePtrMap;
	};
	
	
	
}// end of namespace vgFoundationSystem

#endif // end of __TVGARCHIVEMANAGER_H__