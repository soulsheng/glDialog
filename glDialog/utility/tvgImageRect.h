/*
-----------------------------------------------------------------------------
  VRES(Virtual Reality GIS Engine System)    
  -------------------------------------------------------------
  License details ... waiting to add...
  -------------------------------------------------------------
  Copyright  (C) 2007 , leven - All Rights Reserved
-----------------------------------------------------------------------------
*/

#ifndef __TVGIMAGERECT_H__
#define __TVGIMAGERECT_H__

#include "vgPrerequisites.h"
#include "tvgImagePos.h"

namespace vgFoundationSystem {

	class ImageRect;

	/// ��װͼ���С.
	class ImageSize
	{
	public:
		ImageSize()
			: width (0) , height(0)
		{

		}
		ImageSize( Integer w , Integer h)
			: width (w) , height(h)
		{

		}
		~ImageSize()
		{

		}

		inline bool operator == ( const ImageSize& size ) const
		{
			return size.width == width && size.height == height;
		}
		inline bool operator != ( const ImageSize& size ) const
		{
			return size.width != width || size.height != height;
		}


		inline ImageRect getLocalRect() const;

		inline ImagePos getMaxPos() const
		{
			return ImagePos( getWidth() , getHeight() );
		}

		inline const Integer		getWidth() const 
		{ 
			return this->width; 
		}

		inline Integer&		getWidth()  
		{ 
			return this->width; 
		}

		inline void		setWidth( const Integer& val ) 
		{ 
			this->width = val;
		}

		inline const Integer		getHeight() const 
		{ 
			return this->height; 
		}

		inline Integer&		getHeight()  
		{ 
			return this->height; 
		}

		inline void		setHeight( const Integer& val ) 
		{ 
			this->height = val;
		}

		inline const string getDetails() const
		{
			std::ostringstream o;
			o<< "ImageSize( width = " << width
				<< " , height = " <<  height << " )";

			return o.str();
		}

	private:
		Integer height;
		Integer width;

	};

	const ImageSize IMAGESIZE_ZERO = ImageSize(0,0);


	class ImageRect
	{
	public:
		ImageRect()
		{
		}
		ImageRect( const ImagePos Fmin , const ImagePos Fmax )
			: minPos(Fmin) , maxPos(Fmax) 
		{
		}
		ImageRect( const Integer Fminx , const Integer Fminy , const Integer Fmaxx , const Integer Fmaxy )
		{
			minPos = ImagePos( Fminx , Fminy );
			maxPos = ImagePos( Fmaxx , Fmaxy );
		}
		~ImageRect()
		{
		}

		inline ImageRect& operator = ( const ImageRect& rkrect )
		{
			minPos = rkrect.minPos;
			maxPos = rkrect.maxPos;
			return *this;
		}

		inline ImageRect operator - () const
		{
			return ImageRect( -minPos , -maxPos );
		}

		inline bool operator == ( const ImageRect& rkrect ) const
		{
			return rkrect.minPos == minPos && rkrect.maxPos == maxPos;
		}
		inline bool operator != ( const ImageRect& rkrect ) const
		{
			return rkrect.minPos != minPos || rkrect.maxPos != maxPos;
		}

		/// ƽ�ƺ���.����һ������,ƽ�Ƶ��˴�.
		inline ImageRect& translate(const ImagePos& rkpos)
		{
			minPos += rkpos;
			maxPos += rkpos;
			return *this;
		}

		/// ƽ�ƺ���.����һ������,ƽ�Ƶ��˴�.
		inline ImageRect translateCopy(const ImagePos& rkpos) const
		{
			ImageRect copy;
			copy.minPos = this->minPos + rkpos;
			copy.maxPos = this->maxPos + rkpos;
			return copy;
		}


		/// �鿴x�����Ƿ����ص�
		inline bool hasXOverlap (const ImageRect& rkBox) const
		{
			assert( isSafe() && "The ImageRect is not safe.");
			return maxPos[0] >= rkBox.minPos[0] && minPos[0] <= rkBox.maxPos[0];
		}
		/// �鿴y�����Ƿ����ص�
		inline bool hasYOverlap (const ImageRect& rkBox) const
		{
			assert( isSafe() && "The ImageRect is not safe.");
			return maxPos[1] >= rkBox.minPos[1] && minPos[1] <= rkBox.maxPos[1];
		}

		/// ����2��ImageRect�Ƿ����ص�.
		/// ע��:������������������findIntersection��ע��ĵط�:
		/// ��������ɢֵϵͳ,�����е�����������ཻ.
		/// �ٸ�����:
		///		ImageRect  bb( ImagePos(0) , ImagePos(100) );
		///		ImageRect  cc ( ImagePos(100) , ImagePos(200) ) ;
		///		��û���ཻ,������������.
		///	����ʸ��ϵͳ��,���������Ӧ�������ཻ.
		inline bool testIntersection (const ImageRect& rkBox) const
		{
			assert( isSafe() && "The ImageRect is not safe.");
			for (int i = 0; i < 2; i++)
			{
				if (maxPos[i] <= rkBox.minPos[i] || minPos[i] >= rkBox.maxPos[i])
				{
					return false;
				}
			}
			return true;
		}

		/// �ཻ����.��this��rkBox���ص�,�򷵻�true,�����ص����ֵ�RECT������rkIntr��.
		// The return value is 'true' if there is overlap.  In this case the
		// intersection is stored in rkIntr.  If the return value is 'false',
		// if there is no overlap.  In this case rkIntr is undefined.
		bool findIntersection (const ImageRect& rkBox,
			ImageRect& rkIntr) const
		{
			int i;
			if ( testIntersection( rkBox ) == false )
			{
				return false;
			}

			for (i = 0; i < 2; i++)
			{
				if (maxPos[i] <= rkBox.maxPos[i])
				{
					rkIntr.maxPos[i] = maxPos[i];
				}
				else
				{
					rkIntr.maxPos[i] = rkBox.maxPos[i];
				}

				if (minPos[i] <= rkBox.minPos[i])
				{
					rkIntr.minPos[i] = rkBox.minPos[i];
				}
				else
				{
					rkIntr.minPos[i] = minPos[i];
				}
			}
			return true;
		}


		/// �鿴���Ƿ��ڱ�rect��.
		inline bool testInside(const ImagePos& point) const
		{
			assert( isSafe() && "The ImageRect is not safe.");
			for (int i = 0; i < 2; i++)
			{
				if (maxPos[i] < point[i] || minPos[i] > point[i])
				{
					return false;
				}
			}
			return true;
		}

		/// �鿴����rect�Ƿ��ڱ�rect�ڲ�,ע�Ȿrect�Ͳ���Rect�Ĺ�ϵ.
		/// ע��:�غ�ʱ�����Ҳ��������.
		/// ���غ�ʱ,����true.
		inline bool testInside(const ImageRect& testRect) const
		{
			assert( isSafe() && "The ImageRect is not safe.");
			for (int i = 0; i < 2; i++)
			{
				if (maxPos[i] < testRect.maxPos[i] || minPos[i] > testRect.minPos[i])
				{
					return false;
				}
			}
			return true;
		}

		inline Integer getWidth() const
		{
			assert( isSafe() && "The ImageRect is not safe.");
			return maxPos.x - minPos.x;
		}

		inline Integer getHeight() const 
		{
			assert( isSafe() && "The ImageRect is not safe.");
			return maxPos.y - minPos.y;
		}

		inline ImageSize getImageSize() const
		{
			return ImageSize( getWidth() , getHeight() );
		}

		inline void setImageSize( const ImageSize& imgsize )
		{
			this->maxPos.x = this->minPos.x + imgsize.getWidth();
			this->maxPos.y = this->minPos.y + imgsize.getHeight();
		}

		inline bool isSafe() const
		{
			return minPos <= maxPos;
		}

		/// �������εõ�rect���ĸ���,����Ϊ�����Ͻ�˳ʱ�뿪ʼ.
		/// ��ΧΪ��0-3
		/// ����getPos(2)ָ�������½ǵĵ�.
		inline ImagePos getPos( const Integer& n ) const
		{
			assert( n >= 0 && n <4 );
			switch( n % 4 )
			{
			case 0:
				return minPos;
			case 1:
				return ImagePos( minPos.y , maxPos.x );
			case 2:
				return maxPos;
			case 3:
				return ImagePos( minPos.x , maxPos.y );
			}
			return minPos;
		}

		inline String getDetails() const
		{
			std::ostringstream o;

			o << "ImageRect( minPos = " << minPos.getDetails()
				<< " , maxPos = " << maxPos.getDetails() 
				<< " , width = " << this->getWidth()
				<< " , height = " << this->getHeight()
				<< " )";

			return o.str();
		}

	public:

		/// ע��������Ӧ��set ��get����.
		ImagePos minPos;
		ImagePos maxPos;

		static const ImageRect ZERO;

	};

	#include "tvgImageRect.inl"
		
}

#endif // end of __TVGIMAGERECT_H__