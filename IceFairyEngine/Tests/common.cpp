#include "common.h"

void V2M_FuzzyFloatMatch(IceFairy::Vector2f expected, IceFairy::Vector2f actual) {
    EXPECT_NEAR(expected.x, actual.x, 1e-5f);
    EXPECT_NEAR(expected.y, actual.y, 1e-5f);
}

void V3M_FuzzyFloatMatch(IceFairy::Vector3f expected, IceFairy::Vector3f actual) {
    EXPECT_NEAR(expected.x, actual.x, 1e-5f);
    EXPECT_NEAR(expected.y, actual.y, 1e-5f);
    EXPECT_NEAR(expected.z, actual.z, 1e-5f);
}

void M3_FuzzyMatch(IceFairy::Matrix3f expected, IceFairy::Matrix3f actual) {
    for (unsigned int i = 0; i < 9; i++) {
        EXPECT_NEAR(expected[i], actual[i], 1e-5f);
    }
}

void M3_Match(IceFairy::Matrix3f expected, IceFairy::Matrix3f actual) {
    const float epsilon = 1e-5f;

    for (unsigned int i = 0; i < 9; i++) {
        EXPECT_EQ(expected[i], actual[i]);
    }
}

void M4_FuzzyMatch(IceFairy::Matrix4f expected, IceFairy::Matrix4f actual) {
    for (unsigned int i = 0; i < 16; i++) {
        EXPECT_NEAR(expected[i], actual[i], 1e-5f);
    }
}

void M4_Match(IceFairy::Matrix4f expected, IceFairy::Matrix4f actual) {
    const float epsilon = 1e-5f;

    for (unsigned int i = 0; i < 16; i++) {
        EXPECT_EQ(expected[i], actual[i]);
    }
}