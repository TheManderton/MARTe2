/**
 * @file RealTimeDataSourceDef.h
 * @brief Header file for class RealTimeDataSourceDef
 * @date 29/02/2016
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

 * @details This header file contains the declaration of the class RealTimeDataSourceDef
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEDATASOURCEDEF_H_
#define REALTIMEDATASOURCEDEF_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "GAM.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class RealTimeDataSourceDef: public ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    RealTimeDataSourceDef();

    bool AddConsumer(const char8 *stateIn,
                     ReferenceT<GAM> gam);

    bool AddProducer(const char8 *stateIn,
                     ReferenceT<GAM> gam);

    void SetDefaultValue(const char8* defaultIn);

    const char8 *GetDefaultValue();

    uint32 GetNumberOfConsumers(const char8 * stateIn);

    uint32 GetNumberOfProducers(const char8 * stateIn);

    bool Verify();

    bool SetType(const char8 *typeName);

    const char8 *GetType();

    void **GetDataSourcePointer(uint8 bufferIndex);

    bool PrepareNextState(const RealTimeStateInfo &status);

    bool Allocate(MemoryArea &dsMemory);

    void SetNumberOfElements(uint8 dimension, uint32 nElements);

    void SetNumberOfDimensions(uint8 nDimensions);

private:

    StreamString type;

    StreamString defaultValue;

    uint32 bufferPtrOffset[2];

    void *usedBuffer[2];

    uint8 numberOfDimensions;

    uint32 numberOfElements[3];

    MemoryArea *memory;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEDATASOURCEDEF_H_ */

