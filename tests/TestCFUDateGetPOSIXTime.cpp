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
 *      This file implements a unit test for CFUDateGetPOSIXTime.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <time.h>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUDateGetPOSIXTime :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUDateGetPOSIXTime);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestNonNull);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestNonNull(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUDateGetPOSIXTime);

void
TestCFUDateGetPOSIXTime :: TestNull(void)
{
    CFDateRef lNowDateRef = NULL;
    time_t    lResult;

    lResult = CFUDateGetPOSIXTime(lNowDateRef);
    CPPUNIT_ASSERT(lResult == 0);
}

void
TestCFUDateGetPOSIXTime :: TestNonNull(void)
{
    const time_t         lPOSIXNow     = time(NULL);
    const time_t         lPOSIXEpsilon = 1;
    const CFAbsoluteTime lCFNow        = CFAbsoluteTimeGetCurrent();
    CFDateRef            lNowDateRef   = NULL;
    time_t               lResult;

    lNowDateRef = CFDateCreate(kCFAllocatorDefault, lCFNow);
    CPPUNIT_ASSERT(lNowDateRef != NULL);

    lResult = CFUDateGetPOSIXTime(lNowDateRef);
    CPPUNIT_ASSERT(lResult > lPOSIXNow - lPOSIXEpsilon);
    CPPUNIT_ASSERT(lResult < lPOSIXNow + lPOSIXEpsilon);

    if (lNowDateRef != NULL) {
        CFRelease(lNowDateRef);
    }
}
