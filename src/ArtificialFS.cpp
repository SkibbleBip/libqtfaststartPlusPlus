/**
    Artificial File Stream Implementation
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
* File:  ArtificialFS.cpp
* Author:  SkibbleBip
* Procedures:
* QtFastStartSTD::ArtificialFileStream::getSize -get the size in bytes of the QT atom size segment passed to the parameter
* QtFastStartSTD::ArtificialFileStream::getType -Get the type of the bytes of the QT atom type segment passed to the parameter
* QtFastStartSTD::ArtificialFileStream::~ArtificialFileStream   -Default destructor
* QtFastStartSTD::ArtificialFileStream::ArtificialFileStream    -Default constructor
* QtFastStartSTD::ArtificialFileStream::ArtificialFileStream    -Overloaded constructor, takes in array of bytes of the file and it's length
* QtFastStartSTD::ArtificialFileStream::getPosition     -returns the position of the artificial file stream
* QtFastStartSTD::ArtificialFileStream::size    -returns size of the artificial filestream
* QtFastStartSTD::ArtificialFileStream::getByteArray    -returns pointer to internal byte array
* QtFastStartSTD::ArtificialFileStream::setPosition     -sets the position of the artificial file stream
* QtFastStartSTD::ArtificialFileStream::read    -read and fill an input byte array of len size, increases position by number of bytes read.
* QtFastStartSTD::ArtificialFileStream::read    -Read into input byte array of len size, at specified position
* QtFastStartSTD::ArtificialFileStream::read    -read into an input ByteBuffer pointer
* QtFastStartSTD::ArtificialFileStream::write   -Writes byte array of specified length to the file stream. Overwrites any occupied data
* QtFastStartSTD::ArtificialFileStream::write   -Writes byte array of specified length to the file stream, at specified position in stream. Overwrites any occupied data
* QtFastStartSTD::ArtificialFileStream::write   -write bytebuffer to the artificial file stream
* QtFastStartSTD::ArtificialFileStream::write   -write a bytebuffer to the file stream at the specified position in the stream
* QtFastStartSTD::ArtificialFileStream::transferTo      -transfers data from this artificial file stream to an inputted stream
* QtFastStartSTD::ArtificialFileStream::ArtificialFileStream    -Copy constructor
***************************************************************************/


#include "ArtificialFS.hpp"
#include <stdlib.h>
#include <stdio.h>


#ifdef __unix__
#include <endian.h>
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
/*Definitions of endian byte swapping for windows*/
const int16_t __num = 1;
const bool __littleEndian = (*(int8_t *)&__num == 1 ? true : false);


    #if defined(__GNUC__ )
    /*For GCC*/
    #define be16toh(X) (__littleEndian ? ((X & 0xff00) >> 8) + ((X & 0x00ff) << 8) : X)
    #define le16toh(X) (__littleEndian ? X : ((X & 0xff00) >> 8) + ((X & 0x00ff) << 8))
    #define be32toh(X) (__littleEndian ? __builtin_bswap32(X) : X)
    #define le32toh(X) (__littleEndian ? X : __builtin_bswap32(X))
    #define be64toh(X) (__littleEndian ? __builtin_bswap64(X) : X)
    #define le64toh(X) (__littleEndian ? X : __builtin_bswap64(X))


    #elif defined(_MSC_VER)
    /*For MSVC*/
    #define be16toh(X) (__littleEndian ? _byteswap_ushort(X) : X)
    #define le16toh(X) (__littleEndian ? X : _byteswap_ushort(X))
    #define be32toh(X) (__littleEndian ? _byteswap_ulong(X) : X)
    #define le32toh(X) (__littleEndian ? X : _byteswap_ulong(X))
    #define be64toh(X) (__littleEndian ? _byteswap_uint64(X) : X)
    #define le64toh(X) (__littleEndian ? X : _byteswap_uint64(X))

    #endif // defined

    #define htobe16(X) be16toh(X)
    #define htole16(X) le16toh(X)
    #define htobe32(X) be32toh(X)
    #define htole32(X) le32toh(X)
    #define htobe64(X) be64toh(X)
    #define htole64(X) le64toh(X)



#else
        #error Operating system not supported
#endif // linux

extern "C"
{
/***************************************************************************
* uint32_t QtFastStartSTD::ArtificialFileStream::getSize(byte* in)
* Author: SkibbleBip
* Date: 07/26/2022
* Description: get the size in bytes of the QT atom size segment passed to the parameter
*
* Parameters:
*        in     I/P     byte*   input file array of atom
*        getSize        O/P     uint32_t        returned size in bytes as a uint32_t
**************************************************************************/
        uint32_t QtFastStartSTD::ArtificialFileStream::getSize(byte* in)
        {
                uint32_t q;
                memcpy(&q, &in[0], sizeof(q));
                return be32toh(q);

        }
/***************************************************************************
* uint32_t QtFastStartSTD::ArtificialFileStream::getType(byte* in)
* Author: SkibbleBip
* Date: 07/26/2022
* Description: Get the type of the bytes of the QT atom type segment passed to the parameter
*
* Parameters:
*        in     I/P     byte*   input array of atom
*        getType        O/P     uint32_t        returned type in uint32_t format
**************************************************************************/
        uint32_t QtFastStartSTD::ArtificialFileStream::getType(byte* in)
        {
                uint32_t q;
                memcpy(&q, &in[4], sizeof(q));
                return q;
        }

/***************************************************************************
* QtFastStartSTD::ArtificialFileStream::~ArtificialFileStream(void)
* Author: SkibbleBip
* Date: 07/26/2022
* Description: Default destructor
*
* Parameters:
**************************************************************************/
        QtFastStartSTD::ArtificialFileStream::~ArtificialFileStream(void)
        {
                free(this->data);
        }

/***************************************************************************
* QtFastStartSTD::ArtificialFileStream::ArtificialFileStream(void)
* Author: SkibbleBip
* Date: 07/26/2022
* Description: Default constructor
*
* Parameters:
**************************************************************************/
        QtFastStartSTD::ArtificialFileStream::ArtificialFileStream(void)
        {
                this->data = NULL;
                this->position = 0;
                this->totalSize = 0;
        }
/***************************************************************************
* QtFastStartSTD::ArtificialFileStream::ArtificialFileStream(byte* in, uint64_t len)
* Author: SkibbleBip
* Date: 07/26/2022
* Description: Overloaded constructor, takes in array of bytes of the file and it's length
*
* Parameters:
*        in     I/P     byte*   input byte array of file
*        len    I/P     uint64_t        length of input file
**************************************************************************/
        QtFastStartSTD::ArtificialFileStream::ArtificialFileStream(byte* in, uint64_t len)
        {
                this->data = (byte*)malloc(len);
                memcpy(this->data, in, len);
                this->totalSize = len;
                this->position = 0;
        }
/***************************************************************************
* uint64_t QtFastStartSTD::ArtificialFileStream::getPosition(void)
* Author: SkibbleBip
* Date: 08/02/2022
* Description: returns the position of the artificial file stream
*
* Parameters:
*        getPosition      O/P     uint64_t       position the file stream is currently at
**************************************************************************/
        uint64_t QtFastStartSTD::ArtificialFileStream::getPosition(void)
        {
                return this->position;
        }
/***************************************************************************
* uint64_t QtFastStartSTD::ArtificialFileStream::size(void)
* Author: SkibbleBip
* Date: 08/02/2022
* Description: returns size of the artificial filestream
*
* Parameters:
*        size   O/P     uint64_t        size of the file stream
**************************************************************************/
        uint64_t QtFastStartSTD::ArtificialFileStream::size(void)
        {
                return this->totalSize;
        }
/***************************************************************************
* const byte* QtFastStartSTD::ArtificialFileStream::getByteArray(void)
* Author: SkibbleBip
* Date: 08/02/2022
* Description: returns pointer to internal byte array
*
* Parameters:
*        getByteArray   O/P     const byte*     byte array pointer
**************************************************************************/
        const byte* QtFastStartSTD::ArtificialFileStream::getByteArray(void)
        {
                return this->data;
        }
/***************************************************************************
* void QtFastStartSTD::ArtificialFileStream::setPosition(uint64_t newPos)
* Author: SkibbleBip
* Date: 08/02/2022
* Description: sets the position of the artificial file stream
*
* Parameters:
*        newPos   I/P     uint64_t     new position
**************************************************************************/
        void QtFastStartSTD::ArtificialFileStream::setPosition(uint64_t newPos)
        {
                this->position = newPos;
                //return *this;
        }
/***************************************************************************
* uint64_t QtFastStartSTD::ArtificialFileStream::read(byte* dest, uint64_t len)
* Author: SkibbleBip
* Date: 08/05/2022
* Description: read and fill an input byte array of len size, increases position by number
*       of bytes read.
*
* Parameters:
*        dest   I/O     byte*   pointer to byte array
*        len    I/P     uint64_t        number of bytes to read up to. May not read full number if less
*                                               bytes remain in the artificial file stream
*        read   O/P     uint64_t        number of bytes actually read to the input array
**************************************************************************/
        uint64_t QtFastStartSTD::ArtificialFileStream::read(byte* dest, uint64_t len)
        {
                uint32_t q;

                if(this->totalSize - this->position < len)
                        q = this->totalSize - this->position;
                else
                        q = len; ///TODO: VERIFY THIS

                memcpy(dest, &this->data[this->position], q);
                this->position += q;
                return q;
        }
/***************************************************************************
* uint64_t QtFastStartSTD::ArtificialFileStream::read(uint64_t pos, byte* dest, uint64_t len)
* Author: SkibbleBip
* Date: 08/05/2022
* Description: Read into input byte array of len size, at specified position
*
* Parameters:
*        pos    I/P     uint64_t        position in file stream to start reading from
*        dest   I/O     byte*   pointer to input byte array to read to
*        len    I/P     uint64_t        len of input array. May read less if less bytes in file stream remain
*        read   O/P     uint64_t        number of bytes actually read
**************************************************************************/
        uint64_t QtFastStartSTD::ArtificialFileStream::read(uint64_t pos, byte* dest, uint64_t len)
        {
                uint64_t q;
                if(this->totalSize < pos){
                        throw Bad_Position(pos, this->totalSize);
                }
                if(this->totalSize - pos < len)
                        q = this->totalSize = pos;
                else
                        q = len; ///TODO: VERIFY THIS
                memcpy(dest, &this->data[position], q);
                this->position = pos + q;
                return q;

        }
/***************************************************************************
* uint64_t QtFastStartSTD::ArtificialFileStream::read(BYTEBUFFER::ByteBuffer *buff)
* Author: SKibbleBip
* Date: 08/05/2022
* Description: read into an input ByteBuffer pointer
*
* Parameters:
*        buff   I/O     BYTEBUFFER::ByteBuffer*  input bytebuffer to read to
*        read   O/P     uint64_t        number of bytes read. May not be equal
*                                       to size of bytebuffer, depending on the
*                                       number of remaining bytes in the file stream
**************************************************************************/
        uint64_t QtFastStartSTD::ArtificialFileStream::read(BYTEBUFFER::ByteBuffer *buff)
        {
                uint64_t q;
                if(this->totalSize - this->position < buff->getCapacity())
                        q = this->totalSize - this->position;
                else
                        q = buff->remaining(); ///TODO: VERIFY THIS
                buff->put(&this->data[this->position], q);
                this->position += q;
                return q;
        }
/***************************************************************************
* uint64_t QtFastStartSTD::ArtificialFileStream::read(BYTEBUFFER::ByteBuffer *buff)
* Author: SKibbleBip
* Date: 08/05/2022
* Description: read into an input ByteBuffer pointer
*
* Parameters:
*        buff   I/O     BYTEBUFFER::ByteBuffer*  input bytebuffer to read to
*        read   O/P     uint64_t        number of bytes read. May not be equal
*                                       to size of bytebuffer, depending on the
*                                       number of remaining bytes in the file stream
**************************************************************************/
        uint64_t QtFastStartSTD::ArtificialFileStream::read(uint64_t pos, BYTEBUFFER::ByteBuffer *buff)
        {
                uint64_t q;
                if(this->totalSize < pos){
                        throw Bad_Position(pos, this->totalSize);
                }

                if(this->totalSize - pos < buff->getCapacity())
                        q = this->totalSize - pos;
                else
                        q = buff->remaining();

                buff->put(&this->data[pos], q);
                this->position += q;
                return q;
        }
/***************************************************************************
* uint64_t QtFastStartSTD::ArtificialFileStream::write(const byte* src, uint64_t len)
* Author: SkibbleBip
* Date: 08/05/2022
* Description: Writes byte array of specified length to the file stream. Overwrites
*               any occupied data
*
* Parameters:
*        src    I/P     const byte*     source array to write
*        len    I/P     uint64_t        length of bytes to write
*        write  O/P     uint64_t        number of bytes actually written, may be
*                                       less than len depending how much bytes
*                                       left in the file stream
**************************************************************************/
        uint64_t QtFastStartSTD::ArtificialFileStream::write(const byte* src, uint64_t len)
        {
                byte* tmp = (byte*)realloc(this->data, this->totalSize + len);
                if(!tmp){
                        throw Alloc_Fail();
                }
                this->data = tmp;
                memcpy(&this->data[this->totalSize], src, len);
                this->totalSize += len;
                return len;
        }
/***************************************************************************
* uint64_t QtFastStartSTD::ArtificialFileStream::write(uint64_t pos, const byte* src, uint64_t len)
* Author: SkibbleBip
* Date: 08/05/2022
* Description: Writes byte array of specified length to the file stream, at
*               specified position in stream. Overwrites any occupied data
*
*
* Parameters:
*        pos    I/P     uint64_t        position in file stream to write to
*        src    I/P     const byte*     source array to write
*        len    I/P     uint64_t        length of bytes to write
*        write  O/P     uint64_t        number of bytes actually written, may be
*                                       less than len depending how much bytes
*                                       left in the file stream
**************************************************************************/
        uint64_t QtFastStartSTD::ArtificialFileStream::write(uint64_t pos, const byte* src, uint64_t len)
        {
                if(pos > this->totalSize){
                        byte* tmp = (byte*)realloc(this->data, pos);
                        if(!tmp){
                                throw Alloc_Fail();
                        }
                        this->data = tmp;
                        memcpy(&this->data[totalSize], src, len);
                        return len;
                }

                if(pos + len > this->totalSize){
                        byte* tmp = (byte*)realloc(this->data, pos + len);
                        if(!tmp){
                                throw Alloc_Fail();
                        }
                        this->data = tmp;

                }
                memcpy(&this->data[position], src, len);
                return len;

        }
/***************************************************************************
* uint64_t QtFastStartSTD::ArtificialFileStream::write(BYTEBUFFER::ByteBuffer *buff)
* Author: SKibbleBip
* Date: 08/05/2022
* Description: write bytebuffer to the artificial file stream
*
* Parameters:
*        buff   I/P     BYTEBUFFER::ByteBuffer* input byte buffer to write
*        write  O/P     uint64_t        number of bytes written
**************************************************************************/
        uint64_t QtFastStartSTD::ArtificialFileStream::write(BYTEBUFFER::ByteBuffer *buff)
        {
                byte* tmp = (byte*)realloc(this->data, this->totalSize + buff->getCapacity());
                if(!tmp){
                        throw Alloc_Fail();
                }
                this->data = tmp;

                memcpy(&this->data[totalSize], buff->getData(), buff->getCapacity());
                this->totalSize+=buff->getCapacity();
                return buff->getCapacity();
        }
/***************************************************************************
* uint64_t QtFastStartSTD::ArtificialFileStream::write(uint64_t pos, BYTEBUFFER::ByteBuffer *buff)
* Author: SkibbleBip
* Date: 08/06/2022
* Description: write a bytebuffer to the file stream at the specified position in the stream
*
* Parameters:
*        pos    I/P     uint64_t        position to write to
*        buff   I/P     BYTEBUFFER::ByteBuffer* bytebuffer to write
*        write  O/P     uint64_t        number of bytes written
**************************************************************************/
        uint64_t QtFastStartSTD::ArtificialFileStream::write(uint64_t pos, BYTEBUFFER::ByteBuffer *buff)
        {
                if(pos > this->totalSize){
                        byte* tmp = (byte*)realloc(this->data, pos);
                        if(!tmp){
                                throw Alloc_Fail();
                        }
                        this->data = tmp;

                        memcpy(&(this->data[pos - buff->getCapacity()]), buff->getData(), buff->getCapacity());
                        this->totalSize = pos;
                        return buff->getCapacity();
                }

                if(pos + buff->getCapacity() > this->totalSize){
                        byte* tmp = (byte*)realloc(this->data, pos + buff->getCapacity());
                        if(!tmp){
                                throw Alloc_Fail();
                        }
                        this->data = tmp;
                        memcpy(&(this->data[pos]), buff->getData(), buff->getCapacity());
                        this->totalSize = pos + buff->getCapacity();
                        return buff->getCapacity();
                }

                memcpy(&(this->data[pos]), buff->getData(), buff->getCapacity());
                return buff->getCapacity();

        }
/***************************************************************************
* uint64_t QtFastStartSTD::ArtificialFileStream::transferTo(uint64_t pos, uint64_t count, ArtificialFileStream *target)
* Author: SkibbleBip
* Date: 08/06/2022
* Description: transfers data from this artificial file stream to an inputted stream
*
* Parameters:
*        pos    I/P     uint64_t        position in this stream to begin transfering from
*        count  I/P     uint64_t        number of bytes to transfer
*        target I/O     ArtificialFileStream*   The stream to write to
*        transferTo     O/P     uint64_t        number of bytes written
**************************************************************************/
        uint64_t QtFastStartSTD::ArtificialFileStream::transferTo(uint64_t pos, uint64_t count, ArtificialFileStream *target)
        {
                byte* tmp = (byte*)malloc(count);
                if(!tmp){
                        throw Alloc_Fail();
                }
                uint64_t q = read(pos, tmp, count);
                target->write(tmp, count);
                free(tmp);
                return q;
        }
/***************************************************************************
* QtFastStartSTD::ArtificialFileStream::ArtificialFileStream(const QtFastStartSTD::ArtificialFileStream& afs)
* Author: SkibbleBip
* Date: 08/06/2022
* Description: Copy constructor
*
* Parameters:
*        afs    I/P     QtFastStartSTD::Arconst QtFastStartSTD::ArtificialFileStream&   Source file stream
**************************************************************************/
        QtFastStartSTD::ArtificialFileStream::ArtificialFileStream(const QtFastStartSTD::ArtificialFileStream& afs)
        {
                this->position = 0;
                this->totalSize = afs.totalSize;
                this->data = (byte*)malloc(this->totalSize);
                if(!this->data)
                        throw Alloc_Fail();
                memcpy(this->data, afs.data, this->totalSize);
        }


}


