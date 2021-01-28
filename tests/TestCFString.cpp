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
 *      This file implements a unit test for CFString.
 */

#include <CFUtilities/CFString.hpp>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFString :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFString);
    CPPUNIT_TEST(TestDefaultConstruction);
    CPPUNIT_TEST(TestCFStringRefConstruction);
    CPPUNIT_TEST(TestCFStringConstruction);
    CPPUNIT_TEST(TestCFStringRefAssignment);
    CPPUNIT_TEST(TestCFStringAssignment);
    CPPUNIT_TEST(TestEquality);
    CPPUNIT_TEST(TestSwap);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestDefaultConstruction(void);
    void TestCFStringRefConstruction(void);
    void TestCFStringConstruction(void);
    void TestCFStringRefAssignment(void);
    void TestCFStringAssignment(void);
    void TestEquality(void);
    void TestSwap(void);

private:
    void Test(const CFString & aString,
              CFStringRef      aCFString,
              const char *     aCString);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFString);

void
TestCFString :: TestDefaultConstruction(void)
{
    CFString     lString;
    CFIndex      lLength;
    bool         lEmpty;
    const char * lCString;
    int          l3WayComparison;
    CFStringRef  lCFString;

    lLength = lString.GetLength();
    CPPUNIT_ASSERT(lLength == 0);

    lEmpty = lString.IsEmpty();
    CPPUNIT_ASSERT(lEmpty == true);

    lCString = lString.GetCString();
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = strcmp(lCString, "");
    CPPUNIT_ASSERT(l3WayComparison == 0);

    lCFString = lString.GetString();
    CPPUNIT_ASSERT(lCFString == NULL);
}

void
TestCFString :: TestCFStringRefConstruction(void)
{
    CFStringRef lCFStringInput = CFSTR("Test String");
    CFString    lString(lCFStringInput);

    Test(lString, lCFStringInput, "Test String");
}

void
TestCFString :: TestCFStringConstruction(void)
{
    CFStringRef lCFStringInput = CFSTR("Test String");
    CFString    lInitialString(lCFStringInput);
    CFString    lSecondaryString(lInitialString);

    Test(lSecondaryString, lCFStringInput, "Test String");
}

void
TestCFString :: TestCFStringRefAssignment(void)
{
    CFStringRef lCFStringInput = CFSTR("Test String");
    CFString    lString;

    lString = lCFStringInput;

    Test(lString, lCFStringInput, "Test String");
}

void
TestCFString :: TestCFStringAssignment(void)
{
    CFStringRef lCFStringInput = CFSTR("Test String");
    CFString    lInitialString(lCFStringInput);
    CFString    lSecondaryString;

    lSecondaryString = lInitialString;

    Test(lSecondaryString, lCFStringInput, "Test String");
}

void
TestCFString :: Test(const CFString & aString,
                     CFStringRef      aCFString,
                     const char *     aCString)
{
    CFIndex      lLength;
    bool         lEmpty;
    const char * lCString;
    int          l3WayComparison;
    CFStringRef  lCFString;

    lLength = aString.GetLength();
    CPPUNIT_ASSERT(lLength == CFStringGetLength(aCFString));

    lEmpty = aString.IsEmpty();
    CPPUNIT_ASSERT(lEmpty == false);

    lCString = aString.GetCString();
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = strcmp(lCString, aCString);
    CPPUNIT_ASSERT(l3WayComparison == 0);

    lCFString = aString.GetString();
    CPPUNIT_ASSERT(lCFString != NULL);
    CPPUNIT_ASSERT(lCFString == aCFString);
}

void
TestCFString :: TestEquality(void)
{
    CFStringRef lCFStringInput = CFSTR("Test String");
    CFString    lInitialString(lCFStringInput);
    CFString    lSecondaryString(lInitialString);
    bool        lEqual;

    lEqual = (lSecondaryString == lCFStringInput);
    CPPUNIT_ASSERT(lEqual == true);

    lEqual = (lSecondaryString == lInitialString);
    CPPUNIT_ASSERT(lEqual == true);
}

void
TestCFString :: TestSwap(void)
{
    CFStringRef lCFStringInput = CFSTR("Test String");
    CFString    lDefaultString;
    CFString    lNonDefaultString(lCFStringInput);
    CFStringRef lStringRef;

    lDefaultString.Swap(lNonDefaultString);

    lStringRef = lDefaultString.GetString();
    CPPUNIT_ASSERT(lStringRef == lCFStringInput);

    lStringRef = lNonDefaultString.GetString();
    CPPUNIT_ASSERT(lStringRef == NULL);
}
