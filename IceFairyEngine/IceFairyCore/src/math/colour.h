// Robert Ambrose 2015

#ifndef __ice_fairy_colour_h__
#define __ice_fairy_colour_h__

#include <stdint.h>
#include <string>
#include <sstream>

#include "../core/utilities/icexception.h"

namespace IceFairy {
	class InvalidColourHexString : public ICException {
	public:
		InvalidColourHexString()
			: ICException("Invalid colour hex string") {
		}
	};

	// Basic colour class, takes values r, g, b.
	// Sample usage:
	//		Colour3<float> red(1.0f, 0.0f, 0.0f);
	//		Colour3<float> blue(0.0f, 0.0f, 1.0f);
	//		Colour3<float> purple = red * blue;
	template <class T>
	class Colour3 {
	public:
		T r, g, b;

		Colour3()
			: r(0),
			g(0),
			b(0) {
		}

		Colour3(T r, T g, T b)
			: r(r),
			g(g),
			b(b) {
		}

		Colour3 operator*(float scale) {
			return Colour3(r * scale, g * scale, b * scale);
		}

		Colour3 operator*(const Colour3& other) {
			return Colour3(r * other.r, g * other.g, b * other.b);
		}

		Colour3 operator+(const Colour3& other) {
			return Colour3(r + other.r, g + other.g, b + other.b);
		}

		Colour3 operator=(const Colour3& other) {
			if (this != &other) {
				r = other.r;
				g = other.g;
				b = other.b;
			}
			return *this;
		}

		bool operator==(const Colour3& other) {
			return r == other.r && g == other.g && b == other.b;
		}

		bool operator!=(const Colour3& other) {
			return !(*this == other);
		}

		// Returns the colour at the value interpolated between another colour.
		Colour3 Interpolate(Colour3 other, T t) {
			if (t >= 1) return other;
			else if (t <= 0) return *this;
			else return (*this * (1 - t)) + (other * t);
		}

		// Returns a colour with the given hex string.
		// e.g. the hex string FF0000 will return Colour3(1.0f, 0.0f, 0.0f).
		// Throws InvalidColourHexString exception for any invalid hex strings.
		static Colour3 HexToColour3(std::string hexString) {
			if (hexString.length() != 6) {
				throw InvalidColourHexString();
			}

			int32_t r, g, b;

			std::stringstream ss;
			ss << std::hex << hexString.substr(0, 2);
			ss >> r;
			ss.clear();
			ss << std::hex << hexString.substr(2, 2);
			ss >> g;
			ss.clear();
			ss << std::hex << hexString.substr(4, 2);
			ss >> b;

			return Colour3(r / (T) 255, g / (T) 255, b / (T) 255);
		}

		// Returns the hexidecimal representation of this colour.
		// e.g. Colour3(1.0f, 1.0f, 0.0f) becomes FFFF00.
		std::string ToHex(void) {
			std::string out;

			char r_c[255];
			sprintf_s(r_c, "%.2X", (int32_t) (r * (T) 255));
			out += r_c;

			char g_c[255];
			sprintf_s(g_c, "%.2X", (int32_t) (g * (T) 255));
			out += g_c;

			char b_c[255];
			sprintf_s(b_c, "%.2X", (int32_t) (b * (T) 255));
			out += b_c;

			return out;
		}
	};

	typedef Colour3<int>    Colour3i;
	typedef Colour3<float>  Colour3f;
	typedef Colour3<double> Colour3d;

	template <class T>
	bool operator==(const Colour3<T>& lhs, const Colour3<T>& rhs) {
		return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
	}

	// Similar to the basic Colour3 class but takes an aditional alpha
	// value takingvalues r, g, b, a in total.
	// Sample usage:
	//		Colour4 red(1.0f, 0.0f, 0.0f, 1.0f);
	//		Colour4 blue(0.0f, 0.0f, 1.0f, 1.0f);
	//		Colour4 purple = red * blue;
	template <class T>
	class Colour4 {
	public:
		T r, g, b, a;

		Colour4()
			: r(1),
			g(1),
			b(1),
			a(1) {
		}

		Colour4(Colour3<T> c, T alpha)
			: r(c.r),
			g(c.g),
			b(c.b),
			a(alpha) {
		}

		Colour4(T r, T g, T b)
			: r(r),
			g(g),
			b(b),
			a(1) {
		}

		Colour4(T r, T g, T b, T a)
			: r(r),
			g(g),
			b(b),
			a(a) {
		}

		Colour4 operator*(T scale) {
			return Colour4(r * scale, g * scale, b * scale, a * scale);
		}

		Colour4 operator*(const Colour4& other) {
			return Colour4(r * other.r, g * other.g, b * other.b, a * other.a);
		}

		Colour4 operator+(const Colour4& other) {
			return Colour4(r + other.r, g + other.g, b + other.b, a + other.a);
		}

		Colour4 operator=(const Colour4& other) {
			if (this != &other) {
				r = other.r;
				g = other.g;
				b = other.b;
				a = other.a;
			}
			return *this;
		}

		bool operator==(const Colour4& other) {
			return r == other.r && g == other.g && b == other.b && a == other.a;
		}

		bool operator!=(const Colour4& other) {
			return !(*this == other);
		}

		Colour3<T> ToColour3(void) {
			return Colour3<T>(r, g, b);
		}

		// Returns the colour at the value interpolated between another colour.
		Colour4 Interpolate(Colour4 other, T t) {
			if (t >= 1) return other;
			else if (t <= 0) return *this;
			else return (*this * (1 - t)) + (other * t);
		}
	};

	typedef Colour4<int>    Colour4i;
	typedef Colour4<float>  Colour4f;
	typedef Colour4<double> Colour4d;

	template <class T>
	bool operator==(const Colour4<T>& lhs, const Colour4<T>& rhs) {
		return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
	}
}

#endif /* __ice_fairy_colour_h__ */