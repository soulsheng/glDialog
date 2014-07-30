
#include "StdAfx.h"
#include "tvgArchive.h"


#include <sys/types.h>
#include <sys/stat.h>


#include <direct.h>
#include <io.h>

namespace vgFoundationSystem {
	
	
    //-----------------------------------------------------------------------
    FileSystemArchive::FileSystemArchive(const String& name, const String& archType )
        : Archive(name, archType)
    {
    }
    //-----------------------------------------------------------------------
    bool FileSystemArchive::isCaseSensitive(void) const
    {
         return false;
    }
    //-----------------------------------------------------------------------
    static bool is_reserved_dir (const char *fn)
    {
        return (fn [0] == '.' && (fn [1] == 0 || (fn [1] == '.' && fn [2] == 0)));
    }
    //-----------------------------------------------------------------------
    static bool is_absolute_path(const char* path)
    {
        if (isalpha(uchar(path[0])) && path[1] == ':')
		{
			return true;
		}
		else
		{
			return false;
		}
    }
    //-----------------------------------------------------------------------
    static String concatenate_path(const String& base, const String& name)
    {
        if (base.empty() || is_absolute_path(name.c_str()))
            return name;
        else
            return base + '/' + name;
    }
    //-----------------------------------------------------------------------
    void FileSystemArchive::findFiles(const String& pattern, bool recursive, 
        bool dirs, StringVector* simpleList, FileInfoList* detailList)
    {
        long lHandle, res;
        struct _finddata_t tagData;

        // pattern can contain a directory name, separate it from mask
        size_t pos1 = pattern.rfind ('/');
        size_t pos2 = pattern.rfind ('\\');
        if (pos1 == pattern.npos || ((pos2 != pattern.npos) && (pos1 < pos2)))
            pos1 = pos2;
        String directory;
        if (pos1 != pattern.npos)
            directory = pattern.substr (0, pos1 + 1);

        String full_pattern = concatenate_path(mName, pattern);

        lHandle = _findfirst(full_pattern.c_str(), &tagData);
        res = 0;
        while (lHandle != -1 && res != -1)
        {
            if ((dirs == ((tagData.attrib & _A_SUBDIR) != 0)) &&
                (!dirs || !is_reserved_dir (tagData.name)))
            {
                if (simpleList)
                {
                    simpleList->push_back(directory + tagData.name);
                }
                else if (detailList)
                {
                    FileInfo fi;
                    fi.archive = this;
                    fi.filename = directory + tagData.name;
                    fi.basename = tagData.name;
                    fi.path = directory;
                    fi.compressedSize = tagData.size;
                    fi.uncompressedSize = tagData.size;
                    detailList->push_back(fi);
                }
            }
            res = _findnext( lHandle, &tagData );
        }
        // Close if we found any files
        if(lHandle != -1)
            _findclose(lHandle);

        // Now find directories
        if (recursive)
        {
            String base_dir = mName;
            if (!directory.empty ())
            {
                base_dir = concatenate_path(mName, directory);
                // Remove the last '/'
                base_dir.erase (base_dir.length () - 1);
            }
            base_dir.append ("/*");

            // Remove directory name from pattern
            String mask ("/");
            if (pos1 != pattern.npos)
                mask.append (pattern.substr (pos1 + 1));
            else
                mask.append (pattern);

            lHandle = _findfirst(base_dir.c_str (), &tagData);
            res = 0;
            while (lHandle != -1 && res != -1)
            {
                if ((tagData.attrib & _A_SUBDIR) &&
                    !is_reserved_dir (tagData.name))
                {
                    // recurse
                    base_dir = directory;
                    base_dir.append (tagData.name).append (mask);
                    findFiles(base_dir, recursive, dirs, simpleList, detailList);
                }
                res = _findnext( lHandle, &tagData );
            }
            // Close if we found any files
            if(lHandle != -1)
                _findclose(lHandle);
        }
    }
    //-----------------------------------------------------------------------
    FileSystemArchive::~FileSystemArchive()
    {

    }
    //-----------------------------------------------------------------------
    void FileSystemArchive::load()
    {
        // do nothing here, what has to be said will be said later
    }
    //-----------------------------------------------------------------------
    void FileSystemArchive::unload()
    {
        // nothing to see here, move along
    }
    //-----------------------------------------------------------------------
    StreamReaderPtr FileSystemArchive::open(const String& filename) const
    {
        String full_path = concatenate_path(mName, filename);

        // Use filesystem to determine size 
        // (quicker than streaming to the end and back)
        struct stat tagStat;
		int ret = stat(full_path.c_str(), &tagStat);
        assert(ret == 0 && "Problem getting file size" );

        // Always open in binary mode
        std::ifstream *origStream = new std::ifstream();
        origStream->open(full_path.c_str(), std::ios::in | std::ios::binary);

        // Should check ensure open succeeded, in case fail for some reason.
        if (origStream->fail())
        {
            delete origStream;

			//����ֻ���ؿգ����׳��쳣��

            //VG_EXCEPT(Exception::ERR_FILE_NOT_FOUND,
            //    "Cannot open file: " + filename,
            //    "FileSystemArchive::open");

			return StreamReaderPtr();
        }

        /// Construct return stream, tell it to delete on destroy
        FileStreamReader* stream = new FileStreamReader(filename,
            origStream, tagStat.st_size, true);
        return StreamReaderPtr(stream);
    }
    //-----------------------------------------------------------------------
    StringVectorPtr FileSystemArchive::list(bool recursive, bool dirs)
    {
		// directory change requires locking due to saved returns
        StringVectorPtr ret(new StringVector());

        findFiles("*", recursive, dirs, ret.getPointer(), 0);

        return ret;
    }
    //-----------------------------------------------------------------------
    FileInfoListPtr FileSystemArchive::listFileInfo(bool recursive, bool dirs)
    {
        FileInfoListPtr ret(new FileInfoList());

        findFiles("*", recursive, dirs, 0, ret.getPointer());

        return ret;
    }
    //-----------------------------------------------------------------------
    StringVectorPtr FileSystemArchive::find(const String& pattern,
                                            bool recursive, bool dirs)
    {
        StringVectorPtr ret(new StringVector());

        findFiles(pattern, recursive, dirs, ret.getPointer(), 0);

        return ret;

    }
    //-----------------------------------------------------------------------
    FileInfoListPtr FileSystemArchive::findFileInfo(const String& pattern, 
        bool recursive, bool dirs)
    {
        FileInfoListPtr ret(new FileInfoList());

        findFiles(pattern, recursive, dirs, 0, ret.getPointer());

        return ret;
    }
    //-----------------------------------------------------------------------
	bool FileSystemArchive::exists(const String& filename)
	{
        String full_path = concatenate_path(mName, filename);

        struct stat tagStat;
        bool ret = (stat(full_path.c_str(), &tagStat) == 0);

		return ret;
	}
    //-----------------------------------------------------------------------



	
	//----------------------------------------------------------------
	String Archive::getDetails()
	{
		std::ostringstream o;
		o << " ------------ Archive Information Details begin-----------------"
			<< "\n\t# Archive Name = " << this->getName() 
			<< "\n\t# Archive Type = " << this->getType() 
			<< "\n\t# Contain Files : \n";

		FileInfoListPtr pinfo = listFileInfo();
		if ( pinfo.isNull() )
		{
			o << "  This Archive contains nothing!!!";
			o << " ------------ Archive Information Details end-----------------\n";
			return o.str();
		}

		if ( pinfo->size() == 0 )
		{
			o << "  This Archive contains nothing!!!";
			o << " ------------ Archive Information Details end-----------------\n";
			return o.str();
		}

		FileInfoList::iterator iter = pinfo->begin();
		FileInfoList::iterator iter_end = pinfo->end();

		for ( ; iter != iter_end ; ++ iter )
		{
			o << "\t\t" << iter->getDetails() << '\n';
		}

		o << " ------------ Archive Information Details end-----------------\n";
		return o.str();
	}
	//----------------------------------------------------------------




}// end of namespace vgFoundationSystem
