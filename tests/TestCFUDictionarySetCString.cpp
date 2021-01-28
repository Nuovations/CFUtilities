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
 *      This file implements a unit test for CFUDictionarySetCString.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUDictionarySetCString :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUDictionarySetCString);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestNonNull);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestNonNull(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUDictionarySetCString);

void
TestCFUDictionarySetCString :: TestNull(void)
{
    CFMutableDictionaryRef lMutableDictionaryRef = NULL;
    CFStringRef            lKey                  = CFSTR("Test Key");
    const char *           lString               = "Test Value";
    Boolean                lResult;

    lMutableDictionaryRef = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                      0,
                                                      &kCFTypeDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lMutableDictionaryRef != NULL);

    // Null dictionary

    lResult = CFUDictionarySetCString(NULL, lKey, lString);
    CPPUNIT_ASSERT(lResult == false);

    // Null key

    lResult = CFUDictionarySetCString(lMutableDictionaryRef, NULL, lString);
    CPPUNIT_ASSERT(lResult == false);

    // Null value

    lResult = CFUDictionarySetCString(lMutableDictionaryRef, lKey, NULL);
    CPPUNIT_ASSERT(lResult == false);

    // Null dictionary and value

    lResult = CFUDictionarySetCString(NULL, lKey, NULL);
    CPPUNIT_ASSERT(lResult == false);

    // Null dictionary and key

    lResult = CFUDictionarySetCString(NULL, NULL, lString);
    CPPUNIT_ASSERT(lResult == false);

    // Null key and value

    lResult = CFUDictionarySetCString(lMutableDictionaryRef, NULL, NULL);
    CPPUNIT_ASSERT(lResult == false);

    // Null dictionary, key, and value

    lResult = CFUDictionarySetCString(NULL, NULL, NULL);
    CPPUNIT_ASSERT(lResult == false);

    if (lMutableDictionaryRef != NULL) {
        CFRelease(lMutableDictionaryRef);
    }
}

void
TestCFUDictionarySetCString :: TestNonNull(void)
{
    CFMutableDictionaryRef lMutableDictionaryRef = NULL;
    CFStringRef            lKey                  = CFSTR("Test Key");
    const char *           lCStringInput         = "Test Value";
    CFStringRef            lValue                = NULL;
    const char *           lCStringOutput        = NULL;
    Boolean                lResult;
    int                    l3WayResult;

    lMutableDictionaryRef = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                      0,
                                                      &kCFTypeDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lMutableDictionaryRef != NULL);

    lResult = CFUDictionarySetCString(lMutableDictionaryRef,
                                      lKey,
                                      lCStringInput);
    CPPUNIT_ASSERT(lResult == true);

    lResult = CFDictionaryContainsKey(lMutableDictionaryRef, lKey);
    CPPUNIT_ASSERT(lResult == true);

    lValue = reinterpret_cast<CFStringRef>(
        CFDictionaryGetValue(lMutableDictionaryRef, lKey));
    CPPUNIT_ASSERT(lValue != NULL);

    lCStringOutput = CFStringGetCStringPtr(lValue, CFStringGetSystemEncoding());
    CPPUNIT_ASSERT(lCStringOutput != NULL);

    l3WayResult = strcmp(lCStringOutput, lCStringInput);
    CPPUNIT_ASSERT(l3WayResult == 0);

    if (lMutableDictionaryRef != NULL) {
        CFRelease(lMutableDictionaryRef);
    }
}
