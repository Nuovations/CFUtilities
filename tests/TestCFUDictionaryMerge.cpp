/*
 *    Copyright (c) 2021 Nuovation System Designs, LLC
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *      This file implements a unit test for CFUDictionaryMerge.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUDictionaryMerge :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUDictionaryMerge);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestNonNullNoIntersectionWithoutReplacement);
    CPPUNIT_TEST(TestNonNullNoIntersectionWithReplacement);
    CPPUNIT_TEST(TestNonNullIntersectionWithoutReplacement);
    CPPUNIT_TEST(TestNonNullIntersectionWithReplacement);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestNonNullNoIntersectionWithoutReplacement(void);
    void TestNonNullNoIntersectionWithReplacement(void);
    void TestNonNullIntersectionWithoutReplacement(void);
    void TestNonNullIntersectionWithReplacement(void);

private:
    void TestNonNullNoIntersection(const bool & aReplace);
    void TestNonNullIntersection(const bool & aReplace);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUDictionaryMerge);

void
TestCFUDictionaryMerge :: TestNull(void)
{
    const bool             kReplace              = true;
    CFMutableDictionaryRef lMutableDictionaryRef = NULL;
    CFDictionaryRef        lDictionaryRef        = NULL;

    lMutableDictionaryRef = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                      0,
                                                      &kCFTypeDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lMutableDictionaryRef != NULL);

    lDictionaryRef = CFDictionaryCreate(kCFAllocatorDefault,
                                        NULL,
                                        NULL,
                                        0,
                                        &kCFTypeDictionaryKeyCallBacks,
                                        &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lDictionaryRef != NULL);

    // Test NULL destination and non-NULL source

    CFUDictionaryMerge(NULL, lDictionaryRef, kReplace);

    // Test non-NULL destination and NULL source

    CFUDictionaryMerge(lMutableDictionaryRef, NULL, kReplace);

    // Test NULL destination and source

    CFUDictionaryMerge(NULL, NULL, kReplace);

    if (lDictionaryRef != NULL) {
        CFRelease(lDictionaryRef);
    }

    if (lMutableDictionaryRef != NULL) {
        CFRelease(lMutableDictionaryRef);
    }
}

void
TestCFUDictionaryMerge :: TestNonNullNoIntersectionWithoutReplacement(void)
{
    const bool kReplace = true;

    TestNonNullNoIntersection(!kReplace);
}

void
TestCFUDictionaryMerge :: TestNonNullNoIntersectionWithReplacement(void)
{
    const bool kReplace = true;

    TestNonNullNoIntersection(kReplace);
}

void
TestCFUDictionaryMerge :: TestNonNullNoIntersection(const bool & aReplace)
{
    const size_t           kDestinationKeyCount                   = 2;
    const size_t           kSourceKeyCount                        = 2;
    CFMutableDictionaryRef lMutableDictionaryRef                  = NULL;
    CFDictionaryRef        lDictionaryRef                         = NULL;
    const void *           kDestinationKeys[kDestinationKeyCount] = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3")
    };
    const void *           kDestinationValues[kDestinationKeyCount] = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3")
    };
    const void *           kSourceKeys[kSourceKeyCount] = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kSourceValues[kSourceKeyCount] = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4")
    };
    CFIndex                lDictionaryCount;
    CFStringRef            lStringValue;
    CFComparisonResult     lComparisonResult;

    lMutableDictionaryRef = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                      0,
                                                      &kCFTypeDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lMutableDictionaryRef != NULL);

    CFDictionarySetValue(lMutableDictionaryRef,
                         kDestinationKeys[0],
                         kDestinationValues[0]);
    CFDictionarySetValue(lMutableDictionaryRef,
                         kDestinationKeys[1],
                         kDestinationValues[1]);

    lDictionaryRef = CFDictionaryCreate(kCFAllocatorDefault,
                                        &kSourceKeys[0],
                                        &kSourceValues[0],
                                        kSourceKeyCount,
                                        &kCFTypeDictionaryKeyCallBacks,
                                        &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lDictionaryRef != NULL);

    // Merge the two dictionaries (source into destination,
    // non-mutable into mutable).

    CFUDictionaryMerge(lMutableDictionaryRef, lDictionaryRef, aReplace);
    CPPUNIT_ASSERT(lMutableDictionaryRef != NULL);
    CPPUNIT_ASSERT(lDictionaryRef != NULL);

    CFShow(lMutableDictionaryRef);

    // Confirm, post merge, the destination and source counts.

    lDictionaryCount = CFDictionaryGetCount(lMutableDictionaryRef);
    CPPUNIT_ASSERT(lDictionaryCount == (kDestinationKeyCount + kSourceKeyCount));

    lDictionaryCount = CFDictionaryGetCount(lDictionaryRef);
    CPPUNIT_ASSERT(lDictionaryCount == kSourceKeyCount);

    // Confirm the contents of the destination dictionary.

    lStringValue = reinterpret_cast<CFStringRef>(
        CFDictionaryGetValue(lMutableDictionaryRef, kDestinationKeys[0]));
    CPPUNIT_ASSERT(lStringValue != NULL);

    lComparisonResult = CFStringCompare(
        lStringValue,
        reinterpret_cast<CFStringRef>(kDestinationValues[0]),
        0);
    CPPUNIT_ASSERT(lComparisonResult == kCFCompareEqualTo);

    lStringValue = reinterpret_cast<CFStringRef>(
        CFDictionaryGetValue(lMutableDictionaryRef, kDestinationKeys[1]));
    CPPUNIT_ASSERT(lStringValue != NULL);

    lComparisonResult = CFStringCompare(
        lStringValue,
        reinterpret_cast<CFStringRef>(kDestinationValues[1]),
        0);
    CPPUNIT_ASSERT(lComparisonResult == kCFCompareEqualTo);

    lStringValue = reinterpret_cast<CFStringRef>(
        CFDictionaryGetValue(lMutableDictionaryRef, kSourceKeys[0]));
    CPPUNIT_ASSERT(lStringValue != NULL);

    lComparisonResult = CFStringCompare(
        lStringValue,
        reinterpret_cast<CFStringRef>(kSourceValues[0]),
        0);
    CPPUNIT_ASSERT(lComparisonResult == kCFCompareEqualTo);

    lStringValue = reinterpret_cast<CFStringRef>(
        CFDictionaryGetValue(lMutableDictionaryRef, kSourceKeys[1]));
    CPPUNIT_ASSERT(lStringValue != NULL);

    lComparisonResult = CFStringCompare(
        lStringValue,
        reinterpret_cast<CFStringRef>(kSourceValues[1]),
        0);
    CPPUNIT_ASSERT(lComparisonResult == kCFCompareEqualTo);

    if (lDictionaryRef != NULL) {
        CFRelease(lDictionaryRef);
    }

    if (lMutableDictionaryRef != NULL) {
        CFRelease(lMutableDictionaryRef);
    }
}

void
TestCFUDictionaryMerge :: TestNonNullIntersectionWithoutReplacement(void)
{
    const bool kReplace = true;

    TestNonNullIntersection(!kReplace);
}

void
TestCFUDictionaryMerge :: TestNonNullIntersectionWithReplacement(void)
{
    const bool kReplace = true;

    TestNonNullIntersection(kReplace);
}

void
TestCFUDictionaryMerge :: TestNonNullIntersection(const bool & aReplace)
{
    const size_t           kDestinationKeyCount                   = 3;
    const size_t           kSourceKeyCount                        = 2;
    const size_t           kFinalKeyCount                         = 4;
    CFMutableDictionaryRef lMutableDictionaryRef                  = NULL;
    CFDictionaryRef        lDictionaryRef                         = NULL;
    const void *           kDestinationKeys[kDestinationKeyCount] = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 2"),
        CFSTR("Test Key 3")
    };
    const void *           kDestinationValues[kDestinationKeyCount] = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value D2"),
        CFSTR("Test Value 3")
    };
    const void *           kSourceKeys[kSourceKeyCount] = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kSourceValues[kSourceKeyCount] = {
        CFSTR("Test Value S2"),
        CFSTR("Test Value 4")
    };
    CFIndex                lDictionaryCount;
    CFStringRef            lStringValue;
    CFComparisonResult     lComparisonResult;

    lMutableDictionaryRef = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                      0,
                                                      &kCFTypeDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lMutableDictionaryRef != NULL);

    CFDictionarySetValue(lMutableDictionaryRef,
                         kDestinationKeys[0],
                         kDestinationValues[0]);
    CFDictionarySetValue(lMutableDictionaryRef,
                         kDestinationKeys[1],
                         kDestinationValues[1]);
    CFDictionarySetValue(lMutableDictionaryRef,
                         kDestinationKeys[2],
                         kDestinationValues[2]);

    lDictionaryRef = CFDictionaryCreate(kCFAllocatorDefault,
                                        &kSourceKeys[0],
                                        &kSourceValues[0],
                                        kSourceKeyCount,
                                        &kCFTypeDictionaryKeyCallBacks,
                                        &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lDictionaryRef != NULL);

    // Merge the two dictionaries (source into destination,
    // non-mutable into mutable).

    CFUDictionaryMerge(lMutableDictionaryRef, lDictionaryRef, aReplace);
    CPPUNIT_ASSERT(lMutableDictionaryRef != NULL);
    CPPUNIT_ASSERT(lDictionaryRef != NULL);

    CFShow(lMutableDictionaryRef);

    // Confirm, post merge, the destination and source counts.

    lDictionaryCount = CFDictionaryGetCount(lMutableDictionaryRef);
    CPPUNIT_ASSERT(lDictionaryCount == kFinalKeyCount);

    lDictionaryCount = CFDictionaryGetCount(lDictionaryRef);
    CPPUNIT_ASSERT(lDictionaryCount == kSourceKeyCount);

    // Confirm the contents of the destination dictionary.

    // "Test Key 1"

    lStringValue = reinterpret_cast<CFStringRef>(
        CFDictionaryGetValue(lMutableDictionaryRef, kDestinationKeys[0]));
    CPPUNIT_ASSERT(lStringValue != NULL);

    lComparisonResult = CFStringCompare(
        lStringValue,
        reinterpret_cast<CFStringRef>(kDestinationValues[0]),
        0);
    CPPUNIT_ASSERT(lComparisonResult == kCFCompareEqualTo);

    // Depending on the replacement flag, the value will either be
    // that of the destination (wihtout replacement) or the source
    // (with replacement).

    // "Test Key 2"

    lStringValue = reinterpret_cast<CFStringRef>(
        CFDictionaryGetValue(lMutableDictionaryRef, kDestinationKeys[1]));
    CPPUNIT_ASSERT(lStringValue != NULL);

    lComparisonResult = CFStringCompare(
        lStringValue,
        reinterpret_cast<CFStringRef>(aReplace ? kSourceValues[0]
                                               : kDestinationValues[1]),
        0);
    CPPUNIT_ASSERT(lComparisonResult == kCFCompareEqualTo);

    // "Test Key 3"

    lStringValue = reinterpret_cast<CFStringRef>(
        CFDictionaryGetValue(lMutableDictionaryRef, kDestinationKeys[2]));
    CPPUNIT_ASSERT(lStringValue != NULL);

    lComparisonResult = CFStringCompare(
        lStringValue,
        reinterpret_cast<CFStringRef>(kDestinationValues[2]),
        0);
    CPPUNIT_ASSERT(lComparisonResult == kCFCompareEqualTo);

    // "Test Key 4"

    lStringValue = reinterpret_cast<CFStringRef>(
        CFDictionaryGetValue(lMutableDictionaryRef, kSourceKeys[1]));
    CPPUNIT_ASSERT(lStringValue != NULL);

    lComparisonResult = CFStringCompare(
        lStringValue,
        reinterpret_cast<CFStringRef>(kSourceValues[1]),
        0);
    CPPUNIT_ASSERT(lComparisonResult == kCFCompareEqualTo);

    if (lDictionaryRef != NULL) {
        CFRelease(lDictionaryRef);
    }

    if (lMutableDictionaryRef != NULL) {
        CFRelease(lMutableDictionaryRef);
    }
}
