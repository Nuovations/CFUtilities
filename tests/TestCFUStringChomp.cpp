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
 *      This file implements a unit test for CFUStringChomp.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUStringChomp :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUStringChomp);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestMatching);
    CPPUNIT_TEST(TestNotMatching);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestMatching(void);
    void TestNotMatching(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUStringChomp);

void
TestCFUStringChomp :: TestNull(void)
{
    CFMutableStringRef  lMutableStringRef = nullptr;
    size_t              lLength           = 0;
    bool                lStatus;

    lStatus = CFUStringChomp(lMutableStringRef);
    CPPUNIT_ASSERT(lStatus == false);

    lStatus = CFUStringChomp(lMutableStringRef, lLength);
    CPPUNIT_ASSERT(lStatus == false);
}

void
TestCFUStringChomp :: TestMatching(void)
{
    CFStringRef         lStringRef        = CFSTR("This string has a trailing newline\n");
    CFMutableStringRef  lMutableStringRef = nullptr;
    CFIndex             lInitialLength;
    CFIndex             lMutatedLength;
    size_t              lLength;
    bool                lStatus;

    // Test the one-argument interface

    lMutableStringRef = CFStringCreateMutableCopy(kCFAllocatorDefault,
                                                  0,
                                                  lStringRef);
    CPPUNIT_ASSERT(lMutableStringRef != nullptr);

    lStatus = CFUStringChomp(lMutableStringRef);
    CPPUNIT_ASSERT(lStatus == true);

    lInitialLength = CFStringGetLength(lStringRef);
    lMutatedLength = CFStringGetLength(lMutableStringRef);
    CPPUNIT_ASSERT(lMutatedLength == (lInitialLength - 1));

    CFRelease(lMutableStringRef);

    // Test the two-argument interface

    lMutableStringRef = CFStringCreateMutableCopy(kCFAllocatorDefault,
                                                  0,
                                                  lStringRef);
    CPPUNIT_ASSERT(lMutableStringRef != nullptr);

    lInitialLength = CFStringGetLength(lStringRef);
    lLength        = lInitialLength;

    lStatus = CFUStringChomp(lMutableStringRef, lLength);
    CPPUNIT_ASSERT(lStatus == true);

    lMutatedLength = CFStringGetLength(lMutableStringRef);
    CPPUNIT_ASSERT(lMutatedLength == (lInitialLength - 1));
    CPPUNIT_ASSERT(static_cast<size_t>(lMutatedLength) == lLength);

    CFRelease(lMutableStringRef);
}

void
TestCFUStringChomp :: TestNotMatching(void)
{
    CFStringRef         lStringRef        = CFSTR("This string has no trailing newline");
    CFMutableStringRef  lMutableStringRef = nullptr;
    CFIndex             lInitialLength;
    CFIndex             lMutatedLength;
    size_t              lLength;
    bool                lStatus;

    // Test the one-argument interface

    lMutableStringRef = CFStringCreateMutableCopy(kCFAllocatorDefault,
                                                  0,
                                                  lStringRef);
    CPPUNIT_ASSERT(lMutableStringRef != nullptr);

    lStatus = CFUStringChomp(lMutableStringRef);
    CPPUNIT_ASSERT(lStatus == false);

    lInitialLength = CFStringGetLength(lStringRef);
    lMutatedLength = CFStringGetLength(lMutableStringRef);
    CPPUNIT_ASSERT(lMutatedLength == lInitialLength);

    CFRelease(lMutableStringRef);

    // Test the two-argument interface

    lMutableStringRef = CFStringCreateMutableCopy(kCFAllocatorDefault,
                                                  0,
                                                  lStringRef);
    CPPUNIT_ASSERT(lMutableStringRef != nullptr);

    lInitialLength = CFStringGetLength(lStringRef);
    lLength        = lInitialLength;

    lStatus = CFUStringChomp(lMutableStringRef, lLength);
    CPPUNIT_ASSERT(lStatus == false);

    lMutatedLength = CFStringGetLength(lMutableStringRef);
    CPPUNIT_ASSERT(lMutatedLength == lInitialLength);
    CPPUNIT_ASSERT(static_cast<size_t>(lMutatedLength) == lLength);

    CFRelease(lMutableStringRef);
}
