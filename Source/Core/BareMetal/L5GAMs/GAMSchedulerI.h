/**
 * @file GAMSchedulerI.h
 * @brief Header file for class GAMSchedulerI
 * @date 22/02/2016
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class GAMSchedulerI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMSCHEDULERI_H_
#define GAMSCHEDULERI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "RealTimeThread.h"
#include "GAMSchedulerRecord.h"
#include "BasicRealTimeDataSourceOutputWriter.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class DLL_API GAMSchedulerI: public ReferenceContainer {

public:

    /**
     * @brief Constructor
     */
    GAMSchedulerI();

    /**
     * @brief Inserts a thread to be executed in a specific state.
     */
    bool InsertRecord(const char8 * stateName,
                      ReferenceT<RealTimeThread> thread);

    /**
     * @brief Stores the GAMSchedulerRecord for the new state in the next buffer.
     * @param[in] info contains information about the current and the next state
     * and the active buffer index.
     * @return true if the next state name is found, false otherwise.
     */
    bool PrepareNextState(RealTimeStateInfo info);

    /**
     * @brief Stops the execution of the current state and starts the execution
     * of the new state threads.
     * @param[in] activeBuffer is the current state buffer active index .
     */
    void ChangeState(const uint32 activeBuffer);


    /**
     * @brief Starts the execution of the next state threads.
     */
    virtual void StartExecution(const uint32 activeBuffer)=0;

    /**
     * @brief Stops the execution of the current state threads.
     */
    virtual void StopExecution()=0;

protected:

    /**
     * Double buffer accelerator to the threads to be executed for the current
     * and next state.
     */
    ReferenceT<GAMSchedulerRecord> statesInExecution[2];

    /**
     * Double buffer array of writers (one for each thread).
     */
    ReferenceT<BasicRealTimeDataSourceOutputWriter>* writer[2];

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMSCHEDULERI_H_ */

