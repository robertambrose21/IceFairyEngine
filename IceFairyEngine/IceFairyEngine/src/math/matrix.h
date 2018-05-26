// Robert Ambrose 2014

#ifndef __matrix_h__
#define __matrix_h__

#include <string>
#include <sstream>
#include <math.h>

#include "vector.h"
#include "../core/utilities/icexception.h"

// TODO: Move to a separate github and use glm instead (makes sense right?)

namespace IceFairy {
    class MatrixNoInverseExistsException : public ICException {
    public:
        MatrixNoInverseExistsException()
            : ICException("Vector subscript out of range")
        { }
    };

    // 3x3 Matrix class with various functionality.
    // TODO: Make Inverse return the actual inverse, not the identity.
    // Sample usage:
    //		Matrix3 m = Matrix3::Identity();
    //		Vector3 v = Vector3::ZERO * m;
    template <class T>
    class Matrix3 {
    public:
        // Creates a 3x3 0'd matrix
        Matrix3() {
            for (int i = 0; i < 9; i++) v[i] = 0.0f;
        }

        Matrix3(const T vals[9]) {
            for (int i = 0; i < 9; i++) v[i] = vals[i];
        }

        Matrix3(
            T a1, T a2, T a3,
            T b1, T b2, T b3,
            T c1, T c2, T c3
            )
        {
            v[0] = a1;
            v[1] = a2;
            v[2] = a3;
            v[3] = b1;
            v[4] = b2;
            v[5] = b3;
            v[6] = c1;
            v[7] = c2;
            v[8] = c3;
        }

        // Returns the 3x3 identity matrix.
        static Matrix3 Identity(void) {
            Matrix3 m;
            m.Val(0, 0) = m.Val(1, 1) = m.Val(2, 2) = 1.0f;
            return m;
        }

        // Returns the inverse of this matrix.
        Matrix3 Inverse(void) {
            T det = Val(0, 0) * (Val(1, 1) * Val(2, 2) - Val(2, 1) * Val(1, 2)) -
                    Val(0, 1) * (Val(1, 0) * Val(2, 2) - Val(1, 2) * Val(2, 0)) +
                    Val(0, 2) * (Val(1, 0) * Val(2, 1) - Val(1, 1) * Val(2, 0));

            if (det == 0) {
                throw MatrixNoInverseExistsException();
            }

            T invdet = 1 / det;

            Matrix3 m;
            m.Val(0, 0) = (Val(1, 1) * Val(2, 2) - Val(2, 1) * Val(1, 2)) * invdet;
            m.Val(0, 1) = (Val(0, 2) * Val(2, 1) - Val(0, 1) * Val(2, 2)) * invdet;
            m.Val(0, 2) = (Val(0, 1) * Val(1, 2) - Val(0, 2) * Val(1, 1)) * invdet;
            m.Val(1, 0) = (Val(1, 2) * Val(2, 0) - Val(1, 0) * Val(2, 2)) * invdet;
            m.Val(1, 1) = (Val(0, 0) * Val(2, 2) - Val(0, 2) * Val(2, 0)) * invdet;
            m.Val(1, 2) = (Val(1, 0) * Val(0, 2) - Val(0, 0) * Val(1, 2)) * invdet;
            m.Val(2, 0) = (Val(1, 0) * Val(2, 1) - Val(2, 0) * Val(1, 1)) * invdet;
            m.Val(2, 1) = (Val(2, 0) * Val(0, 1) - Val(0, 0) * Val(2, 1)) * invdet;
            m.Val(2, 2) = (Val(0, 0) * Val(1, 1) - Val(1, 0) * Val(0, 1)) * invdet;

            return m;
        }

        // Returns the transpose of this index.
        Matrix3 Transpose(void) {
            Matrix3 m;

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    m.Val(i, j) = Val(j, i);
                    m.Val(j, i) = Val(i, j);
                }
            }

            return m;
        }

        // Returns the dot product of this matrixes row given by rowIndex
        // with a vector 'v'.
        T Dot(int rowIndex, Vector3<T> v) {
            T finalVal = 0;

            for (int i = 0; i < 3; i++)
                finalVal += Val(i, rowIndex) * v[i];

            return finalVal;
        }

        // Returns the value at a point in the matrix e.g (1, 2)
        const T& Val(int x, int y) const {
            return v[y * 3 + x];
        }

        T& Val(int x, int y) {
            return v[y * 3 + x];
        }

        // Returns a string representation of this matrix for debugging.
        std::string Str(void) {
            std::stringstream out;

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    out << Val(i, j) << ", ";
                }
                out << '\n';
            }

            return out.str();
        }

        Matrix3 operator=(const Matrix3& rhs) {
            if (this != &rhs) {
                for (int i = 0; i < 9; i++) v[i] = rhs.v[i];
            }

            return *this;
        }

        Matrix3 operator+(const Matrix3& rhs) {
            Matrix3 m;

            for (int i = 0; i < 9; i++) {
                m.v[i] = v[i] + rhs.v[i];
            }

            return m;
        }

        Vector3<T> operator*(const Vector3<T>& rhs) {
            Vector3<T> product;

            product.x = Val(0, 0) * rhs.x + Val(1, 0) * rhs.y + Val(2, 0) * rhs.z;
            product.y = Val(0, 1) * rhs.x + Val(1, 1) * rhs.y + Val(2, 1) * rhs.z;
            product.z = Val(0, 2) * rhs.x + Val(1, 2) * rhs.y + Val(2, 2) * rhs.z;

            return product;
        }

        T operator[](unsigned int i) {
            return v[i];
        }

        T v[9];
    };

    template <class T>
    Matrix3<T> operator*(const Matrix3<T>& lhs, const Matrix3<T>& rhs) {
        Matrix3<T> m;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    m.Val(i, j) += lhs.Val(i, k) * rhs.Val(k, j);
                }
            }
        }

        return m;
    }

    template <class T>
    Matrix3<T> operator==(const Matrix3<T>& lhs, const Matrix3<T>& rhs) {
        for (unsigned int i = 0; i < 9; i++) {
            if (lhs[i] != rhs[i])
                return false;
        }

        return true;
    }

    typedef Matrix3<int>    Matrix3i;
    typedef Matrix3<float>  Matrix3f;
    typedef Matrix3<double> Matrix3d;

    // 4x4 Matrix class with various functionality.
    // Sample usage:
    //		Matrix4 rot = Matrix4::Rotate(90.0f, 0, 1, 0);
    //		Vector3 rotatedPosition = position * rot;
    template <class T>
    class Matrix4 {
    public:
        // Create a 4x4 0'd matrix
        Matrix4() {
            for (int i = 0; i < 16; i++) v[i] = 0.0f;
        }

        Matrix4(const T vals[16]) {
            for (int i = 0; i < 16; i++) v[i] = vals[i];
        }

        Matrix4(
            T a1, T a2, T a3, T a4,
            T b1, T b2, T b3, T b4,
            T c1, T c2, T c3, T c4,
            T d1, T d2, T d3, T d4
            ) 
        {
            v[0] = a1;
            v[1] = b1;
            v[2] = c1;
            v[3] = d1;

            v[4] = a2;
            v[5] = b2;
            v[6] = c2;
            v[7] = d2;

            v[8] = a3;
            v[9] = b3;
            v[10] = c3;
            v[11] = d3;

            v[12] = a4;
            v[13] = b4;
            v[14] = c4;
            v[15] = d4;
        }

        // Returns the 4x4 identity matrix.
        static Matrix4 Identity(void) {
            Matrix4 m;
            m.Val(0, 0) = m.Val(1, 1) = m.Val(2, 2) = m.Val(3, 3) = 1.0f;
            return m;
        }

        // Returns the orthogrpahic projection matrix given by the listed parameters.
        static Matrix4<float> Ortho(float l, float r, float b, float t, float n, float f) {
            return Matrix4<float>(
                2 / (r - l), 0.0f,           0.0f,            -(r + l) / (r - l),
                0.0f,        2.0f / (t - b), 0.0f,            -(t + b) / (t - b),
                0.0f,        0.0f,           -2.0f / (f - n), -(f + n) / (f - n),
                0.0f,        0.0f,           0.0f,            1.0f);
        }

        // Returns the frustum projection matrix given by the listed parameters.
        static Matrix4<float> Frustum(float left, float right, float bottom, float top, float n, float f) {
            Matrix4<float> m;
        
            m.Val(0, 0) = (2.0f * n) / (right - left);
            m.Val(2, 0) = (right + left) / (right - left);

            m.Val(1, 1) = (2.0f * n) / (top - bottom);
            m.Val(2, 1) = (top + bottom) / (top - bottom);

            m.Val(2, 2) = -(f + n) / (f - n);
            m.Val(3, 2) = (-2.0f * f * n) / (f - n);

            m.Val(2, 3) = -1.0f;

            return m;
        }
        // Returns the perspective projection matrix with a given field of view, aspect ratio
        // and near and far distances.
        static Matrix4<float> Perspective(float fov, float aspect, float n, float f) {
            float y = n * tan(fov * (float) M_PI / 180.0f / 2.0f);
            float x = y * aspect;
            return Frustum(-x, x, -y, y, n, f);
        }

        static Matrix4<float> LookAt(const Vector3f& eye, const Vector3f& centre, const Vector3f& up) {
            Matrix4<float> m = Identity();

            Vector3f forward = (centre - eye).Normalise();
            Vector3f upNorm = up;
            upNorm = upNorm.Normalise();
            Vector3f right = forward.Cross(upNorm).Normalise();
            upNorm = right.Cross(forward);

            m.Val(0, 0) = right.x;
            m.Val(1, 0) = right.y;
            m.Val(2, 0) = right.z;

            m.Val(0, 1) = upNorm.x;
            m.Val(1, 1) = upNorm.y;
            m.Val(2, 1) = upNorm.z;

            m.Val(0, 2) = -forward.x;
            m.Val(1, 2) = -forward.y;
            m.Val(2, 2) = -forward.z;

            m.Val(3, 0) = -right.Dot(eye);
            m.Val(3, 1) = -upNorm.Dot(eye);
            m.Val(3, 2) = forward.Dot(eye);

            return m;
        }

        // Returns a Ray3 object UnProjected from a given screen point, projection matrix
        // and the screen dimensions. The 'origin' of the Ray3 can be used as a world coordinate
        // for orthographic projections.
        static Ray3<T> UnProject(Vector2<T> screen, Matrix4 projection, unsigned int screenWidth, unsigned int screenHeight) {
            float winx = screen.x;
            float winy = screen.y;
            float winz = 0.0f;

            Matrix4 inv = projection.Inverse();

            // Get origin
            T vx = ((2 * winx) / screenWidth) - 1;
            T vy = ((2 * (screenHeight - winy)) / screenHeight) - 1;
            T vz = 2 * winz - 1;

            Vector4 v(vx, vy, vz, 1);
            T x = inv.Dot(0, v);
            T y = inv.Dot(1, v);
            T z = inv.Dot(2, v);
            T w = inv.Dot(3, v);

            float d = 1 / w;

            Vector3 origin(x*d, y*d, z*d);

            // Get direction
            winz = 1;

            vx = ((2 * winx) / screenWidth) - 1;
            vy = ((2 * (screenHeight - winy)) / (T)screenHeight) - 1;
            vz = 2 * winz - 1;

            v = Vector4(vx, vy, vz, 1);
            x = inv.Dot(0, v);
            y = inv.Dot(1, v);
            z = inv.Dot(2, v);
            w = inv.Dot(3, v);

            d = 1 / w;

            return Ray3<T>(origin, Vector3<T>(x*d, y*d, z*d) - origin);
        }

        // Returns a screen point from a given world position, projection matrix and the screen dimensions.
        static Vector2<T> Project(Vector3<T> world, Matrix4 projection, unsigned int screenWidth, unsigned int screenHeight) {
            Vector4<T> v(world, 1.0f);

            v = projection * v;

            if (v[3] != 0.0f) {
                v = Vector4<T>(v[0] / v[3], v[1] / v[3], v[2] / v[3], v[3]);

                v[0] = v[0] * 0.5 + 0.5;
                v[1] = v[1] * 0.5 + 0.5;
                v[2] = v[2] * 0.5 + 0.5;

                v[0] = v[0] * screenWidth;
                v[1] = screenHeight - (v[1] * screenHeight);
            }

            return v.ToVector3().ToVector2();
        }

        // Returns a scale transformation matrix given by the parameters x, y and z.
        static Matrix4 Scale(T x, T y, T z) {
            Matrix4 m = Identity();

            m.Val(0, 0) = x;
            m.Val(1, 1) = y;
            m.Val(2, 2) = z;

            return m;
        }

        // Returns a translation transformation matrix given by the parameters x, y and z.
        static Matrix4 Translate(T x, T y, T z) {
            Matrix4 m = Identity();

            m.Val(3, 0) = x;
            m.Val(3, 1) = y;
            m.Val(3, 2) = z;

            return m;
        }

        // Returns a rotation transformation matrix. Similar to OpenGLs glRotate function.
        // Degrees determines the amount of rotation and the axis to rotate around is given
        // by setting the appopriate parameter to 1 and the rest to 0.
        // Example, rotate around the y axis by 90 degrees:
        //		Matrix4 rot = Matrix4::Rotate(90.0f, 0, 1, 0);
        static Matrix4 Rotate(T degrees, T x, T y, T z) {
            Matrix4 m = Identity();

            T radians = degrees * (T) (M_PI / 180.0);
            T s = sin(radians);
            T c = cos(radians);

            m.Val(0, 0) = (T) (1 - c) * x * x + c;
            m.Val(0, 1) = (T) (1 - c) * x * y - s * z;
            m.Val(0, 2) = (T) (1 - c) * x * z + s * y;
            m.Val(1, 0) = (T) (1 - c) * y * x + s * z;
            m.Val(1, 1) = (T) (1 - c) * y * y + c;
            m.Val(1, 2) = (T) (1 - c) * y * z - s * x;
            m.Val(2, 0) = (T) (1 - c) * z * x - s * y;
            m.Val(2, 1) = (T) (1 - c) * z * y + s * x;
            m.Val(2, 2) = (T) (1 - c) * z * z + c;

            return m;
        }

        // Returns a scale transformation matrix given by a vector.
        static Matrix4 Scale(const Vector3<T>& scale) {
            return Matrix4::Scale(scale.x, scale.y, scale.z);
        }

        // Returns a translation transformation matrix given by a vector.
        static Matrix4 Translate(const Vector3<T>& translate) {
            return Matrix4::Translate(translate.x, translate.y, translate.z);
        }

        // Returns a rotation transformation matrix, similar to above but using a Vector3
        // axis in place of x, y, z parameters.
        static Matrix4 Rotate(T degrees, const Vector3<T>& rotate) {
            return Matrix4::Rotate(degrees, rotate.x, rotate.y, rotate.z);
        }

        // Returns a rotation transformation matrix using a quaternion.
        // TODO
        //static Matrix4 QuarternionRotate(btQuaternion& quaternion);

        // Returns the inverse of this matrix.
        Matrix4 Inverse(void) {
            T inv[16], invOut[16], det;

            inv[0] = v[5] * v[10] * v[15] -
                v[5] * v[11] * v[14] -
                v[9] * v[6] * v[15] +
                v[9] * v[7] * v[14] +
                v[13] * v[6] * v[11] -
                v[13] * v[7] * v[10];

            inv[4] = -v[4] * v[10] * v[15] +
                v[4] * v[11] * v[14] +
                v[8] * v[6] * v[15] -
                v[8] * v[7] * v[14] -
                v[12] * v[6] * v[11] +
                v[12] * v[7] * v[10];

            inv[8] = v[4] * v[9] * v[15] -
                v[4] * v[11] * v[13] -
                v[8] * v[5] * v[15] +
                v[8] * v[7] * v[13] +
                v[12] * v[5] * v[11] -
                v[12] * v[7] * v[9];

            inv[12] = -v[4] * v[9] * v[14] +
                v[4] * v[10] * v[13] +
                v[8] * v[5] * v[14] -
                v[8] * v[6] * v[13] -
                v[12] * v[5] * v[10] +
                v[12] * v[6] * v[9];

            inv[1] = -v[1] * v[10] * v[15] +
                v[1] * v[11] * v[14] +
                v[9] * v[2] * v[15] -
                v[9] * v[3] * v[14] -
                v[13] * v[2] * v[11] +
                v[13] * v[3] * v[10];

            inv[5] = v[0] * v[10] * v[15] -
                v[0] * v[11] * v[14] -
                v[8] * v[2] * v[15] +
                v[8] * v[3] * v[14] +
                v[12] * v[2] * v[11] -
                v[12] * v[3] * v[10];

            inv[9] = -v[0] * v[9] * v[15] +
                v[0] * v[11] * v[13] +
                v[8] * v[1] * v[15] -
                v[8] * v[3] * v[13] -
                v[12] * v[1] * v[11] +
                v[12] * v[3] * v[9];

            inv[13] = v[0] * v[9] * v[14] -
                v[0] * v[10] * v[13] -
                v[8] * v[1] * v[14] +
                v[8] * v[2] * v[13] +
                v[12] * v[1] * v[10] -
                v[12] * v[2] * v[9];

            inv[2] = v[1] * v[6] * v[15] -
                v[1] * v[7] * v[14] -
                v[5] * v[2] * v[15] +
                v[5] * v[3] * v[14] +
                v[13] * v[2] * v[7] -
                v[13] * v[3] * v[6];

            inv[6] = -v[0] * v[6] * v[15] +
                v[0] * v[7] * v[14] +
                v[4] * v[2] * v[15] -
                v[4] * v[3] * v[14] -
                v[12] * v[2] * v[7] +
                v[12] * v[3] * v[6];

            inv[10] = v[0] * v[5] * v[15] -
                v[0] * v[7] * v[13] -
                v[4] * v[1] * v[15] +
                v[4] * v[3] * v[13] +
                v[12] * v[1] * v[7] -
                v[12] * v[3] * v[5];

            inv[14] = -v[0] * v[5] * v[14] +
                v[0] * v[6] * v[13] +
                v[4] * v[1] * v[14] -
                v[4] * v[2] * v[13] -
                v[12] * v[1] * v[6] +
                v[12] * v[2] * v[5];

            inv[3] = -v[1] * v[6] * v[11] +
                v[1] * v[7] * v[10] +
                v[5] * v[2] * v[11] -
                v[5] * v[3] * v[10] -
                v[9] * v[2] * v[7] +
                v[9] * v[3] * v[6];

            inv[7] = v[0] * v[6] * v[11] -
                v[0] * v[7] * v[10] -
                v[4] * v[2] * v[11] +
                v[4] * v[3] * v[10] +
                v[8] * v[2] * v[7] -
                v[8] * v[3] * v[6];

            inv[11] = -v[0] * v[5] * v[11] +
                v[0] * v[7] * v[9] +
                v[4] * v[1] * v[11] -
                v[4] * v[3] * v[9] -
                v[8] * v[1] * v[7] +
                v[8] * v[3] * v[5];

            inv[15] = v[0] * v[5] * v[10] -
                v[0] * v[6] * v[9] -
                v[4] * v[1] * v[10] +
                v[4] * v[2] * v[9] +
                v[8] * v[1] * v[6] -
                v[8] * v[2] * v[5];

            det = v[0] * inv[0] + v[1] * inv[4] + v[2] * inv[8] + v[3] * inv[12];

            if (det == 0) {
                throw MatrixNoInverseExistsException();
            }

            det = 1.0f / det;

            for (int i = 0; i < 16; i++)
                invOut[i] = inv[i] * det;

            return Matrix4(invOut);
        }

        // Returns the transpose of this matrix.
        Matrix4 Transpose(void) {
            Matrix4 m;

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    m.Val(i, j) = Val(j, i);
                    m.Val(j, i) = Val(i, j);
                }
            }

            return m;
        }

        // Returns the dot product of this matrixes row given by rowIndex
        // with a vector 'v'.
        T Dot(int rowIndex, const Vector4<T>& v) {
            T finalVal = 0.0f;

            for (int i = 0; i < 4; i++)
                finalVal += Val(i, rowIndex) * v[i];

            return finalVal;
        }

        // Returns the value at a point in the matrix e.g (1, 2)
        const T& Val(int x, int y) const {
            return v[x * 4 + y];
        }

        T& Val(int x, int y) {
            return v[x * 4 + y];
        }

        // Returns this matrix as a btTransform matrix
        // TODO
        //btTransform ToBtMatrix(void);

        // Returns a string representation of this matrix for debugging.
        std::string Str(void) {
            std::stringstream out;

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    out << Val(j, i) << ", ";
                }
                out << '\n';
            }

            return out.str();
        }

        Matrix4 operator=(const Matrix4& rhs) {
            if (this != &rhs) {
                for (int i = 0; i < 16; i++) v[i] = rhs.v[i];
            }
            return *this;
        }

        Matrix4 operator*=(const Matrix4& rhs) {
            *this = *this * rhs;
            return *this;
        }

        Matrix4 operator+(const Matrix4& rhs) {
            Matrix4 m;

            for (int i = 0; i < 16; i++) {
                m.v[i] = v[i] + rhs.v[i];
            }

            return m;
        }

        Matrix4 operator*(const T& rhs) {
            if (this != &rhs) {
                for (int i = 0; i < 16; i++) v[i] = rhs.v[i];
            }
            return *this;
        }

        Vector3<T> operator*(const Vector3<T>& rhs) {
            Vector3<T> product;

            product.x = Val(0, 0) * rhs.x + Val(1, 0) * rhs.y + Val(2, 0) * rhs.z;
            product.y = Val(0, 1) * rhs.x + Val(1, 1) * rhs.y + Val(2, 1) * rhs.z;
            product.z = Val(0, 2) * rhs.x + Val(1, 2) * rhs.y + Val(2, 2) * rhs.z;

            return product;
        }

        Vector4<T> operator*(const Vector4<T>& rhs) {
            T x = Val(0, 0) * rhs[0] + Val(1, 0) * rhs[1] + Val(2, 0) * rhs[2] + Val(3, 0) * rhs[3];
            T y = Val(0, 1) * rhs[0] + Val(1, 1) * rhs[1] + Val(2, 1) * rhs[2] + Val(3, 1) * rhs[3];
            T z = Val(0, 2) * rhs[0] + Val(1, 2) * rhs[1] + Val(2, 2) * rhs[2] + Val(3, 2) * rhs[3];
            T w = Val(0, 3) * rhs[0] + Val(1, 3) * rhs[1] + Val(2, 3) * rhs[2] + Val(3, 3) * rhs[3];

            return Vector4<T>(x, y, z, w);
        }

        T operator[](unsigned int i) {
            return v[i];
        }

        T v[16];
    };

    template <class T>
    Matrix4<T> operator*(const Matrix4<T>& lhs, const Matrix4<T>& rhs) {
        Matrix4<T> m;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    m.Val(j, i) += lhs.Val(k, i) * rhs.Val(j, k);
                }
            }
        }

        return m;
    }

    template <class T>
    Matrix4<T> operator==(const Matrix4<T>& lhs, const Matrix4<T>& rhs) {
        for (unsigned int i = 0; i < 16; i++) {
            if (lhs[i] != rhs[i])
                return false;
        }

        return true;
    }

    typedef Matrix4<int>    Matrix4i;
    typedef Matrix4<float>  Matrix4f;
    typedef Matrix4<double> Matrix4d;
}

#endif /* __matrix_h__ */
