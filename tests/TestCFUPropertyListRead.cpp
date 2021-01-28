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
 *      This file implements a unit test for
 *      CFUPropertyListReadFromFile and CFUPropertyListReadFromURL.
 */

#include <CFUtilities/CFUtilities.hpp>

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>

static const char * const kValidPropertyListBuffer =
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
    "<plist version=\"1.0\">\n"
    "<dict>\n"
    "    <key>Boolean</key>\n"
    "    <true/>\n"
    "    <key>String</key>\n"
    "    <string>String</string>\n"
    "    <key>Integer</key>\n"
    "    <integer>42</integer>\n"
    "    <key>Real</key>\n"
    "    <real>3.14</real>\n"
    "</dict>\n"
    "</plist>";
static const char * const kInvalidPropertyListBuffer =
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
    "<plist version=\"1.0\">\n"
    "<dict>\n"
    "    <key>Key</key>\n"
    "    <value>Value</value>\n"
    "</dict>\n"
    "</plist>";

class TestCFUPropertyListRead :
    public CppUnit::TestFixture
{
public:
    ~TestCFUPropertyListRead(void) = default;

protected:
    TestCFUPropertyListRead(void) = default;

    void SetUp(void);
    void TearDown(void);

    void TestValid(CFPropertyListRef inPropertyList);

private:
    void WriteTemporary(char *       aPathBuffer,
                        const char * aPathPattern,
                        const char * aBuffer);

protected:
    char mValidPropertyListTemporaryPath[PATH_MAX];
    char mInvalidPropertyListTemporaryPath[PATH_MAX];
};

class TestCFUPropertyListReadFromFile :
    public TestCFUPropertyListRead
{
    CPPUNIT_TEST_SUITE(TestCFUPropertyListReadFromFile);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestValidNonNullCString);
    CPPUNIT_TEST(TestValidNonNullCFString);
    CPPUNIT_TEST(TestInvalidNonNullCString);
    CPPUNIT_TEST(TestInvalidNonNullCFString);
    CPPUNIT_TEST(TestNonexistentNonNullCString);
    CPPUNIT_TEST(TestNonexistentNonNullCFString);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestValidNonNullCString(void);
    void TestValidNonNullCFString(void);
    void TestInvalidNonNullCString(void);
    void TestInvalidNonNullCFString(void);
    void TestNonexistentNonNullCString(void);
    void TestNonexistentNonNullCFString(void);

    void setUp(void);
    void tearDown(void);
};

class TestCFUPropertyListReadFromURL :
    public TestCFUPropertyListRead
{
    CPPUNIT_TEST_SUITE(TestCFUPropertyListReadFromURL);
    CPPUNIT_TEST(TestNull);
    CPPUNIT_TEST(TestValidNonNull);
    CPPUNIT_TEST(TestInvalidNonNull);
    CPPUNIT_TEST(TestNonexistentNonNull);
    CPPUNIT_TEST_SUITE_END();

public:
    void TestNull(void);
    void TestNonNull(void);
    void TestValidNonNull(void);
    void TestInvalidNonNull(void);
    void TestNonexistentNonNull(void);

    void setUp(void);
    void tearDown(void);
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUPropertyListReadFromFile);
CPPUNIT_TEST_SUITE_REGISTRATION(TestCFUPropertyListReadFromURL);

void
TestCFUPropertyListRead :: SetUp(void)
{
    WriteTemporary(mValidPropertyListTemporaryPath,
                   "/tmp/cfu-valid-plistXXXXXX",
                   kValidPropertyListBuffer);

    WriteTemporary(mInvalidPropertyListTemporaryPath,
                   "/tmp/cfu-invalid-plistXXXXXX",
                   kInvalidPropertyListBuffer);
}

void
TestCFUPropertyListRead :: TearDown(void)
{
    int lStatus;

    lStatus = unlink(mValidPropertyListTemporaryPath);
    CPPUNIT_ASSERT(lStatus == 0);

    lStatus = unlink(mInvalidPropertyListTemporaryPath);
    CPPUNIT_ASSERT(lStatus == 0);
}

void
TestCFUPropertyListRead :: WriteTemporary(char *       aPathBuffer,
                                          const char * aPathPattern,
                                          const char * aBuffer)
{
    const size_t lLength     = strlen(aBuffer);
    int          lDescriptor = -1;
    ssize_t      lStatus;

    CPPUNIT_ASSERT(aPathBuffer != NULL);
    CPPUNIT_ASSERT(aPathPattern != NULL);
    CPPUNIT_ASSERT(aBuffer != NULL);

    CPPUNIT_ASSERT(lLength > 0);

    aPathBuffer[0] = '\0';
    strcat(aPathBuffer, aPathPattern);

    lStatus = mkstemp(aPathBuffer);
    CPPUNIT_ASSERT(lStatus > 0);

    lDescriptor = static_cast<int>(lStatus);

    lStatus = write(lDescriptor, aBuffer, lLength);
    CPPUNIT_ASSERT(lStatus > 0);
    CPPUNIT_ASSERT(static_cast<size_t>(lStatus) == lLength);

    close(lDescriptor);
}

void
TestCFUPropertyListRead :: TestValid(CFPropertyListRef inPropertyList)
{
    const CFPropertyListFormat kFormat     = kCFPropertyListXMLFormat_v1_0;
    CFDictionaryRef            lDictionary = NULL;
    bool                       lStatus;

    lStatus = CFPropertyListIsValid(inPropertyList, kFormat);
    CPPUNIT_ASSERT(lStatus == true);

    lDictionary = static_cast<CFDictionaryRef>(inPropertyList);

    lStatus = CFDictionaryContainsKey(lDictionary, CFSTR("Boolean"));
    CPPUNIT_ASSERT(lStatus == true);

    lStatus = CFDictionaryContainsKey(lDictionary, CFSTR("Integer"));
    CPPUNIT_ASSERT(lStatus == true);

    lStatus = CFDictionaryContainsKey(lDictionary, CFSTR("Real"));
    CPPUNIT_ASSERT(lStatus == true);

    lStatus = CFDictionaryContainsKey(lDictionary, CFSTR("String"));
    CPPUNIT_ASSERT(lStatus == true);

    if (inPropertyList != NULL) {
        CFRelease(inPropertyList);
    }
}

void
TestCFUPropertyListReadFromFile :: setUp(void)
{
    TestCFUPropertyListRead::SetUp();
}

void
TestCFUPropertyListReadFromFile :: tearDown(void)
{
    TestCFUPropertyListRead::TearDown();
}

void
TestCFUPropertyListReadFromFile :: TestNull(void)
{
    const CFPropertyListMutabilityOptions kMutability = kCFPropertyListImmutable;
    CFPropertyListRef                     lPropertyList;
    bool                                  lStatus;

    // NULL-terminated C string path

    lStatus = CFUPropertyListReadFromFile(static_cast<const char *>(0),
                                          kMutability,
                                          &lPropertyList,
                                          NULL);
    CPPUNIT_ASSERT(lStatus == false);

    lStatus = CFUPropertyListReadFromFile("/tmp/test.plist",
                                          kMutability,
                                          NULL,
                                          NULL);
    CPPUNIT_ASSERT(lStatus == false);

    lStatus = CFUPropertyListReadFromFile(static_cast<const char *>(0),
                                          kMutability,
                                          NULL,
                                          NULL);
    CPPUNIT_ASSERT(lStatus == false);

    // CoreFoundation String path

    lStatus = CFUPropertyListReadFromFile(static_cast<CFStringRef>(0),
                                          kMutability,
                                          &lPropertyList,
                                          NULL);
    CPPUNIT_ASSERT(lStatus == false);

    lStatus = CFUPropertyListReadFromFile(CFSTR("/tmp/test.plist"),
                                          kMutability,
                                          NULL,
                                          NULL);
    CPPUNIT_ASSERT(lStatus == false);

    lStatus = CFUPropertyListReadFromFile(static_cast<CFStringRef>(0),
                                          kMutability,
                                          NULL,
                                          NULL);
    CPPUNIT_ASSERT(lStatus == false);
}

void
TestCFUPropertyListReadFromFile :: TestValidNonNullCString(void)
{
    const CFPropertyListMutabilityOptions kMutability   = kCFPropertyListImmutable;
    const char *                          lPath;
    CFPropertyListRef                     lPropertyList = NULL;
    CFStringRef                           lError        = NULL;
    bool                                  lStatus;

    lPath = mValidPropertyListTemporaryPath;

    lStatus = CFUPropertyListReadFromFile(lPath,
                                          kMutability,
                                          &lPropertyList,
                                          &lError);
    CPPUNIT_ASSERT(lStatus == true);
    CPPUNIT_ASSERT(lPropertyList != NULL);
    CPPUNIT_ASSERT(lError == NULL);

    TestValid(lPropertyList);
}

void
TestCFUPropertyListReadFromFile :: TestValidNonNullCFString(void)
{
    const CFPropertyListMutabilityOptions kMutability   = kCFPropertyListImmutable;
    CFStringRef                           lPath         = NULL;
    CFPropertyListRef                     lPropertyList = NULL;
    CFStringRef                           lError        = NULL;
    bool                                  lStatus;

    lPath = CFStringCreateWithCString(kCFAllocatorDefault,
                                      mValidPropertyListTemporaryPath,
                                      CFStringGetSystemEncoding());
    CPPUNIT_ASSERT(lPath != NULL);

    lStatus = CFUPropertyListReadFromFile(lPath,
                                          kMutability,
                                          &lPropertyList,
                                          &lError);
    CPPUNIT_ASSERT(lStatus == true);
    CPPUNIT_ASSERT(lPropertyList != NULL);
    CPPUNIT_ASSERT(lError == NULL);

    TestValid(lPropertyList);

    if (lPath != NULL) {
        CFRelease(lPath);
    }
}

void
TestCFUPropertyListReadFromFile :: TestInvalidNonNullCString(void)
{
    const CFPropertyListMutabilityOptions kMutability   = kCFPropertyListImmutable;
    const char *                          lPath;
    CFPropertyListRef                     lPropertyList = NULL;
    CFStringRef                           lError        = NULL;
    bool                                  lStatus;

    lPath = mInvalidPropertyListTemporaryPath;

    lStatus = CFUPropertyListReadFromFile(lPath,
                                          kMutability,
                                          &lPropertyList,
                                          &lError);
    CPPUNIT_ASSERT(lStatus == false);
    CPPUNIT_ASSERT(lPropertyList == NULL);
    CPPUNIT_ASSERT(lError != NULL);

    if (lError != NULL) {
        CFRelease(lError);
    }
}

void
TestCFUPropertyListReadFromFile :: TestInvalidNonNullCFString(void)
{
    const CFPropertyListMutabilityOptions kMutability   = kCFPropertyListImmutable;
    CFStringRef                           lPath         = NULL;
    CFPropertyListRef                     lPropertyList = NULL;
    CFStringRef                           lError        = NULL;
    bool                                  lStatus;

    lPath = CFStringCreateWithCString(kCFAllocatorDefault,
                                      mInvalidPropertyListTemporaryPath,
                                      CFStringGetSystemEncoding());
    CPPUNIT_ASSERT(lPath != NULL);

    lStatus = CFUPropertyListReadFromFile(lPath,
                                          kMutability,
                                          &lPropertyList,
                                          &lError);
    CPPUNIT_ASSERT(lStatus == false);
    CPPUNIT_ASSERT(lPropertyList == NULL);
    CPPUNIT_ASSERT(lError != NULL);

    if (lError != NULL) {
        CFRelease(lError);
    }

    if (lPath != NULL) {
        CFRelease(lPath);
    }
}

void
TestCFUPropertyListReadFromFile :: TestNonexistentNonNullCString(void)
{
    const CFPropertyListMutabilityOptions kMutability   = kCFPropertyListImmutable;
    char                                  lPath[PATH_MAX];
    CFPropertyListRef                     lPropertyList = NULL;
    CFStringRef                           lError        = NULL;
    int                                   lStatus;

    // Create, close, and immediately unlink a file such that we have
    // a randomly-named and likely non-existent file.

    lPath[0] = '\0';
    strcat(lPath, "/tmp/cfu-nonexistent-plistXXXXXX");

    lStatus = mkstemp(lPath);
    CPPUNIT_ASSERT(lStatus > 0);

    close(lStatus);

    lStatus = unlink(lPath);
    CPPUNIT_ASSERT(lStatus == 0);

    // Attempt to read from the non-existent file.

    lStatus = CFUPropertyListReadFromFile(lPath,
                                          kMutability,
                                          &lPropertyList,
                                          &lError);
    CPPUNIT_ASSERT(lStatus == false);
    CPPUNIT_ASSERT(lPropertyList == NULL);
    CPPUNIT_ASSERT(lError == NULL);
}

void
TestCFUPropertyListReadFromFile :: TestNonexistentNonNullCFString(void)
{
    const CFPropertyListMutabilityOptions kMutability     = kCFPropertyListImmutable;
    char                                  lCPath[PATH_MAX];
    CFStringRef                           lCFPath         = NULL;
    CFPropertyListRef                     lPropertyList   = NULL;
    CFStringRef                           lError          = NULL;
    bool                                  lStatus;

    // Create and immediately unlink a file such that we have a
    // randomly-named and likely non-existent file.

    lCPath[0] = '\0';
    strcat(lCPath, "/tmp/cfu-nonexistent-plistXXXXXX");

    lStatus = mkstemp(lCPath);
    CPPUNIT_ASSERT(lStatus > 0);

    lStatus = unlink(lCPath);
    CPPUNIT_ASSERT(lStatus == 0);

    lCFPath = CFStringCreateWithCString(kCFAllocatorDefault,
                                        lCPath,
                                        CFStringGetSystemEncoding());
    CPPUNIT_ASSERT(lCFPath != NULL);

    // Attempt to read from the non-existent file.

    lStatus = CFUPropertyListReadFromFile(lCFPath,
                                          kMutability,
                                          &lPropertyList,
                                          &lError);
    CPPUNIT_ASSERT(lStatus == false);
    CPPUNIT_ASSERT(lPropertyList == NULL);
    CPPUNIT_ASSERT(lError == NULL);

    if (lCFPath != NULL) {
        CFRelease(lCFPath);
    }
}

void
TestCFUPropertyListReadFromURL :: setUp(void)
{
    TestCFUPropertyListRead::SetUp();
}

void
TestCFUPropertyListReadFromURL :: tearDown(void)
{
    TestCFUPropertyListRead::TearDown();
}

void
TestCFUPropertyListReadFromURL :: TestNull(void)
{
    const CFPropertyListMutabilityOptions kMutability  = kCFPropertyListImmutable;
    bool                                  kIsDirectory = true;
    CFURLRef                              lURLRef      = NULL;
    CFPropertyListRef                     lPropertyList;
    bool                                  lStatus;

    lURLRef = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                            CFSTR("/tmp/test.plist"),
                                            kCFURLPOSIXPathStyle,
                                            !kIsDirectory);
    CPPUNIT_ASSERT(lURLRef != NULL);

    lStatus = CFUPropertyListReadFromURL(NULL,
                                         kMutability,
                                         &lPropertyList,
                                         NULL);
    CPPUNIT_ASSERT(lStatus == false);

    lStatus = CFUPropertyListReadFromURL(lURLRef, kMutability, NULL, NULL);
    CPPUNIT_ASSERT(lStatus == false);

    lStatus = CFUPropertyListReadFromURL(NULL, kMutability, NULL, NULL);
    CPPUNIT_ASSERT(lStatus == false);

    if (lURLRef != NULL) {
        CFRelease(lURLRef);
    }
}

void
TestCFUPropertyListReadFromURL :: TestValidNonNull(void)
{
    const CFPropertyListMutabilityOptions kMutability   = kCFPropertyListImmutable;
    bool                                  kIsDirectory  = true;
    CFStringRef                           lPath         = NULL;
    CFURLRef                              lURLRef       = NULL;
    CFPropertyListRef                     lPropertyList = NULL;
    CFStringRef                           lError        = NULL;
    bool                                  lStatus;

    lPath = CFStringCreateWithCString(kCFAllocatorDefault,
                                      mValidPropertyListTemporaryPath,
                                      CFStringGetSystemEncoding());
    CPPUNIT_ASSERT(lPath != NULL);

    lURLRef = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                            lPath,
                                            kCFURLPOSIXPathStyle,
                                            !kIsDirectory);
    CPPUNIT_ASSERT(lURLRef != NULL);

    lStatus = CFUPropertyListReadFromURL(lURLRef,
                                         kMutability,
                                         &lPropertyList,
                                         &lError);
    CPPUNIT_ASSERT(lStatus == true);
    CPPUNIT_ASSERT(lPropertyList != NULL);
    CPPUNIT_ASSERT(lError == NULL);

    TestValid(lPropertyList);

    if (lPath != NULL) {
        CFRelease(lPath);
    }

    if (lURLRef != NULL) {
        CFRelease(lURLRef);
    }
}

void
TestCFUPropertyListReadFromURL :: TestInvalidNonNull(void)
{
    const CFPropertyListMutabilityOptions kMutability   = kCFPropertyListImmutable;
    bool                                  kIsDirectory  = true;
    CFStringRef                           lPath         = NULL;
    CFURLRef                              lURLRef       = NULL;
    CFPropertyListRef                     lPropertyList = NULL;
    CFStringRef                           lError        = NULL;
    bool                                  lStatus;

    lPath = CFStringCreateWithCString(kCFAllocatorDefault,
                                      mInvalidPropertyListTemporaryPath,
                                      CFStringGetSystemEncoding());
    CPPUNIT_ASSERT(lPath != NULL);

    lURLRef = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                            lPath,
                                            kCFURLPOSIXPathStyle,
                                            !kIsDirectory);
    CPPUNIT_ASSERT(lURLRef != NULL);

    lStatus = CFUPropertyListReadFromURL(lURLRef,
                                         kMutability,
                                         &lPropertyList,
                                         &lError);
    CPPUNIT_ASSERT(lStatus == false);
    CPPUNIT_ASSERT(lPropertyList == NULL);
    CPPUNIT_ASSERT(lError != NULL);

    if (lError != NULL) {
        CFRelease(lError);
    }

    if (lPath != NULL) {
        CFRelease(lPath);
    }

    if (lURLRef != NULL) {
        CFRelease(lURLRef);
    }
}

void
TestCFUPropertyListReadFromURL :: TestNonexistentNonNull(void)
{
    const CFPropertyListMutabilityOptions kMutability     = kCFPropertyListImmutable;
    bool                                  kIsDirectory    = true;
    char                                  lCPath[PATH_MAX];
    CFStringRef                           lCFPath         = NULL;
    CFURLRef                              lURLRef         = NULL;
    CFPropertyListRef                     lPropertyList   = NULL;
    CFStringRef                           lError          = NULL;
    bool                                  lStatus;

    // Create and immediately unlink a file such that we have a
    // randomly-named and likely non-existent file.

    lCPath[0] = '\0';
    strcat(lCPath, "/tmp/cfu-nonexistent-plistXXXXXX");

    lStatus = mkstemp(lCPath);
    CPPUNIT_ASSERT(lStatus > 0);

    lStatus = unlink(lCPath);
    CPPUNIT_ASSERT(lStatus == 0);

    lCFPath = CFStringCreateWithCString(kCFAllocatorDefault,
                                        lCPath,
                                        CFStringGetSystemEncoding());
    CPPUNIT_ASSERT(lCFPath != NULL);

    lURLRef = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                            lCFPath,
                                            kCFURLPOSIXPathStyle,
                                            !kIsDirectory);
    CPPUNIT_ASSERT(lURLRef != NULL);

    // Attempt to read from the non-existent URL.

    lStatus = CFUPropertyListReadFromURL(lURLRef,
                                         kMutability,
                                         &lPropertyList,
                                         &lError);
    CPPUNIT_ASSERT(lStatus == false);
    CPPUNIT_ASSERT(lPropertyList == NULL);
    CPPUNIT_ASSERT(lError == NULL);

    if (lCFPath != NULL) {
        CFRelease(lCFPath);
    }

    if (lURLRef != NULL) {
        CFRelease(lURLRef);
    }
}
