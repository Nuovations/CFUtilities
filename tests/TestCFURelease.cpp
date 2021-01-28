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
 *      This file implements a unit test for CFURelease.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFURelease :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFURelease);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestNonNull);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestNonNull(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFURelease);

void
TestCFURelease :: TestNull(void)
{
    CFTypeRef lTypeRef = NULL;


    CFURelease(lTypeRef);
}

void
TestCFURelease :: TestNonNull(void)
{
    const char * const kTestString = "";
    CFStringRef        lStringRef  = NULL;


    lStringRef = CFStringCreateWithCString(kCFAllocatorDefault,
                                           kTestString,
                                           CFStringGetSystemEncoding());
    CPPUNIT_ASSERT(lStringRef != NULL);

    CFURelease(lStringRef);
}
