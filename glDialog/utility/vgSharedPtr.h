

#ifndef __VGSHAREDPTR_H__
#define __VGSHAREDPTR_H__

#include "vgPrerequisites.h"

	/**
		@date 	2007/03/01  12:15	
		@author  leven
	
		@brief 	boost::shared_ptrʵ��.ע��,�����ʵ�ֲ������̰߳�ȫ��,ֻ����ʱʵ��,
		��Ҫ�Ϻõ�ʵ��,Ҫʹ��boost::shared_ptr.
		  
		  ����ע��,ʹ����shared_ptr֮��Ҫ�ٶ�ԭʵ�����������delete����,���������ڴ�
		  ָ��ָ��δ��ʼ����ַ����.
	
		@see   ogre::shardPtr.boost::shared_ptr.
	*/

    template<class T>
	class SharedPtr 
	{

	protected:
		T* pRep;
		unsigned int* pUseCount;

	public:

		/// ע��!���ʱ��Ҫ����bind,��Ȼʹ��"->"��"*"���Ż��׳�����.
		SharedPtr() : pRep(0), pUseCount(0)
        {
        }

        template<class Y>
		explicit SharedPtr(Y* rep) : pRep(rep), pUseCount(new unsigned int(1))
		{

		}
		SharedPtr(const SharedPtr& r)
			: pRep(0), pUseCount(0)
		{
			pRep = r.pRep;
			pUseCount = r.pUseCount; 
			// Handle zero pointer gracefully to manage STL containers
			if(pUseCount)
			{
				++(*pUseCount); 
			}

		}

		virtual ~SharedPtr() 
		{
			release();
		}

		inline SharedPtr& operator = (const SharedPtr& r)
		{
			if (pRep == r.pRep)
				return *this;

			/*
			release();
			pRep = r.pRep;
			pUseCount = r.pUseCount;
			if (pUseCount)
			{
				++(*pUseCount);
			}

			return *this;
			*/

			// Swap current data into a local copy
			// this ensures we deal with rhs and this being dependent
			SharedPtr<T> tmp(r);
			swap(tmp);
			return *this;
		}

#if _MSC_VER > 1200	

		template< class Y >
		inline SharedPtr& operator = (const SharedPtr<Y>& r) 
		{
			/// �����,�򷵻�(�ޱ仯)
			if (pRep == r.pRep)
				return *this;

			/*
			///�������,�൱�ں����滻ǰ��,����ǰ�ߵ�ȻҪreleaseһ��.
			release();

			pRep = r.getPointer();
			pUseCount = r.useCountPointer();
			if (pUseCount)
			{
				++(*pUseCount);
			}

			return *this;
			*/

			// Swap current data into a local copy
			// this ensures we deal with rhs and this being dependent
			SharedPtr<T> tmp(r);
			swap(tmp);
			return *this;
		}

		template< class Y>
		SharedPtr(const SharedPtr<Y>& r)
			: pRep(0), pUseCount(0)
		{
			pRep = r.getPointer();
			pUseCount = r.useCountPointer();
			// Handle zero pointer gracefully to manage STL containers
			if(pUseCount)
			{
				++(*pUseCount);
			}
		}

#endif

		inline T& operator*() const { assert(pRep); return *pRep; }
		inline T* operator->() const { assert(pRep); return pRep; }

		///�õ�T*.
		//inline T* get() const { return pRep; }
		inline T* getPointer() const { return pRep; }

		/// ��һ��ָ���.ע����뱣֤δ��ʼ��!
		/// ����һ�������׷��Ĵ���,Ҫע����.
		inline void bind(T* rep)
		{
			assert(!pRep && !pUseCount);

			pUseCount = new unsigned int(1);
			pRep = rep;
		}

		inline bool unique() const
		{ 
			assert(pUseCount); 
			return *pUseCount == 1;
		}
		inline unsigned int useCount() const
		{ 
			assert(pUseCount); 
			return *pUseCount; 
		}

		/// ���ؼ�������ָ��.
		inline unsigned int* useCountPointer() const
		{ 
			return pUseCount;
		}

		inline bool isNull(void) const 
		{ 
			return pRep == 0; 
		}

		/// ����Ϊ��.�൱��boost::shared_ptr�е�reset().
        inline void setNull(void) 
		{ 
			if (pRep)
			{
				// can't scope lock mutex before release incase deleted
				release();
				pRep = 0;
				pUseCount = 0;
			}
        }

    protected:

        inline void release(void)
        {
			bool destroyThis = false;

			if (pUseCount)
			{
				if (--(*pUseCount) == 0) 
				{
					destroyThis = true;
				}
			}

			if (destroyThis)
				destroy();
        }

        inline void destroy(void)
        {
            delete pRep;
            delete pUseCount;
        }

		inline void swap(SharedPtr<T> &other) 
		{
			std::swap(pRep, other.pRep);
			std::swap(pUseCount, other.pUseCount);
		}


	};

	template<class T, class U> 
	inline bool operator == ( SharedPtr<T> const& a , SharedPtr<U> const& b)
	{
		return a.getPointer() == b.getPointer();
	}

	template<class T, class U> 
	inline bool operator != ( SharedPtr<T> const& a , SharedPtr<U> const& b)
	{
		return a.getPointer() != b.getPointer();
	}





#endif // end of __VGSHAREDPTR_H__