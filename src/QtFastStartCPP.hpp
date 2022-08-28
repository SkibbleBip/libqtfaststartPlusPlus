/**
    MOV QT FastStart Implementation library
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

#ifndef MAIN_H
#define MAIN_H


#include <inttypes.h>
#include <string.h>
#include <limits.h>
#include "ByteBuffer.hpp"
#include "ArtificialFS.hpp"


#define         FREE_ATOM       1701147238
#define         JUNK_ATOM       1802401130
#define         MDAT_ATOM       1952539757
#define         MOOV_ATOM       1987014509
#define         PNOT_ATOM       1953459824
#define         SKIP_ATOM       1885956979
#define         WIDE_ATOM       1701079415
#define         PICT_ATOM       1413695824
#define         FTYP_ATOM       1887007846
#define         UUID_ATOM       1684632949

#define         CMOV_ATOM       1987013987
#define         STCO_ATOM       1868788851
#define         CO64_ATOM       875982691


namespace QtFastStartSTD{



        class QtFastStart{
                private:
                        const byte* data;
                        uint64_t data_len;
                        BYTEBUFFER::ByteBuffer *ftypAtom = nullptr;
                        BYTEBUFFER::ByteBuffer *moovAtom = nullptr;
                        void fastStartImpl(void);

                        QtFastStartSTD::ArtificialFileStream *inFile;
                        QtFastStartSTD::ArtificialFileStream *outFile;

                public:
                        explicit QtFastStart(byte* in = NULL, uint64_t len = 0);
                        QtFastStartSTD::ArtificialFileStream fastStart(void);
                        ~QtFastStart(void);


        };


        class Compressed_Moov : std::exception{
                public:
                        const char* what(void) const noexcept{return "This utility does not support compressed moov atoms yet\n";}
        };

        class Bad_Atom_Size : std::exception{
                public:
                        const char* what(void) const noexcept{return "Bad atom size\n";}
        };

        class Malformed_Atom : std::exception{
                private:
                        const char* res = NULL;

                public:
                        Malformed_Atom(const char* msg){this->res = msg;}
                        const char* what(void) const noexcept{return this->res;}
        };


        uint64_t readAndFill(ArtificialFileStream *infile, BYTEBUFFER::ByteBuffer *buffer);
        uint64_t readAndFill(ArtificialFileStream *infile, BYTEBUFFER::ByteBuffer *buffer, uint64_t pos);



}


#endif // MAIN_H
