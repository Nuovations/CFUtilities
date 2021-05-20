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
    CPPUNIT_TEST(TestEncodingCache);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestDefaultConstruction(void);
    void TestCFStringRefConstruction(void);
    void TestCFStringConstruction(void);
    void TestCFStringRefAssignment(void);
    void TestCFStringAssignment(void);
    void TestEquality(void);
    void TestSwap(void);
    void TestEncodingCache(void);

private:
    void Test(const CFString & aString,
              CFStringRef      aCFString,
              const char *     aCString);
    void Test(const CFString & aString,
              CFStringRef      aCFString,
              const char *     aCString,
              const bool &     aEmpty);
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

    lCString = lString.GetUTF8String();
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = strcmp(lCString, "");
    CPPUNIT_ASSERT(l3WayComparison == 0);

    lCFString = lString.GetString();
    CPPUNIT_ASSERT(lCFString == NULL);
}

void
TestCFString :: TestCFStringRefConstruction(void)
{
    static const bool kIsEmpty = true;
    CFStringRef       lCFStringInput_1 = CFSTR("");
    CFString          lString_1(lCFStringInput_1);
    CFStringRef       lCFStringInput_2 = CFSTR("CFStringRef Construction Test String");
    CFString          lString_2(lCFStringInput_2);

    Test(lString_1, lCFStringInput_1, "", kIsEmpty);
    Test(lString_2, lCFStringInput_2, "CFStringRef Construction Test String");
}

void
TestCFString :: TestCFStringConstruction(void)
{
    static const bool kIsEmpty = true;
    CFStringRef       lCFStringInput_1 = CFSTR("");
    CFString          lInitialString_1(lCFStringInput_1);
    CFString          lSecondaryString_1(lInitialString_1);
    CFStringRef       lCFStringInput_2 = CFSTR("CFString Construction Test String");
    CFString          lInitialString_2(lCFStringInput_2);
    CFString          lSecondaryString_2(lInitialString_2);

    Test(lSecondaryString_1, lCFStringInput_1, "", kIsEmpty);
    Test(lSecondaryString_2, lCFStringInput_2, "CFString Construction Test String");
}

void
TestCFString :: TestCFStringRefAssignment(void)
{
    static const bool kIsEmpty = true;
    CFStringRef       lCFStringInput_1 = CFSTR("");
    CFString          lString_1;
    CFStringRef       lCFStringInput_2 = CFSTR("CFStringRef Assignment Test String");
    CFString          lString_2;

    lString_1 = lCFStringInput_1;

    Test(lString_1, lCFStringInput_1, "", kIsEmpty);

    lString_2 = lCFStringInput_2;

    Test(lString_2, lCFStringInput_2, "CFStringRef Assignment Test String");
}

void
TestCFString :: TestCFStringAssignment(void)
{
    static const bool kIsEmpty = true;
    CFStringRef       lCFStringInput_1 = CFSTR("");
    CFString          lInitialString_1(lCFStringInput_1);
    CFString          lSecondaryString_1;
    CFStringRef       lCFStringInput_2 = CFSTR("CFString Assignment Test String");
    CFString          lInitialString_2(lCFStringInput_2);
    CFString          lSecondaryString_2;

    lSecondaryString_1 = lInitialString_1;

    Test(lSecondaryString_1, lCFStringInput_1, "", kIsEmpty);

    lSecondaryString_2 = lInitialString_2;

    Test(lSecondaryString_2, lCFStringInput_2, "CFString Assignment Test String");
}

void
TestCFString :: Test(const CFString & aString,
                     CFStringRef      aCFString,
                     const char *     aCString,
                     const bool &     aEmpty)
{
    CFIndex      lLength;
    bool         lEmpty;
    const char * lCString;
    int          l3WayComparison;
    CFStringRef  lCFString;


    lLength = aString.GetLength();
    CPPUNIT_ASSERT(lLength == CFStringGetLength(aCFString));

    lEmpty = aString.IsEmpty();
    CPPUNIT_ASSERT(lEmpty == aEmpty);

    lCString = aString.GetCString();
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = strcmp(lCString, aCString);
    CPPUNIT_ASSERT(l3WayComparison == 0);

    lCString = aString.GetUTF8String();
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = strcmp(lCString, aCString);
    CPPUNIT_ASSERT(l3WayComparison == 0);

    lCFString = aString.GetString();
    CPPUNIT_ASSERT(lCFString != NULL);
    CPPUNIT_ASSERT(lCFString == aCFString);
}

void
TestCFString :: Test(const CFString & aString,
                     CFStringRef      aCFString,
                     const char *     aCString)
{
    static const bool kIsEmpty = true;

    Test(aString, aCFString, aCString, !kIsEmpty);
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

void
TestCFString :: TestEncodingCache(void)
{
    const bool   kIsExternalRepresentation = true;
    CFStringRef  lCFStringASCIIInput = CFSTR("Test String");
    CFString     lCFStringASCII(lCFStringASCIIInput);
    UInt8        lNonASCIIUTF8Bytes[]     = { 'T', 'e', 's', 't',
                                              's', 't', 'r',
                                              0xc3, 0xa4,
                                              'n', 'g' };
    CFIndex      lNonASCIIUTF8Length      = 11;
    UInt8        lNonASCIIMacRomanBytes[] = { 'T', 'e', 's', 't',
                                              's', 't', 'r',
                                              0x8a,
                                              'n', 'g' };
    CFIndex      lNonASCIIMacRomanLength  = 10;
    CFStringRef  lCFStringNonASCIIInput;
    CFString     lCFStringNonASCII;
    const char * lCString;
    int          l3WayComparison;

    lCFStringNonASCIIInput = CFStringCreateWithBytes(kCFAllocatorDefault,
                                                     &lNonASCIIUTF8Bytes[0],
                                                     lNonASCIIUTF8Length,
                                                     kCFStringEncodingUTF8,
                                                     !kIsExternalRepresentation);
    CPPUNIT_ASSERT(lCFStringNonASCIIInput != NULL);

    lCFStringNonASCII = lCFStringNonASCIIInput;

    // 1: Test encoding wrappers on ASCII input content with
    //    ASCII-compatible encodings for the input.

    lCString = lCFStringASCII.GetCString();
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = strcmp(lCString, "Test String");
    CPPUNIT_ASSERT(l3WayComparison == 0);

    lCString = lCFStringASCII.GetUTF8String();
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = strcmp(lCString, "Test String");
    CPPUNIT_ASSERT(l3WayComparison == 0);

    lCString = lCFStringASCII.GetCString(kCFStringEncodingASCII);
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = strcmp(lCString, "Test String");
    CPPUNIT_ASSERT(l3WayComparison == 0);

    lCString = lCFStringASCII.GetCString(kCFStringEncodingMacRoman);
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = strcmp(lCString, "Test String");
    CPPUNIT_ASSERT(l3WayComparison == 0);

    lCString = lCFStringASCII.GetCString(kCFStringEncodingWindowsLatin1);
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = strcmp(lCString, "Test String");
    CPPUNIT_ASSERT(l3WayComparison == 0);

    lCString = lCFStringASCII.GetCString(kCFStringEncodingISOLatin1);
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = strcmp(lCString, "Test String");
    CPPUNIT_ASSERT(l3WayComparison == 0);

    lCString = lCFStringASCII.GetCString(kCFStringEncodingNextStepLatin);
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = strcmp(lCString, "Test String");
    CPPUNIT_ASSERT(l3WayComparison == 0);

    lCString = lCFStringASCII.GetCString(kCFStringEncodingUTF8);
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = strcmp(lCString, "Test String");
    CPPUNIT_ASSERT(l3WayComparison == 0);

    // 2: Test encoding wrappers on non-ASCII input content with
    //    select encodings for the input.

    // 2.1: UTF-8

    // 2.1.1: Likely cache-miss (non-O(1) access).

    lCString = lCFStringNonASCII.GetCString(kCFStringEncodingUTF8);
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = memcmp(lCString,
                             &lNonASCIIUTF8Bytes[0],
                             static_cast<size_t>(lNonASCIIUTF8Length));
    CPPUNIT_ASSERT(l3WayComparison == 0);

    // 2.1.2: Likely cache-hit (O(1) access).

    lCString = lCFStringNonASCII.GetCString(kCFStringEncodingUTF8);
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = memcmp(lCString,
                             &lNonASCIIUTF8Bytes[0],
                             static_cast<size_t>(lNonASCIIUTF8Length));
    CPPUNIT_ASSERT(l3WayComparison == 0);

    // 2.2: Mac Roman

    // 2.2.1: Likely cache-miss (non-O(1) access).

    lCString = lCFStringNonASCII.GetCString(kCFStringEncodingMacRoman);
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = memcmp(lCString,
                             &lNonASCIIMacRomanBytes[0],
                             static_cast<size_t>(lNonASCIIMacRomanLength));
    CPPUNIT_ASSERT(l3WayComparison == 0);

    // 2.2.2: Likely cache-hit (O(1) access).

    lCString = lCFStringNonASCII.GetCString(kCFStringEncodingMacRoman);
    CPPUNIT_ASSERT(lCString != NULL);

    l3WayComparison = memcmp(lCString,
                             &lNonASCIIMacRomanBytes[0],
                             static_cast<size_t>(lNonASCIIMacRomanLength));
    CPPUNIT_ASSERT(l3WayComparison == 0);
}
