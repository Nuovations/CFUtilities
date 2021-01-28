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
 *      This file implements a unit test for CFUDateCreate.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <time.h>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUDateCreate :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUDateCreate);
    CPPUNIT_TEST(Test);
    CPPUNIT_TEST_SUITE_END();

public:
    void Test(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUDateCreate);

void
TestCFUDateCreate :: Test(void)
{
    const time_t         lPOSIXNow      = time(NULL);
    const CFAbsoluteTime lCFNow         = CFAbsoluteTimeGetCurrent();
    const CFTimeInterval lCFTimeEpsilon = 1;
    CFDateRef            lCFNowDateRef  = NULL;
    CFDateRef            lCFUNowDateRef = NULL;
    CFTimeInterval       lResult;

    lCFNowDateRef = CFDateCreate(kCFAllocatorDefault, lCFNow);
    CPPUNIT_ASSERT(lCFNowDateRef != NULL);

    lCFUNowDateRef = CFUDateCreate(kCFAllocatorDefault, lPOSIXNow);
    CPPUNIT_ASSERT(lCFUNowDateRef != NULL);

    lResult = CFDateGetTimeIntervalSinceDate(lCFNowDateRef, lCFUNowDateRef);
    CPPUNIT_ASSERT(lResult >= -lCFTimeEpsilon);
    CPPUNIT_ASSERT(lResult <=  lCFTimeEpsilon);

    if (lCFNowDateRef != NULL) {
        CFRelease(lCFNowDateRef);
    }

    if (lCFUNowDateRef != NULL) {
        CFRelease(lCFUNowDateRef);
    }
}
