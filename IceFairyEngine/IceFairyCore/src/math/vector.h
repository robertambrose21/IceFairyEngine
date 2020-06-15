// TODO: Doc

// Robert Ambrose 2015

#ifndef __ice_fairy_vector_h__
#define __ice_fairy_vector_h__

// Allow use of M_PI for Visual C++
#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif

#include <string>
#include <sstream>
#include <math.h>
#include <exception>

#include "../core/utilities/icexception.h"

namespace IceFairy {
	template<class T>
	class Matrix4;

	class VectorOutOfBoundsException : public ICException {
	public:
		VectorOutOfBoundsException()
			: ICException("Vector subscript out of range") {
		}
	};

	// 2-space vector.
	// 2 dimensional vector class for useful mathematical operations, this class
	// may be doubly used to represent points.
	// Sample usage:
	//		float dotProduct = Vector2<float>(1.0f, 3.5f).Dot(Vector2<float>(2.2f, 0.8f));
	template <class T>
	class Vector2 {
	public:
		Vector2()
			: x(0),
			y(0) {
		}

		Vector2(T xy)
			: x(xy),
			y(xy) {
		}

		Vector2(T x, T y)
			: x(x),
			y(y) {
		}

		Vector2(T vals[2])
			: x(vals[0]),
			y(vals[1]) {
		}

		Vector2 operator=(const Vector2& other) {
			if (this != &other) {
				x = other.x;
				y = other.y;
			}
			return *this;
		}

		Vector2 operator+=(const Vector2& other) {
			x += other.x;
			y += other.y;
			return *this;
		}

		Vector2 operator-=(const Vector2& other) {
			x -= other.x;
			y -= other.y;
			return *this;
		}

		Vector2 operator*(const T scale) const {
			return Vector2(x * scale, y * scale);
		}

		Vector2 operator*(const Vector2& other) const {
			return Vector2(x * other.x, y * other.y);
		}

		Vector2 operator*=(const Vector2& other) const {
			x *= other.x;
			y *= other.y;
			return *this;
		}

		Vector2 operator/(const T scale) const {
			return Vector2(x / scale, y / scale);
		}

		Vector2 operator*=(const T scale) {
			x *= scale;
			y *= scale;
			return *this;
		}

		Vector2 operator/=(const T scale) {
			x /= scale;
			y /= scale;
			return *this;
		}

		Vector2 operator-(void) const {
			return Vector2(-x, -y);
		}

		bool operator==(const Vector2& other) const {
			return x == other.x && y == other.y;
		}

		bool operator!=(const Vector2& other) const {
			return !(*this == other);
		}

		// Returns the x or y element depending on input i.
		// Only values 0 or 1 may be used for axes x and y respectively.
		// Throws VectorOutOfBoundsException.
		T& operator[](int i) {
			if (i == 0)
				return x;
			else if (i == 1)
				return y;
			else
				throw VectorOutOfBoundsException();
		}

		// Returns the dot product of this vector and another.
		T Dot(const Vector2& other) const {
			return (x * other.x) + (y * other.y);
		}

		// Returns the perp dot product of this vector and another
		T DotPerp(const Vector2& other) const {
			return (x * other.y) - (y * other.x);
		}

		// Returns the length of this vector.
		T Length(void) const {
			return std::sqrt(this->Dot(*this));
		}
		// Returns the distance from this vector to another.
		T Distance(const Vector2& other) {
			return std::sqrt(DistanceSQ(other));
		}

		// Returns the squared distance from this vector to another.
		// This function is useful if you only want the distance for comparison
		// purposes as it removes the need for the computationally expensive sqrt.
		T DistanceSQ(const Vector2& other) {
			return std::pow(other.y - this->y, 2) + std::pow(other.x - this->x, 2);
		}

		// Returns the normalised version of this vector (while normalising this
		// vector itself).
		Vector2 Normalise(void) {
			if (*this != Vector2(0, 0))
				*this = *this / Length();

			return *this;
		}
		// Returns a vector which has been interpolated between this vector and
		// another at a given position 't'.
		Vector2 Interpolate(const Vector2& other, T t) {
			if (t >= 1) return other;
			else if (t <= 0) return *this;
			else return (*this * (1 - t)) + (other * t);
		}

		// Rotates this vector by a specific rotation in degrees
		Vector2 RotateBy(double rotation) {
			double radians = rotation * M_PI / 180.0;
			double s = sin(radians);
			double c = cos(radians);

			double _x = (((double) x) * c) - (((double) y) * s);
			double _y = (((double) y) * c) + (((double) x) * s);

			return Vector2((T) _x, (T) _y);
		}
		// Returns the projection of this vector onto another vector.
		Vector2 ProjectOnto(const Vector2& other) {
			T pLength = other.Length();
			return other * (other.Dot(*this) / (pLength * pLength));
		}

		// Returns the string format of this vector for easy debugging.
		// Format: Vector2(0, 0)
		std::string Str(void) const {
			std::stringstream out;
			out << "Vector2(" << x << ", " << y << ")";
			return out.str();
		}

		// Returns a T value based on the interpolation of vector c
		// between line <a, b>.
		// NOTE: Vector2 c must be on the line <a, b> or the outcome is undefined.
		// Should be the same result for every <x, y> field, so long
		// as the c vector is on the line.
		static T InverseInterpolate(Vector2 a, Vector2 b, Vector2 c) {
			return std::sqrt(c.DistanceSQ(a) / b.DistanceSQ(a));
		}

		T x;
		T y;
	};

	typedef Vector2<int>			Vector2i;
	typedef Vector2<unsigned int>	Vector2ui;
	typedef Vector2<float>			Vector2f;
	typedef Vector2<double>			Vector2d;

	// 3-space vector.
	// dimensional vector class for useful mathematical operations,
	// an extension of the Vector2 class. This class may be doubly
	// used to represent points.
	// Sample usage:
	//		Vector3<int> zAxis = Vector3<int>(0, 1, 0).Cross(Vector3<int>(1, 0, 0));
	template <class T>
	class Vector3 {
	public:
		Vector3()
			: x(0),
			y(0),
			z(0) {
		}

		Vector3(T xyz)
			: x(xyz),
			y(xyz),
			z(xyz) {
		}

		Vector3(T x, T y, T z)
			: x(x),
			y(y),
			z(z) {
		}

		Vector3(T vals[3])
			: x(vals[0]),
			y(vals[1]),
			z(vals[2]) {
		}

		Vector3(Vector2<T> v2, T z)
			: x(v2.x),
			y(v2.y),
			z(z) {
		}

		// Simple operator functions
		Vector3 operator=(const Vector3& other) {
			if (this != &other) {
				x = other.x;
				y = other.y;
				z = other.z;
			}
			return *this;
		}

		Vector3 operator+=(const Vector3& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		Vector3 operator-=(const Vector3& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		Vector3 operator*(const T scale) const {
			return Vector3(x * scale, y * scale, z * scale);
		}

		Vector3 operator*(const Vector3& other) const {
			return Vector3(x * other.x, y * other.y, z * other.z);
		}

		Vector3 operator*=(const Vector3& other) const {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		Vector3 operator/(const T scale) const {
			return Vector3(x / scale, y / scale, z / scale);
		}

		Vector3 operator*=(const T scale) {
			x *= scale;
			y *= scale;
			z *= scale;
			return *this;
		}

		Vector3 operator/=(const T scale) {
			x /= scale;
			y /= scale;
			z /= scale;
			return *this;
		}

		Vector3 operator-(void) const {
			return Vector3(-x, -y, -z);
		}

		bool operator==(const Vector3& other) const {
			return x == other.x && y == other.y && z == other.z;
		}

		bool operator!=(const Vector3& other) const {
			return !(*this == other);
		}

		// Returns the x, y or z element depending on input i.
		// Only values 0, 1 and 2 may be used for axes x, y and z respectively.
		// Throws VectorOutOfBoundsException.
		T& operator[](int i) {
			if (i == 0)
				return x;
			else if (i == 1)
				return y;
			else if (i == 2)
				return z;
			else
				throw VectorOutOfBoundsException();
		}

		// Returns the dot product of this vector and another.
		T Dot(const Vector3& other) const {
			return (x * other.x) + (y * other.y) + (z * other.z);
		}

		// Returns the cross product of this vector and another.
		Vector3 Cross(const Vector3& other) const {
			return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
		}

		// Returns the length of this vector.
		T Length(void) const {
			return std::sqrt(this->Dot(*this));
		}

		// Returns the distance from this vector to another.
		T Distance(const Vector3& other) {
			return std::sqrt(std::pow(other.x - this->x, 2) +
				std::pow(other.y - this->y, 2) +
				std::pow(other.z - this->z, 2));
		}

		// Returns the squared distance from this vector to another.
		// This function is useful if you only want the distance for comparison
		// purposes as it removes the need for the computationally expensive sqrt.
		T DistanceSQ(const Vector3& other) {
			return std::pow(other.x - this->x, 2) +
				std::pow(other.y - this->y, 2) +
				std::pow(other.z - this->z, 2);
		}

		// Returns the normalised version of this vector (while normalising this
		// vector itself).
		Vector3 Normalise(void) {
			if (*this != Vector3(0, 0, 0))
				*this = *this / Length();

			return *this;
		}

		// Returns a vector which has been interpolated between this vector and
		// another at a given position 't'.
		Vector3 Interpolate(const Vector3& other, T t) {
			if (t >= 1) return other;
			else if (t <= 0) return *this;
			else return (*this * (1 - t)) + (other * t);
		}

		// Returns the projection of this vector onto another vector.
		Vector3 ProjectOnto(const Vector3& other) {
			T pLength = other.Length();
			return other * (other.Dot(*this) / (pLength * pLength));
		}

		// Returns the string format of this vector for easy debugging.
		// Format: Vector3(0, 0, 0)
		std::string Str(void) const {
			std::stringstream out;
			out << "Vector3(" << x << ", " << y << ", " << z << ")";
			return out.str();
		}

		// Returns this 3D vector as a 2D vector, omitting the z value.
		Vector2<T> ToVector2(void) {
			return Vector2<T>(x, y);
		}

		// Returns a T value based on the interpolation of vector c
		// between line <a, b>.
		// NOTE: Vector2 c must be on the line <a, b> or the outcome is undefined.
		static T InverseInterpolate(Vector3 a, Vector3 b, Vector3 c) {
			return std::sqrt(c.DistanceSQ(a) / b.DistanceSQ(a));
		}

		// Returns a vector which has been bilinearly interpolated within four vectors.
		// In format of:
		// v3 --- v4
		// |      |
		// v1 --- v2
		static Vector3 BilinearInterpolate(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4, T s, T t) {
			Vector3 xlerp1 = v2.Interpolate(v1, s);
			Vector3 xlerp2 = v4.Interpolate(v3, s);

			return xlerp1.Interpolate(xlerp2, t);
		}

		T x;
		T y;
		T z;
	};

	typedef Vector3<int>			Vector3i;
	typedef Vector3<unsigned int>	Vector3ui;
	typedef Vector3<float>			Vector3f;
	typedef Vector3<double>			Vector3d;

	// 4-space vector.
	// 4 dimensional vector class for useful mathematical operations.
	// NOTE: This class is currently unfinished.
	template <class T>
	class Vector4 {
	public:
		Vector4()
			: x(0),
			y(0),
			z(0),
			w(0) {
		}

		Vector4(T xyzw)
			: x(xyzw),
			y(xyzw),
			z(xyzw),
			w(xyzw) {
		}

		Vector4(T x, T y, T z, T w)
			: x(x),
			y(y),
			z(z),
			w(w) {
		}

		Vector4(Vector3<T> v3, T w)
			: x(v3.x),
			y(v3.y),
			z(v3.z),
			w(w) {
		}

		Vector3<T> ToVector3(void) {
			return Vector3<T>(x, y, z);
		}

		T operator[](int i) const {
			if (i == 0)
				return x;
			else if (i == 1)
				return y;
			else if (i == 2)
				return z;
			else if (i == 3)
				return w;
			else
				throw VectorOutOfBoundsException();
		}

		bool operator==(const Vector4& other) const {
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}

		T x;
		T y;
		T z;
		T w;
	};

	typedef Vector4<int>    Vector4i;
	typedef Vector4<float>  Vector4f;
	typedef Vector4<double> Vector4d;

	template <class T>
	Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs) {
		return Vector3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	template <class T>
	Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs) {
		return Vector3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	template <class T>
	Vector3<T> operator*(const Vector3<T>& lhs, const Vector3<T>& rhs) {
		return Vector3<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
	}

	template <class T>
	Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs) {
		return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	template <class T>
	Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs) {
		return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	template <class T>
	Vector2<T> operator*(const Vector2<T>& lhs, const Vector2<T>& rhs) {
		return Vector2<T>(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	// 3-space ray.
	// 3 dimensional ray for ray casting operations.
	// Sample usage:
	//		Ray3 ray(eyePoint, eyePoint - lookAt);
	//		Vector3 objectPosition 
	template <class T>
	class Ray3 {
	public:
		Ray3() { }
		Ray3(Vector3<T> origin, Vector3<T> direction) {
			this->origin = origin;
			this->direction = direction.Normalise();
		}

		// Returns the point at t along the ray.
		Vector3<T> Position(T t) {
			return origin + direction * t;
		}

		Vector3<T> origin;
		Vector3<T> direction;
	};

	typedef Ray3<int>    Ray3i;
	typedef Ray3<float>  Ray3f;
	typedef Ray3<double> Ray3d;
};

#endif /* __ice_fairy_vector_h__ */
