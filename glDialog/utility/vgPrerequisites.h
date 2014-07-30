

#ifndef __VGPREREQUISITES_H__
#define __VGPREREQUISITES_H__

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

#include "vgSharedPtr.h"

#define	PI	3.14159

	///float����Ч������7.22��,��Ҫ���Ǹ��ߵľ���,��Ҫʹ��double,12����Ч����
	typedef float	Real;		

	typedef unsigned char	uchar;
	typedef unsigned short	ushort;
	typedef unsigned int	uint;
	typedef unsigned long	ulong;

	// Integer formats of fixed bit width
	typedef unsigned int uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;

	typedef uint32 RGBA;
	typedef uint32 ARGB;
	typedef uint32 ABGR;
	typedef uint32 BGRA;

	/// ����������λ.
	typedef long  Integer;



	typedef  std::string  String;
	typedef  std::wstring WString;
	typedef  std::vector<std::string> StringVector;

	typedef std::pair<String,String> StringPair;
	typedef std::vector< StringPair > StringPairList;


	template<class T>
	class SharedPtr;

	typedef SharedPtr<StringVector> StringVectorPtr;

	typedef std::pair<String,String> StringPair;
	typedef std::vector< StringPair > StringPairList;
	typedef SharedPtr< StringPairList > StringPairListPtr;

	typedef streamoff StreamPos;
	typedef ios_base::seek_dir StreamWriterSeekDir;

	namespace vgFoundationSystem {

		class	OpenGLSupport;

	}

#include "vgWarning.h"
#include "vgException.h"
#include "tvgString.h"

#endif // end of __VGPREREQUISITES_H__