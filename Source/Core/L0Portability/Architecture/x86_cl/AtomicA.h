/**
 * @file AtomicA.h
 * @brief Header file for class AtomicA
 * @date 08/giu/2015
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
 *
 * @details This header file contains the declaration of the class AtomicA
 * (all of its public, protected and private members). It may also include
 * definitions for inline and friend methods which need to be visible to
 * the compiler.
 */

#ifndef ATOMICA_H_
#define 		ATOMICA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include <intrin.h>

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/**
 * @brief Atomically increment a 32 bit integer in memory.
 * @param[in][out] p is the 32 bit variable which must be incremented.
 */
inline void AtomicIncrement32(volatile int32 *p) {
    volatile long* pp = (volatile long *) p;
    _InterlockedIncrement(pp);
}

/**
 * @brief Atomically increment a 16 bit integer in memory.
 * @param[in][out] p is the 16 bits variable which must be incremented.
 */
inline void AtomicIncrement16(volatile int16 *p) {
    volatile short* pp = (volatile short*) p;
    _InterlockedIncrement16(pp);
}

/**
 * @brief Atomically increment a 8 bit integer in memory.
 * @param[in][out] p is 8 bits variable which must be incremented.
 */
inline void AtomicIncrement8(volatile int8 *p) {
    //There is not _InterlockedIncrement8 function.
    volatile char* pp = (volatile char*) p;
    _InterlockedExchangeAdd8(pp, 1);
}

/**
 * @brief Atomically decrement a 32 bit integer in memory.
 * @param[in][out] p is the 32 bits variable to decrement.
 */
inline void AtomicDecrement32(volatile int32 *p) {
    volatile long* pp = (volatile long *) p;
    _InterlockedDecrement(pp);
}

/**
 * @brief Atomically decrement a 16 bit integer in memory.
 * @param[in][out] p is the 16 bits variable to decrement.
 */
inline void AtomicDecrement16(volatile int16 *p) {
    volatile short* pp = (volatile short *) p;
    _InterlockedDecrement16(pp);
}

/**
 * @brief Atomically decrement a 8 bit integer in memory.
 * @param[in][out] p is the 8 bits variable to decrement.
 */
inline void AtomicDecrement8(volatile int8 *p) {
    volatile char* pp = (volatile char *) p;
    _InterlockedExchangeAdd8(pp, -1);
}

/**
 * @brief Atomically exchange the contents of a variable with the specified memory location.
 * @param[out] p is the variable to change.
 * @param[in] v is the variable to store.
 */
inline int32 AtomicExchange32(volatile int32 *p,
                              int32 v) {
    volatile long* pp = (volatile long *) p;
    return _InterlockedExchange(pp, v);
}

/**
 * @brief Test and set a 32 bit memory location in a thread safe way.
 * @param[in][out] p is the 32 bit variable to test and set.
 * @return return true if p=0 and it sets p to one, else return false.
 */
inline bool AtomicTestAndSet32(int32 volatile *p) {
    volatile long* pp = (volatile long *) p;
    //returns the initial value of *pp
    return _InterlockedCompareExchange(pp, 1, 0) == 0;
}

/**
 * @brief Test and set a 16 bit memory location in a thread safe way.
 * @param[in][out] p is the 16 bit variable to test and set.
 * @return return true if p=0 and it sets p to one, else return false.
 */

inline bool AtomicTestAndSet16(int16 volatile *p) {
    volatile short* pp = (volatile short *) p;
    //returns the initial value of *pp
    return _InterlockedCompareExchange16(pp, 1, 0) == 0;
}

/**
 * @brief Test and set a 8 bit memory location in a thread safe way.
 * @param[in][out] p is the 8 bit variable to test and set.
 * @return return true if p=0 and it sets p to one, else return false.
 */
inline bool AtomicTestAndSet8(int8 volatile *p) {
    volatile char* pp = (volatile char*) p;

    //returns the initial value of *pp
    return _InterlockedCompareExchange8(pp, 1, 0) == 0;
}

/**
 * @brief Atomic addition.
 * @param[in][out] p is the variable.
 * @param[in] value is the value to sum to p.
 */
inline void AtomicAdd32(volatile int32 *p,
                        int32 value) {

    volatile long* pp = (volatile long *) p;
    _InterlockedExchangeAdd(pp, value);
}

/**
 * @brief Atomic subtraction.
 * @param[in][out] p is the variable.
 * @param[in] value is the value to subtract to p.
 */
inline void AtomicSub32(volatile int32 *p,
                        int32 value) {
    volatile long* pp = (volatile long *) p;
    _InterlockedExchangeAdd(pp, -value);
}

/*---------------------------------------------------------------------------*/
/*                        Friend method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ATOMICA_H_ */

