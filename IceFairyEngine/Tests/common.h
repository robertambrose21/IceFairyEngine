#ifndef __ice_fairy_tests_common_h__
#define __ice_fairy_tests_common_h__

#include "gtest\gtest.h"
#include "math\vector.h"
#include "math\matrix.h"

void V2M_FuzzyFloatMatch(IceFairy::Vector2f expected, IceFairy::Vector2f actual);

void V3M_FuzzyFloatMatch(IceFairy::Vector3f expected, IceFairy::Vector3f actual);

void M3_FuzzyMatch(IceFairy::Matrix3f expected, IceFairy::Matrix3f actual);

void M3_Match(IceFairy::Matrix3f expected, IceFairy::Matrix3f actual);

void M4_FuzzyMatch(IceFairy::Matrix4f expected, IceFairy::Matrix4f actual);

void M4_Match(IceFairy::Matrix4f expected, IceFairy::Matrix4f actual);

#endif /* __ice_fairy_tests_common_h__ */