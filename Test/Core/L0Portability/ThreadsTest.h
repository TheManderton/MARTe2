/**
 * @file ThreadsTest.h
 * @brief Header file for class ThreadsTest
 * @date 25/06/2015
 * @author Giuseppe Ferr�
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class ThreadsTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef THREADSTEST_H_
#define 		THREADSTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Threads.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test the Threads functions.
 */
class ThreadsTest {
public:

    ThreadsTest();

    /**
     * @brief Tests the Threads::BeginThread function.
     * @details nOfThreads are lunched. Each thread increments a variable, waits
     * for the main permission and then exits incrementing a exitCondition allowing a new cycle.
     * Notice that never two threads are running in parallel. The next thread is lunched after the previous
     * thread ends. The main has timeout, if the thread is not correctly lunched, after the time out
     * time, the main returns false;
     * @param[in] name is the desired thread name.
     * @param[in] stackSize is the desired stack size.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the main arrives to the end. False if the time exceeds the timeout.
     */
    bool TestBeginThread(const char8 *name,
                         uint32 stackSize,
                         uint32 nOfThreads);
    /**
     * @brief Tests the Threads::BeginThread function.
     * @details lunch a thread with 0 stacSizes.
     * @param[in] name is the desired thread name.
     * @return true if the main arrive to the end. False if the the thread is not lunched correctly and
     * the time exceeds the timeout.
     **/

    bool TestBeginThreadStackSize0(const char8 *name);

    /**
     * @brief Tests the Threads::BeginThread function.
     * @details lunch a thread with a NULL function.
     * @param[in] name is the desired thread name.
     * @return true if no function is lunched.
     **/
    bool TestBeginThreadNullFunction(const char8 *name);

    /**
     * @brief Tests the Threads::EndThread function.
     * @details Checks if the threads terminate after the end function call.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if all all functions are ended correctly, false otherwise.
     */
    bool TestEndThread(uint32 nOfThreads);

    /**
     * @brief Tests the Threads::IsAlive function.
     * @details Launches a thread and checks if IsAlive returns the expected result.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the thread is alive, false otherwise.
     */
    bool TestIsAlive(uint32 nOfThreads);

    /**
     * @brief Tests the Threads::IsAlive function.
     * @details Launches a thread and checks if IsAlive after kill.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the thread is not alive after kill, false otherwise.
     */
    bool TestIsAliveAfterkill(uint32 nOfThreads);

    /**
     * @brief Tests the Threads::Kill function.
     * @details Kills a thread with an invalid ID.
     * @return true if Threads::Kill return false.
     */
    bool TestKillInvalidID();

    /**
     * @brief Tests the Threads::Kill function.
     * @details Kills a thread and checks if it terminates.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the thread is killed, false if the thread is still alive or a second kill returns
     * true.
     */
    bool TestKill(uint32 nOfThreads);

    /**
     * @brief Tests the Threads::SetPriorityLevel and Threads::GetPriorityLevel (for all priority levels) functions.
     * @details Launches a thread for each priority class and level and checks if the priority class and level returned is correct.
     * @return true if the priority level and  priority class are as expected, false otherwise.
     */
    bool TestPriorityLevel();

    /**
     * @brief Tests the Threads::SetPriorityClass and Threads::GetPriorityClass functions.
     * @details Launches a thread for each priority class and level and checks if the priority class and level returned is correct.
     * @return true if the priority level and  priority class are as expected, false otherwise.
     */
    bool TestPriorityClass();

    /**
     * @brief Tests the Threads::GetState function
     * @details In Linux and Windows theGetState() function returns STATE_UNKNOWN.
     * @return true if state is STATE_UNKNOWN, false otherwise.
     */
    bool TestGetState();

    /**
     * @brief Tests the Threads::Id function which should return the own id.
     * @details Launches some threads and checks their id.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the returned value of the Threads::Id() = tidTest, false otherwise.
     */
    bool TestId(uint32 nOfThreads);

    /**
     * @brief Tests the Threads::GetCPUs function.
     * @details Lunch consecutively threads with cpu mask which indicates in which CPU the threat can be
     * running. Then t is check if the assign CPU is as expected.
     * @return true if GetCPUs returns the expected CPU mask, false otherwise.
     */
    bool TestGetCPUs();

    /**
     * @brief Tests the Threads::Name function.
     * @details The return value should be the threads name if the thread is launched with specified name.
     * @param[in] name is the desired thread name.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if Threads::Name(tid) returns the expected name of the thread, false otherwise.
     */
    bool TestName(const char8 *name,
                  uint32 nOfThreads);

    /**
     * @brief Tests the Threads::Name function.
     * @details The return value of Threads::Name(tid) should be "Unknown" if the thread is launched without providing a name (namely NULL name).
     * @param[in] name is the desired thread name.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if Threads::Name(tid) returns "Unknown", false otherwise.
     */
    bool TestNameNull();

    /**
     * @brief Tests the Threads::NumberOfThreads function
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the number of threads in the database is the expected number, false otherwise.
     */
    bool TestNumberOfThreads(uint32 nOfThreads);

    /**
     * @brief Tests the Threads::FindByIndex function.
     * @details Stores some threads in the database sequentially, then search them by index and checks if the returned tid is correct.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the searched index thread has the correct ID, false otherwise.
     */
    bool TestFindByIndex(uint32 nOfThreads);

    /**
     * @brief Tests the Threads::GetThreadInfoCopy function.
     * @details Launches nOfThreads threads and get the thread information structure in the database
     * checking if it is consistent with the thread's specifications.
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] name is the desired threads name.
     * @return true if the properties of the threads (name, priorityClass, priorityLevel and threadId), are
     *  as expected, false otherwise.
     */
    bool TestGetThreadInfoCopy(uint32 nOfThreads,
                               const char8 *name);

    /**
     * @brief Tests the Threads::GetThreadInfoCopy function.
     * @details Call Threads::GetThreadInfoCopy() with an invalid thread ID
     * @return true if Threads::GetThreadInfoCopy() returns false, false otherwise
     */
    bool TestGetThreadInfoCopyInvalidID();

    /**
     * @brief Tests the Threads::FindByName function.
     * @details Launches thread with the specified name and the others without name, then checks if the returned
     * tid of the FindByName function is correct.
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] name is the name to be searched in the database.
     * @param[in] position is the index of the thread with the specified name.
     * @return true if can find a thread with "name" and another with "known" as a name, false otherwise.
     */
    bool TestFindByName(uint32 nOfThreads,
                        const char8 *name,
                        uint32 position);

    /**
     * @brief Tests the Threads::ProtectedExecute function.
     * @details A thread increment a variable unprotected and lunch
     * thread::ProtectedExecute which increments the same variable
     * @return true if the incremented value is as expected, false otherwise.
     */
    bool TestProtectedExecute();

    /**
     * A shared variable used for synchronization.
     */
    int32 exitCondition;

    /**
     * A variable used to save a thread identifier.
     */
    TID tidTest;

    /**
     * A boolean to store the return value.
     */
    bool retValue;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* THREADSTEST_H_ */

