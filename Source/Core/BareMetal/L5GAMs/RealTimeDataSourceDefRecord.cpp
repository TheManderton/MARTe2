/**
 * @file RealTimeDataSourceDefRecord.cpp
 * @brief Source file for class RealTimeDataSourceDefRecord
 * @date 01/mar/2016
 * @author pc
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
 * the class RealTimeDataSourceDefRecord (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceDefRecord.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeDataSourceDefRecord::RealTimeDataSourceDefRecord() {

    // can be explored
    ReferenceT<ReferenceContainer> prod(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    prod->SetName("Producers");
    if (prod.IsValid()) {
        if (Insert(prod)) {
            producers = prod;
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed in creation of the producers container");
        }
    }

    ReferenceT<ReferenceContainer> cons(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    cons->SetName("Consumers");
    if (cons.IsValid()) {
        if (Insert(cons)) {
            consumers = cons;
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed in creation of the consumers container");
        }
    }
}

bool RealTimeDataSourceDefRecord::AddConsumer(ReferenceT<GAM> gamConsumer) {

    bool ret = consumers.IsValid();
    if (ret) {
        ret = consumers->Insert(gamConsumer);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Consumers container not found or invalid");
    }
    return ret;
}

bool RealTimeDataSourceDefRecord::AddProducer(ReferenceT<GAM> gamProducer) {
    bool ret = producers.IsValid();
    if (ret) {
        ret = producers->Insert(gamProducer);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Producers container not found or invalid");
    }
    return ret;
}

uint32 RealTimeDataSourceDefRecord::GetNumberOfConsumers() {
    return consumers->Size();
}

uint32 RealTimeDataSourceDefRecord::GetNumberOfProducers() {
    return producers->Size();
}

ReferenceT<ReferenceContainer> RealTimeDataSourceDefRecord::GetConsumers() {
    return consumers;
}

ReferenceT<ReferenceContainer> RealTimeDataSourceDefRecord::GetProducers() {
    return producers;
}

CLASS_REGISTER(RealTimeDataSourceDefRecord, "1.0")

}

