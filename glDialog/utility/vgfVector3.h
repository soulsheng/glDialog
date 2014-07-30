/*
-----------------------------------------------------------------------------
  VRES(Virtual Reality GIS Engine System)    
  -------------------------------------------------------------
  License details ... waiting to add...
  -------------------------------------------------------------
  Copyright  (C) 2007 , leven - All Rights Reserved
-----------------------------------------------------------------------------
*/

#ifndef __VGFVECTOR3_H__
#define __VGFVECTOR3_H__

//#include "vgfPrerequisites.h"
//#include "vgfMath.h"
#include <sstream>
#include <string>
#include <assert.h>

namespace vgFoundationSystem {

	typedef float	Real;
	typedef std::string	String;
	/**
		@date 	2007/02/19  23:11	
		@author  leven
	
		@brief 	3Ԫ������.Ϊ���Ч��,�󲿷�ʹ��inlineʵ��,��������Vector3Impl.inl��
		����.
	
		@see   Matrix3 Vector2
	*/
	class Vector3
	{
	public:

		///��Ա����,�ô��ַ�ʽ��������߷��ʵķ�ʽ.
		union
		{
			struct
			{  Real x , y , z;  };
			Real v[3];
		};

	public:

		///���ֹ��캯��,����Ӧ��,ע��Ĭ�ϵ�Vector3������Ϊ0����.
		inline Vector3()
			: x( 0.0 ), y( 0.0 ), z( 0.0 )
		{
		}

		inline Vector3( const Real fX, const Real fY, const Real fZ )
			: x( fX ), y( fY ), z( fZ )
		{
		}

		inline explicit Vector3( const Real vec[3] )
			: x( vec[0] ),
			y( vec[1] ),
			z( vec[2] )
		{
		}

		inline explicit Vector3( const int vec[3] )
		{
			x = (Real)vec[0];
			y = (Real)vec[1];
			z = (Real)vec[2];
		}

		inline explicit Vector3( Real* const r )
			: x( r[0] ), y( r[1] ), z( r[2] )
		{
		}

		inline explicit Vector3( const Real scaler )
			: x( scaler )
			, y( scaler )
			, z( scaler )
		{
		}

		inline Vector3( const Vector3& rkVector )
			: x( rkVector.x ), y( rkVector.y ), z( rkVector.z )
		{
		}

		inline Real operator [] ( const size_t i ) const;
		inline Real& operator [] ( const size_t i );

		///���ַ����
		inline Real* ptr();
		inline const Real* ptr() const;

		///operator����������д
		inline Vector3& operator = ( const Vector3& rkVector );
		inline Vector3& operator = ( const Real fScaler );
		inline bool operator == ( const Vector3& rkVector ) const;
		inline bool operator != ( const Vector3& rkVector ) const;
		inline Vector3 operator + ( const Vector3& rkVector ) const;
		inline Vector3 operator - ( const Vector3& rkVector ) const;
		inline Vector3 operator * ( const Real fScalar ) const;

		///һ���ò���
		inline Vector3 operator * ( const Vector3& rhs) const;
		inline Vector3 operator / ( const Real fScalar ) const;
		inline Vector3 operator / ( const Vector3& rhs) const;

		///Ӧ��ע������:��������Ϊ��ʱ.
		inline const Vector3& operator + () const;
		inline Vector3 operator - () const;

		///��������Ϊ�������ʱ,ע��Ϊ��Ԫ����.
		inline friend Vector3 operator * ( const Real fScalar, const Vector3& rkVector );
		inline friend Vector3 operator / ( const Real fScalar, const Vector3& rkVector );
		inline friend Vector3 operator + (const Vector3& lhs, const Real rhs);
		inline friend Vector3 operator + (const Real lhs, const Vector3& rhs);
		inline friend Vector3 operator - (const Vector3& lhs, const Real rhs);
		inline friend Vector3 operator - (const Real lhs, const Vector3& rhs);

		///����߼���Ӧ��
		// arithmetic updates
		inline Vector3& operator += ( const Vector3& rkVector );
		inline Vector3& operator += ( const Real fScalar );
		inline Vector3& operator -= ( const Vector3& rkVector );
		inline Vector3& operator -= ( const Real fScalar );
		inline Vector3& operator *= ( const Real fScalar );
		inline Vector3& operator *= ( const Vector3& rkVector );
		inline Vector3& operator /= ( const Real fScalar );
		inline Vector3& operator /= ( const Vector3& rkVector );

		///���Ⱥ���
		/** Returns the length (magnitude) of the vector.
		@warning
		This operation requires a square root and is expensive in
		terms of CPU operations. If you don't need to know the exact
		length (e.g. for just comparing lengths) use squaredLength()
		instead.
		*/
		inline Real length () const;

		///ʱ���õ����ȵ�ƽ��,����Ƚϵ�ʱ��
		/** Returns the square of the length(magnitude) of the vector.
		@remarks
		This  method is for efficiency - calculating the actual
		length of a vector requires a square root, which is expensive
		in terms of the operations required. This method returns the
		square of the length of the vector, i.e. the same as the
		length but before the square root is taken. Use this if you
		want to find the longest / shortest vector without incurring
		the square root.
		*/
		inline Real squaredLength () const;


		///���뺯��,����2��������ľ���
		///ע���ʱ����cpu��Դ�ܴ�,�������Ϊ�˱Ƚ�2���ľ���,��squaredDistance()
		/** Returns the distance to another vector.
		@warning
		This operation requires a square root and is expensive in
		terms of CPU operations. If you don't need to know the exact
		distance (e.g. for just comparing distances) use squaredDistance()
		instead.
		*/
		inline Real distance(const Vector3& rhs) const;

		/** Returns the square of the distance to another vector.
		@remarks
		This method is for efficiency - calculating the actual
		distance to another vector requires a square root, which is
		expensive in terms of the operations required. This method
		returns the square of the distance to another vector, i.e.
		the same as the distance but before the square root is taken.
		Use this if you want to find the longest / shortest distance
		without incurring the square root.
		*/
		inline Real squaredDistance(const Vector3& rhs) const;


		///����2���ĵ��.
		/** Calculates the dot (scalar) product of this vector with another.
		@remarks
		The dot product can be used to calculate the angle between 2
		vectors. If both are unit vectors, the dot product is the
		cosine of the angle; otherwise the dot product must be
		divided by the product of the lengths of both vectors to get
		the cosine of the angle. This result can further be used to
		calculate the distance of a point from a plane.
		@param
		vec Vector with which to calculate the dot product (together
		with this one).
		@returns
		A float representing the dot product value.
		*/
		inline Real dotProduct(const Vector3& vec) const;

		///��׼������,ע��:			
		///1. ��ʱ���Զž�0���������.
		///2. ���з���ֵ,����ֵΪ��ǰ�����ĳ���,ע�ⲻ��vector3,��Ҫ����vector3 ,��ʹ��normalisedRef().
		/** Normalises the vector.
		@remarks
		This method normalises the vector such that it's
		length / magnitude is 1. The result is called a unit vector.
		@note
		This function will not crash for zero-sized vectors, but there
		will be no changes made to their components.
		@returns The previous length of the vector.
		*/
		inline Real normalise();

		///���ؾ�����λ������������.
		inline Vector3& normalisedRef();

		///������
		inline Vector3 crossProduct( const Vector3& rkVector ) const;

		///������������͵�һ��.
		/** Returns a vector at a point half way between this and the passed
		in vector.
		*/
		inline Vector3 midPoint( const Vector3& vec = ZERO ) const;

		///�ȽϺ���
		/** Returns true if the vector's scalar components are all greater
		that the ones of the vector it is compared against.
		*/
		inline bool operator < ( const Vector3& rhs ) const;

		/** Returns true if the vector's scalar components are all smaller
		that the ones of the vector it is compared against.
		*/
		inline bool operator > ( const Vector3& rhs ) const;

		///ȡ���������Ƚ϶��õ�����Сֵ,���Ҹ��豾��
		/** Sets this vector's components to the minimum of its own and the
		ones of the passed in vector.
		@remarks
		'Minimum' in this case means the combination of the lowest
		value of x, y and z from both vectors. Lowest is taken just
		numerically, not magnitude, so -1 < 0.
		*/
		inline Vector3 makeFloor( const Vector3& cmp );

		///ȡ���������Ƚ϶��õ������ֵ,���豾��,��������ֵ.
		/** Sets this vector's components to the maximum of its own and the
		ones of the passed in vector.
		@remarks
		'Maximum' in this case means the combination of the highest
		value of x, y and z from both vectors. Highest is taken just
		numerically, not magnitude, so 1 > -3.
		*/
		inline Vector3 makeCeil( const Vector3& cmp );
		inline Vector3 ceilProduct( const Vector3& c ) const;
		inline Vector3 floorProduct( const Vector3& c ) const;

		///�õ�һ����������.ע��,��ʱ��������������ȷ��,���õ�׼ȷ����������,��ʹ��4Ԫ��.
		/** Generates a vector perpendicular to this vector (eg an 'up' vector).
		@remarks
		This method will return a vector which is perpendicular to this
		vector. There are an infinite number of possibilities but this
		method will guarantee to generate one of them. If you need more
		control you should use the Quaternion class.
		*/
		inline Vector3 perpendicular(void) const;


		/** Returns true if this vector is zero length. */
		inline bool isZeroLength(void) const;

		/** As normalise, except that this vector is unaffected and the
		normalised vector is returned as a copy. */
		inline Vector3 normalisedCopy(void) const;

		///���㷴������
		///ע��:
		///1.��ʱnormalҪΪ��λ����
		///2.ͨ����������׺���������Ū��,��ע����Եķ���.
		/** Calculates a reflection vector to the plane with the given normal .
		@remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
		*/
		inline Vector3 reflect(const Vector3& normal) const;

		/** Returns whether this vector is within a positional tolerance
		of another vector.
		@param rhs The vector to compare with
		@param tolerance The amount that each element of the vector may vary by
		and still be considered equal
		*/
		inline bool operator <= ( const Vector3& rhs ) const;

		/** Returns true if the vector's scalar components are all smaller
		that the ones of the vector it is compared against.
		*/
		inline bool operator >= ( const Vector3& rhs ) const;


		///�����˵ķ�������
		inline Vector3 operator ^ ( const Vector3 &rhs ) const;

		///�������,�������
		inline /*VgfExport*/ friend std::ostream& operator <<
			( std::ostream& o, const Vector3& v );

		inline  void  reset( const Real& X ,
			const Real& Y ,const Real& Z )
		{
			this->x = X;
			this->y = Y;
			this->z = Z;
		}

		inline String getDetails() const
		{
			std::stringstream o;
			//o << "Vector3( " <<  x << " , " << y << " , " << z << " )";
			o << (*this);
			return o.str();
		}

		inline friend std::istream& operator >> ( std::istream& in , Vector3& v );

	public:			

		///�ر�ĵ�λ����,��Ϊ���泣��.
		// special points
		static const Vector3 ZERO;
		static const Vector3 UNIT_X;
		static const Vector3 UNIT_Y;
		static const Vector3 UNIT_Z;
		static const Vector3 NEGATIVE_UNIT_X;
		static const Vector3 NEGATIVE_UNIT_Y;
		static const Vector3 NEGATIVE_UNIT_Z;
		static const Vector3 UNIT_SCALE;

	};

	///inline ����ʵ��.
	#include "vgfVector3Impl.inl"
}

#endif // end of __VGFVECTOR3_H__