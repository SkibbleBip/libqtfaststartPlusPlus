/**
    ByteBuffer implementation
    Copyright (C) 2022  SkibbleBip
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
**/

/***************************************************************************
* File:  ByteBuffer.cpp
* Author:  SkibbleBip
* Procedures:
* BYTEBUFFER::ByteBuffer::ByteBuffer    -Overloaded Constructor
* BYTEBUFFER::ByteBuffer::ByteBuffer    -Default constructor
* BYTEBUFFER::ByteBuffer::~ByteBuffer   -Destructor
* BYTEBUFFER::ByteBuffer::rewind        -Resets the position of the buffer back to 0
* BYTEBUFFER::ByteBuffer::put   -Put a byte array from an offset in the array of specific length into the bytebuffer
* BYTEBUFFER::ByteBuffer::put   -copy a byte array of specified length into the byte buffer
* BYTEBUFFER::ByteBuffer::put   -BYTEBUFFER::ByteBuffer::put
* BYTEBUFFER::ByteBuffer::remaining     -Returns the remaining number of bytes in the bytebuffer
* BYTEBUFFER::ByteBuffer::getLimit      -Returns limit of the bytebuffer
* BYTEBUFFER::ByteBuffer::setLimit      -Sets the limit of the bytebuffer
* BYTEBUFFER::ByteBuffer::getPosition   -returns the position of the bytebuffer
* BYTEBUFFER::ByteBuffer::setPosition   -sets the position of the bytebuffer
* uintX_t BYTEBUFFER::ByteBuffer::getUint_X(void / uint64_t pos)        -Obtain the current specified bytes as an unsigned integer-type, either from the current position or the requested position in the bytebuffer
* void BYTEBUFFER::ByteBuffer::putUint_X(uintX_t value) -put an unsigned integer-type into the bytebuffer at the specified position
* BYTEBUFFER::ByteBuffer::clear -sets the position of the bytebuffer to 0, sets the limit to the capacity of the bytebuffer
* BYTEBUFFER::ByteBuffer::getData       -returns the byte array retained in the buffer
* BYTEBUFFER::ByteBuffer::getCapacity   -returns the capacity of the bytebuffer
***************************************************************************/


#include "ByteBuffer.hpp"
#include <string.h>
#include <endian.h>

#include <stdio.h>

//#define DEBUG

/***************************************************************************
* BYTEBUFFER::ByteBuffer::ByteBuffer(uint64_t size, BYTEBUFFER::ByteOrder order)
* Author: SkibbleBip
* Date: 07/22/2022
* Description: Overloaded Constructor
*
* Parameters:
*        size   I/P     uint64_t        limit and capacity of bytebuffer initialized
*        order  I/P     BYTEBUFFER::ByteOrder   Byte order of the buffer
*                                                       (little endian, big endian)
**************************************************************************/
BYTEBUFFER::ByteBuffer::ByteBuffer(uint64_t size, BYTEBUFFER::ByteOrder order)
{
        //delete[] this->data;
        this->position = 0;
        this->limit = size;
        this->capacity = size;
        this->data = new uint8_t[size];
        this->order = order;
        //printf("Size: %d\n", size);
        //printf("address: %d\n", this->data);
}

/***************************************************************************
* BYTEBUFFER::ByteBuffer::ByteBuffer(void)
* Author: SkibbleBip
* Date: 07/22/2022
* Description: Default constructor
*
* Parameters:
**************************************************************************/
BYTEBUFFER::ByteBuffer::ByteBuffer(void)
{
        //delete[] this->data;
        this->position = 0;
        this->limit = 0;
        this->capacity = 0;
        this->data = nullptr;
}


/***************************************************************************
* BYTEBUFFER::ByteBuffer::~ByteBuffer(void)
* Author: SkibbleBip
* Date: 07/22/2022
* Description: Destructor
*
* Parameters:
**************************************************************************/
BYTEBUFFER::ByteBuffer::~ByteBuffer(void)
{
        delete[] this->data;
        this->position = 0;
        this->limit = 0;
        this->capacity = 0;
        this->data = nullptr;
        this->order = BYTEBUFFER::B_ENDIAN;
}

/***************************************************************************
* void BYTEBUFFER::ByteBuffer::rewind(void)
* Author: SkibbleBip
* Date: 07/22/2022
* Description: Resets the position of the buffer back to 0
*
* Parameters:
**************************************************************************/
void BYTEBUFFER::ByteBuffer::rewind(void)
{
        this->position = 0;
}

/***************************************************************************
* void BYTEBUFFER::ByteBuffer::put(uint64_t offset, const uint8_t *src, uint64_t len)
* Author: SkibbleBip
* Date: 07/22/2022
* Description: Put a byte array from an offset in the array of specific length
*               into the bytebuffer
*
* Parameters:
*        offset I/P     uint64_t        offset of the source array
*        src    I/P     const uint8_t*  source array to copy into the buffer
*        len    I/P     uint64_t        length of bytes to copy from source
**************************************************************************/
void BYTEBUFFER::ByteBuffer::put(uint64_t offset, const uint8_t *src, uint64_t len)
{
#ifdef DEBUG
        printf("offset: %lu len: %lu buffsize: %lu\n", offset, len, this->getCapacity());
#endif // DEBUG
        if(this->remaining() < len)
                throw BYTEBUFFER::Buffer_Underflow(len, this->remaining());
        if(offset > len)
                throw BYTEBUFFER::IndexOutOfBounds(offset, len);

        memcpy(&this->data[this->position], &src[offset], len - offset);
        this->position += len;

}


/***************************************************************************
* void BYTEBUFFER::ByteBuffer::put(const uint8_t *src, uint64_t len)
* Author: SkibbleBip
* Date: 07/22/2022
* Description: copy a byte array of specified length into the byte buffer
*
* Parameters:
*        src    I/P     const uint8_t*  source array
*        len    I/P     uint64_t        length of array to copy
**************************************************************************/
void BYTEBUFFER::ByteBuffer::put(const uint8_t *src, uint64_t len)
{
        this->put(0, src, len);
}

/***************************************************************************
* void BYTEBUFFER::ByteBuffer::put(BYTEBUFFER::ByteBuffer *src)
* Author: SkibbleBip
* Date: 07/22/2022
* Description: Puts a source bytebuffer into this buffer
*
* Parameters:
*        src    I/P     BYTEBUFFER::ByteBuffer*  the source bytebuffer
**************************************************************************/
void BYTEBUFFER::ByteBuffer::put(BYTEBUFFER::ByteBuffer *src)
{
        if(src->remaining() > this->remaining())
                throw BYTEBUFFER::Buffer_Overflow(src->remaining(), this->remaining());
        const uint8_t* tmp = src->getData();
        uint64_t rem = src->remaining();

        this->put(&tmp[src->getPosition()], rem);
        src->setPosition(rem);
}

/***************************************************************************
* uint64_t BYTEBUFFER::ByteBuffer::remaining(void)
* Author: SkibbleBip
* Date: 07/22/2022
* Description: Returns the remaining number of bytes in the bytebuffer
*
* Parameters:
*        remaining      O/P     uint64_t        number of bytes that remain
**************************************************************************/
uint64_t BYTEBUFFER::ByteBuffer::remaining(void)
{
        return this->limit - position;
}

/***************************************************************************
* uint64_t BYTEBUFFER::ByteBuffer::getLimit(void)
* Author: SkibbleBip
* Date: 07/22/2022
* Description: Returns limit of the bytebuffer
*
* Parameters:
*        getLimit       O/P     uint64_t        the limit of the bytebuffer
**************************************************************************/
uint64_t BYTEBUFFER::ByteBuffer::getLimit(void)
{
        return this->limit;
}

/***************************************************************************
* void BYTEBUFFER::ByteBuffer::setLimit(uint64_t lim)
* Author: SkibbleBip
* Date: 07/22/2022
* Description: Sets the limit of the bytebuffer
*
* Parameters:
*        lim    I/P     uint64_t        limit to set
**************************************************************************/
void BYTEBUFFER::ByteBuffer::setLimit(uint64_t lim)
{
        if(lim > this->capacity)
                throw BYTEBUFFER::Bad_Limit(lim, this->capacity);
        this->limit = lim;
}

/***************************************************************************
* uint64_t BYTEBUFFER::ByteBuffer::getPosition(void)
* Author: SkibbleBip
* Date: 07/22/2022
* Description: returns the position of the bytebuffer
*
* Parameters:
*        getPosition    O/P     uint64_t        returns the position of the bytebuffer
**************************************************************************/
uint64_t BYTEBUFFER::ByteBuffer::getPosition(void)
{
        return this->position;
}


/***************************************************************************
* void BYTEBUFFER::ByteBuffer::setPosition(uint64_t newPos)
* Author: SkibbleBip
* Date: 07/25/2022
* Description: sets the position of the bytebuffer
*
* Parameters:
*        newPos I/P     uint64_t        the new position
**************************************************************************/
void BYTEBUFFER::ByteBuffer::setPosition(uint64_t newPos)
{
        if(newPos > this->limit)
                throw BYTEBUFFER::Bad_Position(newPos, this->limit);
        this->position = newPos;
}



/***************************************************************************
* uintX_t BYTEBUFFER::ByteBuffer::getUint_X(void / uint64_t pos)
* Author: SkibbleBip
* Date: 07/25/2022
* Description: Obtain the current specified bytes as an unsigned integer-type,
*               either from the current position or the requested position in
*               the bytebuffer
*
* Parameters:
*        pos    I/P     uint64_t        position to obtain the bytes
*        getUintX_t        O/P     uintX_t returned interger-type
**************************************************************************/
uint8_t BYTEBUFFER::ByteBuffer::getUint_8(void)
{
        if(sizeof(uint8_t) > this->limit - this->position)
                throw BYTEBUFFER::Buffer_Underflow(sizeof(uint8_t), limit - position);
        uint8_t ret;
        memcpy(&ret, &data[this->position], sizeof(uint8_t));
        this->position+=sizeof(uint8_t);
        return ret;
}

uint16_t BYTEBUFFER::ByteBuffer::getUint_16(void)
{
        if(sizeof(uint16_t) > this->limit - this->position)
                throw BYTEBUFFER::Buffer_Underflow(sizeof(uint16_t), limit - position);;
        uint16_t ret;
        memcpy(&ret, &data[this->position], sizeof(uint16_t));
        this->position+=sizeof(uint16_t);

        if(this->order == BYTEBUFFER::B_ENDIAN)
                return be16toh(ret);
        return le16toh(ret);
}

uint16_t BYTEBUFFER::ByteBuffer::getUint_16(uint64_t pos)
{
        if(sizeof(uint16_t) > this->limit - pos)
                throw BYTEBUFFER::Buffer_Underflow(sizeof(uint16_t), limit - position);;
        uint16_t ret;
        memcpy(&ret, &data[pos], sizeof(uint16_t));

        if(this->order == BYTEBUFFER::B_ENDIAN)
                return be16toh(ret);
        return le16toh(ret);
}

uint32_t BYTEBUFFER::ByteBuffer::getUint_32(void)
{
        if(sizeof(uint32_t) > this->limit - this->position)
                throw BYTEBUFFER::Buffer_Underflow(sizeof(uint32_t), limit - position);;
        uint32_t ret;
        memcpy(&ret, &data[this->position], sizeof(uint32_t));
        this->position+=sizeof(uint32_t);

        if(this->order == BYTEBUFFER::B_ENDIAN)
                return be32toh(ret);
        return le32toh(ret);
}

uint32_t BYTEBUFFER::ByteBuffer::getUint_32(uint64_t pos)
{
        if(sizeof(uint32_t) > this->limit - pos)
                throw BYTEBUFFER::Buffer_Underflow(sizeof(uint32_t), limit - position);
        uint32_t ret;
        memcpy(&ret,  &data[pos], sizeof(uint32_t));

        if(this->order == BYTEBUFFER::B_ENDIAN)
                return be32toh(ret);
        return le32toh(ret);
}

uint64_t BYTEBUFFER::ByteBuffer::getUint_64(void)
{
        if(sizeof(uint64_t) > this->limit - this->position)
                throw BYTEBUFFER::Buffer_Underflow(sizeof(uint64_t), limit - position);
        uint64_t ret;
        memcpy(&ret, &data[this->position], sizeof(uint64_t));
        this->position+=sizeof(uint64_t);

        if(this->order == BYTEBUFFER::B_ENDIAN)
                return be64toh(ret);
        return le64toh(ret);
}

uint64_t BYTEBUFFER::ByteBuffer::getUint_64(uint64_t pos)
{
        if(sizeof(uint64_t) > this->limit - pos)
                throw BYTEBUFFER::Buffer_Underflow(sizeof(uint64_t), limit - position);
        uint64_t ret;
        memcpy(&ret,  &data[pos], sizeof(uint64_t));
        if(this->order == BYTEBUFFER::B_ENDIAN)
                return be64toh(ret);
        return le64toh(ret);
}
/******************************************************************************
        End of get functions
*******************************************************************************/

/***************************************************************************
* void BYTEBUFFER::ByteBuffer::putUint_X(uintX_t value)
* Author: SkibbleBip
* Date: 07/25/2022
* Description: put an unsigned integer-type into the bytebuffer at the specified position
*
* Parameters:
*        value  I/P     uintX_t the unsigned integer-type to insert into the bytebuffer
**************************************************************************/
void BYTEBUFFER::ByteBuffer::putUint_8(uint8_t value)
{
        if(sizeof(uint8_t) > this->limit - this->position)
                throw BYTEBUFFER::Buffer_Overflow(sizeof(uint8_t), limit - position);

        memcpy(&data[this->position], &value, sizeof(uint8_t));
        this->position += sizeof(uint8_t);
}

void BYTEBUFFER::ByteBuffer::putUint_16(uint16_t value)
{
        if(sizeof(uint16_t) > this->limit - this->position)
                throw BYTEBUFFER::Buffer_Overflow(sizeof(uint16_t), limit - position);

        uint16_t out;
        if(this->order == BYTEBUFFER::B_ENDIAN)
                out = htobe16(value);
        else
                out = htole16(value);
        memcpy(&data[this->position], &out, sizeof(uint16_t));
        this->position += sizeof(uint16_t);
}

void BYTEBUFFER::ByteBuffer::putUint_32(uint32_t value)
{
        if(sizeof(uint32_t) > this->limit - this->position)
                throw BYTEBUFFER::Buffer_Overflow(sizeof(uint32_t), limit - position);

        uint32_t out;
        if(this->order == BYTEBUFFER::B_ENDIAN)
                out = htobe32(value);
        else
                out = htole32(value);
        memcpy(&data[this->position], &out, sizeof(uint32_t));
        this->position += sizeof(uint32_t);
}

void BYTEBUFFER::ByteBuffer::putUint_64(uint64_t value)
{
        if(sizeof(uint64_t) > this->limit - this->position)
                throw BYTEBUFFER::Buffer_Overflow(sizeof(uint64_t), limit - position);

        uint64_t out;
        if(this->order == BYTEBUFFER::B_ENDIAN)
                out = htobe64(value);
        else
                out = htole64(value);
        memcpy(&data[this->position], &out, sizeof(uint64_t));
        this->position += sizeof(uint64_t);
}
/******************************************************************************
        End of put functions
*******************************************************************************/

/***************************************************************************
* void BYTEBUFFER::ByteBuffer::clear(void)
* Author: SkibbleBip
* Date: 07/25/2022
* Description: sets the position of the bytebuffer to 0, sets the limit to the
*               capacity of the bytebuffer
*
* Parameters:
**************************************************************************/
void BYTEBUFFER::ByteBuffer::clear(void)
{
        this->position = 0;
        this->limit = this->capacity;
}

/***************************************************************************
* const uint8_t* BYTEBUFFER::ByteBuffer::getData(void)
* Author: SkibbleBip
* Date: 07/26/2022
* Description: returns the byte array retained in the buffer
*
* Parameters:
*        getData        O/P     const uint8_t*  const pointer to the returned data
**************************************************************************/
const uint8_t* BYTEBUFFER::ByteBuffer::getData(void)
{
        return this->data;
}

/***************************************************************************
* uint64_t BYTEBUFFER::ByteBuffer::getCapacity(void)
* Author: SkibbleBip
* Date: 07/26/2022
* Description: returns the capacity of the bytebuffer
*
* Parameters:
*        getCapacity    O/P     uint64_t        returned capacity
**************************************************************************/
uint64_t BYTEBUFFER::ByteBuffer::getCapacity(void)
{
        return this->capacity;
}



