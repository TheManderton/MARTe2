/**
 * @file HighResolutionTimerCalibrator.h
 * @brief Header file for class HighResolutionTimerCalibrator
 * @date 17/06/2015
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

 * @details This header file contains the declaration of the class HighResolutionTimerCalibrator
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HIGHRESOLUTIONTIMERCALIBRATOR_ENV_H_
#define HIGHRESOLUTIONTIMERCALIBRATOR_ENV_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#ifndef LINT
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#else
#include "lint-linux.h"
#endif

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "../../GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief A class to get the period and the frequency of the cpu clock depending on the operating system.
 */
class HighResolutionTimerCalibrator {

public:

    /**
     * @brief Estimates the period and the frequency of the cpu clock.
     * @details The period and frequency of the CPU clock are estimated upon
     * construction.
     * In the Linux implementation these values are read from the /proc/cpuinfo file.
     * The number of elapsed cpu ticks is also stored at this moment.
     */
    HighResolutionTimerCalibrator();

    /**
     * @brief Get the current time stamp.
     * @param[in] timeStamp is a structure which contains the time stamp fields.
     * @return true if the time can be successfully retrieved from the operating system.
     */
    bool GetTimeStamp(TimeValues &timeStamp) const;

    /**
     * @brief Returns the calibrated CPU frequency.
     * @return the calibrated CPU frequency.
     */
    int64 GetFrequency() const;

    /**
     * @brief Returns the calibrated CPU period.
     * @return the calibrated CPU period.
     */
    float64 GetPeriod() const;

private:

    /**
     * Number of cpu ticks in a second
     */
    int64 frequency;

    /**
     * Time between two ticks in seconds
     */
    float64 period;

    /**
     * Stores the time (counting from the epoch) at which a framework instance was executed.
     */
    struct timeval initialTime;

    /**
     * Number of elapsed ticks at the time at which a framework instance was executed.
     */
    int64 initialTicks;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HIGHRESOLUTIONTIMERCALIBRATOR_ENV_H_ */
