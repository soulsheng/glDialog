

#ifndef __VGPREREQUISITES_H__
#define __VGPREREQUISITES_H__



#include "math_src/vgfPrerequisites.h"
#include "vgGlobalDefinition.h"
#include "vgImportStaticLib.h"
#include "vgStandardHeaders.h"


	///float的有效数字是7.22个,若要考虑更高的精度,需要使用double,12个有效数字
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

	/// 定义整数单位.
	typedef long  Integer;

	///use vgFoundationSystem's classes
// 	typedef		vgFoundationSystem::Angle				Angle;
// 	typedef		vgFoundationSystem::AxisAlignedBox		AxisAlignedBox;
// 	typedef		vgFoundationSystem::Degree				Degree;
// 	typedef		vgFoundationSystem::Math				Math;
// 	typedef		vgFoundationSystem::Matrix3				Matrix3;
// 	typedef		vgFoundationSystem::Matrix4				Matrix4;
// 	typedef		vgFoundationSystem::Plane				Plane;
// 	typedef		vgFoundationSystem::Quaternion			Quaternion;
// 	typedef		vgFoundationSystem::Radian				Radian;
// 	typedef		vgFoundationSystem::Ray					Ray;
// 	typedef		vgFoundationSystem::Sphere				Sphere;
// 	typedef		vgFoundationSystem::Vector2				Vector2;
// 	typedef		vgFoundationSystem::Vector3				Vector3;
// 	typedef		vgFoundationSystem::Vector4				Vector4;
	//typedef		vgFoundationSystem::ColorValue			ColorValue;
	


	typedef  std::string  String;
	typedef  std::wstring WString;
	typedef  std::vector<std::string> StringVector;


	template<class T>
	class SharedPtr;

	typedef SharedPtr<StringVector> StringVectorPtr;



namespace vgFoundationSystem {

	typedef std::pair<String,String> StringPair;
	typedef std::vector< StringPair > StringPairList;
	typedef SharedPtr< StringPairList > StringPairListPtr;


	typedef streamoff StreamPos;
	typedef ios_base::seek_dir StreamWriterSeekDir;



	class  Root;
	class  Warning;
	class  WarningFactory;


	class ColorValue;

	class Billboard;
	class BillboardSet;
	class Texture;
	class TextureBuilder;
	class TextureBuilderUtil;
	class TextureManager;

	typedef		vgFoundationSystem::Frustum				Frustum;

	class TexturePtr;
	class TextureBuildOption;
	class HugeImageRender;
	class HugeImageProcessor;

	class GpuSupport;
	class GpuContext;
	class GpuProgram;
	class GpuManager;

	class Archive;
	class ArchiveManager;
	class ArchiveFactory;
	class ZipArchive;
	class FileSystemArchive;
}

namespace vgtest = vgFoundationSystem;

#endif // end of __VGPREREQUISITES_H__