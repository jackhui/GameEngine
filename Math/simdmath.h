#pragma once

/** Maths Libraray, defining vector, matrix and quaternion, with Intel SSE4.1 extensions and MSVC++ */

#include <xmmintrin.h> // intrinics
#include <smmintrin.h> // intrinics
#include <math.h> // sin cos
#include <DirectXMath.h> // DirectX helper methods
#include "..\Memory\MemoryManager.h"

#define PI 3.1415926535f

// 4x4 Matrix with SIMD
__declspec(align(16)) class SIMDMatrix4
{
private:
	__m128 _rows[4];
	// row-major matrix
public:
	friend class SIMDVector3;

	static const SIMDMatrix4 Identity;

	// Default constructor
	inline SIMDMatrix4(){};

	// Construct with given value
	inline SIMDMatrix4(float other[4][4])
	{
		_rows[0] = _mm_setr_ps(other[0][0], other[0][1], other[0][2], other[0][3]);
		_rows[1] = _mm_setr_ps(other[1][0], other[1][1], other[1][2], other[1][3]);
		_rows[2] = _mm_setr_ps(other[2][0], other[2][1], other[2][2], other[2][3]);
		_rows[3] = _mm_setr_ps(other[3][0], other[3][1], other[3][2], other[3][3]);
	}

	// Construct with given m128 data
	inline SIMDMatrix4(__m128 data[4])
	{
		_rows[0] = data[0];
		_rows[1] = data[1];
		_rows[2] = data[2];
		_rows[3] = data[3];
	}

	// Set the translation
	inline void setTranslate(const float& x, const float& y, const float& z)
	{
		_rows[0].m128_f32[3] = x;
		_rows[1].m128_f32[3] = y;
		_rows[2].m128_f32[3] = z;
	}


	// get translate vector x, y, z
	inline float getTranslateX() const
	{
		return _rows[0].m128_f32[3];
	}

	inline float getTranslateY() const
	{
		return _rows[1].m128_f32[3];
	}

	inline float getTranslateZ() const
	{
		return _rows[2].m128_f32[3];
	}

	// Copy constructor
	inline SIMDMatrix4(const SIMDMatrix4& other)
	{
		_rows[0] = other._rows[0];
		_rows[1] = other._rows[1];
		_rows[2] = other._rows[2];
		_rows[3] = other._rows[3];
	}

	// Assignment operator
	inline SIMDMatrix4& operator=(const SIMDMatrix4& other)
	{
		_rows[0] = other._rows[0];
		_rows[1] = other._rows[1];
		_rows[2] = other._rows[2];
		_rows[3] = other._rows[3];
		return *this;
	}

	// Set data values
	inline void Set(float other[4][4])
	{
		_rows[0] = _mm_setr_ps(other[0][0], other[0][1], other[0][2], other[0][3]);
		_rows[1] = _mm_setr_ps(other[1][0], other[1][1], other[1][2], other[1][3]);
		_rows[2] = _mm_setr_ps(other[2][0], other[2][1], other[2][2], other[2][3]);
		_rows[3] = _mm_setr_ps(other[3][0], other[3][1], other[3][2], other[3][3]);
	}

	// Add another matrix to the matrix, store the result back to this
	inline void Add(SIMDMatrix4& other)
	{
		_rows[0] = _mm_add_ps(_rows[0], other._rows[0]);
		_rows[1] = _mm_add_ps(_rows[1], other._rows[1]);
		_rows[2] = _mm_add_ps(_rows[2], other._rows[2]);
		_rows[3] = _mm_add_ps(_rows[3], other._rows[3]);
	}

	// Overload + operator
	inline SIMDMatrix4& operator+(SIMDMatrix4& other)
	{
		_rows[0] = _mm_add_ps(_rows[0], other._rows[0]);
		_rows[1] = _mm_add_ps(_rows[1], other._rows[1]);
		_rows[2] = _mm_add_ps(_rows[2], other._rows[2]);
		_rows[3] = _mm_add_ps(_rows[3], other._rows[3]);
		return *this;
	}

	// Overload += operator
	inline void operator+=(SIMDMatrix4& other)
	{
		_rows[0] = _mm_add_ps(_rows[0], other._rows[0]);
		_rows[1] = _mm_add_ps(_rows[1], other._rows[1]);
		_rows[2] = _mm_add_ps(_rows[2], other._rows[2]);
		_rows[3] = _mm_add_ps(_rows[3], other._rows[3]);
	}

	// Subtract the matrix by another matrix, store the result back to this
	inline void Sub(SIMDMatrix4& other)
	{
		_rows[0] = _mm_sub_ps(_rows[0], other._rows[0]);
		_rows[1] = _mm_sub_ps(_rows[1], other._rows[1]);
		_rows[2] = _mm_sub_ps(_rows[2], other._rows[2]);
		_rows[3] = _mm_sub_ps(_rows[3], other._rows[3]);
	}

	// Overload - operator
	inline SIMDMatrix4& operator-(SIMDMatrix4& other)
	{
		_rows[0] = _mm_sub_ps(_rows[0], other._rows[0]);
		_rows[1] = _mm_sub_ps(_rows[1], other._rows[1]);
		_rows[2] = _mm_sub_ps(_rows[2], other._rows[2]);
		_rows[3] = _mm_sub_ps(_rows[3], other._rows[3]);
		return *this;
	}

	// Overload -= operator
	inline void operator-=(SIMDMatrix4& other)
	{
		_rows[0] = _mm_sub_ps(_rows[0], other._rows[0]);
		_rows[1] = _mm_sub_ps(_rows[1], other._rows[1]);
		_rows[2] = _mm_sub_ps(_rows[2], other._rows[2]);
		_rows[3] = _mm_sub_ps(_rows[3], other._rows[3]);
	}

	// Multiply by another matrix, store the result back to this
	inline void Multiply(const SIMDMatrix4& mat)
	{
		__m128 mat_rows0 = mat._rows[0];
		__m128 mat_rows1 = mat._rows[1];
		__m128 mat_rows2 = mat._rows[2];
		__m128 mat_rows3 = mat._rows[3];
		_MM_TRANSPOSE4_PS(mat_rows0, mat_rows1, mat_rows2, mat_rows3);

		for (int i = 0; i < 4; ++i)
		{
			__m128 x = _mm_dp_ps(_rows[i], mat_rows0, 0xF1);
			__m128 y = _mm_dp_ps(_rows[i], mat_rows1, 0xF2);
			__m128 z = _mm_dp_ps(_rows[i], mat_rows2, 0xF4);
			__m128 w = _mm_dp_ps(_rows[i], mat_rows3, 0xF8);

			_rows[i] = _mm_add_ps(x, y);
			_rows[i] = _mm_add_ps(_rows[i], z);
			_rows[i] = _mm_add_ps(_rows[i], w);
		}
	}

	// Overload * operator
	inline SIMDMatrix4& operator*(const SIMDMatrix4& mat)
	{
		__m128 mat_rows0 = mat._rows[0];
		__m128 mat_rows1 = mat._rows[1];
		__m128 mat_rows2 = mat._rows[2];
		__m128 mat_rows3 = mat._rows[3];
		_MM_TRANSPOSE4_PS(mat_rows0, mat_rows1, mat_rows2, mat_rows3);

		for (int i = 0; i < 4; ++i)
		{
			__m128 x = _mm_dp_ps(_rows[i], mat_rows0, 0xF1);
			__m128 y = _mm_dp_ps(_rows[i], mat_rows1, 0xF2);
			__m128 z = _mm_dp_ps(_rows[i], mat_rows2, 0xF4);
			__m128 w = _mm_dp_ps(_rows[i], mat_rows3, 0xF8);

			_rows[i] = _mm_add_ps(x, y);
			_rows[i] = _mm_add_ps(_rows[i], z);
			_rows[i] = _mm_add_ps(_rows[i], w);
		}
		return *this;
	}

	// Overload *= operator
	inline void operator*=(const SIMDMatrix4& mat)
	{
		__m128 mat_rows0 = mat._rows[0];
		__m128 mat_rows1 = mat._rows[1];
		__m128 mat_rows2 = mat._rows[2];
		__m128 mat_rows3 = mat._rows[3];
		_MM_TRANSPOSE4_PS(mat_rows0, mat_rows1, mat_rows2, mat_rows3);

		for (int i = 0; i < 4; ++i)
		{
			__m128 x = _mm_dp_ps(_rows[i], mat_rows0, 0xF1);
			__m128 y = _mm_dp_ps(_rows[i], mat_rows1, 0xF2);
			__m128 z = _mm_dp_ps(_rows[i], mat_rows2, 0xF4);
			__m128 w = _mm_dp_ps(_rows[i], mat_rows3, 0xF8);

			_rows[i] = _mm_add_ps(x, y);
			_rows[i] = _mm_add_ps(_rows[i], z);
			_rows[i] = _mm_add_ps(_rows[i], w);
		}
	}

	// Set a scale transformation given a uniform scale
	inline void CreateScale(float scalar)
	{
		__m128 scale = _mm_set_ps1(scalar);

		_rows[0] = _mm_insert_ps(_rows[0], scale, 0x0E);
		_rows[1] = _mm_insert_ps(_rows[1], scale, 0x5D);
		_rows[2] = _mm_insert_ps(_rows[2], scale, 0xAB);
		_rows[3] = _mm_set_ps1(1.0f);
		_rows[3] = _mm_insert_ps(_rows[3], _rows[3], 0x07);
	}

	// Set a rotation transformation about the X axis given an angle in radian
	inline void CreateRotationX(float radian)
	{
		float cosTheta = cosf(radian);
		float sinTheta = sinf(radian);

		_rows[0] = _mm_set_ps1(1.0f);
		_rows[0] = _mm_insert_ps(_rows[0], _rows[0], 0x0E);
		_rows[1] = _mm_setr_ps(0.0f, cosTheta, -sinTheta, 0.0f);
		_rows[2] = _mm_setr_ps(0.0f, sinTheta, cosTheta, 0.0f);
		_rows[3] = _mm_set_ps1(1.0f);
		_rows[3] = _mm_insert_ps(_rows[3], _rows[3], 0x07);
	}

	// Set a rotation transformation about the Y axis given an angle in radian
	inline void CreateRotationY(float radian)
	{
		float cosTheta = cosf(radian);
		float sinTheta = sinf(radian);

		_rows[0] = _mm_setr_ps(cosTheta, 0.0f, sinTheta, 0.0f);
		_rows[1] = _mm_set_ps1(1.0f);
		_rows[1] = _mm_insert_ps(_rows[1], _rows[1], 0x0D);
		_rows[2] = _mm_setr_ps(-sinTheta, 0.0f, cosTheta, 0.0f);
		_rows[3] = _mm_set_ps1(1.0f);
		_rows[3] = _mm_insert_ps(_rows[3], _rows[3], 0x07);
	}

	// Set a rotation transformation about the Z axis given an angle in radian
	inline void CreateRotationZ(float radian)
	{
		float cosTheta = cosf(radian);
		float sinTheta = sinf(radian);

		_rows[0] = _mm_setr_ps(cosTheta, -sinTheta, 0.0f, 0.0f);
		_rows[1] = _mm_setr_ps(sinTheta, cosTheta, 0.0f, 0.0f);
		_rows[2] = _mm_set_ps1(1.0f);
		_rows[2] = _mm_insert_ps(_rows[2], _rows[2], 0x0B);
		_rows[3] = _mm_set_ps1(1.0f);
		_rows[3] = _mm_insert_ps(_rows[3], _rows[3], 0x07);
	}

	// Interpolate between two matrices with float t, return the resultant matrix
	// i.e. result = a * (1 - t) + b * t
	inline friend SIMDMatrix4 Lerp(const SIMDMatrix4& a, const SIMDMatrix4& b, float t)
	{
		__m128 resultRows[4];
		__m128 oneMinusT = _mm_set_ps1(1.0f - t);
		__m128 oneT = _mm_set_ps1(t);

		resultRows[0] = _mm_mul_ps(a._rows[0], oneMinusT);
		resultRows[0] = _mm_add_ps(resultRows[0], _mm_mul_ps(b._rows[0], oneT));
		resultRows[1] = _mm_mul_ps(a._rows[1], oneMinusT);
		resultRows[1] = _mm_add_ps(resultRows[1], _mm_mul_ps(b._rows[1], oneT));
		resultRows[2] = _mm_mul_ps(a._rows[2], oneMinusT);
		resultRows[2] = _mm_add_ps(resultRows[2], _mm_mul_ps(b._rows[2], oneT));
		resultRows[3] = _mm_mul_ps(a._rows[3], oneMinusT);
		resultRows[3] = _mm_add_ps(resultRows[3], _mm_mul_ps(b._rows[3], oneT));

		return SIMDMatrix4(resultRows);
	}

	// Set a translation transformation given a vector
	void CreateTranslation(const SIMDVector3& translation);

	// Set a rotation transformation given a quaternion
	// void CreateRotationFromQuaternion(const SIMDQuaternion& q);

	// Set a look-at matrix
	// vUp MUST be normalized or bad things will happen
	void CreateLookAt(const SIMDVector3& vEye, const SIMDVector3& vAt, const SIMDVector3& vUp);

	// Set a perspective FOV matrix
	void CreatePerspectiveFOV(float fFOVy, float fAspectRatio, float fNear, float fFar);

	// Inverts the matrix, store the result back to this
	void Invert();
};

// 3D Vector with SIMD
__declspec(align(16)) class SIMDVector3
{
private:
	__m128 _data;
	// _data.m128_f32[0] = x
	// _data.m128_f32[1] = y
	// _data.m128_f32[2] = z
	// _data.m128_f32[3] = w
	// row-vector
public:
	friend class SIMDMatrix4;
	friend class SIMDQuaternion;

	static const SIMDVector3 Zero;
	static const SIMDVector3 UnitX;
	static const SIMDVector3 UnitY;
	static const SIMDVector3 UnitZ;
	static const SIMDVector3 NegativeUnitX;
	static const SIMDVector3 NegativeUnitY;
	static const SIMDVector3 NegativeUnitZ;

	// Default Constructor
	inline SIMDVector3(){};

	// Overload new with handle
	void* operator new(size_t size, Handle hle)
	{
		return MemoryManager::GetInstance()->GetMemoryAddressFromHandle(hle);
	}

	void operator delete(void* ptr, Handle hle)
	{}

	// Construct with given float values
	inline SIMDVector3(float x, float y, float z, float w = 1.0f)
	{
		_data = _mm_setr_ps(x, y, z, w);
	};

	// Construct with given m128 data
	inline SIMDVector3(__m128 value)
	{
		_data = value;
	}

	// Copy constructor
	inline SIMDVector3(const SIMDVector3& other)
	{
		_data = other._data;
	}

	// Assignment constructor
	inline SIMDVector3& operator=(const SIMDVector3& other)
	{
		_data = other._data;
		return *this;
	}

	// Set data values
	inline void Set(float x, float y, float z)
	{
		_data = _mm_setr_ps(x, y, z, 1.0f);
	}

	inline void SetX(float x)
	{
		__m128 temp = _mm_set_ss(x);
		_data = _mm_insert_ps(_data, temp, 0x00);
	}

	inline void SetY(float y)
	{
		__m128 temp = _mm_set_ss(y);
		_data = _mm_insert_ps(_data, temp, 0x10);
	}

	inline void SetZ(float z)
	{
		__m128 temp = _mm_set_ss(z);
		_data = _mm_insert_ps(_data, temp, 0x20);
	}

	inline float GetX() const
	{
		return _data.m128_f32[0];
	}

	inline float GetY() const
	{
		return _data.m128_f32[1];
	}

	inline float GetZ() const
	{
		return _data.m128_f32[2];
	}

	inline float GetW() const
	{
		return _data.m128_f32[3];
	}

	// Dot product, return a float
	inline float Dot(const SIMDVector3& other)
	{
		__m128 temp = _mm_dp_ps(_data, other._data, 0x78);
		return	temp.m128_f32[3];
	}

	// Add two vector, store result back to this
	inline void Add(const SIMDVector3& other)
	{
		_data = _mm_add_ps(_data, other._data);
	}

	// Overload + operator
	inline SIMDVector3 operator+(const SIMDVector3& other)
	{
		SIMDVector3 result(_data);
		result._data = _mm_add_ps(_data, other._data);
		return result;
	}

	// Overload += operator
	inline void operator+=(const SIMDVector3& other)
	{
		_data = _mm_add_ps(_data, other._data);
	}

	// Substract the other vector from this, store result back to this
	inline void Substract(const SIMDVector3& other)
	{
		_data = _mm_sub_ps(_data, other._data);
	}

	// Overload - operator
	inline SIMDVector3 operator-(const SIMDVector3& other)
	{
		SIMDVector3 result(_data);
		result._data = _mm_sub_ps(_data, other._data);
		return result;
	}

	// Overload -= operator
	inline void operator-=(const SIMDVector3& other)
	{
		_data = _mm_sub_ps(_data, other._data);
	}

	// Multiple the vector by a scalar, stire result back to this
	inline void Multiply(float scalar)
	{
		__m128 temp = _mm_set_ps1(scalar);
		_data = _mm_mul_ps(_data, temp);
	}

	// Overload * operator
	inline SIMDVector3 operator*(float scalar)
	{
		SIMDVector3 result(_data);
		__m128 slr = _mm_set_ps1(scalar);
		result._data = _mm_mul_ps(_data, slr);
		return result;
	}

	// Normalize the vector, store result back to this
	/*
	inline void Normalize()
	{
		__m128 length = _mm_dp_ps(_data, _data, 0x77);
		length = _mm_rsqrt_ps(length);
		_data = _mm_mul_ps(_data, length);
	}
	*/

	inline SIMDVector3& Normalize()
	{
		__m128 length = _mm_dp_ps(_data, _data, 0x77);
		length = _mm_rsqrt_ps(length);
		_data = _mm_mul_ps(_data, length);
		return *this;
	}

	// Return the square of the length of vector
	inline float LengthSquared() const
	{
		__m128 length = _mm_dp_ps(_data, _data, 0x78);
		return length.m128_f32[3];
	}

	// Return the length of vector
	inline float Length() const
	{
		__m128 length = _mm_dp_ps(_data, _data, 0x78);
		return sqrtf(length.m128_f32[3]);
	}

	// Return the cross product as SIMDVector3 of two vectors
	inline friend SIMDVector3 CrossProduct(const SIMDVector3& a, const SIMDVector3& b)
	{
		__m128 te = _mm_shuffle_ps(a._data, a._data, _MM_SHUFFLE(3, 0, 2, 1));
		__m128 result = _mm_mul_ps(_mm_shuffle_ps(a._data, a._data, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b. _data, b._data, _MM_SHUFFLE(3, 1, 0, 2)));
		__m128 temp = _mm_mul_ps(_mm_shuffle_ps(a._data, a._data, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(b._data, b._data, _MM_SHUFFLE(3, 0, 2, 1)));
		result = _mm_sub_ps(result, temp);
		return SIMDVector3(result);
	}

	// Interpolate between two vectors with float t, return the resultant vector
	// i.e. result = a * (1 - t) + b * t
	inline friend SIMDVector3 Lerp(const SIMDVector3& a, const SIMDVector3& b, float t)
	{
		__m128 tempA = _mm_set_ps1(1.0f - t);
		tempA = _mm_mul_ps(a._data, tempA); 
		__m128 tempB = _mm_set_ps1(t);
		tempB = _mm_mul_ps(b._data, tempA);
		__m128 result = _mm_add_ps(tempA, tempB);
		return SIMDVector3(result);
	}

	// 4-way blend of vectors(colors), return the resultant vector
	// i.e. result = a * t1 + b * t2 + c * t3 + d * (1 - t1 -t2 - t3)
	inline friend SIMDVector3 Blend(const SIMDVector3& a, const SIMDVector3& b, const SIMDVector3& c, const SIMDVector3& d, float t1, float t2, float t3)
	{
		__m128 tempA = _mm_set_ps1(t1);
		tempA = _mm_mul_ps(a._data, tempA);
		__m128 tempB = _mm_set_ps1(t2);
		tempB = _mm_mul_ps(b._data, tempB);
		__m128 tempC = _mm_set_ps1(t3);
		tempC = _mm_mul_ps(c._data, tempC);
		__m128 tempD = _mm_set_ps1(1.0f - t1 - t2 - t3);
		tempD = _mm_mul_ps(d._data, tempD);

		__m128 result = _mm_add_ps(tempA, tempB);
		result = _mm_add_ps(result, tempC);
		result = _mm_add_ps(result, tempD);
		return SIMDVector3(result);
	}

	// Transform the vector as a point by a 4x4 Matrix, store result back to this
	inline void Transform(const SIMDMatrix4& mat)
	{
		// set w to 1.0f
		__m128 one = _mm_set_ss(1.0f);
		_data = _mm_insert_ps(_data, one, 0x30);

		__m128 x = _mm_dp_ps(_data, mat._rows[0], 0xF1);
		__m128 y = _mm_dp_ps(_data, mat._rows[1], 0xF2);
		__m128 z = _mm_dp_ps(_data, mat._rows[2], 0xF4);
		__m128 w = _mm_dp_ps(_data, mat._rows[3], 0xF8);

		_data = _mm_insert_ps(_data, x, 0x00);
		_data = _mm_insert_ps(_data, y, 0x50);
		_data = _mm_insert_ps(_data, z, 0xA0);
		_data = _mm_insert_ps(_data, w, 0xF0);
	}

	// Transform the vector by a 4x4 Matrix, store result back to this
	inline void TransformAsVector(const SIMDMatrix4& mat)
	{
		// Set w to 0.0f
		_data = _mm_insert_ps(_data, _data, 0x08);

		__m128 x = _mm_dp_ps(_data, mat._rows[0], 0xF1);
		__m128 y = _mm_dp_ps(_data, mat._rows[1], 0xF2);
		__m128 z = _mm_dp_ps(_data, mat._rows[2], 0xF4);
		__m128 w = _mm_dp_ps(_data, mat._rows[3], 0xF8);

		_data = _mm_insert_ps(_data, x, 0x00);
		_data = _mm_insert_ps(_data, y, 0x50);
		_data = _mm_insert_ps(_data, z, 0xA0);
		_data = _mm_insert_ps(_data, w, 0xF0);
	}
};

__declspec(align(16)) class SIMDQuaternion
{
private:
	__m128 _data;
	// _data.m128_f32[0] = x
	// _data.m128_f32[1] = y
	// _data.m128_f32[2] = z
	// _data.m128_f32[3] = w
public:
	friend class SIMDMatrix4;
	friend class SIMDVector3;

	static const SIMDQuaternion Identity;

	// Default constructor
	SIMDQuaternion(){};

	// Construct with given axis and angle in radian
	SIMDQuaternion(SIMDVector3& axis, float radian)
	{
		__m128 sinTheta = _mm_set_ps1(sinf(radian / 2.0f));
		__m128 cosTheta = _mm_set_ps1(cosf(radian / 2.0f));
		axis.Normalize();
		_data = _mm_mul_ps(axis._data, sinTheta);
		_data = _mm_insert_ps(_data, cosTheta, 0xF0);
	}

	// Copy constructor
	SIMDQuaternion(const SIMDQuaternion& other)
	{
		_data = other._data;
	}

	// Assignment operator
	inline SIMDQuaternion& operator=(const SIMDQuaternion& other)
	{
		_data = other._data;
		return *this;
	}

	inline float GetX() const
	{
		return _data.m128_f32[0];
	}

	inline float GetY() const
	{
		return _data.m128_f32[1];
	}

	inline float GetZ() const
	{
		return _data.m128_f32[2];
	}

	inline float GetW() const
	{
		return _data.m128_f32[3];
	}

	// Multiply this quaternion with another quaternion, store result back to this
	inline void Multiply(const SIMDQuaternion& other)
	{
		__m128 result;
		__m128 q2X = _mm_shuffle_ps(other._data, other._data, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 q2Y = _mm_shuffle_ps(other._data, other._data, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 q2Z = _mm_shuffle_ps(other._data, other._data, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 q2W = _mm_shuffle_ps(other._data, other._data, _MM_SHUFFLE(3, 3, 3, 3));

		q2X = _mm_mul_ps(q2X, _mm_shuffle_ps(_data, _data, _MM_SHUFFLE(0, 1, 2, 3)));
		q2Y = _mm_mul_ps(q2X, _mm_shuffle_ps(_data, _data, _MM_SHUFFLE(1, 0, 3, 2)));
		q2Z = _mm_mul_ps(q2X, _mm_shuffle_ps(_data, _data, _MM_SHUFFLE(2, 3, 0, 1)));
		q2W = _mm_mul_ps(q2W, _data);

		result = _mm_addsub_ps(_mm_shuffle_ps(q2W, q2W, _MM_SHUFFLE(2, 3, 0, 1)), _mm_shuffle_ps(q2X, q2X, _MM_SHUFFLE(2, 3, 0, 1)));
		result = _mm_addsub_ps(_mm_shuffle_ps(result, result, _MM_SHUFFLE(2, 0, 3, 1)), _mm_shuffle_ps(q2Y, q2Y, _MM_SHUFFLE(3, 1, 2, 0)));
		result = _mm_addsub_ps(_mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(q2Z, q2Z, _MM_SHUFFLE(2, 3, 1, 0)));
		result = _mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 2, 0, 1));
		_data = result;
	}

	inline void MultiplyDX(const SIMDQuaternion& other)
	{
		_data = DirectX::XMQuaternionMultiply(_data, other._data);
	}

	// Normalize the quaternion, store the result to this
	inline void Normalize()
	{
		__m128 length = _mm_dp_ps(_data, _data, 0x77);
		length = _mm_rsqrt_ps(length);
		_data = _mm_mul_ps(_data, length);
	}
};

