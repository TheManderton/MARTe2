/**
 * @file DoubleBufferedStream.h
 * @brief Header file for class DoubleBufferedStream
 * @date 06/10/2015
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

 * @details This header file contains the declaration of the class DoubleBufferedStream
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DOUBLEBUFFEREDSTREAM_H_
#define DOUBLEBUFFEREDSTREAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#include "TimeoutType.h"
#include "IOBuffer.h"
#include "AnyType.h"
#include "BufferedStream.h"
#include "FormatDescriptor.h"
#include "BufferedStreamIOBuffer.h"
#include "BufferedStream.h"

namespace MARTe {

/**
 * @brief Buffered stream implementation (double buffer).
 * @details This class offers a buffering mechanism for character streams.
 * It supplements two independent low-level RawStreams (which implements the low-level calls
 * such as Read, Write, Seek, ...) with a buffering scheme.
 */
class DoubleBufferedStream: public BufferedStream {

public:
    /**
     * @brief Default constructor.
     * @post
     *   CanRead() == false
     *   CanWrite() == false
     *   GetInputBuffer() == BufferedStreamIOBuffer
     *   GetOutputBuffer() == BufferedStreamIOBuffer
     *   GetTimeout() == TTInfiniteWait
     */
    DoubleBufferedStream();

    /**
     * @brief Default constructor.
     * @post
     *   CanRead() == false
     *   CanWrite() == false
     *   GetInputBuffer() == BufferedStreamIOBuffer
     *   GetOutputBuffer() == BufferedStreamIOBuffer
     *   GetTimeout() == timeoutIn
     */
    DoubleBufferedStream(const TimeoutType &timeoutIn);

    /**
     * @brief Default destructor.
     */
    virtual ~DoubleBufferedStream();

    /**
     * @brief Sets the buffer size.
     * @param[in] bufferSize the desired size for the buffer.
     * @return true if the buffer memory is reallocated correctly.
     * @pre
     *    readBufferSize > 8u
     *    writeBufferSize > 8u
     *    CanRead() == true || CanWrite() == true
     * @post
     *    GetInputStream()->BufferSize() == readBufferSize
     *    GetOutputStream()->BufferSize() == writeBufferSize
     */
    virtual bool SetBufferSize(uint32 readBufferSize,
                               uint32 writeBufferSize);

    /**
     * @brief Writes a single character to the stream.
     * @details if the size of the buffer is zero the character is directly written in the low level RawStream.
     * @param[in] c the character to be written on the stream.
     * @return true if the character is successfully written to the stream.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2]. Justification: The Printf function uses the standard Write(1), but
     * this inline implementation could be faster if the write buffer is not full */
    inline bool PutC(const char8 c);

    /**
     * @brief Reads a single character from the stream.
     * @details if the size of the buffer (see SetBufferSize) is zero the character is directly written to the low level RawStream.
     * @param[out] c the character read from the stream.
     * @return true if the character is successfully read to the stream.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2]. Justification: The Printf function uses the standard Read(1), but
     * this inline implementation could be faster if the read buffer is not empty */
    inline bool GetC(char8 &c);

    /**
     * @see BufferedStream::Read
     * @post
     *   Position() == this'old->Position() + size
     */
    virtual bool Read(char8 * bufferIn,
                      uint32 & size);

    /**
     * @see BufferedStream::Write
     * @post
     *   Position() == this'old->Position() + size
     */
    virtual bool Write(const char8* bufferIn,
                       uint32 & size);

    /**
     * @see BufferedStream::Size
     */
    virtual uint64 Size();

    /**
     * @see BufferedStream::Seek
     */
    virtual bool Seek(uint64 pos);

    /**
     * @see BufferedStream::RelativeSeek
     */
    virtual bool RelativeSeek(int32 deltaPos);

    /**
     * @see BufferedStream::Position
     */
    virtual uint64 Position();

    /**
     * @see BufferedStream::SetSize
     */
    virtual bool SetSize(uint64 size);

protected:
    /**
     * @brief Gets the read buffer.
     * @return the BufferedStreamIOBuffer readBuffer pointer.
     */
    virtual IOBuffer *GetInputBuffer();

    /**
     * @brief Gets the write buffer.
     * @return the BufferedStreamIOBuffer writeBuffer pointer.
     */
    virtual IOBuffer *GetOutputBuffer();

private:

    /**
     * The read buffer. It is used as
     * a middle buffer between the stream and the input.
     */
    BufferedStreamIOBuffer readBuffer;

    /**
     * The write buffer. It is used as
     * a middle buffer between the stream and the output.
     * For each write operation this buffer is filled completely
     * and then the desired size is copied on the output.
     */
    BufferedStreamIOBuffer writeBuffer;

    /**
     * @brief Flushes the write buffer.
     * @return true if the Flush of the buffer succeeds.
     */
    inline bool Flush();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

bool DoubleBufferedStream::Flush() {

    bool ret = true;
    // some data in writeBuffer
    // we can flush in all cases then
    if (writeBuffer.UsedSize() > 0u) {
        ret = writeBuffer.Flush();
    }
    return ret;
}

bool DoubleBufferedStream::PutC(const char8 c) {

    bool ret = false;
    if (writeBuffer.BufferSize() > 0u) {
        ret = writeBuffer.PutC(c);
    }
    else {
        uint32 size = 1u;
        ret = UnbufferedWrite(&c, size);
    }
    return ret;
}

bool DoubleBufferedStream::GetC(char8 &c) {

    bool ret = false;

    if (readBuffer.BufferSize() > 0u) {
        ret = readBuffer.GetC(c);
    }
    else {

        uint32 size = 1u;
        ret = UnbufferedRead(&c, size);
    }
    return ret;
}

}
#endif /* DOUBLEBUFFEREDSTREAM_H_ */

