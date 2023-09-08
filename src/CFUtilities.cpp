/*
 *    Copyright (c) 2008-2021 Nuovation System Designs, LLC
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
 *      This file implements C and C++ utility interfaces for
 *      interacting with Apple's CoreFoundation framework.
 */

#include <vector>

#include <sys/stat.h>

#include <AssertMacros.h>

#include <CoreFoundation/CoreFoundation.h>

#include <CFUtilities/CFUtilities.h>
#include <CFUtilities/CFUtilities.hpp>

#if defined(HAVE_CONFIG_H)
#include "CFUtilities/CFUConfig.h"
#endif


using namespace std;


/* Type Definitions */

/*
 * NOTE: Regrettably, clang-format is disabled below since
 *       clang-format, as of version 11, cannot both align consecutive
 *       member declarations and comments.
 */

/**
 * Iterator context for the dictionary merge interface.
 *
 * @private
 */
struct CFUDictionaryMergeContext {
    // clang-format off
    CFMutableDictionaryRef mDestinationDictionary;  //!< A reference to the
                                                    //!< mutable dictionary to
                                                    //!< merge keys and values
                                                    //!< to.
    const bool             mReplace;                //!< A Boolean indicating
                                                    //!< whether or not
                                                    //!< duplicate keys should
                                                    //!< be replaced.
    // clang-format on
};

/**
 * Iterator context used for the set intersection and union interfaces.
 *
 * @private
 */
struct CFUSetContext {
    // clang-format off
    CFMutableSetRef        mDestinationSet;  //!< The mutable set that serves
                                             //!< as the destination of the set
                                             //!< intersection or union
                                             //!< operation.
    CFSetRef               mSourceSet;       //!< The immutable set that serves
                                             //!< as the source of the set
                                             //!< intersection or union
                                             //!< operation.
    // clang-format on
};

/* Global Variables */

static const CFTreeContext kCFUTreeContextInitializer = { 0, 0, 0, 0, 0 };


/**
 *  This routine checks the type of the specified CoreFoundation
 *  reference against the specified type.
 *
 *  @param[in]  inReference  A CoreFoundation reference to type check.
 *  @param[in]  inID         The CoreFoundation type identifier to check
 *                           the reference type against.
 *
 *  @returns
 *    True if the specified reference matches the provided type;
 *    otherwise, false.
 *
 *  @ingroup base
 *
 */
bool
CFUIsTypeID(CFTypeRef inReference, CFTypeID inID)
{
    bool status = false;

    __Require_Quiet(inReference != nullptr, done);

    status = (CFGetTypeID(inReference) == inID);

done:
    return (status);
}

/**
 *  @brief
 *    Releases a reference to a Core Foundation object.
 *
 *  This routine attempts to release the specified CoreFoundation
 *  object.
 *
 *  @note
 *    In contrast to the @a CFRelease interface, a null reference results
 *    in no action being taken.
 *
 *  @param[in,out]  inReference  A reference to the CoreFoundation
 *                               object to release. On success, if
 *                               @a inReference is non-null, the
 *                               reference is released.
 *
 *  @ingroup base
 *
 */
void
CFURelease(CFTypeRef inReference)
{
    if (inReference != nullptr) {
        CFRelease(inReference);
    }
}

/**
 *  This routine returns a reference to a CoreFoundation Boolean
 *  object equivalent to the specified Boolean value.
 *
 *  @param[in] inBoolean  A Boolean value for which the corresponding
 *                        CoreFoundation Boolean object reference is
 *                        to be returned.
 *
 *  @returns
 *    kCFBooleanTrue if true; otherwise, kCFBooleanFalse.
 *
 *  @ingroup boolean
 *
 */
CFBooleanRef
CFUBooleanCreate(bool inBoolean)
{
    return (inBoolean ? kCFBooleanTrue : kCFBooleanFalse);
}

/**
 *  This routine converts the specified CoreFoundation absolute time
 *  into an equivalent POSIX time.
 *
 *  @param[in]  inAt  The CoreFoundation absolute time to convert.
 *
 *  @returns
 *    The POSIX time.
 *
 *  @sa CFUPOSIXTimeGetAbsoluteTime
 *
 *  @ingroup date-time
 *
 */
time_t
CFUAbsoluteTimeGetPOSIXTime(CFAbsoluteTime inAt)
{
    return (static_cast<time_t>(inAt + kCFAbsoluteTimeIntervalSince1970));
}

/**
 *  This routine converts the specified POSIX time into an
 *  equivalent CoreFoundation absolute time.
 *
 *  @param[in]  inTime  The POSIX time to convert.
 *
 *  @returns
 *    The CoreFoundation absolute time.
 *
 *  @sa CFUAbsoluteTimeGetPOSIXTime
 *
 *  @ingroup date-time
 *
 */
CFAbsoluteTime
CFUPOSIXTimeGetAbsoluteTime(time_t inTime)
{
    return (static_cast<CFAbsoluteTime>(inTime) - kCFAbsoluteTimeIntervalSince1970);
}

/**
 *  This routine converts the specified CoreFoundation date
 *  reference into an equivalent POSIX time.
 *
 *  @param[in]  inDate  The CoreFoundation date reference to convert.
 *
 *  @returns
 *    The POSIX time.
 *
 *  @ingroup date-time
 *
 */
time_t
CFUDateGetPOSIXTime(CFDateRef inDate)
{
    time_t tempTime = 0;

    __Require(inDate != nullptr, done);

    tempTime = CFUAbsoluteTimeGetPOSIXTime(CFDateGetAbsoluteTime(inDate));

done:
    return (tempTime);
}

/**
 *  This routine creates a CoreFoundation date object from the
 *  specified POSIX time.
 *
 *  @param[in]  inAllocator  The allocator with which to create the
 *                           CoreFoundation date object.
 *  @param[in]  inTime       The POSIX time to convert.
 *
 *  @returns
 *    A reference to a CoreFoundation date object if OK; otherwise, null.
 *
 *  @ingroup date-time
 *
 */
CFDateRef
CFUDateCreate(CFAllocatorRef inAllocator, time_t inTime)
{
    return (CFDateCreate(inAllocator, CFUPOSIXTimeGetAbsoluteTime(inTime)));
}

/**
 *  This routine returns a new array containing all the keys in the
 *  specified dictionary. The caller owns the returned array.
 *
 *  @param[in]  inDictionary  A reference to the dictionary to get the
 *                            keys from.
 *
 *  @returns
 *    A reference to an array containing the keys on success;
 *    otherwise, null on error.
 *
 *  @ingroup dictionary
 *
 */
CFArrayRef
CFUDictionaryCopyKeys(CFDictionaryRef inDictionary)
{
    CFIndex              numKeys;
    vector<const void *> theKeys;
    CFArrayRef           arrayRef = nullptr;

    __Require(inDictionary != nullptr, done);

    numKeys = CFDictionaryGetCount(inDictionary);

    theKeys.reserve(static_cast<size_t>(numKeys));

    CFDictionaryGetKeysAndValues(inDictionary, &theKeys[0], nullptr);

    arrayRef = CFArrayCreate(kCFAllocatorDefault,
                             &theKeys[0],
                             numKeys,
                             &kCFTypeArrayCallBacks);

done:
    return (arrayRef);
}

/**
 *  This routine is a CoreFoundation dictionary applier function
 *  that iterates on each entry in a source dictionary and performs
 *  the actual merge of those entries with a destination dictionary.
 *
 *  @param[in]      inKey      A pointer to the key of the current
 *                             key/value pair being iterated upon.
 *  @param[in]      inValue    A pointer to the value of the current
 *                             key/value pair being iterated upon.
 *  @param[in,out]  inContext  A pointer to the iterator context. On
 *                             completion, the context's dictionary
 *                             member will have been updated with the
 *                             results of the current key/value pair
 *                             iteration merge.
 *
 */
static void
CFUDictionaryMergeApplier(const void * inKey,
                          const void * inValue,
                          void *       inContext)
{
    CFUDictionaryMergeContext * theContext = nullptr;
    bool                        hasKey;

    __Require(inKey != nullptr, done);
    __Require(inContext != nullptr, done);

    theContext = static_cast<CFUDictionaryMergeContext *>(inContext);

    hasKey = CFDictionaryContainsKey(theContext->mDestinationDictionary, inKey);

    /*
     * If the key is absent or if the key is present and replacement
     * has been requested, add or replace the key/value pair as
     * appropriate.
     */

    if (!hasKey) {
        CFDictionaryAddValue(theContext->mDestinationDictionary,
                             inKey,
                             inValue);

    } else if (hasKey && theContext->mReplace) {
        CFDictionaryReplaceValue(theContext->mDestinationDictionary,
                                 inKey,
                                 inValue);
    }

done:
    return;
}

/**
 *  @brief
 *    Merge two CoreFoundation dictionaries.
 *
 *  This routine adds the key and value pairs from the specified
 *  source dictionary to the destination dictionary, replacing values
 *  with matching keys if requested.
 *
 *  @param[in,out]  inDestination  A reference to the mutable dictionary
 *                                 to merge keys and values to. On
 *                                 success, @a inDestination contains
 *                                 a reference to the destination
 *                                 dictionary with entries from the
 *                                 source dictionary merged in.
 *  @param[in]      inSource       A reference to the dictionary to merge
 *                                 keys and values from.
 *  @param[in]      inReplace      A Boolean flag indicating whether
 *                                 values from the source dictionary
 *                                 will replace values in the
 *                                 destination when the keys for those
 *                                 values already exist in the
 *                                 destination.
 *
 *  @ingroup dictionary
 *
 */
void
CFUDictionaryMerge(CFMutableDictionaryRef inDestination,
                   CFDictionaryRef        inSource,
                   bool                   inReplace)
{
    CFUDictionaryMergeContext theContext = { inDestination, inReplace };

    __Require(inDestination != nullptr, done);
    __Require(inSource != nullptr, done);

    CFDictionaryApplyFunction(inSource, CFUDictionaryMergeApplier, &theContext);

done:
    return;
}

/**
 *  @brief
 *    Get a Boolean value from a CoreFoundation dictionary.
 *
 *  This routine is a type-safe wrapper that allows using intrinsic
 *  C/C++ standard @a bool value types rather than
 *  CoreFoundation-defined @a Boolean value types.
 *
 *  @param[in]      inDictionary  A reference to the CoreFoundation
 *                                dictionary to get the Boolean from.
 *  @param[in]      inKey         A pointer to the key for which to
 *                                find a Boolean in the dictionary.
 *  @param[in,out]  outValue      A reference to storage for the
 *                                converted Boolean.
 *
 *  @returns
 *    True if the requested Boolean was successfully retrieved;
 *    otherwise, false.
 *
 *  @sa CFUDictionarySetBoolean
 *  @sa CFUDictionarySetNumber
 *
 *  @ingroup dictionary
 *
 */
Boolean
CFUDictionaryGetBoolean(CFDictionaryRef inDictionary,
                        const void *    inKey,
                        bool &          outValue)
{
    Boolean status, tempBool;

    status = CFUDictionaryGetBoolean(inDictionary, inKey, tempBool);
    __Require_Quiet(status, done);

    outValue = tempBool;

done:
    return (status);
}

/**
 *  @brief
 *    Get a Boolean value from a CoreFoundation dictionary.
 *
 *  This routine attempts to retrieve and convert the CoreFoundation
 *  Boolean associated with the specified key from the provided
 *  dictionary and convert it to the specified Boolean value.
 *
 *  @param[in]      inDictionary  A reference to the CoreFoundation
 *                                dictionary to get the Boolean from.
 *  @param[in]      inKey         A pointer to the key for which to
 *                                find a Boolean in the dictionary.
 *  @param[in,out]  outValue      A reference to storage for the
 *                                converted Boolean.
 *
 *  @returns
 *    True if the requested Boolean was successfully retrieved;
 *    otherwise, false.
 *
 *  @sa CFUDictionarySetBoolean
 *  @sa CFUDictionarySetNumber
 *
 *  @ingroup dictionary
 *
 */
Boolean
CFUDictionaryGetBoolean(CFDictionaryRef inDictionary,
                        const void *    inKey,
                        Boolean &       outValue)
{
    bool         status      = false;
    CFBooleanRef tempBoolean = nullptr;

    // Sanity check the input parameters.

    __Require(inDictionary != nullptr, done);
    __Require(inKey != nullptr, done);

    // Attempt to get the value from the dictionary.

    tempBoolean = static_cast<CFBooleanRef>(
        CFDictionaryGetValue(inDictionary, inKey));

    // The key being absent is a quiet failure; whereas, the value not
    // being of type Boolean is.

    __Require_Quiet(tempBoolean != nullptr, done);
    __Require(CFUIsTypeID(tempBoolean, CFBooleanGetTypeID()), done);

    outValue = CFBooleanGetValue(tempBoolean);

    status = true;

done:
    return (status);
}

/**
 *  @brief
 *    Set a Boolean value in a CoreFoundation dictionary.
 *
 *  This routine sets the specified Boolean value in the provided
 *  dictionary associated with the specified key.
 *
 *  @param[in,out]  inDictionary  A reference to the CoreFoundation
 *                                dictionary to set the Boolean in. On
 *                                success, the dictionary is modified
 *                                with a reference to the Boolean as a
 *                                CoreFoundation Boolean value.
 *  @param[in]      inKey         A pointer to the key with which to
 *                                associate the Boolean in the
 *                                dictionary.
 *  @param[in]      inValue       The Boolean value to set.
 *
 *  @returns
 *    True if the value was successfully set; otherwise, false.
 *
 *  @sa CFUDictionaryGetBoolean
 *
 *  @ingroup dictionary
 *
 */
Boolean
CFUDictionarySetBoolean(CFMutableDictionaryRef inDictionary,
                        const void *           inKey,
                        Boolean                inValue)
{
    bool status = false;

    __Require(inDictionary != nullptr, done);
    __Require(inKey != nullptr, done);

    CFDictionarySetValue(inDictionary,
                         inKey,
                         (inValue ? kCFBooleanTrue : kCFBooleanFalse));

    status = true;

done:
    return (status);
}

/**
 *  @brief
 *    Add a null-termited C string to a dictionary.

 *  This routine sets the specified null-terminated C string value, as
 *  a CFString, in the provided dictionary associated with the
 *  specified key.
 *
 *  @param[in,out]  inDictionary  A reference to the CoreFoundation
 *                                dictionary to set the
 *                                null-terminated C string in. On
 *                                success, the dictionary is modified
 *                                with a reference to the CFString
 *                                value equivalent to the
 *                                null-terminated C string set.
 *  @param[in]      inKey         A pointer to the key with which to
 *                                associate the null-terminated C
 *                                string in the dictionary.
 *  @param[in]      inString      The null-terminated C string value to set.
 *
 *  @returns
 *    True if the value was successfully set; otherwise, false.
 *
 *  @ingroup dictionary
 *
 */
Boolean
CFUDictionarySetCString(CFMutableDictionaryRef inDictionary,
                        const void *           inKey,
                        const char *           inString)
{
    bool        status     = false;
    CFStringRef tempString = nullptr;

    __Require(inDictionary != nullptr, done);
    __Require(inKey != nullptr, done);
    __Require(inString != nullptr, done);

    tempString = CFStringCreateWithCString(kCFAllocatorDefault,
                                           inString,
                                           CFStringGetSystemEncoding());
    __Require(tempString != nullptr, done);

    CFDictionarySetValue(inDictionary, inKey, tempString);

    status = true;

done:
    CFURelease(tempString);

    return (status);
}

/**
 *  This routines returns the appropriate CoreFoundation number type
 *  for the specified parameters.
 *
 *  @note
 *    Clients are discouraged from calling this interface directly and
 *    should, instead, use the template function version.
 *
 *  @param[in]  inIntegral  A Boolean indicating whether the type is
 *                          integral (true) or floating point
 *                          (false).
 *  @param[in]  inBytes     The size, in bytes, of the number representation.
 *
 *  @returns
 *    The corresponding CoreFoundation number type if a successful
 *    mapping was found; otherwise, false.
 *
 *  @ingroup number
 *
 */
CFNumberType
CFUGetNumberType(bool inIntegral, size_t inBytes)
{
    CFNumberType theType = static_cast<CFNumberType>(false);

    if (inIntegral) {
        if (inBytes == 1) {
            theType = kCFNumberSInt8Type;

        } else if (inBytes == 2) {
            theType = kCFNumberSInt16Type;

        } else if (inBytes == 4) {
            theType = kCFNumberSInt32Type;

        } else if (inBytes == 8) {
            theType = kCFNumberSInt64Type;
        }

    } else {
        if (inBytes == 4) {
            theType = kCFNumberFloat32Type;

        } else if (inBytes == 8) {
            theType = kCFNumberFloat64Type;
        }
    }

    return (theType);
}

/**
 *  This routine attempts to create a property list from the XML or
 *  binary property list data at the specified URL.
 *
 *  @param[in]      inURL         A CoreFoundation URL reference to the
 *                                URL to read the property list data
 *                                from.
 *  @param[in]      inMutability  Specifies the degree of mutability for
 *                                the returned property list.
 *  @param[in,out]  outPlist      A pointer to storage for the returned
 *                                property list object. On success,
 *                                this is a pointer to the property
 *                                list. The caller owns the reference
 *                                and is responsible for releasing the
 *                                object.
 *  @param[in,out]  outError      An optional pointer to storage for a
 *                                returned string indicating the
 *                                nature of the parsing error. On
 *                                failure, this is a reference to the
 *                                parsing error. The caller owns the
 *                                reference and is responsible for
 *                                releasing the object.
 *
 *  @returns
 *    True if OK; otherwise, false on error.
 *
 *  @ingroup plist
 *
 */
Boolean
CFUPropertyListReadFromURL(CFURLRef            inURL,
                           CFOptionFlags       inMutability,
                           CFPropertyListRef * outPlist,
                           CFStringRef *       outError)
{
    bool                 status    = false;
    CFReadStreamRef      theStream = nullptr;
    CFStreamStatus       streamStatus;
    CFPropertyListFormat theFormat;

    __Require(inURL != nullptr, done);
    __Require(outPlist != nullptr, done);

    // Attempt to create a CoreFoundation read file stream from the
    // resulting URL.

    theStream = CFReadStreamCreateWithFile(kCFAllocatorDefault, inURL);
    __Require(theStream != nullptr, done);

    // Attempt to open the stream.

    status = CFReadStreamOpen(theStream);
    __Require(status, done);

    // Check the stream status.

    streamStatus = CFReadStreamGetStatus(theStream);
    __Require_Action(streamStatus == kCFStreamStatusOpen, done, status = false);

    // Check that there are bytes available for reading.

    status = CFReadStreamHasBytesAvailable(theStream);
    __Require(status, done);

#if HAVE_CFPROPERTYLISTCREATEWITHSTREAM
    {
        CFErrorRef theError = nullptr;

        *outPlist = CFPropertyListCreateWithStream(kCFAllocatorDefault,
                                                   theStream,
                                                   0,
                                                   inMutability,
                                                   &theFormat,
                                                   &theError);

        if (theError != nullptr) {
            if (outError != nullptr) {
                *outError = CFErrorCopyDescription(theError);
            }

            CFRelease(theError);
        }

        __Require_Action(*outPlist != nullptr, done, status = false);
    }
#elif HAVE_CFPROPERTYLISTCREATEFROMSTREAM
    *outPlist = CFPropertyListCreateFromStream(kCFAllocatorDefault,
                                               theStream,
                                               0,
                                               inMutability,
                                               &theFormat,
                                               outError);
    __Require_Action(*outPlist != nullptr, done, status = false);
#else // !HAVE_CFPROPERTYLISTCREATEWITHSTREAM || !HAVE_CFPROPERTYLISTCREATEFROMSTREAM
#error "One of 'CFPropertyListCreateWithStream' or 'CFPropertyListCreateFromStream' must be available."
#endif // HAVE_CFPROPERTYLISTCREATEWITHSTREAM

    // At this point, all operations were successful. Set the return
    // status accordingly.

    status = true;

done:
    if (theStream != nullptr) {
        CFReadStreamClose(theStream);
    }

    CFURelease(theStream);

    return (status);
}

/**
 *  This routine attempts to write the property list data to a
 *  property list file in the specified format at the specified
 *  path.
 *
 *  @param[in]      inURL         A CoreFoundation URL reference to the
 *                                URL to write the property list data to.
 *  @param[in]      inFormat      Indicates the format of the property list
 *                                file.
 *  @param[in]      inPlist       The property list data to write.
 *  @param[in,out]  outError      An optional pointer to storage for a
 *                                returned string indicating the
 *                                nature of the parsing error. On
 *                                failure, this is a reference to the
 *                                parsing error. The caller owns the
 *                                reference and is responsible for
 *                                releasing the object.
 *
 *  @returns
 *    True if OK; otherwise, false on error.
 *
 *  @ingroup plist
 *
 */
Boolean
CFUPropertyListWriteToURL(CFURLRef             inURL,
                          CFPropertyListFormat inFormat,
                          CFPropertyListRef    inPlist,
                          CFStringRef *        outError)
{
    bool             status    = false;
    CFWriteStreamRef theStream = nullptr;
    CFStreamStatus   streamStatus;
    CFIndex          theIndex;

    __Require(inURL != nullptr, done);
    __Require(inPlist != nullptr, done);

    // Attempt to create a CoreFoundation write file stream from the
    // specified URL.

    theStream = CFWriteStreamCreateWithFile(kCFAllocatorDefault, inURL);
    __Require(theStream != nullptr, done);

    // Attempt to open the stream.

    status = CFWriteStreamOpen(theStream);
    __Require(status, done);

    // Check the stream status.

    streamStatus = CFWriteStreamGetStatus(theStream);
    __Require_Action(streamStatus == kCFStreamStatusOpen, done, status = false);

#if HAVE_CFPROPERTYLISTWRITE
    {
        CFErrorRef theError = nullptr;

        theIndex = CFPropertyListWrite(inPlist,
                                       theStream,
                                       inFormat,
                                       0,
                                       &theError);

        if (theError != nullptr) {
            if (outError != nullptr) {
                *outError = CFErrorCopyDescription(theError);
            }

            CFRelease(theError);
        }

        __Require_Action(theIndex != 0, done, status = false);
    }
#elif HAVE_CFPROPERTYLISTWRITETOSTREAM
    theIndex = CFPropertyListWriteToStream(inPlist,
                                           theStream,
                                           inFormat,
                                           outError);
    __Require_Action(theIndex != 0, done, status = false);
#else // !HAVE_CFPROPERTYLISTWRITE || !HAVE_CFPROPERTYLISTWRITETOSTREAM
#error "One of 'CFPropertyListWrite' or 'CFPropertyListWriteToStream' must be available."
#endif // HAVE_CFPROPERTYLISTWRITE

    // At this point, all operations were successful. Set the return
    // status accordingly.

    status = true;

done:
    if (theStream != nullptr) {
        CFWriteStreamClose(theStream);
    }

    CFURelease(theStream);

    return (status);
}

/**
 *  @brief
 *    Read a property list from a string representation of a file path.
 *
 *  This routine attempts to create a property list from the XML or
 *  binary property list data at the specified path.
 *
 *  @param[in]      inPath        A CoreFoundation string reference to
 *                                the path to read the property list
 *                                data from.
 *  @param[in]      inMutability  Specifies the degree of mutability for
 *                                the returned property list.
 *  @param[in,out]  outPlist      A pointer to storage for the returned
 *                                property list object. On success,
 *                                this is a pointer to the property
 *                                list. The caller owns the reference
 *                                and is responsible for releasing the
 *                                object.
 *  @param[in,out]  outError      An optional pointer to storage for a
 *                                returned string indicating the
 *                                nature of the parsing error. On
 *                                failure, this is a reference to the
 *                                parsing error. The caller owns the
 *                                reference and is responsible for
 *                                releasing the object.
 *
 *  @returns
 *    True if OK; otherwise, false on error.
 *
 *  @ingroup plist
 *
 */
Boolean
CFUPropertyListReadFromFile(CFStringRef         inPath,
                            CFOptionFlags       inMutability,
                            CFPropertyListRef * outPlist,
                            CFStringRef *       outError)
{
    bool     kIsDirectory = true;
    bool     status       = false;
    CFURLRef theURL       = nullptr;

    __Require(inPath != nullptr, done);
    __Require(outPlist != nullptr, done);

    // Attempt to create a CoreFoundation URL for the specified file
    // path.

    theURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                           inPath,
                                           kCFURLPOSIXPathStyle,
                                           !kIsDirectory);
    __Require(theURL != nullptr, done);

    // Attempt to read the property list from the URL.

    status = CFUPropertyListReadFromURL(theURL,
                                        inMutability,
                                        outPlist,
                                        outError);
    __Require(status, done);

done:
    CFURelease(theURL);

    return (status);
}

/**
 *  @brief
 *    Write a property list to a string representation of a file path.
 *
 *  This routine attempts to write the property list data to a
 *  property list file in the specified format at the specified
 *  path.
 *
 *  @param[in]      inPath        A CoreFoundation string reference to
 *                                the file to write the property list
 *                                data to.
 *  @param[in]      inFormat      Indicates the format of the property list
 *                                file.
 *  @param[in]      inPlist       The property list data to write.
 *  @param[in,out]  outError      An optional pointer to storage for a
 *                                returned string indicating the
 *                                nature of the parsing error. On
 *                                failure, this is a reference to the
 *                                parsing error. The caller owns the
 *                                reference and is responsible for
 *                                releasing the object.
 *
 *  @returns
 *    True if OK; otherwise, false on error.
 *
 *  @ingroup plist
 *
 */
Boolean
CFUPropertyListWriteToFile(CFStringRef          inPath,
                           CFPropertyListFormat inFormat,
                           CFPropertyListRef    inPlist,
                           CFStringRef *        outError)
{
    bool     kIsDirectory = true;
    bool     status       = false;
    CFURLRef theURL       = nullptr;

    __Require(inPath != nullptr, done);
    __Require(inPlist != nullptr, done);

    // Attempt to create a CoreFoundation URL for the specified file
    // path.

    theURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                           inPath,
                                           kCFURLPOSIXPathStyle,
                                           !kIsDirectory);
    __Require(theURL != nullptr, done);

    // Attempt to write the property list to the URL.

    status = CFUPropertyListWriteToURL(theURL, inFormat, inPlist, outError);
    __Require(status, done);

done:
    CFURelease(theURL);

    return (status);
}

/**
 *  @brief
 *    Read a property list from a string representation of a file path.
 *
 *  This routine attempts to create a property list from the XML or
 *  binary property list data at the specified path.
 *
 *  @param[in]      inPath        A pointer to a C string containing the
 *                                path to read the property list data
 *                                from.
 *  @param[in]      inMutability  Specifies the degree of mutability for
 *                                the returned property list.
 *  @param[in,out]  outPlist      A pointer to storage for the returned
 *                                property list object. On success,
 *                                this is a pointer to the property
 *                                list. The caller owns the reference
 *                                and is responsible for releasing the
 *                                object.
 *  @param[in,out]  outError      An optional pointer to storage for a
 *                                returned string indicating the
 *                                nature of the parsing error. On
 *                                failure, this is a reference to the
 *                                parsing error. The caller owns the
 *                                reference and is responsible for
 *                                releasing the object.
 *
 *  @returns
 *    True if OK; otherwise, false on error.
 *
 *  @ingroup plist
 *
 */
Boolean
CFUPropertyListReadFromFile(const char *        inPath,
                            CFOptionFlags       inMutability,
                            CFPropertyListRef * outPlist,
                            CFStringRef *       outError)
{
    bool        status  = false;
    CFStringRef thePath = nullptr;

    __Require(inPath != nullptr, done);
    __Require(outPlist != nullptr, done);

    // Convert the provided path name to a CoreFoundation string.

    thePath = CFStringCreateWithCString(kCFAllocatorDefault,
                                        inPath,
                                        CFStringGetSystemEncoding());
    __Require(thePath != nullptr, done);

    // Attempt to read the property list from the file at the
    // specified path.

    status = CFUPropertyListReadFromFile(thePath,
                                         inMutability,
                                         outPlist,
                                         outError);
    __Require(status, done);

done:
    CFURelease(thePath);

    return (status);
}

/**
 *  @brief
 *    Write a property list to a string representation of a file path.
 *
 *  This routine attempts to write the property list data to a
 *  property list file in the specified format at the specified path.
 *
 *  @param[in]      inPath        A pointer to a C string containing the
 *                                path to write the property list data
 *                                to.
 *  @param[in]      inWritable    Indicates whether the resulting file
 *                                should be writable.
 *  @param[in]      inFormat      Indicates the format of the property list
 *                                file.
 *  @param[in]      inPlist       The property list data to write.
 *  @param[in,out]  outError      An optional pointer to storage for a
 *                                returned string indicating the
 *                                nature of the parsing error. On
 *                                failure, this is a reference to the
 *                                parsing error. The caller owns the
 *                                reference and is responsible for
 *                                releasing the object.
 *
 *  @returns
 *    True if OK; otherwise, false on error.
 *
 *  @ingroup plist
 *
 */
Boolean
CFUPropertyListWriteToFile(const char *         inPath,
                           bool                 inWritable,
                           CFPropertyListFormat inFormat,
                           CFPropertyListRef    inPlist,
                           CFStringRef *        outError)
{
    int          error;
    CFStringRef  thePath     = nullptr;
    const mode_t kReadAll    = (S_IRUSR | S_IRGRP | S_IROTH);
    const mode_t kWriteAll   = (S_IWUSR | S_IWGRP | S_IWOTH);
    const mode_t permissions = inWritable ? (kReadAll | kWriteAll) : kReadAll;
    bool         status      = false;

    __Require(inPath != nullptr, done);
    __Require(inPlist != nullptr, done);

    // Convert the provided path name to a CoreFoundation string.

    thePath = CFStringCreateWithCString(kCFAllocatorDefault,
                                        inPath,
                                        CFStringGetSystemEncoding());
    __Require(thePath != nullptr, done);

    // Attempt to write the property list to the file at the specified
    // path.

    status = CFUPropertyListWriteToFile(thePath, inFormat, inPlist, outError);
    __Require(status, done);

    // Update the permissions according to the writability flag specified.

    error = chmod(inPath, permissions);
    __Require_Action(error == 0, done, status = false);

    // At this point, all operations were successful. Set the return
    // status accordingly.

    status = true;

done:
    CFURelease(thePath);

    return (status);
}

/**
 *  This routine determines whether the specified CoreFoundation set
 *  is an empty set.
 *
 *  @param[in]  inSet  The reference to the set to check for an empty set.
 *
 *  @returns
 *    True if the set is the empty set or the specified reference is
 *    null; otherwise, false.
 *
 *  @ingroup set
 *
 */
bool
CFUSetIsEmptySet(CFSetRef inSet)
{
    return ((inSet == nullptr) ? true : (CFSetGetCount(inSet) == 0));
}

/**
 *  This routine performs the real work of the CFUSetIntersectionSet
 *  function by serving as the iterator call back for the destination
 *  set.
 *
 *  If the object of the destination set does not exist in the souce
 *  set, it is removed from the source set.
 *
 *  @param[in]  inValue    A pointer to the object being iterated from
 *                         the destination set.
 *  @param[in]  inContext  A pointer to the call back context.
 *
 */
static void
CFUSetIntersectionApplier(const void * inValue, void * inContext)
{
    CFUSetContext * theContext = static_cast<CFUSetContext *>(inContext);

    if (!CFSetContainsValue(theContext->mSourceSet, inValue)) {
        CFSetRemoveValue(theContext->mDestinationSet, inValue);
    }
}

/**
 *  @brief
 *    Create the intersection between two CoreFoundation sets.
 *
 *  This routine removes from one set each object not contained in a
 *  second set resulting in the first set forming the intersection
 *  of the two sets.
 *
 *  @param[in,out]  inDestinationSet  The mutable set that serves as
 *                                    the destination of the
 *                                    intersection and whose values
 *                                    are checked against the source
 *                                    set. On success, this contains
 *                                    the intersection of the
 *                                    destination and source sets.
 *  @param[in]      inSourceSet       The immutable set that serves as
 *                                    the source of the intersection.
 *
 *  @sa CFUSetUnionSet
 *
 *  @ingroup set
 *
 */
void
CFUSetIntersectionSet(CFMutableSetRef inDestinationSet, CFSetRef inSourceSet)
{
    CFUSetContext theContext = { inDestinationSet, inSourceSet };

    if (inDestinationSet != nullptr && inSourceSet != nullptr) {
        CFSetApplyFunction(inDestinationSet,
                           CFUSetIntersectionApplier,
                           &theContext);
    }

    return;
}

/**
 *  This routine performs the real work of the CFUSetUnionSet
 *  function by serving as the iterator call back for the source
 *  set.
 *
 *  If the object of the source set does not exist in the destination
 *  set, it is added.
 *
 *  @param[in]  inValue    A pointer to the object being iterated from
 *                         the destination set.
 *  @param[in]  inContext  A pointer to the call back context.
 *
 */
static void
CFUSetUnionApplier(const void * inValue, void * inContext)
{
    CFUSetContext * theContext = static_cast<CFUSetContext *>(inContext);

    CFSetAddValue(theContext->mDestinationSet, inValue);
}

/**
 *  @brief
 *    Create the union between two CoreFoundation sets.
 *
 *  This routine adds to one set each object contained in a second
 *  set not already present in the first set resulting in the first
 *  set forming the union of the two sets.
 *
 *  @param[in,out]  inDestinationSet  The mutable set that serves as the
 *                                    destination of the union. On
 *                                    success, this contains the
 *                                    union of the destination and
 *                                    source sets.
 *  @param[in]      inSourceSet       The immutable set that serves as
 *                                    the source of the union and
 *                                    whose values are checked against
 *
 *  @sa CFUSetIntersectionSet
 *
 *  @ingroup set
 *
 */
void
CFUSetUnionSet(CFMutableSetRef inDestinationSet, CFSetRef inSourceSet)
{
    CFUSetContext theContext = { inDestinationSet, inSourceSet };

    if (inDestinationSet != nullptr && inSourceSet != nullptr) {
        CFSetApplyFunction(inSourceSet, CFUSetUnionApplier, &theContext);
    }

    return;
}

/**
 *  This routine initializes the specified CoreFoundation tree
 *  context to required values prior to first use with other
 *  CoreFoundation tree interfaces.
 *
 *  @param[in,out]  inContext  A pointer to the CoreFoundation tree
 *                             context to initialize. On success, a
 *                             pointer to the initialized
 *                             CoreFoundation tree context.
 *
 *  @ingroup tree
 */
void
CFUTreeContextInit(CFTreeContext * inContext)
{
    if (inContext != nullptr) {
        *inContext = kCFUTreeContextInitializer;
    }
}

/**
 *  This routine creates a new mutable CoreFoundation tree and
 *  associates with it the specified CoreFoundation object.
 *
 *  @param[in,out]  inType  A CoreFoundation base reference to the
 *                          object to be associated with the tree. On
 *                          success, a CoreFoundation base reference
 *                          to the object to be associated with the
 *                          tree with its reference count increased
 *                          by one.
 *
 *  @returns
 *    A reference to the newly-created tree if OK; otherwise, null on
 *    error.
 *
 *  @ingroup tree
 *
 */
CFTreeRef
CFUTreeCreate(CFTypeRef inType)
{
    CFTreeRef     tempTree = nullptr;
    CFTreeContext theContext;

    CFUTreeContextInit(&theContext);

    theContext.info            = const_cast<void *>(inType);
    theContext.retain          = CFRetain;
    theContext.release         = CFRelease;
    theContext.copyDescription = CFCopyDescription;

    tempTree = CFTreeCreate(kCFAllocatorDefault, &theContext);
    __Require(tempTree != nullptr, done);

done:
    return (tempTree);
}

/**
 *  This is helper function to do a quick string comparison between
 *  two CFStringRefs
 *
 *  @param[in]  aFirst   A CoreFoundation string reference to the first string
 *  @param[in]  aSecond  A CoreFoundation string reference to the second string
 *
 *  @returns
 *    A Boolean, true if the strings match; otherwise false
 *
 *  @ingroup string
 *
 */
bool
CFUStringsMatch(CFStringRef aFirst, CFStringRef aSecond)
{
    CFComparisonResult comparison;
    bool               match = false;

    if ((aFirst != nullptr) && (aSecond != nullptr)) {
        comparison = CFStringCompare(aFirst, aSecond, 0);

        match = (comparison == kCFCompareEqualTo);
    }

    return match;
}
