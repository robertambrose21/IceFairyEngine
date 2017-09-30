#include "matrixTest.h"

TEST(Matrix3, Inverse) {
    IceFairy::Matrix3f m(
        1, 2, 3,
        4, 5, 6,
        7, 8, 7);

    IceFairy::Matrix3f mInv = m.Inverse();

    M3_FuzzyMatch(IceFairy::Matrix3f::Identity(), m * mInv);
}

TEST(Matrix3, NoInverse) {
    IceFairy::Matrix3f m(
        1, 2, 3,
        4, 5, 6,
        7, 8, 9);

    ASSERT_THROW(m.Inverse(), IceFairy::MatrixNoInverseExistsException);
}

TEST(Matrix3, Transpose) {
    IceFairy::Matrix3f m(
        1, 2, 3,
        4, 5, 6,
        7, 8, 9);

    IceFairy::Matrix3f expected(
        1, 4, 7,
        2, 5, 8,
        3, 6, 9);

    M3_Match(expected, m.Transpose());
}

TEST(Matrix3, DotProduct) {
    IceFairy::Matrix3f m(
        1, 2, 3,
        4, 5, 6,
        7, 8, 9);

    IceFairy::Vector3f v(1, 2, 3);

    EXPECT_EQ(14, m.Dot(0, v));
    EXPECT_EQ(32, m.Dot(1, v));
    EXPECT_EQ(50, m.Dot(2, v));
}

TEST(Matrix3, Val) {
    IceFairy::Matrix3f m(
        1, 2, 3,
        4, 5, 6,
        7, 8, 9);

    EXPECT_EQ(4, m.Val(0, 1));
    EXPECT_EQ(8, m.Val(1, 2));
    EXPECT_EQ(6, m.Val(2, 1));
}

TEST(Matrix3, MatrixMultiplication) {
    IceFairy::Matrix3f m(
        1, 2, 3,
        4, 5, 6,
        7, 8, 9);

    IceFairy::Matrix3f expected(
        30,  36,  42,
        66,  81,  96,
        102, 126, 150);

    M3_Match(expected, m * m);
}

TEST(Matrix3, VectorMultiplication) {
    IceFairy::Matrix3f m(
        1, 2, 3,
        4, 5, 6,
        7, 8, 9);

    EXPECT_EQ(IceFairy::Vector3f(14, 32, 50), m * IceFairy::Vector3f(1, 2, 3));
}

TEST(Matrix4, Inverse) {
    IceFairy::Matrix4f m(
        1, 2, 3, 1,
        4, 5, 6, 2,
        7, 8, 7, 3,
        9, 2, 3, 2);

    IceFairy::Matrix4f mInv = m.Inverse();

    M4_FuzzyMatch(IceFairy::Matrix4f::Identity(), m * mInv);
}

TEST(Matrix4, NoInverse) {
    IceFairy::Matrix4f m(
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16);

    ASSERT_THROW(m.Inverse(), IceFairy::MatrixNoInverseExistsException);
}

TEST(Matrix4, Transpose) {
    IceFairy::Matrix4f m(
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16);

    IceFairy::Matrix4f expected(
        1, 5, 9,  13,
        2, 6, 10, 14,
        3, 7, 11, 15,
        4, 8, 12, 16);

    M4_Match(expected, m.Transpose());
}

TEST(Matrix4, DotProduct) {
    IceFairy::Matrix4f m(
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16);

    IceFairy::Vector4f v(1, 2, 3, 4);

    EXPECT_EQ(30, m.Dot(0, v));
    EXPECT_EQ(70, m.Dot(1, v));
    EXPECT_EQ(110, m.Dot(2, v));
    EXPECT_EQ(150, m.Dot(3, v));
}

TEST(Matrix4, Val) {
    IceFairy::Matrix4f m(
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16);

    EXPECT_EQ(5, m.Val(0, 1));
    EXPECT_EQ(10, m.Val(1, 2));
    EXPECT_EQ(15, m.Val(2, 3));
    EXPECT_EQ(16, m.Val(3, 3));
}

TEST(Matrix4, MatrixMultiplication) {
    IceFairy::Matrix4f m(
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16);

    IceFairy::Matrix4f expected(
        90,  100, 110, 120,
        202, 228, 254, 280,
        314, 356, 398, 440,
        426, 484, 542, 600);

    M4_Match(expected, m * m);
}

TEST(Matrix4, Vector3Multiplication) {
    IceFairy::Matrix4f m(
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16);

    EXPECT_EQ(IceFairy::Vector3f(14, 38, 62), m * IceFairy::Vector3f(1, 2, 3));
}

TEST(Matrix4, Vector4Multiplication) {
    IceFairy::Matrix4f m(
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16);

    EXPECT_EQ(IceFairy::Vector4f(30, 70, 110, 150), m * IceFairy::Vector4f(1, 2, 3, 4));
}

TEST(Matrix4, Scale) {
    IceFairy::Matrix4f m = IceFairy::Matrix4f::Scale(2, 2, 2);

    EXPECT_EQ(IceFairy::Vector3f(2, 4, 6), m * IceFairy::Vector3f(1, 2, 3));
}

TEST(Matrix4, Translate) {
    IceFairy::Matrix4f m = IceFairy::Matrix4f::Translate(1, 2, 3);

    EXPECT_EQ(IceFairy::Vector3f(2, 4, 6), (m * IceFairy::Vector4f(1, 2, 3, 1)).ToVector3());
}

TEST(Matrix4, Rotate) {
    IceFairy::Matrix4f m = IceFairy::Matrix4f::Rotate(90.0f, 0, 0, 1);

    V3M_FuzzyFloatMatch(IceFairy::Vector3f(1, 0, 0), m * IceFairy::Vector3f(0, 1, 0));
}