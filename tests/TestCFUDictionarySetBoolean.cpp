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
 *      This file implements a unit test for CFUDictionarySetBoolean.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUDictionarySetBoolean :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUDictionarySetBoolean);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestNonNull);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestNonNull(void);

private:
    void TestNonNull(const bool & aBooleanInput);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUDictionarySetBoolean);

void
TestCFUDictionarySetBoolean :: TestNull(void)
{
    CFMutableDictionaryRef lMutableDictionaryRef = NULL;
    CFStringRef            lKey                  = CFSTR("Test Key");
    bool                   lBoolean              = true;
    Boolean                lResult;

    lMutableDictionaryRef = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                      0,
                                                      &kCFTypeDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lMutableDictionaryRef != NULL);

    // Null dictionary

    lResult = CFUDictionarySetBoolean(NULL, lKey, lBoolean);
    CPPUNIT_ASSERT(lResult == false);

    // Null key

    lResult = CFUDictionarySetBoolean(lMutableDictionaryRef, NULL, lBoolean);
    CPPUNIT_ASSERT(lResult == false);

    // Null dictionary and key

    lResult = CFUDictionarySetBoolean(NULL, NULL, lBoolean);
    CPPUNIT_ASSERT(lResult == false);

    if (lMutableDictionaryRef != NULL) {
        CFRelease(lMutableDictionaryRef);
    }
}

void
TestCFUDictionarySetBoolean :: TestNonNull(void)
{
    TestNonNull(false);
    TestNonNull(true);
}

void
TestCFUDictionarySetBoolean :: TestNonNull(const bool & aBooleanInput)
{
    CFMutableDictionaryRef lMutableDictionaryRef = NULL;
    CFStringRef            lKey                  = CFSTR("Test Key");
    CFBooleanRef           lValue                = NULL;
    bool                   lBooleanOutput;
    Boolean                lResult;

    lMutableDictionaryRef = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                      0,
                                                      &kCFTypeDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lMutableDictionaryRef != NULL);

    lResult = CFUDictionarySetBoolean(lMutableDictionaryRef,
                                      lKey,
                                      aBooleanInput);
    CPPUNIT_ASSERT(lResult == true);

    lResult = CFDictionaryContainsKey(lMutableDictionaryRef, lKey);
    CPPUNIT_ASSERT(lResult == true);

    lValue = reinterpret_cast<CFBooleanRef>(
        CFDictionaryGetValue(lMutableDictionaryRef, lKey));
    CPPUNIT_ASSERT(lValue != NULL);

    lBooleanOutput = CFBooleanGetValue(lValue);
    CPPUNIT_ASSERT(lBooleanOutput == aBooleanInput);

    if (lMutableDictionaryRef != NULL) {
        CFRelease(lMutableDictionaryRef);
    }
}
