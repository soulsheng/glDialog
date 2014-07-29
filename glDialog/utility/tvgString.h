

#ifndef __TVGSTRING_H__
#define __TVGSTRING_H__

#include "vgPrerequisites.h"
#include "vgSharedPtr.h"

namespace vgFoundationSystem {


	typedef std::string String;


	class StringPtr : public SharedPtr<String>
	{
	public:
		StringPtr() : SharedPtr<String>() {}
		explicit StringPtr(String* rep) : SharedPtr<String>(rep) {}
		StringPtr(const StringPtr& r) : SharedPtr<String>(r) {} 
	

		inline bool isEmpty() const
		{
			if ( SharedPtr<String>::isNull() )
			{
				return true;
			}
			if ( SharedPtr<String>::operator ->()->empty() )
			{
				return true;
			}
			return false;
		}

		/**
			ע�⣡���������map�����ıȽϲ�����
			��ÿ��string��ַΨһ���ʿ��������Ƚϣ����Ч�ʡ�
		*/
		inline bool operator > ( const StringPtr& p2 ) const
		{
			if( *this->getPointer() > *p2.getPointer() )
			{
				return true;
			}
			return false;
		}

		inline bool operator < ( const StringPtr& p2 )  const
		{
			if( *this->getPointer() < *p2.getPointer() )
			{
				return true;
			}
			return false;
		}

		/// the following derives from SharedPtr

		//inline bool operator == ( const StringPtr& p2 )  const
		//{
		//	if ( (**this) == (*p2) )
		//	{
		//		return true;
		//	}
		//	return false;
		//}

		//inline bool operator == ( const String& str )  const
		//{
		//	if ( (**this) == str )
		//	{
		//		return true;
		//	}
		//	return false;
		//}

		static const StringPtr EmptyStringPtr; 

		static const StringPtr EmptyTexStringPtr;

		static const StringPtr UndefinedTexStringPtr;

		static const StringPtr UnbindedTexStringPtr;

		static const StringPtr EmptyBillboardStrPtr;

	};
	
	
}// end of namespace vgFoundationSystem

#endif // end of __TVGSTRING_H__