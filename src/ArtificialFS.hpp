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

#ifndef ARTIFICIALFS_H
#define ARTIFICIALFS_H


#include <stdint.h>
#include <string.h>
#include <limits.h>
#include "ByteBuffer.hpp"
#include <exception>      // std::exception
#include <sstream>


typedef         uint8_t         byte;

extern "C" namespace QtFastStartSTD{

        class ArtificialFileStream{
                private:
                        uint64_t position;
                        uint64_t totalSize;
                        byte* data = NULL;

                public:
                        static uint32_t getSize(byte* in);
                        static uint32_t getType(byte* in);

                        ArtificialFileStream(byte* in, uint64_t len);
                        ArtificialFileStream(const ArtificialFileStream& afs);
                        ArtificialFileStream(void);
                        ~ArtificialFileStream(void);

                        uint64_t getPosition(void);
                        uint64_t size(void);
                        const byte* getByteArray(void);
                        void /*ArtificialFileStream*/ setPosition(uint64_t newPos);
                        uint64_t read(byte* dest, uint64_t len);
                        uint64_t read(uint64_t pos, byte* dest, uint64_t len);
                        uint64_t read(BYTEBUFFER::ByteBuffer *buff);
                        uint64_t read(uint64_t pos, BYTEBUFFER::ByteBuffer *buff);
                        uint64_t write(const byte* src, uint64_t len);
                        uint64_t write(uint64_t pos, const byte* src, uint64_t len);
                        uint64_t write(BYTEBUFFER::ByteBuffer *buff);
                        uint64_t write(uint64_t pos, BYTEBUFFER::ByteBuffer *buff);

                        uint64_t transferTo(uint64_t pos, uint64_t count, ArtificialFileStream *target);




        };

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

        class Alloc_Fail : std::exception{
                public:
                        Alloc_Fail(){}
                        const char* what() const noexcept{ return "Failed to malloc/realloc data";}
        };


}







#endif
