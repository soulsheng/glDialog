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
			ͼ��2D������.���Ա���ͼ���ϵ�ĳ����.ע��ʹ��ʱ������������.
			
		@note �Ժ���Զ���ͨ�õ�template vector2 ��,��������ʡȥ��Ϊ��λ���������д�鷳.
			
		@see    
	*/
	class ImagePos 
	{
	//private:
	public:

		/**
			����Ϊunion,ʹ���ʵķ�ʽ������.
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
			����[]��,����assert���.
		*/
		inline Integer operator [] ( const size_t i ) const;
		inline Integer& operator [] ( const size_t i );

		/**
			��ֵ�����������.
		*/
		inline ImagePos& operator = ( const ImagePos& rkVector );
		inline ImagePos& operator = ( const Integer fScalar);

		/**
			�Ƚ������������.
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
			�����������.
		*/
		inline friend ImagePos operator * ( const Integer fScalar, const ImagePos& rkVector );
		inline friend ImagePos operator / ( const Integer fScalar, const ImagePos& rkVector );
		inline friend ImagePos operator + (const ImagePos& lhs, const Integer rhs);
		inline friend ImagePos operator + (const Integer lhs, const ImagePos& rhs);
		inline friend ImagePos operator - (const ImagePos& lhs, const Integer rhs);
		inline friend ImagePos operator - (const Integer lhs, const ImagePos& rhs);

		/**
			�߼����������������.
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
			�Ƚ������������.
		*/
		inline bool operator < ( const ImagePos& rhs ) const;
		inline bool operator > ( const ImagePos& rhs ) const;

		inline bool operator <= ( const ImagePos& rhs ) const;
		inline bool operator >= ( const ImagePos& rhs ) const;


		/**
			�õ��������Ϣ.
		*/
		String getDetails() const;

	public:
/*
		/// ����.ԭ��ֵ.
		static const ImagePos ZERO;

		/// ����.���ֵ.
		static const ImagePos MAX;

		/// ����.��Сֵ.
		static const ImagePos MIN;*/
	};

#include "tvgImagePos.inl"

	
}

#endif // end of __TVGIMAGEPOS_H__