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
 *      This file implements a unit test for CFUGetNumberType.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <stddef.h>
#include <stdint.h>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>


class TestCFUGetNumberType :
    public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCFUGetNumberType);
    CPPUNIT_TEST(Test);
    CPPUNIT_TEST_SUITE_END();

public:
    void Test(void);

private:
    void TestSInt8Type(void);
    void TestSInt16Type(void);
    void TestSInt32Type(void);
    void TestSInt64Type(void);
    void TestSIntFloat32Type(void);
    void TestSIntFloat64Type(void);
    void TestEnum(void);
    void TestPointer(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUGetNumberType);

void
TestCFUGetNumberType :: Test(void)
{
    TestSInt8Type();
    TestSInt16Type();
    TestSInt32Type();
    TestSInt64Type();
    TestSIntFloat32Type();
    TestSIntFloat64Type();

    TestEnum();
    TestPointer();
}

void
TestCFUGetNumberType :: TestSInt8Type(void)
{
    int8_t       lInt8;
    uint8_t      lUint8;
    CFNumberType lResult;

    lResult = CFUGetNumberType(lInt8);
    CPPUNIT_ASSERT(lResult == kCFNumberSInt8Type);

    lResult = CFUGetNumberType(lUint8);
    CPPUNIT_ASSERT(lResult == kCFNumberSInt8Type);
}

void
TestCFUGetNumberType :: TestSInt16Type(void)
{
    int16_t      lInt16;
    uint16_t     lUint16;
    CFNumberType lResult;

    lResult = CFUGetNumberType(lInt16);
    CPPUNIT_ASSERT(lResult == kCFNumberSInt16Type);

    lResult = CFUGetNumberType(lUint16);
    CPPUNIT_ASSERT(lResult == kCFNumberSInt16Type);
}

void
TestCFUGetNumberType :: TestSInt32Type(void)
{
    int32_t      lInt32;
    uint32_t     lUint32;
    CFNumberType lResult;

    lResult = CFUGetNumberType(lInt32);
    CPPUNIT_ASSERT(lResult == kCFNumberSInt32Type);

    lResult = CFUGetNumberType(lUint32);
    CPPUNIT_ASSERT(lResult == kCFNumberSInt32Type);
}

void
TestCFUGetNumberType :: TestSInt64Type(void)
{
    int64_t      lInt64;
    uint64_t     lUint64;
    CFNumberType lResult;

    lResult = CFUGetNumberType(lInt64);
    CPPUNIT_ASSERT(lResult == kCFNumberSInt64Type);

    lResult = CFUGetNumberType(lUint64);
    CPPUNIT_ASSERT(lResult == kCFNumberSInt64Type);
}

void
TestCFUGetNumberType :: TestSIntFloat32Type(void)
{
    float        lFloat;
    CFNumberType lResult;

    lResult = CFUGetNumberType(lFloat);
    CPPUNIT_ASSERT(lResult == kCFNumberFloat32Type);
}

void
TestCFUGetNumberType :: TestSIntFloat64Type(void)
{
    double       lDouble;
    CFNumberType lResult;

    lResult = CFUGetNumberType(lDouble);
    CPPUNIT_ASSERT(lResult == kCFNumberFloat64Type);
}

void
TestCFUGetNumberType :: TestEnum(void)
{
    enum Enum8 {
        kEnum8Value = INT8_MAX
    };
    enum Enum16 {
        kEnum16Value = INT16_MAX
    };
    enum Enum32 {
        kEnum32Value = INT32_MAX
    };
    enum Enum64 {
        kEnum64Value = INT64_MAX
    };
    Enum8        lEnum8;
    Enum16       lEnum16;
    Enum32       lEnum32;
    Enum64       lEnum64;
    CFNumberType lResult;

    lResult = CFUGetNumberType(lEnum8);
    CPPUNIT_ASSERT(lResult == kCFNumberSInt32Type);

    lResult = CFUGetNumberType(lEnum16);
    CPPUNIT_ASSERT(lResult == kCFNumberSInt32Type);

    lResult = CFUGetNumberType(lEnum32);
    CPPUNIT_ASSERT(lResult == kCFNumberSInt32Type);

    lResult = CFUGetNumberType(lEnum64);
    CPPUNIT_ASSERT(lResult == kCFNumberSInt64Type);
}

void
TestCFUGetNumberType :: TestPointer(void)
{
    void *       lPointer;
    CFNumberType lResult;

    lResult = CFUGetNumberType(lPointer);
#if (_LP64 || __LP64__ || (__POINTER_WIDTH__ == 64) || (__SIZEOF_POINTER__ == 8))
    CPPUNIT_ASSERT(lResult = kCFNumberSInt64Type);
#else
    CPPUNIT_ASSERT(lResult = kCFNumberSInt32Type);
#endif
}
