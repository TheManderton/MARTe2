/**
 * @file MemoryMapInputBroker.cpp
 * @brief Source file for class MemoryMapInputBroker
 * @date 18/07/2016
 * @author Andre Neto
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

 * @details This source file contains the definition of all the methods for
 * the class MemoryMapInputBroker (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MemoryMapInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
MemoryMapInputBroker::MemoryMapInputBroker() :
        MemoryMapBroker() {

}

MemoryMapInputBroker::~MemoryMapInputBroker() {

}

bool MemoryMapInputBroker::Execute() {
    uint32 n;
    bool ret = true;
    for (n = 0u; (n < numberOfCopies) && (ret); n++) {
        if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
            ret = MemoryOperationsHelper::Copy(copyTable[n].gamPointer, copyTable[n].dataSourcePointer, copyTable[n].copySize);
        }
    }
    return ret;
}

CLASS_REGISTER(MemoryMapInputBroker, "1.0")
}

