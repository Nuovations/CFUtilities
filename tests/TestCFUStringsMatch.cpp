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
 *      This file implements a unit test for CFUStringsMatch.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUStringsMatch :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUStringsMatch);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestMatching);
    CPPUNIT_TEST(TestNotMatching);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestMatching(void);
    void TestNotMatching(void);

private:
    void Test(CFStringRef aFirstString, CFStringRef aSecondString, bool aMatches);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUStringsMatch);

void
TestCFUStringsMatch :: TestNull(void)
{
    const bool  lMatches      = false;
    CFStringRef lFirstString  = CFSTR("This matches!");
    CFStringRef lSecondString = CFSTR("This matches!");

    Test(NULL,         lSecondString, lMatches);
    Test(lFirstString, NULL,          lMatches);
    Test(NULL,         NULL,          lMatches);
}

void
TestCFUStringsMatch :: TestMatching(void)
{
    const bool  lMatches      = true;
    CFStringRef lFirstString  = CFSTR("This matches!");
    CFStringRef lSecondString = CFSTR("This matches!");


    Test(lFirstString, lSecondString, lMatches);
}

void
TestCFUStringsMatch :: TestNotMatching(void)
{
    const bool  lMatches      = true;
    CFStringRef lFirstString  = CFSTR("This does not match!");
    CFStringRef lSecondString = CFSTR("This also does not match!");


    Test(lFirstString, lSecondString, !lMatches);
}

void
TestCFUStringsMatch :: Test(CFStringRef aFirstString,
                            CFStringRef aSecondString,
                            bool        aMatches)
{
    bool lStatus;

    lStatus = CFUStringsMatch(aFirstString, aSecondString);
    CPPUNIT_ASSERT(lStatus == aMatches);
}
