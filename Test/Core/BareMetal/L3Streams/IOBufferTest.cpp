/**
 * @file IOBufferTest.cpp
 * @brief Source file for class IOBufferTest
 * @date 09/10/2015
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

 * @details This source file contains the definition of all the methods for
 * the class IOBufferTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "IOBufferTest.h"
#include "StringHelper.h"
#include "printf.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool IOBufferTest::TestDefaultConstructor() {
    IOBuffer ioBuffer;
    if (ioBuffer.AmountLeft() != 0) {
        return false;
    }

    if (ioBuffer.UsedAmountLeft() != 0) {
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != 0) {
        return false;
    }

    if (ioBuffer.Buffer() != NULL) {
        return false;
    }

    return ioBuffer.UndoLevel() == 0;
}

bool IOBufferTest::TestFullConstructor(uint32 undoLevel) {
    IOBuffer ioBuffer(33, undoLevel);

    if (ioBuffer.AmountLeft() != 0) {
        return false;
    }

    if (ioBuffer.UsedAmountLeft() != 0) {
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != 0) {
        return false;
    }

    if (ioBuffer.Buffer() != NULL) {
        return false;
    }

    return ioBuffer.UndoLevel() == undoLevel;
}

bool IOBufferTest::TestDestructor() {
    IOBuffer ioBuffer;

    const uint32 size = 32;
    char8 buffer[size];

    ioBuffer.SetBufferReferencedMemory(buffer, size, 0);
    if (ioBuffer.Buffer() == NULL) {
        return false;
    }

    ioBuffer.~IOBuffer();
    return ioBuffer.Buffer() == NULL;
}

bool IOBufferTest::TestBufferSize(uint32 size,
                                  uint32 granularity) {
    IOBuffer ioBuffer(granularity, 0u);

    if (ioBuffer.GetBufferSize() != 0) {
        return false;
    }
    granularity = ioBuffer.AllocationGranularity();

    uint32 addChunk = ((size % granularity) != 0);

    ioBuffer.SetBufferHeapMemory(size, 0u);

    return (ioBuffer.GetBufferSize() == ((size / granularity) + addChunk) * granularity);
}

bool IOBufferTest::TestMaxUsableAmount(uint32 size,
                                       uint32 endSpace,
                                       uint32 granularity) {

    IOBuffer ioBuffer(granularity, 0u);

    if (ioBuffer.GetBufferSize() != 0) {
        return false;
    }
    ioBuffer.SetBufferHeapMemory(size, endSpace);

    size = ioBuffer.GetBufferSize();
    uint32 expected = (endSpace > size) ? (0) : (size - endSpace);

    printf("\nexpected=%d\n", expected);

    return ioBuffer.MaxUsableAmount() == expected;
}

bool IOBufferTest::TestAmountLeft(uint32 size,
                                  uint32 endSpace,
                                  uint32 granularity) {
    IOBuffer ioBuffer(granularity, 0u);

    if (ioBuffer.AmountLeft() != 0) {
        return false;
    }
    ioBuffer.SetBufferHeapMemory(size, endSpace);

    size = ioBuffer.GetBufferSize();
    uint32 expected = (endSpace > size) ? (0) : (size - endSpace);

    printf("\nexpected=%d\n", expected);

    if (ioBuffer.AmountLeft() != expected) {
        return false;
    }

    for (uint32 i = 0; i < expected; i++) {
        ioBuffer.PutC('a');
        expected--;

        if (ioBuffer.AmountLeft() != expected) {
            return false;
        }
    }

    return true;

}

bool IOBufferTest::TestUsedAmountLeft(uint32 size,
                                      uint32 endSpace,
                                      uint32 granularity) {

    IOBuffer ioBuffer(granularity, 0u);

    if (ioBuffer.UsedAmountLeft() != 0) {
        return false;
    }
    ioBuffer.SetBufferHeapMemory(size, endSpace);

    size = ioBuffer.GetBufferSize();
    uint32 expected = (endSpace > size) ? (0) : (size - endSpace);

    printf("\nexpected=%d\n", expected);

    if (ioBuffer.UsedAmountLeft() != 0) {
        return false;
    }

    //the cursor now is always at the end of the filled memory
    for (uint32 i = 0; i < expected; i++) {
        ioBuffer.PutC('a');

        if (ioBuffer.UsedAmountLeft() != 0) {
            return false;
        }
    }

    //return at the beginning, now the buffer is full
    ioBuffer.Seek(0);

    for (uint32 i = 0; i < expected; i++) {
        ioBuffer.PutC('a');
        expected--;

        if (ioBuffer.UsedAmountLeft() != expected) {
            return false;
        }
    }

    return true;
}

bool IOBufferTest::TestBuffer() {
    IOBuffer ioBuffer;

    if (ioBuffer.Buffer() != NULL) {
        return false;
    }

    char8 bufferIn[32];
    StringHelper::Copy(bufferIn, "HelloWorld");
    ioBuffer.SetBufferReferencedMemory(bufferIn, 1u, 0u);

    return (StringHelper::Compare(bufferIn, ioBuffer.Buffer()) == 0);
}

bool IOBufferTest::TestBufferReference() {
    IOBuffer ioBuffer;

    if (ioBuffer.Buffer() != NULL) {
        return false;
    }

    char8 bufferIn[32];
    StringHelper::Copy(bufferIn, "HelloWorld");
    ioBuffer.SetBufferReferencedMemory(bufferIn, 1u, 0u);

    //write access
    char8 *ret = ioBuffer.BufferReference();
    ret[10] = '!';
    ret[11] = '\0';
    return (StringHelper::Compare(ret, "HelloWorld!") == 0);

}

bool IOBufferTest::TestPosition() {
    IOBuffer ioBuffer;

    if (ioBuffer.Position() != 0) {
        return false;
    }

    const uint32 size = 32;
    ioBuffer.SetBufferHeapMemory(size, 0u);

    //the putC change the position
    for (uint32 i = 0; i < size; i++) {
        ioBuffer.PutC('a');

        if (ioBuffer.Position() != (i + 1)) {
            printf("\n1\n");
            return false;
        }
    }

    ioBuffer.Seek(0);

    //the getC change the position
    for (uint32 i = 0; i < size; i++) {
        char c;
        ioBuffer.GetC(c);

        if (ioBuffer.Position() != (i + 1)) {
            printf("\n2\n");
            return false;
        }
    }

    //check if the position is reset after a reallocation
    char8 buffer[size];
    ioBuffer.SetBufferReferencedMemory(buffer, size, 0);
    //the write change the position

    const char8 *toWrite = "HelloWorld";
    uint32 expectedPosition = StringHelper::Length(toWrite);
    ioBuffer.Write(toWrite, expectedPosition);
    if (ioBuffer.Position() != expectedPosition) {
        printf("\n3\n");
        return false;
    }

    ioBuffer.Seek(0);

    //the read change the position

    char8 toRead[32];

    ioBuffer.Read(toRead, expectedPosition);
    if (ioBuffer.Position() != expectedPosition) {
        printf("\n4\n");
        return false;
    }

    //the seek change the position
    ioBuffer.Seek(3);
    if (ioBuffer.Position() != 3) {
        printf("\n5\n");
        return false;
    }

    //the relative seek change the position
    ioBuffer.RelativeSeek(2);
    if (ioBuffer.Position() != 5) {
        printf("\n6 %d\n", ioBuffer.Position());
        return false;
    }

    ioBuffer.Empty();
    return ioBuffer.Position() == 0;

}

bool IOBufferTest::TestUsedSize(uint32 size,
                                uint32 endSpace,
                                uint32 granularity) {
    IOBuffer ioBuffer(granularity, 0u);

    if (ioBuffer.UsedSize() != 0) {
        return false;
    }
    ioBuffer.SetBufferHeapMemory(size, endSpace);

    size = ioBuffer.GetBufferSize();
    uint32 expected = (endSpace > size) ? (0) : (size - endSpace);

    printf("\nexpected=%d\n", expected);

    if (ioBuffer.UsedSize() != 0) {
        return false;
    }

    for (uint32 i = 0; i < expected; i++) {
        ioBuffer.PutC('a');

        if (ioBuffer.UsedSize() != (i + 1)) {
            return false;
        }
    }

    return true;
}

bool IOBufferTest::TestAllocationGranularity(uint32 granularity,
                                             uint32 expected) {

    IOBuffer ioBuffer(granularity, 0u);

    return ioBuffer.AllocationGranularity() == expected;

}

bool IOBufferTest::TestUndoLevel(uint32 undoLevel) {
    IOBuffer ioBuffer(33, undoLevel);

    return ioBuffer.UndoLevel() == undoLevel;

}

bool IOBufferTest::TestCanWrite() {
    IOBuffer ioBuffer;

    const uint32 allocationSize = 32;

    char8 bufferIn[allocationSize];

    ioBuffer.SetBufferReferencedMemory(bufferIn, allocationSize, 0);
    if (!ioBuffer.CanWrite()) {
        return false;
    }

    const char8 *bufferInConst = "";

    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, allocationSize, 0);
    if (ioBuffer.CanWrite()) {
        return false;
    }

    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    return ioBuffer.CanWrite();
}

bool IOBufferTest::TestSetBufferHeapMemory(uint32 size,
                                           uint32 endSpace,
                                           uint32 granularity) {

    const char8* initialMemory = "Untouchable";

    IOBuffer ioBuffer(granularity, 0u);
    ioBuffer.SetBufferReadOnlyReferencedMemory(initialMemory, 10, 0);
    ioBuffer.SetUsedSize(10);
    ioBuffer.Seek(3);

    if ((!ioBuffer.SetBufferHeapMemory(size, endSpace)) && (size != 0)) {
        return false;
    }

    size = ioBuffer.GetBufferSize();
    uint32 expected = (endSpace > size) ? (0) : (size - endSpace);

    printf("\nexpected=%d\n", expected);

    if (ioBuffer.MaxUsableAmount() != expected) {
        return false;
    }

    return (StringHelper::Compare(initialMemory, "Untouchable") == 0) && (ioBuffer.Position() == 0);
}

bool IOBufferTest::TestSetBufferHeapMemoryIncrease() {

    IOBuffer ioBuffer;

    uint32 size = 32;
    uint32 endSpace = 0;
    ioBuffer.SetBufferHeapMemory(size, endSpace);

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n1\n");
        return false;
    }

    char8 buffer[16];
    StringHelper::Copy(buffer, "Hello");
    uint32 fillSize = 16;

    ioBuffer.Write(buffer, fillSize);

    if (ioBuffer.UsedSize() != fillSize) {
        printf("\n1\n");
        return false;
    }
    if (ioBuffer.Position() != fillSize) {
        printf("\n2\n");
        return false;
    }

    //increase the size
    uint32 increase = 32;
    size += increase;

    ioBuffer.SetBufferHeapMemory(size, endSpace);

    if (ioBuffer.UsedSize() != fillSize) {
        printf("\n3\n");
        return false;
    }
    if (ioBuffer.Position() != fillSize) {
        printf("\n4\n");
        return false;
    }

    //increase also the reserved space to clip the used size
    endSpace += (increase + fillSize + 2);

    ioBuffer.SetBufferHeapMemory(size, endSpace);

    uint32 expectedUsedSize = fillSize - 2;
    if (ioBuffer.UsedSize() != expectedUsedSize) {
        printf("\n5 %d\n", ioBuffer.UsedSize());
        return false;
    }
    if (ioBuffer.Position() != expectedUsedSize) {
        printf("\n6\n");
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n1\n");
        return false;
    }

    if (ioBuffer.GetBufferSize() != size) {
        printf("\n1\n");
        return false;
    }

    return (StringHelper::Compare(ioBuffer.Buffer(), "Hello") == 0);
}

bool IOBufferTest::TestSetBufferHeapMemoryDecrease() {

    IOBuffer ioBuffer;

    uint32 size = 32;
    uint32 endSpace = 0;
    ioBuffer.SetBufferHeapMemory(size, endSpace);

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n1\n");
        return false;
    }

    char8 buffer[16];
    StringHelper::Copy(buffer, "Hello");
    uint32 fillSize = 16;

    ioBuffer.Write(buffer, fillSize);

    if (ioBuffer.UsedSize() != fillSize) {
        printf("\n1\n");
        return false;
    }
    if (ioBuffer.Position() != fillSize) {
        printf("\n2\n");
        return false;
    }

    //increase the size
    uint32 clip = 2;
    size = fillSize - clip;

    ioBuffer.SetBufferHeapMemory(size, endSpace);

    if (ioBuffer.UsedSize() != (size - endSpace)) {
        printf("\n3\n");
        return false;
    }
    if (ioBuffer.Position() != (size - endSpace)) {
        printf("\n4\n");
        return false;
    }

    //clip again the used size using the space at end
    clip = 1;
    endSpace += clip;

    ioBuffer.SetBufferHeapMemory(size, endSpace);

    if (ioBuffer.UsedSize() != (size - endSpace)) {
        printf("\n5 %d\n", ioBuffer.UsedSize());
        return false;
    }
    if (ioBuffer.Position() != (size - endSpace)) {
        printf("\n6\n");
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n1\n");
        return false;
    }

    if (ioBuffer.GetBufferSize() != size) {
        printf("\n1\n");
        return false;
    }

    return (StringHelper::Compare(ioBuffer.Buffer(), "Hello") == 0);
}

bool IOBufferTest::TestSetBufferReadOnlyReferencedMemory() {

    const char8* bufferIn = "HelloWorld";
    uint32 size = StringHelper::Length(bufferIn);
    uint32 endSpace = 0;
    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(size, endSpace);
    ioBuffer.PutC('a');

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.GetBufferSize() != size) {
        printf("\n1\n");
        return false;
    }

    if (ioBuffer.Position() != 0) {
        printf("\n2\n");
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n3\n");
        return false;
    }

    if (ioBuffer.UsedSize() != 0) {
        printf("\n4\n");
        return false;
    }

    ioBuffer.SetUsedSize(1);
    //clip by endSpace
    endSpace += 3;

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.GetBufferSize() != size) {
        printf("\n5\n");
        return false;
    }

    if (ioBuffer.Position() != 0) {
        printf("\n6\n");
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n7\n");
        return false;
    }

    if (ioBuffer.UsedSize() != 0) {
        printf("\n8\n");
        return false;
    }

    ioBuffer.SetUsedSize(1);
    //clip by size
    size -= 3;

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.GetBufferSize() != size) {
        return false;
    }

    if (ioBuffer.Position() != 0) {
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        return false;
    }

    if (ioBuffer.UsedSize() != 0) {
        return false;
    }

    return true;
}

bool IOBufferTest::TestSetBufferReferencedMemory() {

    char8 bufferIn[32];
    StringHelper::Copy(bufferIn, "HelloWorld");
    uint32 size = StringHelper::Length(bufferIn);
    uint32 endSpace = 0;
    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(size, endSpace);
    ioBuffer.PutC('a');

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.GetBufferSize() != size) {
        printf("\n1\n");
        return false;
    }

    if (ioBuffer.Position() != 0) {
        printf("\n2\n");
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n3\n");
        return false;
    }

    if (ioBuffer.UsedSize() != 0) {
        printf("\n4\n");
        return false;
    }
    ioBuffer.PutC('a');
    //clip by endSpace
    endSpace += 3;

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.GetBufferSize() != size) {
        printf("\n5\n");
        return false;
    }

    if (ioBuffer.Position() != 0) {
        printf("\n6\n");
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n7\n");
        return false;
    }

    if (ioBuffer.UsedSize() != 0) {
        printf("\n8\n");
        return false;
    }

    ioBuffer.PutC('a');
    //clip by size
    size -= 3;

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.GetBufferSize() != size) {
        return false;
    }

    if (ioBuffer.Position() != 0) {
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        return false;
    }

    if (ioBuffer.UsedSize() != 0) {
        return false;
    }

    return true;
}

bool IOBufferTest::TestResync() {
    IOBuffer ioBuffer;
    return !ioBuffer.Resync();
}

bool IOBufferTest::TestSeek(uint32 usedSize,
                            uint32 seek,
                            bool expected) {

    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(usedSize, 0);

    char8 toWrite[64];
    ioBuffer.Write(toWrite, usedSize);

    if (ioBuffer.Position() != usedSize) {
        return false;
    }

    bool ret = ioBuffer.Seek(seek);
    if (ret) {
        if (ioBuffer.Position() != seek) {
            return false;
        }
    }
    else {
        if (ioBuffer.Position() != usedSize) {
            return false;
        }
    }

    return ret == expected;
}

bool IOBufferTest::TestRelativeSeek(uint32 initialPos,
                                    int32 delta,
                                    bool expected) {
    IOBuffer ioBuffer;

    uint32 usedSize = 2 * initialPos;
    ioBuffer.SetBufferHeapMemory(usedSize, 0);

    char8 toWrite[64];
    ioBuffer.Write((const char8*) toWrite, usedSize);

    ioBuffer.Seek(initialPos);

    if (ioBuffer.Position() != initialPos) {
        return false;
    }

    bool ret = ioBuffer.RelativeSeek(delta);
    if (ret) {
        if (ioBuffer.Position() != (initialPos + delta)) {
            return false;
        }
    }
    else {
        if (delta >= 0) {
            if (ioBuffer.Position() != (uint32) (usedSize)) {
                return false;
            }
        }
        else {
            if (ioBuffer.Position() != 0) {
                return false;
            }
        }
    }

    return ret == expected;
}

bool IOBufferTest::TestPutC_Heap() {
    IOBuffer ioBuffer;

    const uint32 size = 32;
    char bufferTest[size];
    // heap memory
    ioBuffer.SetBufferHeapMemory(size, 0);
    for (uint32 i = 0; i < size; i++) {
        if (!ioBuffer.PutC('a')) {
            printf("\n1\n");
            return false;
        }
        bufferTest[i] = 'a';
    }
    if (StringHelper::CompareN(bufferTest, ioBuffer.Buffer(), size) != 0) {
        printf("\n2\n");
        return false;
    }
    //size is finished
    if (ioBuffer.PutC('a')) {
        printf("\n3\n");
        return false;
    }
    ioBuffer.Empty();
    return ioBuffer.PutC('a');
}

bool IOBufferTest::TestPutC_MemoryReference() {
    IOBuffer ioBuffer;

    const uint32 size = 32;
    char bufferIn[size];
    char bufferTest[size];
    // heap memory
    ioBuffer.SetBufferReferencedMemory(bufferIn, size, 0);
    for (uint32 i = 0; i < size; i++) {
        if (!ioBuffer.PutC('a')) {
            return false;
        }
        bufferTest[i] = 'a';
    }
    if (StringHelper::CompareN(bufferTest, ioBuffer.Buffer(), size) != 0) {
        return false;
    }
    //size is finished
    if (ioBuffer.PutC('a')) {
        return false;
    }
    ioBuffer.Empty();
    return ioBuffer.PutC('a');
}

bool IOBufferTest::TestPutC_MemoryReferenceRO() {

    IOBuffer ioBuffer;

    const char8* bufferIn = "";
    const uint32 size = 32;
    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, 0);
    return !ioBuffer.PutC('a');

}

bool IOBufferTest::TestGetC_Heap() {
    IOBuffer ioBuffer;

    const uint32 size = 32;
    ioBuffer.SetBufferHeapMemory(size, 0);

    char8 ret;
    if (ioBuffer.GetC(ret)) {
        return false;
    }

    const char8 *toWrite = "HelloWorld";
    uint32 usedSize = StringHelper::Length(toWrite);
    ioBuffer.Write(toWrite, usedSize);

    ioBuffer.Seek(0);
    for (uint32 i = 0; i < usedSize; i++) {
        ioBuffer.GetC(ret);
        if (ret != toWrite[i]) {
            return false;
        }
    }

    //finished size
    return !ioBuffer.GetC(ret);
}

bool IOBufferTest::TestGetC_MemoryReference() {
    IOBuffer ioBuffer;

    const uint32 size = 32;
    char8 bufferIn[size];

    ioBuffer.SetBufferReferencedMemory(bufferIn, size, 0);

    char8 ret;
    if (ioBuffer.GetC(ret)) {
        return false;
    }

    const char8 *toWrite = "HelloWorld";
    uint32 usedSize = StringHelper::Length(toWrite);
    ioBuffer.Write(toWrite, usedSize);

    ioBuffer.Seek(0);
    for (uint32 i = 0; i < usedSize; i++) {
        ioBuffer.GetC(ret);
        if (ret != toWrite[i]) {
            return false;
        }
    }

    //finished size
    return !ioBuffer.GetC(ret);
}

bool IOBufferTest::TestGetC_MemoryReferenceRO() {
    IOBuffer ioBuffer;

    const char8 *bufferIn = "HelloWorld";
    const uint32 size = StringHelper::Length(bufferIn);

    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, 0);

    char8 ret;
    if (ioBuffer.GetC(ret)) {
        return false;
    }
    ioBuffer.SetUsedSize(size);

    for (uint32 i = 0; i < size; i++) {
        ioBuffer.GetC(ret);
        if (ret != bufferIn[i]) {
            return false;
        }
    }

    //finished size
    return !ioBuffer.GetC(ret);

}

bool IOBufferTest::TestUnPutC() {

    IOBuffer ioBuffer;

    uint32 size = 32;
    ioBuffer.SetBufferHeapMemory(size, 0);

    if (ioBuffer.UnPutC()) {
        printf("\n1\n");
        return false;
    }

    const char8 *toWrite = "HelloWorld";
    uint32 usedSize = StringHelper::Length(toWrite);
    ioBuffer.Write(toWrite, usedSize);

    for (uint32 i = 0; i < usedSize; i++) {
        if (!ioBuffer.UnPutC()) {
            printf("\n2\n");
            return false;
        }
        if (ioBuffer.UsedSize() != (usedSize - i - 1)) {
            printf("\n3\n");
            return false;
        }
    }
    // position = 0 !
    return !ioBuffer.UnPutC();
}

bool IOBufferTest::TestUnGetC() {

    IOBuffer ioBuffer;

    const char8 *bufferIn = "HelloWorld";
    uint32 size = StringHelper::Length(bufferIn);
    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, 0);

    ioBuffer.SetUsedSize(size);

    char8 toRead[size];

    ioBuffer.Read(toRead, size);
    for (uint32 i = 0; i < size; i++) {
        if (!ioBuffer.UnGetC()) {
            printf("\n2\n");
            return false;
        }
        if (ioBuffer.Position() != (size - i - 1)) {
            printf("\n3\n");
            return false;
        }
    }
    // position = 0 !
    return !ioBuffer.UnGetC();
}

bool IOBufferTest::TestSetUsedSize(uint32 allocatedSize,
                                   uint32 desiredSize) {

    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(allocatedSize, 0);
    ioBuffer.SetUsedSize(desiredSize);

    return (desiredSize > allocatedSize) ? (ioBuffer.UsedSize() == allocatedSize) : (ioBuffer.UsedSize() == desiredSize);
}

bool IOBufferTest::TestWrite_Heap(uint32 allocationSize,
                                  uint32 writeSize,
                                  const char8 *string) {

    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    if (!ioBuffer.Write(string, writeSize)) {
        return false;
    }

    uint32 compareSize = (allocationSize > writeSize) ? (writeSize) : (allocationSize);

    return StringHelper::CompareN(ioBuffer.Buffer(), string, compareSize) == 0;
}

bool IOBufferTest::TestWrite_Memoryreference() {

    IOBuffer ioBuffer;

    const uint32 size = 32;
    char8 bufferIn[size];
    ioBuffer.SetBufferReferencedMemory(bufferIn, size, 0);

    const char8* string = "HelloWorld";
    uint32 writeSize = StringHelper::Length(string);
    if (!ioBuffer.Write(string, writeSize)) {
        return false;
    }

    if (StringHelper::CompareN(ioBuffer.Buffer(), string, writeSize) != 0) {
        return false;
    }

    writeSize -= 5;

    if (!ioBuffer.Write(string, writeSize)) {
        return false;
    }

    return StringHelper::CompareN(ioBuffer.Buffer(), string, writeSize) == 0;
}

bool IOBufferTest::TestWrite_MemoryreferenceRO() {

    IOBuffer ioBuffer;

    const uint32 size = 32;
    const char8 *bufferIn = "HelloWorld";
    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, 0);

    const char8* string = "HelloWorld";
    uint32 writeSize = StringHelper::Length(string);
    return !ioBuffer.Write(string, writeSize);
}

bool IOBufferTest::TestWrite_NULL_Buffer() {
    IOBuffer ioBuffer;

    uint32 allocSize = 32;
    ioBuffer.SetBufferHeapMemory(allocSize, 0);

    const char8 *bufferIn = NULL;
    return !ioBuffer.Write(bufferIn, allocSize);

}

bool IOBufferTest::TestRead_Heap(uint32 allocatedSize,
                                 uint32 readSize) {
    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(allocatedSize, 0);

    for (uint32 i = 0; i < allocatedSize; i++) {
        ioBuffer.PutC('a');
    }

    char8 buffer[32];

    ioBuffer.Read(buffer, readSize);

    if (readSize > allocatedSize) {
        readSize = allocatedSize;
    }

    for (uint32 i = 0; i < readSize; i++) {
        if (buffer[i] != 'a') {
            return false;
        }
    }
    return true;
}

bool IOBufferTest::TestRead_MemoryReference() {
    IOBuffer ioBuffer;

    char8 bufferIn[32];
    uint32 allocatedSize = 32;
    ioBuffer.SetBufferReferencedMemory(bufferIn, allocatedSize, 0);

    for (uint32 i = 0; i < allocatedSize; i++) {
        ioBuffer.PutC('a');
    }

    char8 bufferOut[32];

    // greater size
    uint32 readSize = 34;

    ioBuffer.Seek(0);
    ioBuffer.Read(bufferOut, readSize);

    for (uint32 i = 0; i < allocatedSize; i++) {
        if (bufferOut[i] != 'a') {
            return false;
        }
    }

    //empty the out buffer
    for (uint32 i = 0; i < 32; i++) {
        bufferOut[i] = '\0';
    }

    ioBuffer.Seek(0);
    readSize = 5;
    ioBuffer.Read(bufferOut, readSize);

    return StringHelper::Compare(bufferOut, "aaaaa") == 0;
}

bool IOBufferTest::TestRead_NULL_Buffer() {

    const char8 *bufferIn = "Hello";
    IOBuffer ioBuffer;
    uint32 allocSize = 16;

    ioBuffer.SetBufferHeapMemory(allocSize, 0);

    uint32 writeSize = StringHelper::Length(bufferIn);
    ioBuffer.Write(bufferIn, writeSize);

    ioBuffer.Seek(0);
    char8* bufferOut = NULL;
    return !ioBuffer.Read(bufferOut, allocSize);
}

bool IOBufferTest::TestWriteAll(uint32 allocationSize,
                                uint32 writeSize,
                                const char8* string) {

    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    if (allocationSize > writeSize) {
        if (!ioBuffer.WriteAll(string, writeSize)) {
            return false;
        }
    }
    else {
        if (ioBuffer.WriteAll(string, writeSize)) {
            return false;
        }
        writeSize = allocationSize;
    }

    return StringHelper::CompareN(ioBuffer.Buffer(), string, writeSize) == 0;
}

bool IOBufferTest::TestEmpty() {
    IOBuffer ioBuffer;

    uint32 allocationSize = 32;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    //at the beginnig the buffer is empty
    if (ioBuffer.Position() != 0) {
        return false;
    }

    if ((ioBuffer.AmountLeft() != allocationSize) || (ioBuffer.UsedAmountLeft() != 0)) {
        return false;
    }

    // put something in the buffer
    const char8 *toWrite = "HelloWorld";
    uint32 writeSize = StringHelper::Length(toWrite);
    ioBuffer.Write(toWrite, writeSize);

    if (ioBuffer.Position() != writeSize) {
        return false;
    }
    if (ioBuffer.AmountLeft() != (allocationSize - writeSize)) {
        return false;
    }

    ioBuffer.Seek(0);
    if (ioBuffer.UsedAmountLeft() != writeSize) {
        return false;
    }

    ioBuffer.Seek(writeSize);

    ioBuffer.Empty();
    //the buffer is empty
    if (ioBuffer.Position() != 0) {
        return false;
    }

    if ((ioBuffer.AmountLeft() != allocationSize) || (ioBuffer.UsedAmountLeft() != 0)) {
        return false;
    }

    return true;

}

void Clear(IOBuffer &ioBuffer) {
    ioBuffer.Seek(0);
    for (uint32 i = 0; i < ioBuffer.MaxUsableAmount(); i++) {
        ioBuffer.PutC(0);
    }
    ioBuffer.Seek(0);
}

bool IOBufferTest::TestPrintFormatted(uint32 allocationGranularity,
                                      const PrintfNode testTable[]) {

    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(allocationGranularity, 0);

    uint32 i = 0;
    while (StringHelper::Compare(testTable[i].format, "") != 0) {
        Clear(ioBuffer);

        ioBuffer.PrintFormatted(testTable[i].format, testTable[i].inputs);
        if (StringHelper::Compare(testTable[i].expectedResult, ioBuffer.Buffer()) != 0) {
            AnyType data = testTable[i].inputs[i];
            printf("\n%s %s %d %d\n", ioBuffer.Buffer(), testTable[i].expectedResult, i, *((float*) data.GetDataPointer()));

            return false;
        }
        i++;
    }
    return true;

}

bool IOBufferTest::TestPrintFormatted_CCString() {

    IOBuffer ioBuffer;
    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    uint32 i = 0;
    while (printfCStringTable[i][0] != NULL) {
        Clear(ioBuffer);
        AnyType toPrint = printfCStringTable[i][1];
        if (ioBuffer.PrintFormatted(printfCStringTable[i][0], &toPrint)) {
            if (StringHelper::Compare(ioBuffer.Buffer(), printfCStringTable[i][2]) != 0) {
                printf("\n%s %s %d\n", ioBuffer.Buffer(), printfCStringTable[i][2], i);
                return false;
            }
        }
        else {
            return printfCStringTable[i][1] == NULL;
        }
        i++;
    }

    return true;

}

bool IOBufferTest::TestPrintFormatted_Pointer() {

    IOBuffer ioBuffer1;
    IOBuffer ioBuffer2;

    uint32 allocationSize = 64;
    ioBuffer1.SetBufferHeapMemory(allocationSize, 0);
    ioBuffer2.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer1);
    Clear(ioBuffer2);
    //return the pointer thanks to hex notation.

    const char *charPointer = "Hello";
    AnyType toPrintChar = charPointer;
    uint64 UIntPointer = (uint64) charPointer;
    AnyType toPrintUInt = UIntPointer;
    ioBuffer1.PrintFormatted("%x", &toPrintChar);
    ioBuffer2.PrintFormatted("%x", &toPrintUInt);

    if (StringHelper::Compare(ioBuffer1.Buffer(), ioBuffer2.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer1);
    Clear(ioBuffer2);
    //%p format as the complete 32 bit pointer with header

    void* pointer = (void*) charPointer;
    AnyType toPrintPointer = pointer;

    ioBuffer1.PrintFormatted("%p", &toPrintPointer);
    ioBuffer2.PrintFormatted("% #0x", &toPrintUInt);

    if (StringHelper::Compare(ioBuffer1.Buffer(), ioBuffer2.Buffer()) != 0) {
        return false;
    }

    return true;
}

bool IOBufferTest::TestPrintFormatted_Stream() {

    IOBuffer ioBuffer;
    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    DummySingleBufferedStream stream;
    stream.SetBufferSize(32);

    uint32 i = 0;
    while (printfCStringTable[i][0] != NULL) {
        Clear(ioBuffer);
        stream.Clear();
        const char8 * toWrite = printfCStringTable[i][1];

        uint32 writeSize = StringHelper::Length(toWrite);
        stream.Write(toWrite, writeSize);
        stream.FlushAndResync();
        stream.Seek(0);

        AnyType toPrint = stream;
        ioBuffer.PrintFormatted(printfCStringTable[i][0], &toPrint);
        if (StringHelper::Compare(ioBuffer.Buffer(), printfCStringTable[i][2]) != 0) {
            printf("\n%s %s %d\n", ioBuffer.Buffer(), printfCStringTable[i][2], i);
            return false;
        }
        stream.FlushAndResync();
        i++;
    }

    return true;

}

bool IOBufferTest::TestPrintFormatted_TooBigStream() {

    IOBuffer ioBuffer;
    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    DummySingleBufferedStream stream;
    stream.SetBufferSize(32);

    stream.SetFakeSize(10001);

    AnyType toPrint = stream;
    printf("\n%d\n", stream.Size());
    ioBuffer.PrintFormatted("%s", &toPrint);

    return StringHelper::Compare(ioBuffer.Buffer(), "!! too big > 10000 characters!!") == 0;
}

bool IOBufferTest::TestPrintFormatted_Stream_NotSeekable() {

    IOBuffer ioBuffer;
    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    DummySingleBufferedStream stream(false);
    stream.SetBufferSize(32);

    AnyType toPrint = stream;
    ioBuffer.PrintFormatted("%s", &toPrint);
    printf("\n%s\n", ioBuffer.Buffer());

    return StringHelper::Compare(ioBuffer.Buffer(), "!!stream !seek!!") == 0;
}

bool IOBufferTest::TestPrintFormatted_BitSet_Unsigned() {

    uint64 data[5] = { 0x13579BDF02468ACE, 0x13579BDF02468ACE, 0x123456789ABCDEF0, 0xDEADBABEBAB00111 };
    const char streamString[] = "DEADBABEBAB00111123456789ABCDEF013579BDF02468ACE13579BDF02468ACE";
    int32 sizeStr = 63;
    uint32 dataBitSize = 256;
    IOBuffer ioBuffer;
    uint32 allocationSize = 256;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer);

    uint32* source = (uint32*) data;

    //from size =4 to size = 64
    for (uint32 size = 4; size < 32; size += 4) {
        int32 beg = 0;
        int32 end = 0;

        for (uint32 myShift = 0; myShift < dataBitSize; myShift += size) {
            //source and shift are automatically modified by the function.
            TypeDescriptor td(false, UnsignedInteger, size);
            AnyType toPrint(td, myShift, source);

            ioBuffer.PrintFormatted("%0x", &toPrint);
            char buffer[128];

            end = sizeStr - myShift / 4;
            beg = (end - (size / 4)) + 1;
            StringHelper::Substr(beg, end, streamString, buffer);
            //  printf("\n|%s| |%s|\n", buffer, ioBuffer.Buffer());

            if (StringHelper::Compare(buffer, ioBuffer.Buffer()) != 0) {
                printf("\n%d %d\n", myShift, size);
                return false;
            }
            Clear(ioBuffer);

            //Avoids to print shit. (it remains less than size)
            if ((dataBitSize - myShift) < (2 * size)) {
                break;
            }
        }
    }

    return true;

}

bool IOBufferTest::TestPrintFormatted_BitSet_Signed() {

    int64 data[5] = { 0x13579BDF02468ACE, 0x13579BDF02468ACE, 0x123456789ABCDEF0, 0xDEADBABEBAB00111 };
    const char streamString[] = "DEADBABEBAB00111123456789ABCDEF013579BDF02468ACE13579BDF02468ACE";
    int32 sizeStr = 63;
    uint32 dataBitSize = 256;
    IOBuffer ioBuffer;
    uint32 allocationSize = 256;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer);

    int32* source = (int32*) data;

    //from size =4 to size = 64
    for (uint32 size = 4; size < 32; size += 4) {
        int32 beg = 0;
        int32 end = 0;

        for (uint32 myShift = 0; myShift < dataBitSize; myShift += size) {
            //source and shift are automatically modified by the function.
            TypeDescriptor td(false, SignedInteger, size);
            AnyType toPrint(td, myShift, source);

            ioBuffer.PrintFormatted("%0x", &toPrint);
            char buffer[128];

            end = sizeStr - myShift / 4;
            beg = (end - (size / 4)) + 1;
            StringHelper::Substr(beg, end, streamString, buffer);

            //the number is negative
            if (buffer[0] > ('0' + 7)) {
                uint8 numberSize = 2;
                uint32 index = 0;
                char8 prefix[32];
                while (numberSize < (size / 4)) {
                    numberSize *= 2;
                }
                for (uint32 k = index; k < (numberSize - (size / 4)); k++) {
                    prefix[k] = 'F';
                    index++;
                }
                prefix[index] = '\0';
                char result[32];
                StringHelper::Concatenate(prefix, buffer, result);
                StringHelper::Copy(buffer, result);
            }

            //     printf("\n|%s| |%s|\n", buffer, ioBuffer.Buffer());

            if (StringHelper::Compare(buffer, ioBuffer.Buffer()) != 0) {
                printf("\n%d %d\n", myShift, size);
                return false;
            }
            Clear(ioBuffer);

            //Avoids to print shit. (it remains less than size)
            if ((dataBitSize - myShift) < (2 * size)) {
                break;
            }
        }
    }

    return true;
}

bool IOBufferTest::TestGetToken_ConstCharOutput(const TokenTestTableRow *table) {

    IOBuffer ioBuffer;

    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    uint32 i = 0u;
    const TokenTestTableRow *row = &table[i];
    bool result = true;

    while (result && (row->toTokenize != NULL)) {
        Clear(ioBuffer);
        uint32 inputSize = StringHelper::Length(row->toTokenize) + 1;
        ioBuffer.Write(row->toTokenize, inputSize);
        ioBuffer.Seek(0);
        const uint32 bufferSize = 32;
        char buffer[bufferSize];
        char saveTerminator;
        uint32 t = 0u;

        while (ioBuffer.GetToken(buffer, row->terminators, bufferSize, saveTerminator, row->skipCharacters)) {
            if (StringHelper::Compare(buffer, row->expectedResult[t]) != 0) {
                result = false;
            }
            if (row->saveTerminatorResult[t] != saveTerminator) {
                //When it gets to the end of the string the terminator is \0
                if (saveTerminator != '\0') {
                    result = false;
                }
            }
            t++;
        }

        row = &table[++i];

    }
    return result;

}

bool IOBufferTest::GetToken_ConstCharOutput_ClipSize() {

    IOBuffer ioBuffer;

    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);
    const char8 * input = "HelloWorl.d";
    uint32 inputSize = StringHelper::Length(input) + 1;
    ioBuffer.Write(input, inputSize);
    ioBuffer.Seek(0);

    const uint32 outSize = 8;
    char8 outBuffer[outSize];
    char8 saveTerminator;
    ioBuffer.GetToken(outBuffer, ".", outSize, saveTerminator, "");
    return StringHelper::Compare(outBuffer, "HelloWo") == 0;
}

bool IOBufferTest::TestGetToken_IOBufferOutput(const TokenTestTableRow *table) {
    IOBuffer ioBuffer;

    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    IOBuffer outBuffer;
    outBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(outBuffer);

    uint32 i = 0u;
    const TokenTestTableRow *row = &table[i];
    bool result = true;

    while (result && (row->toTokenize != NULL)) {
        Clear(ioBuffer);
        uint32 inputSize = StringHelper::Length(row->toTokenize) + 1;
        ioBuffer.Write(row->toTokenize, inputSize);
        ioBuffer.Seek(0);

        char saveTerminator;
        uint32 t = 0u;

        while (ioBuffer.GetToken(outBuffer, row->terminators, saveTerminator, row->skipCharacters)) {

            if (StringHelper::Compare(outBuffer.Buffer(), row->expectedResult[t]) != 0) {
                printf("\n%s %s %d\n", outBuffer.Buffer(), row->expectedResult[t], t);
                result = false;
            }
            if (row->saveTerminatorResult[t] != saveTerminator) {
                //When it gets to the end of the string the terminator is \0
                if (saveTerminator != '\0') {
                    result = false;
                }
            }
            Clear(outBuffer);

            t++;
        }

        row = &table[++i];

    }
    return result;

}

bool IOBufferTest::TestSkipToken(const SkipTokensTestTableRow *table) {

    IOBuffer ioBuffer;

    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    uint32 i = 0u;
    const SkipTokensTestTableRow *row = &table[i];
    bool result = true;

    while (result && (row->toTokenize != NULL)) {
        Clear(ioBuffer);
        uint32 inputSize = StringHelper::Length(row->toTokenize) + 1;
        ioBuffer.Write(row->toTokenize, inputSize);
        ioBuffer.Seek(0);
        ioBuffer.SkipTokens(row->nOfSkipTokens, row->terminators);

        const uint32 bufferSize = 32;
        char buffer[bufferSize];
        uint32 t = 0u;
        while (row->expectedResult[t] != NULL) {
            char saveTerminator;
            ioBuffer.GetToken(buffer, row->terminators, bufferSize, saveTerminator, NULL);
            if (StringHelper::Compare(buffer, row->expectedResult[t]) != 0) {
                result = false;
            }
            t++;
        }
        row = &table[++i];

    }
    return result;

}
