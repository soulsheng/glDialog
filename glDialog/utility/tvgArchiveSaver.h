

#ifndef __TVGARCHIVESAVER_H__
#define __TVGARCHIVESAVER_H__

#include "vgPrerequisites.h"
#include "tvgArchive.h"
#include "tvgStreamFactory.h"
#include "tvgArchiveDeclaration.h"
//#include "../vgTrace.h"
#include "tvgImage.h"
#include "tvgImageFactory.h"

namespace vgFoundationSystem {


	/**
		@date 	2007/06/12  15:00	
		@author  Leven
	
		@brief 	
	
		@see    
	*/
	class ArchiveSaver
	{

	public:

		ArchiveSaver( StreamWriterPtr& pwriter , const StringPairListPtr& pstrpairlist)
		{
			assert( !pwriter.isNull() );

			// ������д
			assert( !pstrpairlist.isNull() );
			this->mInputListPtr = pstrpairlist;

			this->mOutputStreamPtr = pwriter;

			this->mMetaFileListPtr.bind( new MetaFileList() );

		}
		~ArchiveSaver()
		{
		}
	

		bool beginToWrite()
		{
			assert( !mInputListPtr.isNull() );
			assert( !mOutputStreamPtr.isNull() );

			assert( !this->mMetaFileListPtr.isNull() );
			assert( this->mMetaFileListPtr->empty() );
			
			long whole_data_len = 0;
			StreamPos whole_data_beg = 0;

			// �洢д��ͷ�ļ�ʱ��λ�á�
			this->mHeaderBeginPos = this->mOutputStreamPtr->tell();

			// д���ļ�ͷ���ݡ�
			this->mOutputStreamPtr->write( (const char*)&mHeader , sizeof( Header ) );

			// �洢ͼ�����ݵ�д����ʼ�㡣
			whole_data_beg = this->mOutputStreamPtr->tell();

			// ��ʼд��ͼ������
			StringPairList::iterator input_iter = mInputListPtr->begin();
			StringPairList::iterator input_iter_end = mInputListPtr->end();

			for ( ; input_iter != input_iter_end ; ++ input_iter )
			{
				const String& filename = input_iter->first;
				FileInfoPtr pfileinfo = ArchiveManager::getSingleton().getFileInfo( filename );

				if ( pfileinfo.isNull() )
				{
					continue;
				}

				WriterInfoPtr pwriter( new WriterInfo() );
				pwriter->aimFilename = input_iter->second;

				const StreamPos beg_pos = pwriter->absoluteBeginPos = mOutputStreamPtr->tell();
				pwriter->streamWriterPtr = this->mOutputStreamPtr;

				/// ����DDS format

				if ( saveToStream( make_pair( pfileinfo , pwriter ) ) == false )
				{
					assert( 0 && "д��ʧ�ܣ���");
					mOutputStreamPtr->seek( beg_pos );
					continue;
				}
				
				// �洢MetaFile��Ϣ
				MetaFile tmpmeta;
				assert( input_iter->second.size() <= 256 );

				strcpy( tmpmeta.mFilename , input_iter->second.c_str() );
				tmpmeta.mFileSize = pwriter->fileLength;
				tmpmeta.mStoragePos = beg_pos;

				whole_data_len += tmpmeta.mFileSize;

				this->mMetaFileListPtr->push_back( tmpmeta );
			}

			// д�����ݺ�д�������򣬷���ĩβ��
			const StreamPos index_beg = mOutputStreamPtr->tell();
			const long index_num = this->mMetaFileListPtr->size();
			const long index_filelen = index_num * sizeof( MetaFile );

			// д��������Ϣ
			
			//char* index_buf = new char[index_num * sizeof( MetaFile )];
			//assert( index_buf!= NULL );

			if ( index_filelen != 0 )
			{
				const char* index_mem_beg = reinterpret_cast<const char*>( &(*mMetaFileListPtr)[0] );
				mOutputStreamPtr->write( index_mem_beg , index_filelen );
			}

			const StreamPos whole_file_end = mOutputStreamPtr->tell();

			// �����ļ�ͷ

			this->mHeader.mMetaFieldBegin	= index_beg;
			this->mHeader.mFilesNum			= index_num;
			this->mHeader.mMetaFieldLength	= index_filelen;
			this->mHeader.mWholeDataLength	= whole_data_len;
			this->mHeader.mDataBegin		= whole_data_beg;

			mOutputStreamPtr->seek( mHeaderBeginPos );
			const char* header_mem_beg = reinterpret_cast<const char*>(&mHeader);
			mOutputStreamPtr->write( header_mem_beg , sizeof( Header ) );

			mOutputStreamPtr->seek( whole_file_end );
			return true;
		}


		bool saveToStream( pair<FileInfoPtr, WriterInfoPtr>& input_param )
		{
			assert( !input_param.first.isNull() );
			assert( !input_param.second.isNull() );

			ImagePtr pimg = ImageFactory::createFromFileFormat( input_param.first->basename );

			if( !pimg->loadFromFileInfo( input_param.first ) )
			{
				VG_WARN(Warning::WARN_FILE_NOT_FOUND,
					Warning::BUTTON_NONE,
					"��ȡԭ�ļ�ʱʧ��." ,
					"ArchiveSaver::saveToStream");

				return false;
			}

			if ( !pimg->saveToWriter( input_param.second ) )
			{
				VG_WARN(Warning::WARN_CANNOT_WRITE_TO_FILE,
					Warning::BUTTON_NONE,
					"��ȡ�ļ�ʱʧ��.ԭ�ļ�:" + input_param.first->filename 
					+ " . Ŀ���ļ�: " + input_param.second->aimFilename ,
					"ArchiveSaver::saveToStream");

				return false;
			}

			return true;
		}

		bool saveToStream( const MetaSaverListPtr& saver_list );

		//----------------------------------------------------------------


		bool saveHeader( const StreamWriterPtr& pwriterstream ,
			const StringPairListPtr& pstrpairlist )
		{
			if ( pstrpairlist.isNull() )
			{
				return false;
			}
			if ( pwriterstream.isNull() )
			{
				return false;
			}

		}

		long calculateHeaderLength( const StringPairListPtr& pstrpairlist )
		{
			if ( pstrpairlist.isNull() )
			{
				return 0;
			}
			
			long size1 = 3 * sizeof(long) + 2 * sizeof( StreamPos );
			long size2 = pstrpairlist->size() * sizeof( MetaFile );

			return size1 + size2;
		}


	public:

		//PreHeader	mPreHeader;

		StreamPos   mHeaderBeginPos;
		Header		mHeader;

		MetaFileListPtr mMetaFileListPtr;

		StringPairListPtr mInputListPtr;

		StreamWriterPtr mOutputStreamPtr;
	
	
	};

	typedef SharedPtr<ArchiveSaver> ArchiveSaverPtr;

	typedef ArchiveSaver VgimgArchiveSaver;

	
	
	
}// end of namespace vgFoundationSystem

#endif // end of __TVGARCHIVESAVER_H__