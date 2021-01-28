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
 *      This file implements a unit test driver for the CFUtilities
 *      unit tests.
 */

#include <stdlib.h>

#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int main(void)
{
    // Create the event manager and test controller

    CppUnit::TestResult lController;

    // Add a listener that collects test result

    CppUnit::TestResultCollector lResultCollector;

    lController.addListener(&lResultCollector);

    // Add a listener that prints dots as tests run.

    CppUnit::BriefTestProgressListener lProgressListener;

    lController.addListener(&lProgressListener);

    // Get the top-level test suite from the registry

    CppUnit::Test *lSuite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

    // Adds the test to the list of tests to run

    CppUnit::TextUi::TestRunner lRunner;

    lRunner.addTest(lSuite);
    lRunner.run(lController);

    // Change the default outputter to a compiler error format outputter

    CppUnit::CompilerOutputter lOutputter(&lResultCollector, std::cerr);

    lOutputter.write();

    // Run the tests.

    bool lSuccess = lResultCollector.wasSuccessful();

    return (lSuccess ? EXIT_SUCCESS : EXIT_FAILURE);
}
