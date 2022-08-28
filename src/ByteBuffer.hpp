
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

#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include <stdint.h>
#include <exception>
#include <sstream>




extern "C" namespace BYTEBUFFER{

        enum ByteOrder{
        //determines the byte order of the buffer
                B_ENDIAN = 1,
                ENDIAN_L
        };


/*Class for containing the ByteBuffer data, contains ability to decode data into
unsigned integer types and maintain internal positioning
*/
        class ByteBuffer{
                public:
                        ByteBuffer(uint64_t size, ByteOrder order);
                        ByteBuffer(void);
                        //ByteBuffer(const ByteBuffer &buff);
                        ~ByteBuffer(void);

                        void rewind(void);
                        uint64_t remaining(void);
                        uint64_t getLimit(void);
                        uint64_t getCapacity(void);
                        void setLimit(uint64_t lim);
                        uint64_t getPosition(void);
                        void setPosition(uint64_t newPos);
                        void clear(void);
                        void put(uint64_t pos, const uint8_t *src, uint64_t len);
                        void put(const uint8_t *src, uint64_t len);

                        uint8_t getUint_8(void);
                        uint8_t getUint_8(uint64_t pos);
                        uint16_t getUint_16(void);
                        uint16_t getUint_16(uint64_t pos);
                        uint32_t getUint_32(void);
                        uint32_t getUint_32(uint64_t pos);
                        uint64_t getUint_64(void);
                        uint64_t getUint_64(uint64_t pos);

                        void putUint_8(uint8_t value);
                        void putUint_16(uint16_t value);
                        void putUint_32(uint32_t value);
                        void putUint_64(uint64_t value);
                        void put(BYTEBUFFER::ByteBuffer *src);

                        const uint8_t* getData(void);
                private:
                        uint64_t position;
                        uint64_t limit;
                        uint64_t capacity;
                        //uint64_t mark;
                        uint8_t *data = nullptr;
                        enum ByteOrder order;




        };

/*Exceptions*/
        class Bad_Position : std::exception{
                private:
                        uint64_t position;
                        uint64_t size;

                public:
                        Bad_Position(uint64_t pos, uint64_t siz){position = pos; size = siz;}
                        const char* what() const noexcept {
                                std::stringstream ss;
                                ss << "Position " << position << " is bigger than size "
                                << size;
                                return ss.str().c_str();
                        }
        };


        class Bad_Limit : std::exception{
                private:
                        uint64_t limit;
                        uint64_t capacity;

                public:
                        Bad_Limit(uint64_t lim, uint64_t cap){limit = lim; capacity = cap;}
                        const char* what() const noexcept {
                                std::stringstream ss;
                                ss << "New limit " << limit << " is bigger than capacity "
                                << capacity;
                                return ss.str().c_str();
                        }
        };


        class Buffer_Underflow : std::exception{
                private:
                        uint64_t length;
                        uint64_t remaining;

                public:
                        Buffer_Underflow(uint64_t len, uint64_t rem){length = len; remaining = rem;}
                        const char* what() const noexcept {
                                std::stringstream ss;
                                ss << "Length " << length << " is bigger than remaining "
                                << remaining;
                                return ss.str().c_str();
                        }
        };



        class Buffer_Overflow : std::exception{
                private:
                        uint64_t length;
                        uint64_t remaining;

                public:
                        Buffer_Overflow(uint64_t srcRem, uint64_t destRem){length = srcRem; remaining = destRem;}
                        const char* what() const noexcept {
                                std::stringstream ss;
                                ss << "Source length " << length << " is bigger than destination remaining "
                                << remaining;
                                return ss.str().c_str();
                        }
        };

        class IndexOutOfBounds : std::exception{
                private:
                        uint64_t index;
                        uint64_t length;

                public:
                        IndexOutOfBounds(uint64_t idx, uint64_t len){index = idx; length = len;}
                        const char* what() const noexcept {
                                std::stringstream ss;
                                ss << "Index " << index << " out of bounds in length "
                                << length;
                                return ss.str().c_str();
                        }
        };

}


#endif // BYTEBUFFER_H
