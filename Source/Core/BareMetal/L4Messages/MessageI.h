/**
 * @file MessageI.h
 * @brief Header file for class MessageI
 * @date 05/04/2016
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class MessageI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MESSAGEI_H_
#define MESSAGEI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Message.h"
#include "MessageFilterPool.h"
#include "TimeoutType.h"
#include "ReferenceT.h"
#include "ErrorType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The interface which has to be injected in an Object giving it the skills for sending and receiving
 * Messages.
 * @details To implement a new MARTe::Object able to send and receive messages, it is necessary a declare a class which inherits from both
 * Object and MessageI.
 */
class DLL_API MessageI {
public:

    /**
     * @brief Constructor.
     */
    MessageI();

    /**
     * @brief Destructor.
     */
    virtual ~MessageI();

    /**
     * @brief Sends a Message to the destination (specified in Message).
     * @details Depending on the flags to be read from \a message, this function can behave in a sort of synchronous or asynchronous mode,
     * wait or not for a reply, etc.
     * If Message::ExpectsReply() == true then a reply message is requested. The reply message is the same message marked as reply and with additional or modified content.
     * if Message::ExpectsIndirectReply() == true then the message after being modified to be a reply is also sent back to the sender
     * Synchronous communication is performed when the message is handled by the same thread of the caller. In this case the transformation to reply message has already happened at the end of the sendmessage call
     * Asynchronous communication is performed when the message is handled by a different thread . In this case the transformation to reply message happens asynchronously.
     * @param[in,out] message is the message to be sent. It will be modified to contain the destination reply if ExpectsImmediateReply is true.
     * @param[in] sender is the Object sending the message.

     * @return
     *   ErrorManagement::NoError() if the destination object is found and the called function returns true.
     *   ErrorManagement::FatalError if the function to be called returns false.
     *   ErrorManagement::UnsupportedFeature if no receiver for this message was found
     *   ErrorManagement::Timeout if a wait for reply times out
     *   ErrorManagement::communicationError if trying to send a direct reply
     *   ErrorManagement::ParametersError if the message is invalid or if sender is NULL and reply was expected
     */
    static ErrorManagement::ErrorType SendMessage(ReferenceT<Message> &message,const Object * const sender = NULL_PTR(Object *));

//    * Deals with indirect replies by polling the status of a ReplyMessageCatcher

    /**
     * @brief wait for the reply.
     * @details Deals only with direct replies by polling the status of the Message until it is marked as a reply
     * @param[in,out] message is the message that was sent. It will contain the reply.
     * @param[in] maxWait is the maximum time allowed waiting for the message reply.
     * @param[in] pollingTimeUsec is the period between check of the arrival as us
     * @return
     *   ErrorManagement::NoError() if the reply is obtained on time.
     *   ErrorManagement::Timeout if a wait for reply times out
     *   ErrorManagement::communicationError if no reply expected
     */
    static ErrorManagement::ErrorType WaitForReply(ReferenceT<Message> &message,const TimeoutType &maxWait = TTInfiniteWait, const uint32 pollingTimeUsec=1000);

    /**
     * @brief sends a message expecting direct reply and waits for it
     * @details Before calling SendMessage, the message flag ExpectReply() is set and the message timeout is set as maxWait.
     * @details Combines SendMessage and WaitForReply
     * @param[in,out] message is the message to be sent. It will be modified to contain the reply.
     * @param[in] sender is the Object sending the message.
     * @param[in] maxWait is the maximum time allowed waiting for the message reply.
     * @return
     *   ErrorManagement::NoError() if the reply is obtained on time.
     *   ErrorManagement::ParametersError if message is no valid pointer
     *   ErrorManagement::Timeout if a wait for reply times out
     *   ErrorManagement::communicationError if no reply expected
     */
    static ErrorManagement::ErrorType SendMessageAndWaitReply(ReferenceT<Message> &message,const Object * const sender = NULL_PTR(Object *),
                                                              const TimeoutType &maxWait = TTInfiniteWait, const uint32 pollingTimeUsec=1000);

    /**
     * @brief installs a message filter in a given position
     * TODO (tests missing)
     */
    ErrorManagement::ErrorType InstallMessageFilter(ReferenceT<MessageFilter> messageFilter, int32 position=0);

    /**
     * TODO
     * TODO (tests missing)
     */
    ErrorManagement::ErrorType RemoveMessageFilter(ReferenceT<MessageFilter> messageFilter);

    /**
     * TODO
     * TODO (tests missing)
     */
    ErrorManagement::ErrorType RemoveMessageFilter(CCString name);

    /**
     * @brief a message is sent requiring indirect reply. A Reply message is waited for.
     * @details installs a message Filter. Sends The message. Wait on filter.
     * @param[in,out] message is the message to be sent. It can be modified if the destination re-sends it to the sender as a reply.
     * @param[in] sender is the Object sending the message.
     */
    ErrorManagement::ErrorType SendMessageAndWaitIndirectReply(ReferenceT<Message> &message,const TimeoutType &maxWait = TTInfiniteWait,
                                                                  const uint32 pollingTimeUsec=1000);

protected:
    /**
     * TODO
     *
     * */
    static ReferenceT<MessageI> FindDestination(CCString destination);

    /**
     * TODO
     * The message consuming filters
     * used by SortMessage
     *
     * */
    MessageFilterPool messageFilters;


};


}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MESSAGEI_H_ */

