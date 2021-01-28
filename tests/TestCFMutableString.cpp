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
 *      This file implements a unit test for CFMutableString.
 */

#include <CFUtilities/CFString.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFMutableString :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFMutableString);
    CPPUNIT_TEST(TestDefaultConstruction);
    CPPUNIT_TEST(TestCFMutableStringRefConstruction);
    CPPUNIT_TEST(TestCFMutableStringConstruction);
    CPPUNIT_TEST(TestCFMutableStringRefAssignment);
    CPPUNIT_TEST(TestCFMutableStringAssignment);
    CPPUNIT_TEST(TestEquality);
    CPPUNIT_TEST(TestSwap);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestDefaultConstruction(void);
    void TestCFMutableStringRefConstruction(void);
    void TestCFMutableStringConstruction(void);
    void TestCFMutableStringRefAssignment(void);
    void TestCFMutableStringAssignment(void);
    void TestEquality(void);
    void TestSwap(void);

private:
    void Test(const CFMutableString & aString,
              CFMutableStringRef      aCFString,
              const char *            aCString);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFMutableString);

void
TestCFMutableString :: TestDefaultConstruction(void)
{
    CFMutableString    lMutableString;
    CFIndex            lLength;
    bool               lEmpty;
    const char *       lCString;
    int                l3WayComparison;
    CFMutableStringRef lCFMutableString;

    lLength = lMutableString.GetLength();
    CPPUNIT_ASSERT(lLength == 0);

    lEmpty = lMutableString.IsEmpty();
    CPPUNIT_ASSERT(lEmpty == true);

    lCString = lMutableString.GetCString();
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = strcmp(lCString, "");
    CPPUNIT_ASSERT(l3WayComparison == 0);

    lCFMutableString = lMutableString.GetString();
    CPPUNIT_ASSERT(lCFMutableString == NULL);
}

void
TestCFMutableString :: TestCFMutableStringRefConstruction(void)
{
    CFStringRef        lCFStringInput        = CFSTR("Test String");
    CFMutableStringRef lCFMutableStringInput = NULL;

    lCFMutableStringInput = CFStringCreateMutableCopy(kCFAllocatorDefault,
                                                      0,
                                                      lCFStringInput);
    CPPUNIT_ASSERT(lCFMutableStringInput != NULL);

    {
        CFMutableString lString(lCFMutableStringInput);

        Test(lString, lCFMutableStringInput, "Test String");
    }

    if (lCFMutableStringInput != NULL) {
        CFRelease(lCFMutableStringInput);
    }
}

void
TestCFMutableString :: TestCFMutableStringConstruction(void)
{
    CFStringRef        lCFStringInput        = CFSTR("Test String");
    CFMutableStringRef lCFMutableStringInput = NULL;

    lCFMutableStringInput = CFStringCreateMutableCopy(kCFAllocatorDefault,
                                                      0,
                                                      lCFStringInput);
    CPPUNIT_ASSERT(lCFMutableStringInput != NULL);

    {
        CFMutableString lInitialString(lCFMutableStringInput);
        CFMutableString lSecondaryString(lInitialString);

        Test(lSecondaryString, lCFMutableStringInput, "Test String");
    }

    if (lCFMutableStringInput != NULL) {
        CFRelease(lCFMutableStringInput);
    }
}

void
TestCFMutableString :: TestCFMutableStringRefAssignment(void)
{
    CFStringRef        lCFStringInput        = CFSTR("Test String");
    CFMutableStringRef lCFMutableStringInput = NULL;

    lCFMutableStringInput = CFStringCreateMutableCopy(kCFAllocatorDefault,
                                                      0,
                                                      lCFStringInput);
    CPPUNIT_ASSERT(lCFMutableStringInput != NULL);

    {
        CFMutableString lString;

        lString = lCFMutableStringInput;

        Test(lString, lCFMutableStringInput, "Test String");
    }

    if (lCFMutableStringInput != NULL) {
        CFRelease(lCFMutableStringInput);
    }
}

void
TestCFMutableString :: TestCFMutableStringAssignment(void)
{
    CFStringRef        lCFStringInput        = CFSTR("Test String");
    CFMutableStringRef lCFMutableStringInput = NULL;

    lCFMutableStringInput = CFStringCreateMutableCopy(kCFAllocatorDefault,
                                                      0,
                                                      lCFStringInput);
    CPPUNIT_ASSERT(lCFMutableStringInput != NULL);

    {
        CFMutableString lInitialString(lCFMutableStringInput);
        CFMutableString lSecondaryString;

        lSecondaryString = lInitialString;

        Test(lSecondaryString, lCFMutableStringInput, "Test String");
    }

    if (lCFMutableStringInput != NULL) {
        CFRelease(lCFMutableStringInput);
    }
}

void
TestCFMutableString :: Test(const CFMutableString & aString,
                            CFMutableStringRef      aCFString,
                            const char *            aCString)
{
    CFIndex            lLength;
    bool               lEmpty;
    const char *       lCString;
    int                l3WayComparison;
    CFMutableStringRef lCFMutableString;

    lLength = aString.GetLength();
    CPPUNIT_ASSERT(lLength == CFStringGetLength(aCFString));

    lEmpty = aString.IsEmpty();
    CPPUNIT_ASSERT(lEmpty == false);

    lCString = aString.GetCString();
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = strcmp(lCString, aCString);
    CPPUNIT_ASSERT(l3WayComparison == 0);

    lCFMutableString = aString.GetString();
    CPPUNIT_ASSERT(lCFMutableString != NULL);
    CPPUNIT_ASSERT(lCFMutableString == aCFString);
}

void
TestCFMutableString :: TestEquality(void)
{
    CFStringRef        lCFStringInput        = CFSTR("Test String");
    CFMutableStringRef lCFMutableStringInput = NULL;

    lCFMutableStringInput = CFStringCreateMutableCopy(kCFAllocatorDefault,
                                                      0,
                                                      lCFStringInput);
    CPPUNIT_ASSERT(lCFMutableStringInput != NULL);

    {
        CFMutableString lInitialString(lCFMutableStringInput);
        CFMutableString lSecondaryString(lInitialString);
        bool            lEqual;

        lEqual = (lSecondaryString == lCFMutableStringInput);
        CPPUNIT_ASSERT(lEqual == true);

        lEqual = (lSecondaryString == lInitialString);
        CPPUNIT_ASSERT(lEqual == true);
    }

    if (lCFMutableStringInput != NULL) {
        CFRelease(lCFMutableStringInput);
    }
}

void
TestCFMutableString :: TestSwap(void)
{
    CFStringRef        lCFStringInput        = CFSTR("Test String");
    CFMutableStringRef lCFMutableStringInput = NULL;

    lCFMutableStringInput = CFStringCreateMutableCopy(kCFAllocatorDefault,
                                                      0,
                                                      lCFStringInput);
    CPPUNIT_ASSERT(lCFMutableStringInput != NULL);

    {
        CFMutableString lDefaultString;
        CFMutableString lNonDefaultString(lCFMutableStringInput);
        CFStringRef     lStringRef;

        lDefaultString.Swap(lNonDefaultString);

        lStringRef = lDefaultString.GetString();
        CPPUNIT_ASSERT(lStringRef == lCFMutableStringInput);

        lStringRef = lNonDefaultString.GetString();
        CPPUNIT_ASSERT(lStringRef == NULL);
    }

    if (lCFMutableStringInput != NULL) {
        CFRelease(lCFMutableStringInput);
    }
}
