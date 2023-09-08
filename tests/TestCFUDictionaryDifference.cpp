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
    CPPUNIT_TEST(TestEmptyProposed);
    CPPUNIT_TEST(TestEmptyBase);
    CPPUNIT_TEST(TestIdenticalBaseAndProposed);
    CPPUNIT_TEST(TestDisjointBaseAndProposed);
    CPPUNIT_TEST(TestBaseIsStrictSubsetOfProposed);
    CPPUNIT_TEST(TestBaseIsStrictSupersetOfProposed);
    CPPUNIT_TEST(TestBaseAndProposedDifferInCommonValuesAndHaveNoUniqueEntries);
    CPPUNIT_TEST(TestBaseAndProposedDifferInCommonValuesAndHaveUniqueEntries);
    CPPUNIT_TEST(TestBaseAndProposedHaveCommonValuesAndHaveUniqueEntries);
    CPPUNIT_TEST(TestBaseAndProposedHaveCommonValuesAndHaveUniqueEntriesAddedResultsOnly);
    CPPUNIT_TEST(TestBaseAndProposedHaveCommonValuesAndHaveUniqueEntriesCommonResultsOnly);
    CPPUNIT_TEST(TestBaseAndProposedHaveCommonValuesAndHaveUniqueEntriesRemovedResultsOnly);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestEmptyProposed(void);
    void TestEmptyBase(void);
    void TestIdenticalBaseAndProposed(void);
    void TestDisjointBaseAndProposed(void);
    void TestBaseIsStrictSubsetOfProposed(void);
    void TestBaseIsStrictSupersetOfProposed(void);
    void TestBaseAndProposedDifferInCommonValuesAndHaveNoUniqueEntries(void);
    void TestBaseAndProposedDifferInCommonValuesAndHaveUniqueEntries(void);
    void TestBaseAndProposedHaveCommonValuesAndHaveUniqueEntries(void);
    void TestBaseAndProposedHaveCommonValuesAndHaveUniqueEntriesAddedResultsOnly(void);
    void TestBaseAndProposedHaveCommonValuesAndHaveUniqueEntriesCommonResultsOnly(void);
    void TestBaseAndProposedHaveCommonValuesAndHaveUniqueEntriesRemovedResultsOnly(void);

private:
    void TestSetup(CFMutableDictionaryRef &outAdded,
                   CFMutableDictionaryRef &outCommon,
                   CFMutableDictionaryRef &outRemoved);
    void TestDictionaryCreateWithKeysAndValues(const void **inFirstKey,
                                               const void **inFirstValue,
                                               const size_t &inCount,
                                               CFDictionaryRef &outDictionary);
    void TestDictionaryCreateWithKeysAndValues(const void **inFirstKey,
                                               const void **inFirstValue,
                                               const size_t &inCount,
                                               CFMutableDictionaryRef &outDictionary);
    void TestDictionaryKeysAndValues(CFDictionaryRef inDictionary,
                                     const void **inExpectedFirstKey,
                                     const void **inExpectedFirstValue,
                                     const size_t &inExpectedCount);
    void TestTeardown(CFMutableDictionaryRef inAdded,
                      CFMutableDictionaryRef inCommon,
                      CFMutableDictionaryRef inRemoved);
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

    TestSetup(lAddedDictionaryRef,
              lCommonDictionaryRef,
              lRemovedDictionaryRef);

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

    // Deallocate the added, common, and removed dictionaries

    TestTeardown(lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef);
}

void
TestCFUDictionaryDifference :: TestEmptyProposed(void)
{
    constexpr size_t       kBaseKeyCount              = 2;
    const void *           kBaseKeys[kBaseKeyCount]   = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kBaseValues[kBaseKeyCount] = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4")
    };
    CFMutableDictionaryRef lAddedDictionaryRef    = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef   = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef  = nullptr;
    CFDictionaryRef        lProposedDictionaryRef = nullptr;
    CFMutableDictionaryRef lBaseDictionaryRef     = nullptr;
    Boolean                lStatus;

    // Allocate the added, common, and removed dictionaries

    TestSetup(lAddedDictionaryRef,
              lCommonDictionaryRef,
              lRemovedDictionaryRef);

    // Allocate the base dictionary.

    TestDictionaryCreateWithKeysAndValues(&kBaseKeys[0],
                                          &kBaseValues[0],
                                          kBaseKeyCount,
                                          lBaseDictionaryRef);
    CPPUNIT_ASSERT(lBaseDictionaryRef != nullptr);

    // Allocate the proposed dictionary.

    TestDictionaryCreateWithKeysAndValues(nullptr,
                                          nullptr,
                                          0,
                                          lProposedDictionaryRef);
    CPPUNIT_ASSERT(lProposedDictionaryRef != nullptr);

    // Perform the difference.

    lStatus = CFUDictionaryDifference(lProposedDictionaryRef,
                                      lBaseDictionaryRef,
                                      lAddedDictionaryRef,
                                      lCommonDictionaryRef,
                                      lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Check that we have the expected number of and key/value pairs
    // in the added dictionary.
    //
    // In this case, zero (0).

    TestDictionaryKeysAndValues(lAddedDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Check that we have the expected number of and key/value pairs
    // in the common dictionary.
    //
    // In this case, zero (0).

    TestDictionaryKeysAndValues(lCommonDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Check that we have the expected number of and key/value pairs
    // in the removed dictionary.
    //
    // In this case, two (2), since the proposed has no entries
    // relative to the base two entries.

    TestDictionaryKeysAndValues(lRemovedDictionaryRef,
                                &kBaseKeys[0],
                                &kBaseValues[0],
                                kBaseKeyCount);

    // Deallocate the base and proposed dictionaries.

    CFRelease(lProposedDictionaryRef);
    CFRelease(lBaseDictionaryRef);

    // Deallocate the added, common, and removed dictionaries

    TestTeardown(lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef);
}

void
TestCFUDictionaryDifference :: TestEmptyBase(void)
{
    constexpr size_t       kProposedKeyCount                  = 2;
    const void *           kProposedKeys[kProposedKeyCount]   = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3")
    };
    const void *           kProposedValues[kProposedKeyCount] = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3")
    };
    CFMutableDictionaryRef lAddedDictionaryRef    = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef   = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef  = nullptr;
    CFDictionaryRef        lProposedDictionaryRef = nullptr;
    CFMutableDictionaryRef lBaseDictionaryRef     = nullptr;
    Boolean                lStatus;

    // Allocate the added, common, and removed dictionaries

    TestSetup(lAddedDictionaryRef,
              lCommonDictionaryRef,
              lRemovedDictionaryRef);

    // Allocate the base dictionary.

    TestDictionaryCreateWithKeysAndValues(nullptr,
                                          nullptr,
                                          0,
                                          lBaseDictionaryRef);
    CPPUNIT_ASSERT(lBaseDictionaryRef != nullptr);

    // Allocate the proposed dictionary.

    TestDictionaryCreateWithKeysAndValues(&kProposedKeys[0],
                                          &kProposedValues[0],
                                          kProposedKeyCount,
                                          lProposedDictionaryRef);
    CPPUNIT_ASSERT(lProposedDictionaryRef != nullptr);

    // Perform the difference.

    lStatus = CFUDictionaryDifference(lProposedDictionaryRef,
                                      lBaseDictionaryRef,
                                      lAddedDictionaryRef,
                                      lCommonDictionaryRef,
                                      lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Check that we have the expected number of and key/value pairs
    // in the added dictionary.
    //
    // In this case, two (2), since the proposed has two entries
    // relative to the base no entries.

    TestDictionaryKeysAndValues(lAddedDictionaryRef,
                                &kProposedKeys[0],
                                &kProposedValues[0],
                                kProposedKeyCount);

    // Check that we have the expected number of and key/value pairs
    // in the common dictionary.
    //
    // In this case, zero (0).

    TestDictionaryKeysAndValues(lCommonDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Check that we have the expected number of and key/value pairs
    // in the removed dictionary.
    //
    // In this case, zero (0).

    TestDictionaryKeysAndValues(lRemovedDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Deallocate the base and proposed dictionaries.

    CFRelease(lProposedDictionaryRef);
    CFRelease(lBaseDictionaryRef);

    // Deallocate the added, common, and removed dictionaries

    TestTeardown(lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef);
}

void
TestCFUDictionaryDifference :: TestIdenticalBaseAndProposed(void)
{
    constexpr size_t       kCommonKeyCount                = 2;
    const void *           kCommonKeys[kCommonKeyCount]   = {
        CFSTR("Test Key 5"),
        CFSTR("Test Key 6")
    };
    const void *           kCommonValues[kCommonKeyCount] = {
        CFSTR("Test Value 5"),
        CFSTR("Test Value 6")
    };
    CFMutableDictionaryRef lAddedDictionaryRef    = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef   = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef  = nullptr;
    CFDictionaryRef        lProposedDictionaryRef = nullptr;
    CFMutableDictionaryRef lBaseDictionaryRef     = nullptr;
    Boolean                lStatus;

    // Allocate the added, common, and removed dictionaries

    TestSetup(lAddedDictionaryRef,
              lCommonDictionaryRef,
              lRemovedDictionaryRef);

    // Allocate the base dictionary.

    TestDictionaryCreateWithKeysAndValues(&kCommonKeys[0],
                                          &kCommonValues[0],
                                          kCommonKeyCount,
                                          lBaseDictionaryRef);
    CPPUNIT_ASSERT(lBaseDictionaryRef != nullptr);

    // Allocate the proposed dictionary.

    TestDictionaryCreateWithKeysAndValues(&kCommonKeys[0],
                                          &kCommonValues[0],
                                          kCommonKeyCount,
                                          lProposedDictionaryRef);
    CPPUNIT_ASSERT(lProposedDictionaryRef != nullptr);

    // Perform the difference.

    lStatus = CFUDictionaryDifference(lProposedDictionaryRef,
                                      lBaseDictionaryRef,
                                      lAddedDictionaryRef,
                                      lCommonDictionaryRef,
                                      lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Check that we have the expected number of and key/value pairs
    // in the added dictionary.
    //
    // In this case, zero (0).

    TestDictionaryKeysAndValues(lAddedDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Check that we have the expected number of and key/value pairs
    // in the common dictionary.
    //
    // In this case, two (2), since the proposed and base entries are
    // the same identical set of two.

    TestDictionaryKeysAndValues(lCommonDictionaryRef,
                                &kCommonKeys[0],
                                &kCommonValues[0],
                                kCommonKeyCount);

    // Check that we have the expected number of and key/value pairs
    // in the removed dictionary.
    //
    // In this case, zero (0).

    TestDictionaryKeysAndValues(lRemovedDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Deallocate the base and proposed dictionaries.

    CFRelease(lProposedDictionaryRef);
    CFRelease(lBaseDictionaryRef);

    // Deallocate the added, common, and removed dictionaries

    TestTeardown(lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef);
}

void
TestCFUDictionaryDifference :: TestDisjointBaseAndProposed(void)
{
    constexpr size_t       kBaseKeyCount                      = 2;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4")
    };
    constexpr size_t       kProposedKeyCount                  = 2;
    const void *           kProposedKeys[kProposedKeyCount]   = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3")
    };
    const void *           kProposedValues[kProposedKeyCount] = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3")
    };
    CFMutableDictionaryRef lAddedDictionaryRef    = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef   = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef  = nullptr;
    CFDictionaryRef        lProposedDictionaryRef = nullptr;
    CFMutableDictionaryRef lBaseDictionaryRef     = nullptr;
    Boolean                lStatus;

    // Allocate the added, common, and removed dictionaries

    TestSetup(lAddedDictionaryRef,
              lCommonDictionaryRef,
              lRemovedDictionaryRef);

    // Allocate the base dictionary.

    TestDictionaryCreateWithKeysAndValues(&kBaseKeys[0],
                                          &kBaseValues[0],
                                          kBaseKeyCount,
                                          lBaseDictionaryRef);
    CPPUNIT_ASSERT(lBaseDictionaryRef != nullptr);

    // Allocate the proposed dictionary.

    TestDictionaryCreateWithKeysAndValues(&kProposedKeys[0],
                                          &kProposedValues[0],
                                          kProposedKeyCount,
                                          lProposedDictionaryRef);
    CPPUNIT_ASSERT(lProposedDictionaryRef != nullptr);

    // Perform the difference.

    lStatus = CFUDictionaryDifference(lProposedDictionaryRef,
                                      lBaseDictionaryRef,
                                      lAddedDictionaryRef,
                                      lCommonDictionaryRef,
                                      lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Check that we have the expected number of and key/value pairs
    // in the added dictionary.
    //
    // In this case, two (2), since the proposed has two disjoint entries
    // relative to the base two entries.

    TestDictionaryKeysAndValues(lAddedDictionaryRef,
                                &kProposedKeys[0],
                                &kProposedValues[0],
                                kProposedKeyCount);

    // Check that we have the expected number of and key/value pairs
    // in the common dictionary.
    //
    // In this case, zero (0).

    TestDictionaryKeysAndValues(lCommonDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Check that we have the expected number of and key/value pairs
    // in the removed dictionary.
    //
    // In this case, two (2), since the base has two disjoint entries
    // relative to the proposed two entries.

    TestDictionaryKeysAndValues(lRemovedDictionaryRef,
                                &kBaseKeys[0],
                                &kBaseValues[0],
                                kBaseKeyCount);

    // Deallocate the base and proposed dictionaries.

    CFRelease(lProposedDictionaryRef);
    CFRelease(lBaseDictionaryRef);

    // Deallocate the added, common, and removed dictionaries

    TestTeardown(lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef);
}

void
TestCFUDictionaryDifference :: TestBaseIsStrictSubsetOfProposed(void)
{
    constexpr size_t       kBaseKeyCount                      = 2;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3")
    };
    constexpr size_t       kProposedKeyCount                  = 3;
    const void *           kProposedKeys[kProposedKeyCount]   = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3"),
        CFSTR("Test Key 7"),
    };
    const void *           kProposedValues[kProposedKeyCount] = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3"),
        CFSTR("Test Value 7")
    };
    CFMutableDictionaryRef lAddedDictionaryRef    = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef   = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef  = nullptr;
    CFDictionaryRef        lProposedDictionaryRef = nullptr;
    CFMutableDictionaryRef lBaseDictionaryRef     = nullptr;
    Boolean                lStatus;

    // Allocate the added, common, and removed dictionaries

    TestSetup(lAddedDictionaryRef,
              lCommonDictionaryRef,
              lRemovedDictionaryRef);

    // Allocate the base dictionary.

    TestDictionaryCreateWithKeysAndValues(&kBaseKeys[0],
                                          &kBaseValues[0],
                                          kBaseKeyCount,
                                          lBaseDictionaryRef);
    CPPUNIT_ASSERT(lBaseDictionaryRef != nullptr);

    // Allocate the proposed dictionary.

    TestDictionaryCreateWithKeysAndValues(&kProposedKeys[0],
                                          &kProposedValues[0],
                                          kProposedKeyCount,
                                          lProposedDictionaryRef);
    CPPUNIT_ASSERT(lProposedDictionaryRef != nullptr);

    // Perform the difference.

    lStatus = CFUDictionaryDifference(lProposedDictionaryRef,
                                      lBaseDictionaryRef,
                                      lAddedDictionaryRef,
                                      lCommonDictionaryRef,
                                      lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Check that we have the expected number of and key/value pairs
    // in the added dictionary.
    //
    // In this case, one (1), since the proposed is strictly a
    // superset of the base with a single unique entry.

    TestDictionaryKeysAndValues(lAddedDictionaryRef,
                                &kProposedKeys[kProposedKeyCount - 1],
                                &kProposedValues[kProposedKeyCount - 1],
                                kProposedKeyCount - kBaseKeyCount);

    // Check that we have the expected number of and key/value pairs
    // in the common dictionary.
    //
    // In this case, two (2) since the proposed is strictly a superset
    // of the base with two (2) common entries among them.

    TestDictionaryKeysAndValues(lCommonDictionaryRef,
                                &kBaseKeys[0],
                                &kBaseValues[0],
                                kBaseKeyCount);

    // Check that we have the expected number of and key/value pairs
    // in the removed dictionary.
    //
    // In this case, zero (0), since the proposed is strictly a
    // superset of the base.

    TestDictionaryKeysAndValues(lRemovedDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Deallocate the base and proposed dictionaries.

    CFRelease(lProposedDictionaryRef);
    CFRelease(lBaseDictionaryRef);

    // Deallocate the added, common, and removed dictionaries

    TestTeardown(lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef);
}

void
TestCFUDictionaryDifference :: TestBaseIsStrictSupersetOfProposed(void)
{
    constexpr size_t       kBaseKeyCount                      = 3;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3"),
        CFSTR("Test Key 7")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3"),
        CFSTR("Test Value 7")
    };
    constexpr size_t       kProposedKeyCount                  = 2;
    const void *           kProposedKeys[kProposedKeyCount]   = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3"),

    };
    const void *           kProposedValues[kProposedKeyCount] = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3")
    };
    CFMutableDictionaryRef lAddedDictionaryRef    = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef   = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef  = nullptr;
    CFDictionaryRef        lProposedDictionaryRef = nullptr;
    CFMutableDictionaryRef lBaseDictionaryRef     = nullptr;
    Boolean                lStatus;

    // Allocate the added, common, and removed dictionaries

    TestSetup(lAddedDictionaryRef,
              lCommonDictionaryRef,
              lRemovedDictionaryRef);

    // Allocate the base dictionary.

    TestDictionaryCreateWithKeysAndValues(&kBaseKeys[0],
                                          &kBaseValues[0],
                                          kBaseKeyCount,
                                          lBaseDictionaryRef);
    CPPUNIT_ASSERT(lBaseDictionaryRef != nullptr);

    // Allocate the proposed dictionary.

    TestDictionaryCreateWithKeysAndValues(&kProposedKeys[0],
                                          &kProposedValues[0],
                                          kProposedKeyCount,
                                          lProposedDictionaryRef);
    CPPUNIT_ASSERT(lProposedDictionaryRef != nullptr);

    // Perform the difference.

    lStatus = CFUDictionaryDifference(lProposedDictionaryRef,
                                      lBaseDictionaryRef,
                                      lAddedDictionaryRef,
                                      lCommonDictionaryRef,
                                      lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Check that we have the expected number of and key/value pairs
    // in the added dictionary.
    //
    // In this case, zero (0), since the base is strictly a
    // superset of the proposed.

    TestDictionaryKeysAndValues(lAddedDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Check that we have the expected number of and key/value pairs
    // in the common dictionary.
    //
    // In this case, two (2) since the base is strictly a superset
    // of the proposed with two (2) common entries among them.

    TestDictionaryKeysAndValues(lCommonDictionaryRef,
                                &kProposedKeys[0],
                                &kProposedValues[0],
                                kProposedKeyCount);

    // Check that we have the expected number of and key/value pairs
    // in the removed dictionary.
    //
    // In this case, one (1), since the base is strictly a
    // superset of the proposed with a single unique entry.

    TestDictionaryKeysAndValues(lRemovedDictionaryRef,
                                &kBaseKeys[kBaseKeyCount - 1],
                                &kBaseValues[kBaseKeyCount - 1],
                                kBaseKeyCount - kProposedKeyCount);

    // Deallocate the base and proposed dictionaries.

    CFRelease(lProposedDictionaryRef);
    CFRelease(lBaseDictionaryRef);

    // Deallocate the added, common, and removed dictionaries

    TestTeardown(lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef);
}

void
TestCFUDictionaryDifference :: TestBaseAndProposedDifferInCommonValuesAndHaveNoUniqueEntries(void)
{
    constexpr size_t       kBaseKeyCount                      = 2;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 8"),
        CFSTR("Test Key 10")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 8"),
        CFSTR("Test Value 10")
    };
    constexpr size_t       kProposedKeyCount                  = 2;
    const void *           kProposedKeys[kProposedKeyCount]   = {
        CFSTR("Test Key 8"),
        CFSTR("Test Key 10")
    };
    const void *           kProposedValues[kProposedKeyCount] = {
        CFSTR("Test Value 12"),
        CFSTR("Test Value 14")
    };
    CFMutableDictionaryRef lAddedDictionaryRef    = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef   = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef  = nullptr;
    CFDictionaryRef        lProposedDictionaryRef = nullptr;
    CFMutableDictionaryRef lBaseDictionaryRef     = nullptr;
    Boolean                lStatus;

    // Allocate the added, common, and removed dictionaries

    TestSetup(lAddedDictionaryRef,
              lCommonDictionaryRef,
              lRemovedDictionaryRef);

    // Allocate the base dictionary.

    TestDictionaryCreateWithKeysAndValues(&kBaseKeys[0],
                                          &kBaseValues[0],
                                          kBaseKeyCount,
                                          lBaseDictionaryRef);
    CPPUNIT_ASSERT(lBaseDictionaryRef != nullptr);

    // Allocate the proposed dictionary.

    TestDictionaryCreateWithKeysAndValues(&kProposedKeys[0],
                                          &kProposedValues[0],
                                          kProposedKeyCount,
                                          lProposedDictionaryRef);
    CPPUNIT_ASSERT(lProposedDictionaryRef != nullptr);

    // Perform the difference.

    lStatus = CFUDictionaryDifference(lProposedDictionaryRef,
                                      lBaseDictionaryRef,
                                      lAddedDictionaryRef,
                                      lCommonDictionaryRef,
                                      lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Check that we have the expected number of and key/value pairs
    // in the added dictionary.
    //
    // In this case, zero (0), since the base and proposed share
    // common keys but different values.

    TestDictionaryKeysAndValues(lAddedDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Check that we have the expected number of and key/value pairs
    // in the common dictionary.
    //
    // In this case, two (2), since the base and proposed have common
    // keys but different values.

    TestDictionaryKeysAndValues(lCommonDictionaryRef,
                                &kBaseKeys[0],
                                &kBaseValues[0],
                                kBaseKeyCount);

    // Check that we have the expected number of and key/value pairs
    // in the removed dictionary.
    //
    // In this case, zero (0), since the base and proposed share
    // common keys but different values.

    TestDictionaryKeysAndValues(lRemovedDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Deallocate the base and proposed dictionaries.

    CFRelease(lProposedDictionaryRef);
    CFRelease(lBaseDictionaryRef);

    // Deallocate the added, common, and removed dictionaries

    TestTeardown(lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef);
}

void
TestCFUDictionaryDifference :: TestBaseAndProposedDifferInCommonValuesAndHaveUniqueEntries(void)
{
    constexpr size_t       kBaseKeyCount                      = 4;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4"),
        CFSTR("Test Key 8"),
        CFSTR("Test Key 10")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4"),
        CFSTR("Test Value 8"),
        CFSTR("Test Value 10")
    };
    constexpr size_t       kProposedKeyCount                  = 4;
    const void *           kProposedKeys[kProposedKeyCount]   = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3"),
        CFSTR("Test Key 8"),
        CFSTR("Test Key 10")
    };
    const void *           kProposedValues[kProposedKeyCount] = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3"),
        CFSTR("Test Value 12"),
        CFSTR("Test Value 14")
    };
    CFMutableDictionaryRef lAddedDictionaryRef    = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef   = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef  = nullptr;
    CFDictionaryRef        lProposedDictionaryRef = nullptr;
    CFMutableDictionaryRef lBaseDictionaryRef     = nullptr;
    Boolean                lStatus;

    // Allocate the added, common, and removed dictionaries

    TestSetup(lAddedDictionaryRef,
              lCommonDictionaryRef,
              lRemovedDictionaryRef);

    // Allocate the base dictionary.

    TestDictionaryCreateWithKeysAndValues(&kBaseKeys[0],
                                          &kBaseValues[0],
                                          kBaseKeyCount,
                                          lBaseDictionaryRef);
    CPPUNIT_ASSERT(lBaseDictionaryRef != nullptr);

    // Allocate the proposed dictionary.

    TestDictionaryCreateWithKeysAndValues(&kProposedKeys[0],
                                          &kProposedValues[0],
                                          kProposedKeyCount,
                                          lProposedDictionaryRef);
    CPPUNIT_ASSERT(lProposedDictionaryRef != nullptr);

    // Perform the difference.

    lStatus = CFUDictionaryDifference(lProposedDictionaryRef,
                                      lBaseDictionaryRef,
                                      lAddedDictionaryRef,
                                      lCommonDictionaryRef,
                                      lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Check that we have the expected number of and key/value pairs
    // in the added dictionary.
    //
    // In this case, two (2), since the proposed has two unique
    // entries not present in the base in addition to their common
    // keys.

    TestDictionaryKeysAndValues(lAddedDictionaryRef,
                                &kProposedKeys[0],
                                &kProposedValues[0],
                                2);

    // Check that we have the expected number of and key/value pairs
    // in the common dictionary.
    //
    // In this case, two (2), since the base and proposed have two (2)
    // common keys but with different values.

    TestDictionaryKeysAndValues(lCommonDictionaryRef,
                                &kBaseKeys[2],
                                &kBaseValues[2],
                                2);

    // Check that we have the expected number of and key/value pairs
    // in the removed dictionary.
    //
    // In this case, two (2), since the base has two unique
    // entries not present in the proposed in addition to their common
    // keys.

    TestDictionaryKeysAndValues(lRemovedDictionaryRef,
                                &kBaseKeys[0],
                                &kBaseValues[0],
                                2);

    // Deallocate the base and proposed dictionaries.

    CFRelease(lProposedDictionaryRef);
    CFRelease(lBaseDictionaryRef);

    // Deallocate the added, common, and removed dictionaries

    TestTeardown(lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef);
}

void
TestCFUDictionaryDifference :: TestBaseAndProposedHaveCommonValuesAndHaveUniqueEntries(void)
{
    constexpr size_t       kBaseKeyCount                      = 4;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4"),
        CFSTR("Test Key 8"),
        CFSTR("Test Key 10")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4"),
        CFSTR("Test Value 8"),
        CFSTR("Test Value 10")
    };
    constexpr size_t       kProposedKeyCount                  = 4;
    const void *           kProposedKeys[kProposedKeyCount]   = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3"),
        CFSTR("Test Key 8"),
        CFSTR("Test Key 10")
    };
    const void *           kProposedValues[kProposedKeyCount] = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3"),
        CFSTR("Test Value 8"),
        CFSTR("Test Value 10")
    };
    CFMutableDictionaryRef lAddedDictionaryRef    = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef   = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef  = nullptr;
    CFDictionaryRef        lProposedDictionaryRef = nullptr;
    CFMutableDictionaryRef lBaseDictionaryRef     = nullptr;
    Boolean                lStatus;

    // Allocate the added, common, and removed dictionaries

    TestSetup(lAddedDictionaryRef,
              lCommonDictionaryRef,
              lRemovedDictionaryRef);

    // Allocate the base dictionary.

    TestDictionaryCreateWithKeysAndValues(&kBaseKeys[0],
                                          &kBaseValues[0],
                                          kBaseKeyCount,
                                          lBaseDictionaryRef);
    CPPUNIT_ASSERT(lBaseDictionaryRef != nullptr);

    // Allocate the proposed dictionary.

    TestDictionaryCreateWithKeysAndValues(&kProposedKeys[0],
                                          &kProposedValues[0],
                                          kProposedKeyCount,
                                          lProposedDictionaryRef);
    CPPUNIT_ASSERT(lProposedDictionaryRef != nullptr);

    // Perform the difference.

    lStatus = CFUDictionaryDifference(lProposedDictionaryRef,
                                      lBaseDictionaryRef,
                                      lAddedDictionaryRef,
                                      lCommonDictionaryRef,
                                      lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Check that we have the expected number of and key/value pairs
    // in the added dictionary.
    //
    // In this case, two (2), since the proposed has two unique
    // entries not present in the base in addition to their common
    // keys.

    TestDictionaryKeysAndValues(lAddedDictionaryRef,
                                &kProposedKeys[0],
                                &kProposedValues[0],
                                2);

    // Check that we have the expected number of and key/value pairs
    // in the common dictionary.
    //
    // In this case, two (2), since the base and proposed have two (2)
    // common keys.

    TestDictionaryKeysAndValues(lCommonDictionaryRef,
                                &kBaseKeys[2],
                                &kBaseValues[2],
                                2);

    // Check that we have the expected number of and key/value pairs
    // in the removed dictionary.
    //
    // In this case, two (2), since the base has two unique
    // entries not present in the proposed in addition to their common
    // keys.

    TestDictionaryKeysAndValues(lRemovedDictionaryRef,
                                &kBaseKeys[0],
                                &kBaseValues[0],
                                2);

    // Deallocate the base and proposed dictionaries.

    CFRelease(lProposedDictionaryRef);
    CFRelease(lBaseDictionaryRef);

    // Deallocate the added, common, and removed dictionaries

    TestTeardown(lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef);
}

void
TestCFUDictionaryDifference :: TestBaseAndProposedHaveCommonValuesAndHaveUniqueEntriesAddedResultsOnly(void)
{
    constexpr size_t       kBaseKeyCount                      = 4;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4"),
        CFSTR("Test Key 8"),
        CFSTR("Test Key 10")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4"),
        CFSTR("Test Value 8"),
        CFSTR("Test Value 10")
    };
    constexpr size_t       kProposedKeyCount                  = 4;
    const void *           kProposedKeys[kProposedKeyCount]   = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3"),
        CFSTR("Test Key 8"),
        CFSTR("Test Key 10")
    };
    const void *           kProposedValues[kProposedKeyCount] = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3"),
        CFSTR("Test Value 8"),
        CFSTR("Test Value 10")
    };
    CFMutableDictionaryRef lAddedDictionaryRef    = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef   = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef  = nullptr;
    CFDictionaryRef        lProposedDictionaryRef = nullptr;
    CFMutableDictionaryRef lBaseDictionaryRef     = nullptr;
    Boolean                lStatus;

    // Allocate the added, common, and removed dictionaries

    TestSetup(lAddedDictionaryRef,
              lCommonDictionaryRef,
              lRemovedDictionaryRef);

    // Allocate the base dictionary.

    TestDictionaryCreateWithKeysAndValues(&kBaseKeys[0],
                                          &kBaseValues[0],
                                          kBaseKeyCount,
                                          lBaseDictionaryRef);
    CPPUNIT_ASSERT(lBaseDictionaryRef != nullptr);

    // Allocate the proposed dictionary.

    TestDictionaryCreateWithKeysAndValues(&kProposedKeys[0],
                                          &kProposedValues[0],
                                          kProposedKeyCount,
                                          lProposedDictionaryRef);
    CPPUNIT_ASSERT(lProposedDictionaryRef != nullptr);

    // Perform the difference.

    lStatus = CFUDictionaryDifference(lProposedDictionaryRef,
                                      lBaseDictionaryRef,
                                      lAddedDictionaryRef,
                                      nullptr,
                                      nullptr);
    CPPUNIT_ASSERT(lStatus == true);

    // Check that we have the expected number of and key/value pairs
    // in the added dictionary.
    //
    // In this case, two (2), since the proposed has two unique
    // entries not present in the base in addition to their common
    // keys.

    TestDictionaryKeysAndValues(lAddedDictionaryRef,
                                &kProposedKeys[0],
                                &kProposedValues[0],
                                2);

    // Check that we have the expected number of and key/value pairs
    // in the common dictionary.
    //
    // In this case, zero (0), since no common dictionary was passed
    // to the difference interface.

    TestDictionaryKeysAndValues(lCommonDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Check that we have the expected number of and key/value pairs
    // in the removed dictionary.
    //
    // In this case, zero (0), since no removed dictionary was passed
    // to the difference interface.

    TestDictionaryKeysAndValues(lRemovedDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Deallocate the base and proposed dictionaries.

    CFRelease(lProposedDictionaryRef);
    CFRelease(lBaseDictionaryRef);

    // Deallocate the added, common, and removed dictionaries

    TestTeardown(lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef);
}

void
TestCFUDictionaryDifference :: TestBaseAndProposedHaveCommonValuesAndHaveUniqueEntriesCommonResultsOnly(void)
{
    constexpr size_t       kBaseKeyCount                      = 4;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4"),
        CFSTR("Test Key 8"),
        CFSTR("Test Key 10")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4"),
        CFSTR("Test Value 8"),
        CFSTR("Test Value 10")
    };
    constexpr size_t       kProposedKeyCount                  = 4;
    const void *           kProposedKeys[kProposedKeyCount]   = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3"),
        CFSTR("Test Key 8"),
        CFSTR("Test Key 10")
    };
    const void *           kProposedValues[kProposedKeyCount] = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3"),
        CFSTR("Test Value 8"),
        CFSTR("Test Value 10")
    };
    CFMutableDictionaryRef lAddedDictionaryRef    = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef   = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef  = nullptr;
    CFDictionaryRef        lProposedDictionaryRef = nullptr;
    CFMutableDictionaryRef lBaseDictionaryRef     = nullptr;
    Boolean                lStatus;

    // Allocate the added, common, and removed dictionaries

    TestSetup(lAddedDictionaryRef,
              lCommonDictionaryRef,
              lRemovedDictionaryRef);

    // Allocate the base dictionary.

    TestDictionaryCreateWithKeysAndValues(&kBaseKeys[0],
                                          &kBaseValues[0],
                                          kBaseKeyCount,
                                          lBaseDictionaryRef);
    CPPUNIT_ASSERT(lBaseDictionaryRef != nullptr);

    // Allocate the proposed dictionary.

    TestDictionaryCreateWithKeysAndValues(&kProposedKeys[0],
                                          &kProposedValues[0],
                                          kProposedKeyCount,
                                          lProposedDictionaryRef);
    CPPUNIT_ASSERT(lProposedDictionaryRef != nullptr);

    // Perform the difference.

    lStatus = CFUDictionaryDifference(lProposedDictionaryRef,
                                      lBaseDictionaryRef,
                                      nullptr,
                                      lCommonDictionaryRef,
                                      nullptr);
    CPPUNIT_ASSERT(lStatus == true);

    // Check that we have the expected number of and key/value pairs
    // in the added dictionary.
    //
    // In this case, zero (0), since no added dictionary was passed
    // to the difference interface.

    TestDictionaryKeysAndValues(lAddedDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Check that we have the expected number of and key/value pairs
    // in the common dictionary.
    //
    // In this case, two (2), since the base and proposed have two (2)
    // common keys.

    TestDictionaryKeysAndValues(lCommonDictionaryRef,
                                &kBaseKeys[2],
                                &kBaseValues[2],
                                2);

    // Check that we have the expected number of and key/value pairs
    // in the removed dictionary.
    //
    // In this case, zero (0), since no removed dictionary was passed
    // to the difference interface.

    TestDictionaryKeysAndValues(lRemovedDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Deallocate the base and proposed dictionaries.

    CFRelease(lProposedDictionaryRef);
    CFRelease(lBaseDictionaryRef);

    // Deallocate the added, common, and removed dictionaries

    TestTeardown(lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef);
}

void
TestCFUDictionaryDifference :: TestBaseAndProposedHaveCommonValuesAndHaveUniqueEntriesRemovedResultsOnly(void)
{
    constexpr size_t       kBaseKeyCount                      = 4;
    const void *           kBaseKeys[kBaseKeyCount]           = {
        CFSTR("Test Key 2"),
        CFSTR("Test Key 4"),
        CFSTR("Test Key 8"),
        CFSTR("Test Key 10")
    };
    const void *           kBaseValues[kBaseKeyCount]         = {
        CFSTR("Test Value 2"),
        CFSTR("Test Value 4"),
        CFSTR("Test Value 8"),
        CFSTR("Test Value 10")
    };
    constexpr size_t       kProposedKeyCount                  = 4;
    const void *           kProposedKeys[kProposedKeyCount]   = {
        CFSTR("Test Key 1"),
        CFSTR("Test Key 3"),
        CFSTR("Test Key 8"),
        CFSTR("Test Key 10")
    };
    const void *           kProposedValues[kProposedKeyCount] = {
        CFSTR("Test Value 1"),
        CFSTR("Test Value 3"),
        CFSTR("Test Value 8"),
        CFSTR("Test Value 10")
    };
    CFMutableDictionaryRef lAddedDictionaryRef    = nullptr;
    CFMutableDictionaryRef lCommonDictionaryRef   = nullptr;
    CFMutableDictionaryRef lRemovedDictionaryRef  = nullptr;
    CFDictionaryRef        lProposedDictionaryRef = nullptr;
    CFMutableDictionaryRef lBaseDictionaryRef     = nullptr;
    Boolean                lStatus;

    // Allocate the added, common, and removed dictionaries

    TestSetup(lAddedDictionaryRef,
              lCommonDictionaryRef,
              lRemovedDictionaryRef);

    // Allocate the base dictionary.

    TestDictionaryCreateWithKeysAndValues(&kBaseKeys[0],
                                          &kBaseValues[0],
                                          kBaseKeyCount,
                                          lBaseDictionaryRef);
    CPPUNIT_ASSERT(lBaseDictionaryRef != nullptr);

    // Allocate the proposed dictionary.

    TestDictionaryCreateWithKeysAndValues(&kProposedKeys[0],
                                          &kProposedValues[0],
                                          kProposedKeyCount,
                                          lProposedDictionaryRef);
    CPPUNIT_ASSERT(lProposedDictionaryRef != nullptr);

    // Perform the difference.

    lStatus = CFUDictionaryDifference(lProposedDictionaryRef,
                                      lBaseDictionaryRef,
                                      nullptr,
                                      nullptr,
                                      lRemovedDictionaryRef);
    CPPUNIT_ASSERT(lStatus == true);

    // Check that we have the expected number of and key/value pairs
    // in the added dictionary.
    //
    // In this case, zero (0), since no added dictionary was passed
    // to the difference interface.

    TestDictionaryKeysAndValues(lAddedDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Check that we have the expected number of and key/value pairs
    // in the common dictionary.
    //
    // In this case, zero (0), since no common dictionary was passed
    // to the difference interface.

    TestDictionaryKeysAndValues(lCommonDictionaryRef,
                                nullptr,
                                nullptr,
                                0);

    // Check that we have the expected number of and key/value pairs
    // in the removed dictionary.
    //
    // In this case, two (2), since the base has two unique
    // entries not present in the proposed in addition to their common
    // keys.

    TestDictionaryKeysAndValues(lRemovedDictionaryRef,
                                &kBaseKeys[0],
                                &kBaseValues[0],
                                2);

    // Deallocate the base and proposed dictionaries.

    CFRelease(lProposedDictionaryRef);
    CFRelease(lBaseDictionaryRef);

    // Deallocate the added, common, and removed dictionaries

    TestTeardown(lAddedDictionaryRef,
                 lCommonDictionaryRef,
                 lRemovedDictionaryRef);
}

void
TestCFUDictionaryDifference :: TestDictionaryCreateWithKeysAndValues(const void **inFirstKey,
                                                                     const void **inFirstValue,
                                                                     const size_t &inCount,
                                                                     CFDictionaryRef &outDictionary)
{
    CPPUNIT_ASSERT(outDictionary == nullptr);

    outDictionary = CFDictionaryCreate(kCFAllocatorDefault,
                                       inFirstKey,
                                       inFirstValue,
                                       inCount,
                                       &kCFTypeDictionaryKeyCallBacks,
                                       &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(outDictionary != nullptr);
}

void
TestCFUDictionaryDifference :: TestDictionaryCreateWithKeysAndValues(const void **inFirstKey,
                                                                     const void **inFirstValue,
                                                                     const size_t &inCount,
                                                                     CFMutableDictionaryRef &outDictionary)
{
    CPPUNIT_ASSERT(outDictionary == nullptr);

    outDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                              0,
                                              &kCFTypeDictionaryKeyCallBacks,
                                              &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(outDictionary != nullptr);

    for (size_t i = 0; i < inCount; i++)
    {
        CFDictionaryAddValue(outDictionary, inFirstKey[i], inFirstValue[i]);
    }
}

void
TestCFUDictionaryDifference :: TestSetup(CFMutableDictionaryRef &outAdded,
                                         CFMutableDictionaryRef &outCommon,
                                         CFMutableDictionaryRef &outRemoved)
{
    CPPUNIT_ASSERT(outAdded == nullptr);
    CPPUNIT_ASSERT(outCommon == nullptr);
    CPPUNIT_ASSERT(outRemoved == nullptr);

    outAdded = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                         0,
                                         &kCFTypeDictionaryKeyCallBacks,
                                         &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(outAdded != nullptr);

    outCommon = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                          0,
                                          &kCFTypeDictionaryKeyCallBacks,
                                          &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(outCommon != nullptr);

    outRemoved = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                           0,
                                           &kCFTypeDictionaryKeyCallBacks,
                                           &kCFTypeDictionaryValueCallBacks);
    CPPUNIT_ASSERT(outRemoved != nullptr);
}

void
TestCFUDictionaryDifference :: TestDictionaryKeysAndValues(CFDictionaryRef inDictionary,
                                                           const void **inExpectedFirstKey,
                                                           const void **inExpectedFirstValue,
                                                           const size_t &inExpectedCount)
{
    CFIndex lDictionaryCount;
    
    CPPUNIT_ASSERT(inDictionary != nullptr);

    lDictionaryCount = CFDictionaryGetCount(inDictionary);
    CPPUNIT_ASSERT(static_cast<size_t>(lDictionaryCount) == inExpectedCount);

    for (size_t i = 0; i < inExpectedCount; i++)
    {
        CFStringRef            lStringValue;
        CFComparisonResult     lComparisonResult;

        lStringValue = reinterpret_cast<CFStringRef>(
            CFDictionaryGetValue(inDictionary, inExpectedFirstKey[i]));
        CPPUNIT_ASSERT(lStringValue != NULL);

        lComparisonResult =
            CFStringCompare(lStringValue,
                            reinterpret_cast<CFStringRef>(inExpectedFirstValue[i]),
                            0);
        CPPUNIT_ASSERT(lComparisonResult == kCFCompareEqualTo);
    }
}

void
TestCFUDictionaryDifference :: TestTeardown(CFMutableDictionaryRef inAdded,
                                            CFMutableDictionaryRef inCommon,
                                            CFMutableDictionaryRef inRemoved)
{
    CPPUNIT_ASSERT(inAdded != nullptr);
    CPPUNIT_ASSERT(inCommon != nullptr);
    CPPUNIT_ASSERT(inRemoved != nullptr);

    CFRelease(inAdded);
    CFRelease(inCommon);
    CFRelease(inRemoved);
}
