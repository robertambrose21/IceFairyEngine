#include "vectorTest.h"

TEST(Vector2Math, AcessOperator) {
    IceFairy::Vector2i vec(2, 3);

    EXPECT_EQ(vec[0], 2);
    EXPECT_EQ(vec[1], 3);

    ASSERT_THROW(vec[2], IceFairy::VectorOutOfBoundsException);
}

TEST(Vector2Math, DotProduct) {
    IceFairy::Vector2f v1(1.0f, 2.0f);
    IceFairy::Vector2f v2(3.0f, 4.0f);

    EXPECT_EQ(11.0f, v1.Dot(v2));
}

TEST(Vector2Math, DotPerpProduct) {
    IceFairy::Vector2f v1(1.0f, 2.0f);
    IceFairy::Vector2f v2(3.0f, 4.0f);

    EXPECT_EQ(-2.0f, v1.DotPerp(v2));
}

TEST(Vector2Math, Length) {
    IceFairy::Vector2f vec(2.0f, 3.0f);

    EXPECT_EQ(std::sqrt(13.0f), vec.Length());
}

TEST(Vector2Math, Distance) {
    IceFairy::Vector2f v1(2.0f, 4.0f);
    IceFairy::Vector2f v2(6.0f, 8.0f);

    EXPECT_EQ(std::sqrt(32.0f), v1.Distance(v2));
}

TEST(Vector2Math, Normalise) {
    IceFairy::Vector2f original(3.0f, 2.0f);
    IceFairy::Vector2f normalised = original.Normalise();

    EXPECT_EQ(normalised, original);
    EXPECT_EQ(normalised.Length(), 1);
}

TEST(Vector2Math, Interpolate) {
    IceFairy::Vector2f v1(1.0f, 1.0f);
    IceFairy::Vector2f v2(7.0f, 3.0f);
    
    V2M_FuzzyFloatMatch(IceFairy::Vector2f(4.0f, 2.0f), v1.Interpolate(v2, 0.5f));

    EXPECT_EQ(IceFairy::Vector2f(1.0f, 1.0f), v1.Interpolate(v2, 0.0f));
    EXPECT_EQ(IceFairy::Vector2f(7.0f, 3.0f), v1.Interpolate(v2, 1.0f));
    EXPECT_EQ(IceFairy::Vector2f(1.0f, 1.0f), v1.Interpolate(v2, -1.0f));
    EXPECT_EQ(IceFairy::Vector2f(7.0f, 3.0f), v1.Interpolate(v2, 2.0f));
}

TEST(Vector2Math, RotateBy) {
    IceFairy::Vector2i vec(1, 0);

    EXPECT_EQ(IceFairy::Vector2i(0, 1), vec.RotateBy(90.0f));
}

TEST(Vector2Math, Projection) {
    IceFairy::Vector2f v1(1.0f, 2.0f);
    IceFairy::Vector2f v2(2.0f, 7.0f);
    IceFairy::Vector2f projected = v1.ProjectOnto(v2);

    EXPECT_NEAR((v1 - projected).Dot(v2), 0.0f, 1e-5f);
    V2M_FuzzyFloatMatch(v2.Normalise(), projected.Normalise());
}

TEST(Vector2Math, InverseInterpolate) {
    IceFairy::Vector2f v1(1.0f, 1.0f);
    IceFairy::Vector2f v2(7.0f, 3.0f);
    IceFairy::Vector2f v3(4.0f, 2.0f);

    EXPECT_FLOAT_EQ(0.5f, IceFairy::Vector2f::InverseInterpolate(v1, v2, v3));
}

TEST(Vector3Math, AcessOperator) {
    IceFairy::Vector3i vec(2, 3, 4);

    EXPECT_EQ(vec[0], 2);
    EXPECT_EQ(vec[1], 3);
    EXPECT_EQ(vec[2], 4);

    ASSERT_THROW(vec[3], IceFairy::VectorOutOfBoundsException);
}

TEST(Vector3Math, DotProduct) {
    IceFairy::Vector3f v1(1.0f, 2.0f, 3.0f);
    IceFairy::Vector3f v2(3.0f, 4.0f, 1.0f);

    EXPECT_EQ(14.0f, v1.Dot(v2));
}

TEST(Vector3Math, CrossProduct) {
    IceFairy::Vector3f v1(1.0f, 2.0f, 3.0f);
    IceFairy::Vector3f v2(3.0f, 4.0f, 1.0f);

    EXPECT_EQ(IceFairy::Vector3f(-10.0f, 8.0f, -2.0f), v1.Cross(v2));
}

TEST(Vector3Math, Length) {
    IceFairy::Vector3f vec(2.0f, 3.0f, 1.0f);

    EXPECT_EQ(std::sqrt(14.0f), vec.Length());
}

TEST(Vector3Math, Distance) {
    IceFairy::Vector3f v1(2.0f, 4.0f, 3.0f);
    IceFairy::Vector3f v2(6.0f, 8.0f, 1.0f);

    EXPECT_EQ(std::sqrt(36.0f), v1.Distance(v2));
}

TEST(Vector3Math, Normalise) {
    IceFairy::Vector3f original(3.0f, 2.0f, 2.0f);
    IceFairy::Vector3f normalised = original.Normalise();

    EXPECT_EQ(normalised, original);
    EXPECT_EQ(normalised.Length(), 1.0f);
}

TEST(Vector3Math, Interpolate) {
    IceFairy::Vector3f v1(1.0f, 1.0f, 1.0f);
    IceFairy::Vector3f v2(7.0f, 3.0f, 4.0f);

    v1.Interpolate(v2, 0.5f);
    V3M_FuzzyFloatMatch(IceFairy::Vector3f(4.0f, 2.0f, 2.5f), v1.Interpolate(v2, 0.5f));

    EXPECT_EQ(IceFairy::Vector3f(1.0f, 1.0f, 1.0f), v1.Interpolate(v2, 0.0f));
    EXPECT_EQ(IceFairy::Vector3f(7.0f, 3.0f, 4.0f), v1.Interpolate(v2, 1.0f));
    EXPECT_EQ(IceFairy::Vector3f(1.0f, 1.0f, 1.0f), v1.Interpolate(v2, -1.0f));
    EXPECT_EQ(IceFairy::Vector3f(7.0f, 3.0f, 4.0f), v1.Interpolate(v2, 2.0f));
}

TEST(Vector3Math, Projection) {
    IceFairy::Vector3f v1(1.0f, 2.0f, 1.0f);
    IceFairy::Vector3f v2(2.0f, 7.0f, 3.0f);
    IceFairy::Vector3f projected = v1.ProjectOnto(v2);

    EXPECT_NEAR((v1 - projected).Dot(v2), 0.0f, 1e-5f);
    V3M_FuzzyFloatMatch(v2.Normalise(), projected.Normalise());
}

TEST(Vector3Math, InverseInterpolate) {
    IceFairy::Vector3f v1(1.0f, 1.0f, 1.0f);
    IceFairy::Vector3f v2(7.0f, 3.0f, 4.0f);
    IceFairy::Vector3f v3(4.0f, 2.0f, 2.5f);

    EXPECT_FLOAT_EQ(0.5f, IceFairy::Vector3f::InverseInterpolate(v1, v2, v3));
}

TEST(Vector3Math, BilinearInterpolate) {
    IceFairy::Vector3f v1(0.0f, 0.0f, 0.0f);
    IceFairy::Vector3f v2(2.0f, 0.0f, 0.0f);
    IceFairy::Vector3f v3(2.0f, 2.0f, 0.0f);
    IceFairy::Vector3f v4(0.0f, 2.0f, 0.0f);

    IceFairy::Vector3f result = IceFairy::Vector3f::BilinearInterpolate(v1, v2, v3, v4, 0.5f, 0.25f);
    V3M_FuzzyFloatMatch(IceFairy::Vector3f(1.0f, 0.5f, 0.0f), result);
}

TEST(Ray3Math, Position) {
    IceFairy::Vector3f origin(1.0f, 1.0f, 1.0f);
    IceFairy::Vector3f direction(0.0f, 2.0f, 0.0f);
    IceFairy::Ray3f ray(origin, direction);

    V3M_FuzzyFloatMatch(IceFairy::Vector3f(1.0f, 3.0f, 1.0f), ray.Position(2.0f));
}