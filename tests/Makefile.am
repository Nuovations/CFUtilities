#
#    Copyright (c) 2021-2023 Nuovation System Designs, LLC. All Rights Reserved.
#    Copyright 2016 Nest Labs Inc. All Rights Reserved.
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

#
#    Description:
#      This file is the GNU automake template for the Nuovations
#      CoreFoundation Utilities Library/Framework unit tests.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

#
# Local headers to build against and distribute but not to install
# since they are not part of the package.
#
noinst_HEADERS                                 = \
    $(NULL)

#
# Other files we do want to distribute with the package.
#
EXTRA_DIST                                     = \
    $(NULL)

if CFUTILITIES_BUILD_TESTS
# C preprocessor option flags that will apply to all compiled objects in this
# makefile.

AM_CPPFLAGS                                    = \
    -I$(top_srcdir)/include                      \
    $(CPPUNIT_CPPFLAGS)                          \
    $(NULL)

COMMON_LDADD                                   = \
    $(top_builddir)/src/libCFUtilities.la        \
    $(CPPUNIT_LDFLAGS) $(CPPUNIT_LIBS)           \
    $(NULL)

# Test applications that should be run when the 'check' target is run.

check_PROGRAMS                          = \
    TestCFMutableString                   \
    TestCFString                          \
    TestCFUAbsoluteTimeGetPOSIXTime       \
    TestCFUBooleanCreate                  \
    TestCFUDateCreate                     \
    TestCFUDateGetPOSIXTime               \
    TestCFUDictionaryCopyKeys             \
    TestCFUDictionaryDifference           \
    TestCFUDictionaryMerge                \
    TestCFUDictionaryGetBoolean           \
    TestCFUDictionarySetBoolean           \
    TestCFUDictionarySetCString           \
    TestCFUGetNumberType                  \
    TestCFUIsTypeID                       \
    TestCFUPOSIXTimeGetAbsoluteTime       \
    TestCFUPropertyListRead               \
    TestCFUPropertyListWrite              \
    TestCFUReferenceSet                   \
    TestCFURelease                        \
    TestCFUSetIsEmptySet                  \
    TestCFUSetIntersectionSet             \
    TestCFUSetUnionSet                    \
    TestCFUStringsMatch                   \
    TestCFUTreeContextInit                \
    TestCFUTreeCreate                     \
    $(NULL)

# Test applications and scripts that should be built and run when the
# 'check' target is run.

TESTS                                   = \
    $(check_PROGRAMS)                     \
    $(NULL)

# The additional environment variables and their values that will be
# made available to all programs and scripts in TESTS.

TESTS_ENVIRONMENT                       = \
    $(NULL)

# Source, compiler, and linker options for test programs.

TestCFMutableString_LDADD               = $(COMMON_LDADD)
TestCFMutableString_SOURCES             = TestDriver.cpp                      \
                                          TestCFMutableString.cpp

TestCFString_LDADD                      = $(COMMON_LDADD)
TestCFString_SOURCES                    = TestDriver.cpp                      \
                                          TestCFString.cpp

TestCFUAbsoluteTimeGetPOSIXTime_LDADD   = $(COMMON_LDADD)
TestCFUAbsoluteTimeGetPOSIXTime_SOURCES = TestDriver.cpp                      \
                                          TestCFUAbsoluteTimeGetPOSIXTime.cpp

TestCFUBooleanCreate_LDADD              = $(COMMON_LDADD)
TestCFUBooleanCreate_SOURCES            = TestDriver.cpp                      \
                                          TestCFUBooleanCreate.cpp

TestCFUDateCreate_LDADD                 = $(COMMON_LDADD)
TestCFUDateCreate_SOURCES               = TestDriver.cpp                      \
                                          TestCFUDateCreate.cpp

TestCFUDictionaryCopyKeys_LDADD         = $(COMMON_LDADD)
TestCFUDictionaryCopyKeys_SOURCES       = TestDriver.cpp                      \
                                          TestCFUDictionaryCopyKeys.cpp

TestCFUDictionaryDifference_LDADD       = $(COMMON_LDADD)
TestCFUDictionaryDifference_SOURCES     = TestDriver.cpp                      \
                                          TestCFUDictionaryDifference.cpp

TestCFUDictionaryMerge_LDADD            = $(COMMON_LDADD)
TestCFUDictionaryMerge_SOURCES          = TestDriver.cpp                      \
                                          TestCFUDictionaryMerge.cpp

TestCFUDictionaryGetBoolean_LDADD       = $(COMMON_LDADD)
TestCFUDictionaryGetBoolean_SOURCES     = TestDriver.cpp                      \
                                          TestCFUDictionaryGetBoolean.cpp

TestCFUDictionarySetBoolean_LDADD       = $(COMMON_LDADD)
TestCFUDictionarySetBoolean_SOURCES     = TestDriver.cpp                      \
                                          TestCFUDictionarySetBoolean.cpp

TestCFUDictionarySetCString_LDADD       = $(COMMON_LDADD)
TestCFUDictionarySetCString_SOURCES     = TestDriver.cpp                      \
                                          TestCFUDictionarySetCString.cpp

TestCFUDateGetPOSIXTime_LDADD           = $(COMMON_LDADD)
TestCFUDateGetPOSIXTime_SOURCES         = TestDriver.cpp                      \
                                          TestCFUDateGetPOSIXTime.cpp

TestCFUGetNumberType_LDADD              = $(COMMON_LDADD)
TestCFUGetNumberType_SOURCES            = TestDriver.cpp                      \
                                          TestCFUGetNumberType.cpp

TestCFUIsTypeID_LDADD                   = $(COMMON_LDADD)
TestCFUIsTypeID_SOURCES                 = TestDriver.cpp                      \
                                          TestCFUIsTypeID.cpp

TestCFUPOSIXTimeGetAbsoluteTime_LDADD   = $(COMMON_LDADD)
TestCFUPOSIXTimeGetAbsoluteTime_SOURCES = TestDriver.cpp                      \
                                          TestCFUPOSIXTimeGetAbsoluteTime.cpp

TestCFUPropertyListRead_LDADD           = $(COMMON_LDADD)
TestCFUPropertyListRead_SOURCES         = TestDriver.cpp                      \
                                          TestCFUPropertyListRead.cpp

TestCFUPropertyListWrite_LDADD          = $(COMMON_LDADD)
TestCFUPropertyListWrite_SOURCES        = TestDriver.cpp                      \
                                          TestCFUPropertyListWrite.cpp

TestCFUReferenceSet_LDADD               = $(COMMON_LDADD)
TestCFUReferenceSet_SOURCES             = TestDriver.cpp                      \
                                          TestCFUReferenceSet.cpp

TestCFURelease_LDADD                    = $(COMMON_LDADD)
TestCFURelease_SOURCES                  = TestDriver.cpp                      \
                                          TestCFURelease.cpp

TestCFUSetIntersectionSet_LDADD         = $(COMMON_LDADD)
TestCFUSetIntersectionSet_SOURCES       = TestDriver.cpp                      \
                                          TestCFUSetIntersectionSet.cpp

TestCFUSetIsEmptySet_LDADD              = $(COMMON_LDADD)
TestCFUSetIsEmptySet_SOURCES            = TestDriver.cpp                      \
                                          TestCFUSetIsEmptySet.cpp

TestCFUSetUnionSet_LDADD                = $(COMMON_LDADD)
TestCFUSetUnionSet_SOURCES              = TestDriver.cpp                      \
                                          TestCFUSetUnionSet.cpp

TestCFUStringsMatch_LDADD               = $(COMMON_LDADD)
TestCFUStringsMatch_SOURCES             = TestDriver.cpp                      \
                                          TestCFUStringsMatch.cpp

TestCFUTreeContextInit_LDADD            = $(COMMON_LDADD)
TestCFUTreeContextInit_SOURCES          = TestDriver.cpp                      \
                                          TestCFUTreeContextInit.cpp

TestCFUTreeCreate_LDADD                 = $(COMMON_LDADD)
TestCFUTreeCreate_SOURCES               = TestDriver.cpp                      \
                                          TestCFUTreeCreate.cpp

if CFUTILITIES_BUILD_COVERAGE
CLEANFILES                              = $(wildcard *.gcda *.gcno *.info)

if CFUTILITIES_BUILD_COVERAGE_REPORTS
# The bundle should positively be qualified with the absolute build
# path. Otherwise, VPATH will get auto-prefixed to it if there is
# already such a directory in the non-colocated source tree.

CFUTILITIES_COVERAGE_BUNDLE             = ${abs_builddir}/${PACKAGE}${NL_COVERAGE_BUNDLE_SUFFIX}
CFUTILITIES_COVERAGE_INFO               = ${CFUTILITIES_COVERAGE_BUNDLE}/${PACKAGE}${NL_COVERAGE_INFO_SUFFIX}

$(CFUTILITIES_COVERAGE_BUNDLE):
	$(call create-directory)

# Generate the coverage report, filtering out platform and system
# directories and this test directory.

INCLUDE_PATHS    := $(subst -I,,$(sort $(BOOST_CPPFLAGS) $(CPPUNIT_CPPFLAGS)))
INCLUDE_PATTERNS := $(if $(INCLUDE_PATHS),$(addsuffix *,$(INCLUDE_PATHS)),)
XCODE_PATH       := $(shell xcode-select -p)
XCODE_PATTERN    := $(if $(XCODE_PATH),$(addsuffix *,$(XCODE_PATH)),)

$(CFUTILITIES_COVERAGE_INFO): check-local | $(CFUTILITIES_COVERAGE_BUNDLE)
	$(call generate-coverage-report-with-filter,${top_builddir},${INCLUDE_PATTERNS} ${XCODE_PATTERN} *${subdir}*)

coverage-local: $(CFUTILITIES_COVERAGE_INFO)

clean-local: clean-local-coverage

.PHONY: clean-local-coverage
clean-local-coverage:
	-$(AM_V_at)rm -rf $(CFUTILITIES_COVERAGE_BUNDLE)
endif # CFUTILITIES_BUILD_COVERAGE_REPORTS
endif # CFUTILITIES_BUILD_COVERAGE
endif # CFUTILITIES_BUILD_TESTS

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
