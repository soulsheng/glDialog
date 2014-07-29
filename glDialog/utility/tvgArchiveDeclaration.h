

#ifndef __TVGVGIMGARCHIVEDECLARATION_H__
#define __TVGVGIMGARCHIVEDECLARATION_H__

#include "vgPrerequisites.h"
#include "tvgStreamWriter.h"
#include "tvgArchive.h"


namespace vgFoundationSystem {




	class Archive;


	/** Information about a file/directory within the archive will be
	returned using a FileInfo struct.
	@see
	Archive
	*/
	struct FileInfo {
		/// The archive in which the file has been found (for info when performing
		/// multi-Archive searches, note you should still open through ResourceGroupManager)
		Archive* archive;


		//////////////////////////////////////////////////////////////////////////
		//  �������

		StreamReaderPtr streamReaderPtr;
		//////////////////////////////////////////////////////////////////////////

		/// The file's fully qualified name
		String filename;
		/// Path name; separated by '/' and ending with '/'
		String path;
		/// Base filename
		String basename;
		/// Compressed size
		size_t compressedSize;
		/// Uncompressed size
		size_t uncompressedSize;

	public:

		FileInfo& operator = ( const FileInfo& rhs )
		{
			this->filename = rhs.filename;
			this->path = rhs.path;
			this->basename = rhs.basename;
			this->compressedSize = rhs.compressedSize;
			this->uncompressedSize = rhs.uncompressedSize;
			this->streamReaderPtr = rhs.streamReaderPtr;
			this->archive = rhs.archive;
			return *this;
		}

		String getDetails() const
		{
			std::ostringstream o;
			o << "FileInfo :  filename = " << filename
				<< " , path = " << path 
				<< " , basename = " << basename 
				<< " , compressedSize = " << compressedSize 
				<< " , uncompressedSize = " << uncompressedSize

				<< " , streamReaderPtr.isNull? = " << streamReaderPtr.isNull();

			return o.str();
		}
	};

	typedef SharedPtr<FileInfo> FileInfoPtr;

	typedef std::vector<FileInfo> FileInfoList;
	typedef SharedPtr<FileInfoList> FileInfoListPtr;



	//struct VgimgPreHeader
	//{
	//	char mFourCC[6];
	//	long mHeaderLength;
	//};

	//struct VgimgMetaFile 
	//{
	//	char mFilename[255];
	//	StreamPos mStoragePos;
	//	long  mFileSize;
	//};

	//typedef std::vector<VgimgMetaFile> VgimgMetaFileList;
	//typedef SharedPtr<VgimgMetaFileList> VgimgMetaFileListPtr;

	//struct VgimgHeader
	//{
	//public:
	//	VgimgHeader()
	//	{
	//		
	//	}
	//	~VgimgHeader()
	//	{
	//		
	//	}
	//

	//	long	mWholeFileLength;
	//	long	mFilesNum;
	//	StreamPos mDataBegin;

	//	VgimgMetaFileListPtr mMetaFileListPtr;
	//};


	typedef std::pair<FileInfoPtr , WriterInfoPtr>  MetaSaver;
	typedef std::vector<MetaSaver> MetaSaverList;
	typedef SharedPtr<MetaSaverList> MetaSaverListPtr;


	struct MetaFile
	{
		char mFilename[256];
		StreamPos mStoragePos;
		ulong  mFileSize;
	};

	typedef MetaFile VgimgMetaFile;

	typedef std::vector<MetaFile> MetaFileList;
	typedef SharedPtr<MetaFileList> MetaFileListPtr;


	struct Header
	{
	public:
		Header()
		{
			this->setParamDefault();
		}
		~Header()
		{

		}

		// FourCC code
		char mFourCC[6];

		// ͷ�ļ��Ĵ�С
		long   mHeaderLength;
		// �����ļ��ܴ�С
		long	mWholeDataLength;
		// �ļ�����Ŀ
		long	mFilesNum;

		// �ļ���������ʼ
		StreamPos mMetaFieldBegin;

		// �ļ������ĳ���
		long mMetaFieldLength;

		// ��������ʼ
		StreamPos mDataBegin;

	private:

		void setParamDefault()
		{
			const long DEFAULT_CODE = 52;
			strcpy( this->mFourCC , "VGIMG" );

			this->mHeaderLength = sizeof(Header);

			this->mWholeDataLength	= DEFAULT_CODE;
			this->mFilesNum			= DEFAULT_CODE;
			this->mMetaFieldBegin	= DEFAULT_CODE;
			this->mMetaFieldLength	= DEFAULT_CODE;
			this->mDataBegin		= DEFAULT_CODE;
		}

	};

	typedef Header VgimgHeader;
	
	
	
}// end of namespace vgFoundationSystem

#endif // end of __TVGVGIMGARCHIVEDECLARATION_H__