/*
-----------------------------------------------------------------------------
  VRES(Virtual Reality GIS Engine System)    
  -------------------------------------------------------------
  License details ... waiting to add...
  -------------------------------------------------------------
  Copyright  (C) 2007 , leven - All Rights Reserved
-----------------------------------------------------------------------------
*/

#ifndef __TVGIMAGEPOS_H__
#define __TVGIMAGEPOS_H__

#include "vgPrerequisites.h"


namespace vgFoundationSystem {



	/**
		@date 	2007/04/28  21:04	
		@author  Leven
			
		@brief 	
			图像2D坐标类.用以表征图像上的某个点.注意使用时的坐标轴问题.
			
		@note 以后可以定义通用的template vector2 类,这样可以省去因为单位而引起的重写麻烦.
			
		@see    
	*/
	class ImagePos 
	{
	//private:
	public:

		/**
			声明为union,使访问的方式多样化.
		*/
		union
		{
			struct
			{  Integer x , y ;  };
			Integer v[2];
		};	

	public:

		inline ImagePos()
			: x( 0 ), y( 0 )
		{
		}

		inline ImagePos(const Integer fX, const Integer fY )
			: x( fX ), y( fY )
		{
		}
		inline explicit ImagePos( const Integer scaler )
			: x( scaler), y( scaler )
		{
		}

		/**
			重载[]号,内有assert检查.
		*/
		inline Integer operator [] ( const size_t i ) const;
		inline Integer& operator [] ( const size_t i );

		/**
			赋值运算符的重载.
		*/
		inline ImagePos& operator = ( const ImagePos& rkVector );
		inline ImagePos& operator = ( const Integer fScalar);

		/**
			比较运算符的重载.
		*/
		inline bool operator == ( const ImagePos& rkVector ) const;
		inline bool operator != ( const ImagePos& rkVector ) const;


		// arithmetic operations
		inline ImagePos operator + ( const ImagePos& rkVector ) const;
		inline ImagePos operator - ( const ImagePos& rkVector ) const;
		inline ImagePos operator * ( const Integer fScalar ) const;
		inline ImagePos operator * ( const ImagePos& rhs) const;
		inline ImagePos operator / ( const Integer fScalar ) const;
		inline ImagePos operator / ( const ImagePos& rhs) const;
		inline const ImagePos& operator + () const;
		inline ImagePos operator - () const;

		/**
			运算符的重载.
		*/
		inline friend ImagePos operator * ( const Integer fScalar, const ImagePos& rkVector );
		inline friend ImagePos operator / ( const Integer fScalar, const ImagePos& rkVector );
		inline friend ImagePos operator + (const ImagePos& lhs, const Integer rhs);
		inline friend ImagePos operator + (const Integer lhs, const ImagePos& rhs);
		inline friend ImagePos operator - (const ImagePos& lhs, const Integer rhs);
		inline friend ImagePos operator - (const Integer lhs, const ImagePos& rhs);

		/**
			高级算术运算符的重载.
		*/
		inline ImagePos& operator += ( const ImagePos& rkVector );
		inline ImagePos& operator += ( const Integer fScaler );
		inline ImagePos& operator -= ( const ImagePos& rkVector );
		inline ImagePos& operator -= ( const Integer fScaler );
		inline ImagePos& operator *= ( const Integer fScalar );
		inline ImagePos& operator *= ( const ImagePos& rkVector );
		inline ImagePos& operator /= ( const Integer fScalar );
		inline ImagePos& operator /= ( const ImagePos& rkVector );


		/**
			比较运算符的重载.
		*/
		inline bool operator < ( const ImagePos& rhs ) const;
		inline bool operator > ( const ImagePos& rhs ) const;

		inline bool operator <= ( const ImagePos& rhs ) const;
		inline bool operator >= ( const ImagePos& rhs ) const;


		/**
			得到具体的信息.
		*/
		String getDetails() const;

	public:
/*
		/// 常量.原点值.
		static const ImagePos ZERO;

		/// 常量.最大值.
		static const ImagePos MAX;

		/// 常量.最小值.
		static const ImagePos MIN;*/
	};

#include "tvgImagePos.inl"

	
}

#endif // end of __TVGIMAGEPOS_H__