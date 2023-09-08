/*
 *    Copyright (c) 2023 Nuovation System Designs, LLC
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
 *      This file implements a unit test for CFUDictionaryDifference.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUDictionaryDifference :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUDictionaryDifference);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUDictionaryDifference);

void
TestCFUDictionaryDifference :: TestNull(void)
{
    CFDictionaryRef        lProposedDictionaryRef = nullptr;
    CFMutableDictionaryRef lBaseDictionaryRef     = nullptr;
    CFMutableDictionaryRef lAddedDictionaryRef    = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef   = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef  = nullptr;   
    Boolean                lStatus;

    // Allocate the added, common, and removed dictionaries

    lAddedDictionaryRef = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                    0,
                                                    &kCFTypeDictionaryKeyCallBacks,
                                                    &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lAddedDictionaryRef != NULL);

    lCommonDictionaryRef = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                     0,
                                                     &kCFTypeDictionaryKeyCallBacks,
                                                     &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lCommonDictionaryRef != NULL);

    lRemovedDictionaryRef = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                      0,
                                                      &kCFTypeDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(lRemovedDictionaryRef != NULL);

    // Test the C binding.

    lStatus = CFUDictionaryDifference(lProposedDictionaryRef,
                                      nullptr,
                                      lAddedDictionaryRef,
                                      lCommonDictionaryRef,
                                      lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == false);

    // Test the C++ binding.

    lStatus = CFUDictionaryDifference(lProposedDictionaryRef,
                                      lBaseDictionaryRef,
                                      lAddedDictionaryRef,
                                      lCommonDictionaryRef,
                                      lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == false);

    if (lAddedDictionaryRef != NULL) {
        CFRelease(lAddedDictionaryRef);
    }

    if (lCommonDictionaryRef != NULL) {
        CFRelease(lCommonDictionaryRef);
    }

    if (lRemovedDictionaryRef != NULL) {
        CFRelease(lRemovedDictionaryRef);
    }
}
