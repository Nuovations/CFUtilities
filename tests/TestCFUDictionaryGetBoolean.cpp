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
 *      This file implements a unit test for CFUDictionaryGetBoolean.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUDictionaryGetBoolean :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUDictionaryGetBoolean);
    CPPUNIT_TEST(TestNullCFBoolean);
    CPPUNIT_TEST(TestNullStdBoolean);
    CPPUNIT_TEST(TestNonExistentKey);
    CPPUNIT_TEST(TestNonBooleanType);
    CPPUNIT_TEST(TestNonNullCFBoolean);
    CPPUNIT_TEST(TestNonNullStdBoolean);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNullCFBoolean(void);
    void TestNullStdBoolean(void);
    void TestNonExistentKey(void);
    void TestNonBooleanType(void);
    void TestNonNullCFBoolean(void);
    void TestNonNullStdBoolean(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUDictionaryGetBoolean);

void
TestCFUDictionaryGetBoolean :: TestNullCFBoolean(void)
{
    CFMutableDictionaryRef lMutableDictionaryRef = NULL;
    CFStringRef            lKey                  = CFSTR("Test Key");
    Boolean                lBoolean;
    Boolean                lResult;

    lMutableDictionaryRef = CFDictionaryCreateMutable(
        kCFAllocatorDefault,
        0,
        &kCFTypeDictionaryKeyCallBacks,
        &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lMutableDictionaryRef != NULL);

    // Null dictionary

    lResult = CFUDictionaryGetBoolean(NULL, lKey, lBoolean);
    CPPUNIT_ASSERT(lResult == false);

    // Null key

    lResult = CFUDictionaryGetBoolean(lMutableDictionaryRef, NULL, lBoolean);
    CPPUNIT_ASSERT(lResult == false);

    // Null dictionary and key

    lResult = CFUDictionaryGetBoolean(NULL, NULL, lBoolean);
    CPPUNIT_ASSERT(lResult == false);

    if (lMutableDictionaryRef != NULL) {
        CFRelease(lMutableDictionaryRef);
    }
}

void
TestCFUDictionaryGetBoolean :: TestNullStdBoolean(void)
{
    CFMutableDictionaryRef lMutableDictionaryRef = NULL;
    CFStringRef            lKey                  = CFSTR("Test Key");
    bool                   lBoolean;
    Boolean                lResult;

    lMutableDictionaryRef = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                      0,
                                                      &kCFTypeDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lMutableDictionaryRef != NULL);

    // Null dictionary

    lResult = CFUDictionaryGetBoolean(NULL, lKey, lBoolean);
    CPPUNIT_ASSERT(lResult == false);

    // Null key

    lResult = CFUDictionaryGetBoolean(lMutableDictionaryRef, NULL, lBoolean);
    CPPUNIT_ASSERT(lResult == false);

    // Null dictionary and key

    lResult = CFUDictionaryGetBoolean(NULL, NULL, lBoolean);
    CPPUNIT_ASSERT(lResult == false);

    if (lMutableDictionaryRef != NULL) {
        CFRelease(lMutableDictionaryRef);
    }
}

void
TestCFUDictionaryGetBoolean :: TestNonNullCFBoolean(void)
{
    CFMutableDictionaryRef lMutableDictionaryRef = NULL;
    CFStringRef            lKey                  = CFSTR("Test Key");
    Boolean                lBooleanOutput;
    Boolean                lResult;

    lMutableDictionaryRef = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                      0,
                                                      &kCFTypeDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lMutableDictionaryRef != NULL);

    CFDictionarySetValue(lMutableDictionaryRef, lKey, kCFBooleanTrue);

    lResult = CFDictionaryContainsKey(lMutableDictionaryRef, lKey);
    CPPUNIT_ASSERT(lResult == true);

    lResult = CFUDictionaryGetBoolean(lMutableDictionaryRef,
                                      lKey,
                                      lBooleanOutput);
    CPPUNIT_ASSERT(lResult == true);
    CPPUNIT_ASSERT(lBooleanOutput == true);

    if (lMutableDictionaryRef != NULL) {
        CFRelease(lMutableDictionaryRef);
    }
}

void
TestCFUDictionaryGetBoolean :: TestNonExistentKey(void)
{
    CFMutableDictionaryRef lMutableDictionaryRef = NULL;
    CFStringRef            lActualKey            = CFSTR("Test Key");
    CFStringRef            lNonExistentKey       = CFSTR("Nonexistent Key");
    bool                   lBooleanOutput;
    Boolean                lResult;

    lMutableDictionaryRef = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                      0,
                                                      &kCFTypeDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lMutableDictionaryRef != NULL);

    CFDictionarySetValue(lMutableDictionaryRef, lActualKey, kCFBooleanTrue);

    lResult = CFDictionaryContainsKey(lMutableDictionaryRef, lActualKey);
    CPPUNIT_ASSERT(lResult == true);

    lResult = CFUDictionaryGetBoolean(lMutableDictionaryRef,
                                      lNonExistentKey,
                                      lBooleanOutput);
    CPPUNIT_ASSERT(lResult == false);

    if (lMutableDictionaryRef != NULL) {
        CFRelease(lMutableDictionaryRef);
    }
}

void
TestCFUDictionaryGetBoolean :: TestNonBooleanType(void)
{
    CFMutableDictionaryRef lMutableDictionaryRef = NULL;
    CFStringRef            lKey                  = CFSTR("Test Key");
    CFStringRef            lNonBooleanValue      = CFSTR("Test Value");
    bool                   lBooleanOutput;
    Boolean                lResult;

    lMutableDictionaryRef = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                      0,
                                                      &kCFTypeDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lMutableDictionaryRef != NULL);

    // Intentionally set a non-Boolean value for the test key.

    CFDictionarySetValue(lMutableDictionaryRef, lKey, lNonBooleanValue);

    lResult = CFDictionaryContainsKey(lMutableDictionaryRef, lKey);
    CPPUNIT_ASSERT(lResult == true);

    lResult = CFUDictionaryGetBoolean(lMutableDictionaryRef,
                                      lKey,
                                      lBooleanOutput);
    CPPUNIT_ASSERT(lResult == false);

    if (lMutableDictionaryRef != NULL) {
        CFRelease(lMutableDictionaryRef);
    }
}

void
TestCFUDictionaryGetBoolean :: TestNonNullStdBoolean(void)
{
    CFMutableDictionaryRef lMutableDictionaryRef = NULL;
    CFStringRef            lKey                  = CFSTR("Test Key");
    bool                   lBooleanOutput;
    Boolean                lResult;

    lMutableDictionaryRef = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                      0,
                                                      &kCFTypeDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lMutableDictionaryRef != NULL);

    CFDictionarySetValue(lMutableDictionaryRef, lKey, kCFBooleanTrue);

    lResult = CFDictionaryContainsKey(lMutableDictionaryRef, lKey);
    CPPUNIT_ASSERT(lResult == true);

    lResult = CFUDictionaryGetBoolean(lMutableDictionaryRef,
                                      lKey,
                                      lBooleanOutput);
    CPPUNIT_ASSERT(lResult == true);
    CPPUNIT_ASSERT(lBooleanOutput == true);

    if (lMutableDictionaryRef != NULL) {
        CFRelease(lMutableDictionaryRef);
    }
}
