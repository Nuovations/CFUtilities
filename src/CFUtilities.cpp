/*
 *    Copyright (c) 2008-2023 Nuovation System Designs, LLC
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


// MARK: Type Definitions

/*
 * NOTE: Regrettably, clang-format is disabled below since
 *       clang-format, as of version 11, cannot both align consecutive
 *       member declarations and comments.
 */

/**
 *  Indicator of the phase of the dictionary difference algorithm.
 *
 *  @private
 *
 */
enum CFUDictionaryDifferencePhase
{
    kCFUDictionaryDifferencePhaseAdd,     //!< The algorithm is determining
                                          //!< those entries unique to the
                                          //!< proposed dictionary.
    kCFUDictionaryDifferencePhaseCommon,  //!< The algorithm is determining
                                          //!< those entries in common to both
                                          //!< dictionaries.
    kCFUDictionaryDifferencePhaseRemove   //!< The algorithm is determining
                                          //!< those entries unique to the
                                          //!< base dictionary.
};

/**
 *  Indicator of the phase of the dictionary merge with differences
 *  algorithm.
 *
 *  @private
 *
 */
typedef CFUDictionaryDifferencePhase CFUDictionaryMergeWithDifferencesPhase;

/**
 *  Iterator context for the dictionary difference interface.
 *
 *  @private
 *
 */
struct CFUDictionaryDifferenceContext {
    // clang-format off
    CFUDictionaryDifferencePhase  mPhase;    //!< The current difference
                                             //!< algorithm phase.
    CFDictionaryRef               mProposed; //!< A reference to the
                                             //!< dictionary serving as the
                                             //!< focus of the difference.
    CFDictionaryRef               mBase;     //!< A reference to the
                                             //!< dictionary serving as the
                                             //!< base of the difference.
    CFMutableDictionaryRef        mAdded;    //!< A reference to the mutable
                                             //!< dictionary containing
                                             //!< entries unique to the
                                             //!< proposed dictionary.
    CFMutableDictionaryRef        mCommon;   //!< A reference to the mutable
                                             //!< dictionary containing
                                             //!< entries common to both the
                                             //!< base and current dictionary
                                             //!< but that may be changed
                                             //!< between them in terms of
                                             //!< values.
    CFMutableDictionaryRef        mRemoved;  //!< A reference to the mutable
                                             //!< dictionary containing
                                             //!< entries unique to the
                                             //!< base dictionary.
    // clang-format on
};

/**
 *  Iterator context for the dictionary merge interface.
 *
 *  @private
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
 *  Iterator context for the dictionary merge with differences interfaces.
 *
 *  @private
 */
struct CFUDictionaryMergeWithDifferencesContext {
    // clang-format off
    CFUDictionaryMergeWithDifferencesPhase  mPhase;                  //!< The current merge
                                                                     //!< algorithm phase.
    CFMutableDictionaryRef                  mDestinationDictionary;  //!< A reference to the
                                                                     //!< mutable dictionary to
                                                                     //!< merge keys and values
                                                                     //!< to.
    // clang-format on
};

/**
 *  Iterator context used for the set intersection and union interfaces.
 *
 *  @private
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

// MARK: Global Variables

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

    __Require(inKey     != nullptr, done);
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
 *  This routine is a CoreFoundation dictionary applier function that
 *  iterates on each key/value pair of an added, common, removed
 *  difference dictionary (likely generated from
 *  #CFUDictionaryDifference) and applies the difference to the base
 *  dictionary as appropriate for the current algorithm phase.

 *
 *  @param[in]      inKey      A pointer to the key of the current
 *                             key/value pair being iterated upon.
 *  @param[in]      inValue    A pointer to the value of the current
 *                             key/value pair being iterated upon.
 *  @param[in,out]  inContext  A pointer to the iterator context. On
 *                             completion and depending on the context
 *                             phase, the add phase will add those
 *                             key/value pairs to the base; the remove
 *                             phase will remove those key/value pairs
 *                             from the base; and the phase will
 *                             replace those key/value pairs for which
 *                             the value is different in the common
 *                             relative to the base with the value
 *                             from the common.
 *
 *  @private
 *
 */
static void
CFUDictionaryMergeWithDifferencesApplier(const void * inKey,
                                         const void * inValue,
                                         void *       inContext)
{
    CFStringRef                                 theKey     =
        static_cast<CFStringRef>(inKey);
    CFTypeRef                                   theValue   =
        static_cast<CFTypeRef>(inValue);
    CFUDictionaryMergeWithDifferencesContext *  theContext =
        static_cast<CFUDictionaryMergeWithDifferencesContext *>(inContext);

    __Require(theKey     != nullptr, done);
    __Require(theValue   != nullptr, done);
    __Require(theContext != nullptr, done);

    // If the phase is add, set the key/value pair.
    //
    // If the phase is common, replace the key/value pair if unequal.
    //
    // If the phase is remove, remove the key/value pair.

    if (theContext->mPhase == kCFUDictionaryDifferencePhaseAdd)
    {
        CFDictionarySetValue(theContext->mDestinationDictionary, theKey, theValue);
    }
    else if (theContext->mPhase == kCFUDictionaryDifferencePhaseCommon)
    {
        CFTypeRef      theCurrentValue;
        Boolean        lEqual;

        theCurrentValue = static_cast<CFTypeRef>(CFDictionaryGetValue(theContext->mDestinationDictionary, theKey));
        __Require(theCurrentValue != nullptr, done);

        lEqual = CFEqual(theCurrentValue, theValue);

        if (!lEqual)
        {
            CFDictionaryReplaceValue(theContext->mDestinationDictionary, theKey, theValue);
        }
    }
    else if (theContext->mPhase == kCFUDictionaryDifferencePhaseRemove)
    {
        CFDictionaryRemoveValue(theContext->mDestinationDictionary, theKey);
    }

 done:
    return;
}

/**
 *  This routine is a CoreFoundation array applier function that
 *  iterates on each value of a removed keys array and applies them
 *  difference to the base dictionary, as appropriate for the current
 *  algorithm phase, by removing the corresponding key/value pairs
 *  from the base dictionary.
 *
 *  @param[in]      inValue    A pointer to the current value being
 *                             iterated upon. This value is actually a
 *                             dictionary key.
 *  @param[in,out]  inContext  A pointer to the iterator context. On
 *                             completion and depending on the context
 *                             phase, the remove phase will remove
 *                             those key/value pairs from the base.
 *
 *  @private
 *
 */
static void
CFUDictionaryMergeWithDifferencesApplierAndRemovedKeysApplier(const void * inValue,
                                                              void *       inContext)
{
    // Note that that since this actually matches the signature of an
    // array applier, in this context, the array values are actually
    // dictionary keys. Consequently, the local is named accordingly.

    CFDictionaryRef                             theKey     =
        static_cast<CFDictionaryRef>(inValue);
    CFUDictionaryMergeWithDifferencesContext *  theContext =
        static_cast<CFUDictionaryMergeWithDifferencesContext *>(inContext);

    __Require(theKey     != nullptr, done);
    __Require(theContext != nullptr, done);

    // There is only work to do in this case for the removal
    // phase. Other phases are handled with other applier functions.

    if (theContext->mPhase == kCFUDictionaryDifferencePhaseRemove)
    {
        CFDictionaryRemoveValue(theContext->mDestinationDictionary, theKey);
    }

 done:
    return;
}

/**
 *  @brief
 *    Merge a CoreFoundation dictionary from one or more difference
 *    dictionaries.
 *
 *  This routine merges a mutable CoreFoundation dictionary from one
 *  or more difference dictionaries (likely generated from
 *  #CFUDictionaryDifference). If present, key/value pairs in the @a
 *  inAdded dictionary are added to @a inOutBase and, if present,
 *  key/value pairs in the @a inCommon dictionary replace those in @a
 *  inOutBase if the values are different.
 *
 *  @param[in,out]  inOutBase  A reference to the mutable dictionary
 *                             to merge keys and values to. On
 *                             success, @a inOutBase contains a
 *                             reference to the destination dictionary
 *                             with entries from the added and common
 *                             dictionaries merged in.
 *  @param[out]     inAdded    An optional reference to the dictionary
 *                             containing entries to be added to the
 *                             base dictionary. If null, no such
 *                             entries will be added.
 *  @param[in]      inCommon   An optional reference to the dictionary
 *                             containing entries common to the base
 *                             dictionary, but potentially with
 *                             different values. Values that are
 *                             different will replace those in the
 *                             base. If null, no such entries will be
 *                             replaced.
 *
 *  @private
 *
 */
static void
CFUDictionaryMerge(CFUDictionaryMergeWithDifferencesContext &  inOutContext,
                   CFMutableDictionaryRef                      inOutBase,
                   CFDictionaryRef                             inAdded,
                   CFDictionaryRef                             inCommon)
{
    inOutContext.mDestinationDictionary = inOutBase;

    if (inAdded != nullptr)
    {
        // Add new values

        inOutContext.mPhase = kCFUDictionaryDifferencePhaseAdd;

        CFDictionaryApplyFunction(inAdded,
                                  CFUDictionaryMergeWithDifferencesApplier,
                                  &inOutContext);
    }

    if (inCommon != nullptr)
    {
        // Update common values;

        inOutContext.mPhase = kCFUDictionaryDifferencePhaseCommon;

        CFDictionaryApplyFunction(inCommon,
                                  CFUDictionaryMergeWithDifferencesApplier,
                                  &inOutContext);
    }
}

/**
 *  @brief
 *    Merge a CoreFoundation dictionary from one or more difference
 *    dictionaries.
 *
 *  This routine merges a mutable CoreFoundation dictionary from one
 *  or more difference dictionaries (likely generated from
 *  #CFUDictionaryDifference). If present, key/value pairs in the @a
 *  inAdded dictionary are added to @a inOutBase; if present,
 *  key/value pairs in the @a inCommon dictionary replace those in @a
 *  inOutBase if the values are different; and, if present, key/value
 *  pairs in the @a inRemoved dictionary are removed from @a
 *  inOutBase.
 *
 *  @param[in,out]  inOutBase  A reference to the mutable dictionary
 *                             to merge keys and values to. On
 *                             success, @a inOutBase contains
 *                             a reference to the destination
 *                             dictionary with entries from the
 *                             added, common, and removed
 *                             dictionaries merged in.
 *  @param[out]     inAdded    An optional reference to the dictionary
 *                             containing entries to be added to the
 *                             base dictionary. If null, no such
 *                             entries will be added.
 *  @param[in]      inCommon   An optional reference to the dictionary
 *                             containing entries common to the base
 *                             dictionary, but potentially with
 *                             different values. Values that are
 *                             different will replace those in the
 *                             base. If null, no such entries will be
 *                             replaced.
 *  @param[in]      inRemoved  An optional reference to the dictionary
 *                             containing entries to be removed from
 *                             the base dictionary. If null, no such
 *                             entries will be removed.
 *
 *  @returns
 *    True if the merge was successful; otherwise, false. False
 *    may be returned if an incorrect argument was supplied.
 *
 *  @sa CFUDictionaryDifference
 *
 *  @ingroup dictionary
 *
 */
Boolean
CFUDictionaryMerge(CFMutableDictionaryRef inOutBase,
                   CFDictionaryRef        inAdded,
                   CFDictionaryRef        inCommon,
                   CFDictionaryRef        inRemoved)
{
    CFUDictionaryMergeWithDifferencesContext  theContext;
    Boolean                                   status = true;

    __Require_Action(inOutBase != nullptr, done, status = false);

    // Add new and update common values

    CFUDictionaryMerge(theContext, inOutBase, inAdded, inCommon);

    if (inRemoved != nullptr)
    {
        // Remove old values

        theContext.mPhase = kCFUDictionaryDifferencePhaseRemove;

        CFDictionaryApplyFunction(inRemoved,
                                  CFUDictionaryMergeWithDifferencesApplier,
                                  &theContext);
    }

 done:
    return (status);
}

/**
 *  @brief
 *    Merge a CoreFoundation dictionary from one or more difference
 *    dictionaries.
 *
 *  This routine merges a mutable CoreFoundation dictionary from two
 *  or more difference dictionaries (likely generated from
 *  #CFUDictionaryDifference) and a difference array, containing
 *  removed keys. If present, key/value pairs in the @a inAdded
 *  dictionary are added to @a inOutBase; if present, key/value pairs
 *  in the @a inCommon dictionary replace those in @a inOutBase if the
 *  values are different; and, if present, keys in the @a
 *  inRemovedKeys array are removed from @a inOutBase.
 *
 *  @param[in,out]  inOutBase      A reference to the mutable
 *                                 dictionary to merge keys and values
 *                                 to. On success, @a inOutBase
 *                                 contains a reference to the
 *                                 destination dictionary with entries
 *                                 from the added and common
 *                                 dictionaries and the removed keys
 *                                 array merged in.
 *  @param[out]     inAdded        An optional reference to the
 *                                 dictionary containing entries to be
 *                                 added to the base dictionary. If
 *                                 null, no such entries will be
 *                                 added.
 *  @param[in]      inCommon       An optional reference to the
 *                                 dictionary containing entries
 *                                 common to the base dictionary, but
 *                                 potentially with different
 *                                 values. Values that are different
 *                                 will replace those in the base. If
 *                                 null, no such entries will be
 *                                 replaced.
 *  @param[in]      inRemovedKeys  An optional reference to the array
 *                                 containing key to be removed from
 *                                 the base dictionary. If null, no such
 *                                 entries will be removed.
 *
 *  @returns
 *    True if the merge was successful; otherwise, false. False
 *    may be returned if an incorrect argument was supplied.
 *
 *  @ingroup dictionary
 *
 */
Boolean
CFUDictionaryMerge(CFMutableDictionaryRef inOutBase,
                   CFDictionaryRef        inAdded,
                   CFDictionaryRef        inCommon,
                   CFArrayRef             inRemovedKeys)
{
    CFUDictionaryMergeWithDifferencesContext  theContext;
    Boolean                                   status = true;

    __Require_Action(inOutBase != nullptr, done, status = false);

    // Add new and update common values

    CFUDictionaryMerge(theContext, inOutBase, inAdded, inCommon);

    if (inRemovedKeys != nullptr)
    {
        // Remove old values

        theContext.mPhase = kCFUDictionaryDifferencePhaseRemove;

        CFArrayApplyFunction(inRemovedKeys,
                             CFRangeMake(0, CFArrayGetCount(inRemovedKeys)),
                             CFUDictionaryMergeWithDifferencesApplierAndRemovedKeysApplier,
                             &theContext);
    }

 done:
    return (status);
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
 *  @returns
 *    True if the merge was successful; otherwise, false. False
 *    may be returned if an incorrect argument was supplied.
 *
 *  @ingroup dictionary
 *
 */
Boolean
CFUDictionaryMerge(CFMutableDictionaryRef inDestination,
                   CFDictionaryRef        inSource,
                   bool                   inReplace)
{
    CFUDictionaryMergeContext theContext = { inDestination, inReplace };
    Boolean                   status     = true;

    __Require_Action(inDestination != nullptr, done, status = false);
    __Require_Action(inSource      != nullptr, done, status = false);

    CFDictionaryApplyFunction(inSource, CFUDictionaryMergeApplier, &theContext);

done:
    return (status);
}

/**
 *  @brief
 *    Merge a CoreFoundation dictionary from one or more difference
 *    dictionaries.
 *
 *  This routine merges a mutable CoreFoundation dictionary from one
 *  or more difference dictionaries (likely generated from
 *  #CFUDictionaryDifference). If present, key/value pairs in the @a
 *  inAdded dictionary are added to @a inOutBase; if present,
 *  key/value pairs in the @a inCommon dictionary replace those in @a
 *  inOutBase if the values are different; and, if present, key/value
 *  pairs in the @a inRemoved dictionary are removed from @a
 *  inOutBase.
 *
 *  @param[in,out]  inOutBase  A reference to the mutable dictionary
 *                             to merge keys and values to. On
 *                             success, @a inOutBase contains
 *                             a reference to the destination
 *                             dictionary with entries from the
 *                             added, common, and removed
 *                             dictionaries merged in.
 *  @param[out]     inAdded    An optional reference to the dictionary
 *                             containing entries to be added to the
 *                             base dictionary. If null, no such
 *                             entries will be added.
 *  @param[in]      inCommon   An optional reference to the dictionary
 *                             containing entries common to the base
 *                             dictionary, but potentially with
 *                             different values. Values that are
 *                             different will replace those in the
 *                             base. If null, no such entries will be
 *                             replaced.
 *  @param[in]      inRemoved  An optional reference to the dictionary
 *                             containing entries to be removed from
 *                             the base dictionary. If null, no such
 *                             entries will be removed.
 *
 *  @returns
 *    True if the merge was successful; otherwise, false. False
 *    may be returned if an incorrect argument was supplied.
 *
 *  @ingroup dictionary
 *
 */
Boolean
CFUDictionaryMergeWithDifferences(CFMutableDictionaryRef inOutBase,
                                  CFDictionaryRef        inAdded,
                                  CFDictionaryRef        inCommon,
                                  CFDictionaryRef        inRemoved)
{
    Boolean status = true;

    status = CFUDictionaryMerge(inOutBase,
                                inAdded,
                                inCommon,
                                inRemoved);

    return (status);
}

/**
 *  @brief
 *    Merge a CoreFoundation dictionary from one or more difference
 *    dictionaries.
 *
 *  This routine merges a mutable CoreFoundation dictionary from two
 *  or more difference dictionaries (likely generated from
 *  #CFUDictionaryDifference) and a difference array, containing
 *  removed keys. If present, key/value pairs in the @a inAdded
 *  dictionary are added to @a inOutBase; if present, key/value pairs
 *  in the @a inCommon dictionary replace those in @a inOutBase if the
 *  values are different; and, if present, keys in the @a
 *  inRemovedKeys array are removed from @a inOutBase.
 *
 *  @param[in,out]  inOutBase      A reference to the mutable
 *                                 dictionary to merge keys and values
 *                                 to. On success, @a inOutBase
 *                                 contains a reference to the
 *                                 destination dictionary with entries
 *                                 from the added and common
 *                                 dictionaries and the removed keys
 *                                 array merged in.
 *  @param[out]     inAdded        An optional reference to the
 *                                 dictionary containing entries to be
 *                                 added to the base dictionary. If
 *                                 null, no such entries will be
 *                                 added.
 *  @param[in]      inCommon       An optional reference to the
 *                                 dictionary containing entries
 *                                 common to the base dictionary, but
 *                                 potentially with different
 *                                 values. Values that are different
 *                                 will replace those in the base. If
 *                                 null, no such entries will be
 *                                 replaced.
 *  @param[in]      inRemovedKeys  An optional reference to the array
 *                                 containing key to be removed from
 *                                 the base dictionary. If null, no such
 *                                 entries will be removed.
 *
 *  @returns
 *    True if the merge was successful; otherwise, false. False
 *    may be returned if an incorrect argument was supplied.
 *
 *  @ingroup dictionary
 *
 */
Boolean
CFUDictionaryMergeWithDifferencesAndRemovedKeys(CFMutableDictionaryRef inOutBase,
                                                CFDictionaryRef        inAdded,
                                                CFDictionaryRef        inCommon,
                                                CFArrayRef             inRemovedKeys)
{
    Boolean status = true;

    status = CFUDictionaryMerge(inOutBase,
                                inAdded,
                                inCommon,
                                inRemovedKeys);

    return (status);
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
 *  This routine is a CoreFoundation dictionary applier function
 *  that iterates on each key/value pair of a base or proposed
 *  dictionary and determines whether each entry is unique or common
 *  to the other.
 *
 *  @param[in]      inKey      A pointer to the key of the current
 *                             key/value pair being iterated upon.
 *  @param[in]      inValue    A pointer to the value of the current
 *                             key/value pair being iterated upon.
 *  @param[in,out]  inContext  A pointer to the iterator context. On
 *                             completion and depending on the context
 *                             phase, the context's added dictionary,
 *                             if present, will contain entries unique
 *                             to the proposed dictionary; the
 *                             context's removed dictionary, if
 *                             present, will contain entries unique to
 *                             the base dictionary; and the context's
 *                             common dictionary, if present, will
 *                             contain entries common to both the base
 *                             and proposed dictionaries but for which
 *                             may have different values for their
 *                             keys.
 *
 *  @private
 *
 */
static void
CFUDictionaryDifferenceApplier(const void *inKey,
                               const void *inValue,
                               void *inContext)
{
    CFStringRef                       theKey     = static_cast<CFStringRef>(inKey);
    CFDictionaryRef                   theValue   = static_cast<CFDictionaryRef>(inValue);
    CFUDictionaryDifferenceContext *  theContext = static_cast<CFUDictionaryDifferenceContext *>(inContext);

    __Require(theKey != nullptr,     done);
    __Require(theValue != nullptr,   done);
    __Require(theContext != nullptr, done);

    if (theContext->mPhase == kCFUDictionaryDifferencePhaseAdd)
    {
        const bool theDictionaryHasKey = CFDictionaryContainsKey(theContext->mBase, theKey);

        if (!theDictionaryHasKey)
        {
            if (theContext->mAdded != nullptr)
            {
                CFDictionarySetValue(theContext->mAdded, theKey, theValue);
            }
        }
        else
        {
            if (theContext->mCommon != nullptr)
            {
                CFDictionarySetValue(theContext->mCommon, theKey, theValue);
            }
        }
    }
    else if (theContext->mPhase == kCFUDictionaryDifferencePhaseRemove)
    {
        const bool theDictionaryHasKey = CFDictionaryContainsKey(theContext->mProposed, theKey);

        if (!theDictionaryHasKey)
        {
            if (theContext->mRemoved != nullptr)
            {
                CFDictionarySetValue(theContext->mRemoved, theKey, theValue);
            }
        }
        else
        {
            if (theContext->mCommon != nullptr)
            {
                CFDictionarySetValue(theContext->mCommon, theKey, theValue);
            }
        }
    }

 done:
    return;
}

/**
 *  @brief
 *    Initialize a CoreFoundation dictionary difference context.
 *
 *  This intializes a CoreFoundation dictionary difference context for
 *  a #CFUDictionaryDifference call.
 *
 *  @param[in]      inProposed  A reference to the dictionary serving
 *                              as the focus of the difference.
 *  @param[in,out]  inOutBase   An reference to a mutable dictionary
 *                              reference serving as the base of the
 *                              difference. The reference itself is
 *                              optional and may be null. If the
 *                              reference is null, a mutable
 *                              dictionary will be allocated on the
 *                              caller's behalf that becomes their
 *                              responsiblity to release on success.
 *  @param[out]     outAdded    An optional reference to the mutable
 *                              dictionary containing entries unique
 *                              to the proposed dictionary. If null,
 *                              no such entries will be enumerated and
 *                              populated.
 *  @param[out]     outCommon   An optional reference to the mutable
 *                              dictionary containing entries common
 *                              to both the base and current
 *                              dictionary but that may be changed
 *                              between them in terms of values. If
 *                              null, no such entries will be
 *                              enumerated and populated.
 *  @param[out]     outRemoved  A optional reference to the mutable
 *                              dictionary containing entries unique
 *                              to the base dictionary. If null, no
 *                              such entries will be enumerated and
 *                              populated.
 *  @param[out]     outContext  A reference to the context to be
 *                              intialized.
 *
 *  @returns
 *    True if the difference was successful; otherwise, false. False
 *    may be returned if memory allocation was unsuccessful.
 *
 *  @private
 *
 */
static Boolean
CFUDictionaryDifferenceContextSetup(CFDictionaryRef inProposed,
                                    CFMutableDictionaryRef &inOutBase,
                                    CFMutableDictionaryRef outAdded,
                                    CFMutableDictionaryRef outCommon,
                                    CFMutableDictionaryRef outRemoved,
                                    CFUDictionaryDifferenceContext &outContext)
{
    Boolean status = true;

    if (inOutBase == nullptr)
    {
        CFMutableDictionaryRef theTemporaryDictionary = nullptr;

        theTemporaryDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                           0,
                                                           &kCFTypeDictionaryKeyCallBacks,
                                                           &kCFTypeDictionaryValueCallBacks);
        __Require_Action(theTemporaryDictionary != nullptr,
                         done,
                         status = false);

        CFUReferenceSet(inOutBase, theTemporaryDictionary);

        CFURelease(theTemporaryDictionary);
    }

    // Setup the difference context based on the provided dictionary
    // arguments.

    outContext.mProposed = inProposed;
    outContext.mBase     = inOutBase;
    outContext.mAdded    = outAdded;
    outContext.mCommon   = outCommon;
    outContext.mRemoved  = outRemoved;

 done:
    return (status);

}

/**
 *  @brief
 *    Apply a difference between the proposed and base dictionaries.
 *
 *  The attempts to apply a difference between the proposed and base
 *  dictionaries, returning, if requested by virtue of non-null
 *  mutable dictionary references, the entries between the proposed
 *  and base dictionaries that are unique to the proposed (that is,
 *  added relative to the base), unique to the proposed (that is,
 *  removed relative to the base), and common between them, though
 *  with potentially different values between the base and proposed in
 *  such case, the common dictionary will contain the base value.
 *
 *  @param[in]      inProposed  A reference to the dictionary serving
 *                              as the focus of the difference.
 *  @param[in,out]  inOutBase   An reference to a mutable dictionary
 *                              reference serving as the base of the
 *                              difference. The reference itself is
 *                              optional and may be null. If the
 *                              reference is null, a mutable
 *                              dictionary will be allocated on the
 *                              caller's behalf that becomes their
 *                              responsiblity to release on success.
 *  @param[out]     outAdded    An optional reference to the mutable
 *                              dictionary containing entries unique
 *                              to the proposed dictionary. If null,
 *                              no such entries will be enumerated and
 *                              populated.
 *  @param[out]     outCommon   An optional reference to the mutable
 *                              dictionary containing entries common
 *                              to both the base and current
 *                              dictionary but that may be changed
 *                              between them in terms of values. If
 *                              null, no such entries will be
 *                              enumerated and populated.
 *  @param[out]     outRemoved  A optional reference to the mutable
 *                              dictionary containing entries unique
 *                              to the base dictionary. If null, no
 *                              such entries will be enumerated and
 *                              populated.
 *
 *  @returns
 *    True if the difference was successful; otherwise, false. False
 *    may be returned if an incorrect argument was supplied or if
 *    memory allocation was unsuccessful.
 *
 *  @ingroup dictionary
 *
 */
Boolean
CFUDictionaryDifference(CFDictionaryRef inProposed,
                        CFMutableDictionaryRef &inOutBase,
                        CFMutableDictionaryRef outAdded,
                        CFMutableDictionaryRef outCommon,
                        CFMutableDictionaryRef outRemoved)
{
    CFUDictionaryDifferenceContext theContext;
    Boolean                        status = true;

    __Require_Action(inProposed != nullptr, done, status = false);

    // Setup the difference context.

    status = CFUDictionaryDifferenceContextSetup(inProposed,
                                                 inOutBase,
                                                 outAdded,
                                                 outCommon,
                                                 outRemoved,
                                                 theContext);
    __Require(status, done);
    __Require(inOutBase != nullptr, done);

    // Generate the dictionary unique to the proposed dictionary by
    // iterating over the proposed dictionary, representing "what is".

    theContext.mPhase   = kCFUDictionaryDifferencePhaseAdd;

    CFDictionaryApplyFunction(inProposed,
                              CFUDictionaryDifferenceApplier,
                              &theContext);

    // Generate the dictionary unique to the base dictionary by
    // iterating over the base dictionary, representing "what was".

    theContext.mPhase   = kCFUDictionaryDifferencePhaseRemove;

    CFDictionaryApplyFunction(inOutBase,
                              CFUDictionaryDifferenceApplier,
                              &theContext);

 done:
    return (status);
}

/**
 *  @brief
 *    Apply a difference between the proposed and base dictionaries.
 *
 *  The attempts to apply a difference between the proposed and base
 *  dictionaries, returning, if requested by virtue of non-null
 *  mutable dictionary references, the entries between the proposed
 *  and base dictionaries that are unique to the proposed (that is,
 *  added relative to the base), unique to the proposed (that is,
 *  removed relative to the base), and common between them, though
 *  with potentially different values between the base and proposed in
 *  such case, the common dictionary will contain the base value.
 *
 *  @param[in]      inProposed  A reference to the dictionary serving
 *                              as the focus of the difference.
 *  @param[in,out]  inOutBase   A pointer to a mutable dictionary
 *                              reference serving as the base of the
 *                              difference. The reference itself is
 *                              optional and may be null. If the
 *                              reference is null, a mutable
 *                              dictionary will be allocated on the
 *                              caller's behalf that becomes their
 *                              responsiblity to release on success.
 *  @param[out]     outAdded    An optional reference to the mutable
 *                              dictionary containing entries unique
 *                              to the proposed dictionary. If null,
 *                              no such entries will be enumerated and
 *                              populated.
 *  @param[out]     outCommon   An optional reference to the mutable
 *                              dictionary containing entries common
 *                              to both the base and current
 *                              dictionary but that may be changed
 *                              between them in terms of values. If
 *                              null, no such entries will be
 *                              enumerated and populated.
 *  @param[out]     outRemoved  A optional reference to the mutable
 *                              dictionary containing entries unique
 *                              to the base dictionary. If null, no
 *                              such entries will be enumerated and
 *                              populated.
 *
 *  @returns
 *    True if the difference was successful; otherwise, false. False
 *    may be returned if an incorrect argument was supplied or if
 *    memory allocation was unsuccessful.
 *
 *  @ingroup dictionary
 *
 */
Boolean
CFUDictionaryDifference(CFDictionaryRef inProposed,
                        CFMutableDictionaryRef *inOutBase,
                        CFMutableDictionaryRef outAdded,
                        CFMutableDictionaryRef outCommon,
                        CFMutableDictionaryRef outRemoved)
{
    Boolean status = true;

    __Require_Action(inOutBase != nullptr, done, status = false);

    status = CFUDictionaryDifference(inProposed,
                                     *inOutBase,
                                     outAdded,
                                     outCommon,
                                     outRemoved);

 done:
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
 *  @brief
 *    Remove a trailing newline, if present, from the specified
 *    string.
 *
 *  This attempts to remove, if present, a trailing newline from the
 *  specified string.
 *
 *  @param[in,out]  inOutString  A CoreFoundation mutable string
 *                               reference from which to try to remove
 *                               a trailing newline.
 *  @param[in,out]  inOutLength  The length of @a inOutString, in
 *                               characters, before and, if
 *                               successful, after removal of any
 *                               trailing newline. On success, the
 *                               output value is expected to be one
 *                               less than its input value.
 *
 *  @returns
 *    True if a trailing newline was found and removed; otherwise,
 *    false.
 *
 */
bool
CFUStringChomp(CFMutableStringRef inOutString, size_t &inOutLength)
{
    CFIndex  lReplacedInstances;
    bool     lRetval = false;

    __Require(inOutString != nullptr, done);

    lReplacedInstances = CFStringFindAndReplace(inOutString,
                                                CFSTR("\n"),
                                                CFSTR(""),
                                                CFRangeMake(static_cast<CFIndex>(inOutLength - 1),
                                                            static_cast<CFIndex>(inOutLength)),
                                                kCFCompareBackwards);

    if (lReplacedInstances == 1)
    {
        inOutLength--;

        lRetval = true;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Remove a trailing newline, if present, from the specified
 *    string.
 *
 *  This attempts to remove, if present, a trailing newline from the
 *  specified string.
 *
 *  @param[in,out]  inOutString  A CoreFoundation mutable string
 *                               reference from which to try to remove
 *                               a trailing newline.
 *
 *  @returns
 *    True if a trailing newline was found and removed; otherwise,
 *    false.
 *
 */
bool
CFUStringChomp(CFMutableStringRef inOutString)
{
    size_t  lLength;
    bool    lRetval = false;

    __Require(inOutString != nullptr, done);

    lLength = static_cast<size_t>(CFStringGetLength(inOutString));

    lRetval = CFUStringChomp(inOutString, lLength);

 done:
    return (lRetval);
}

/**
 *  This is a helper function to do a quick string comparison between
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
