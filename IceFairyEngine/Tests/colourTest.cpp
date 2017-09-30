#include "gtest\gtest.h"
#include "math\colour.h"

void C3M_FuzzyFloatMatch(IceFairy::Colour3f expected, IceFairy::Colour3f actual) {
    EXPECT_FLOAT_EQ(expected.r, actual.r);
    EXPECT_FLOAT_EQ(expected.g, actual.g);
    EXPECT_FLOAT_EQ(expected.b, actual.b);
}

void C4M_FuzzyFloatMatch(IceFairy::Colour4f expected, IceFairy::Colour4f actual) {
    EXPECT_FLOAT_EQ(expected.r, actual.r);
    EXPECT_FLOAT_EQ(expected.g, actual.g);
    EXPECT_FLOAT_EQ(expected.b, actual.b);
    EXPECT_FLOAT_EQ(expected.a, actual.a);
}

TEST(Colour3Math, Interpolate) {
    IceFairy::Colour3f red(1.0f, 0.0f, 0.0f);
    IceFairy::Colour3f blue(0.0f, 0.0f, 1.0f);

    C3M_FuzzyFloatMatch(IceFairy::Colour3f(0.5f, 0.0f, 0.5f), red.Interpolate(blue, 0.5f));
}

TEST(Colour3Math, HexToColour3) {
    C3M_FuzzyFloatMatch(IceFairy::Colour3f(1.0f, 0.0f, 1.0f), IceFairy::Colour3f::HexToColour3("FF00FF"));
    ASSERT_THROW(IceFairy::Colour3f::HexToColour3("12345"), IceFairy::InvalidColourHexString);
}

TEST(Colour3Math, ToHex) {
    EXPECT_EQ("FF7FBF", IceFairy::Colour3f(1.0f, 0.5f, 0.75f).ToHex());
}

TEST(Colour4Math, Interpolate) {
    IceFairy::Colour4f red(1.0f, 0.0f, 0.0f, 1.0f);
    IceFairy::Colour4f blue(0.0f, 0.0f, 1.0f, 1.0f);

    C4M_FuzzyFloatMatch(IceFairy::Colour4f(0.5f, 0.0f, 0.5f, 1.0f), red.Interpolate(blue, 0.5f));
}