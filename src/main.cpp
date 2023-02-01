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

/***************************************************************************
* File:  main.cpp
* Author:  SkibbleBip
* Procedures:
* QtFastStartSTD::readAndFill   -Overloaded function for reading from artificial file stream and fully fill a bytebuffer
* QtFastStartSTD::readAndFill   -Overloader function to read from artificial file stream into a bytebuffer at specified position in the stream
* QtFastStartSTD::QtFastStart::fastStart        -Returns an artificial file stream that contains the brand new fast-start converted mp4
* QtFastStartSTD::QtFastStart::QtFastStart      -Constructor, takes in byte array and length of byte array as params
* QtFastStartSTD::QtFastStart::~QtFastStart     -Destructor
* QtFastStartSTD::QtFastStart::fastStartImpl    -performs the implementation of converting the mp4 file into a faststart mp4
***************************************************************************/



//#define DEBUG
#define ATOM_PREAMBLE_SIZE 8

#ifdef DEBUG
#include <iostream>
#endif // DEBUG
//for debugging only


#include "QtFastStartCPP.hpp"
#include "ArtificialFS.hpp"



/***************************************************************************
* uint64_t QtFastStartSTD::readAndFill(QtFastStartSTD::ArtificialFileStream *infile, BYTEBUFFER::ByteBuffer *buffer)
* Author: SkibbleBip
* Date: 07/31/2022
* Description: Overloaded function for reading from artificial file stream and fully fill a bytebuffer
*
* Parameters:
*        infile I/O     QtFastStartSTD::ArtificialFileStream*   artificial filestream to read from
*        buffer I/O     BYTEBUFFER::ByteBuffer* bytebuffer to read into
*        QtFastStartSTD::readAndFill    O/P     uint64_t        number of bytes read
**************************************************************************/
        uint64_t QtFastStartSTD::readAndFill(
                                QtFastStartSTD::ArtificialFileStream *infile,
                                BYTEBUFFER::ByteBuffer *buffer)
        {
                buffer->clear();
                uint64_t r = infile->read(buffer);
                buffer->setLimit(buffer->getPosition());
                buffer->rewind();
                return r;

        }

/***************************************************************************
* uint64_t QtFastStartSTD::readAndFill(QtFastStartSTD::ArtificialFileStream *infile, BYTEBUFFER::ByteBuffer *buffer, uint64_t pos)
* Author: SkibbleBip
* Date: 07/31/2022
* Description: Overloader function to read from artificial file stream into a bytebuffer at specified position in the stream
*
* Parameters:
*        infile I/O     QtFastStartSTD::ArtificialFileStream *  artificial filestream to read from
*        buffer I/O     BYTEBUFFER::ByteBuffer *        bytebuffer to read into
*        pos    I/P     uint64_t        position to read from the stream
*        QtFastStartSTD::readAndFill    O/P     uint64_t        number of bytes read
**************************************************************************/
        uint64_t QtFastStartSTD::readAndFill(
                                        QtFastStartSTD::ArtificialFileStream *infile,
                                        BYTEBUFFER::ByteBuffer *buffer,
                                        uint64_t pos)
        {
                buffer->clear();
                uint64_t r = infile->read(pos, buffer);
                buffer->setLimit(buffer->getPosition());
                buffer->rewind();
                return r;
        }


/***************************************************************************
* QtFastStartSTD::ArtificialFileStream QtFastStartSTD::QtFastStart::fastStart(void)
* Author: SkibbleBip
* Date: 08/02/2022
* Description: Returns an artificial file stream that contains the brand new fast-start converted mp4
*
* Parameters:
*        QtFastStartSTD::QtFastStart::fastStart O/P     QtFastStartSTD::QtFastStart::fastStart  Output artificial filestream
**************************************************************************/
        QtFastStartSTD::ArtificialFileStream QtFastStartSTD::QtFastStart::fastStart(void)
        {
                QtFastStartSTD::ArtificialFileStream afs = *this->outFile;
                return afs;

        }

/***************************************************************************
* QtFastStartSTD::QtFastStart::QtFastStart(byte* in, uint64_t len)
* Author: SkibbleBip
* Date: 08/02/2022
* Description: Constructor, takes in byte array and length of byte array as params
*
* Parameters:
*        in     I/P     byte*   input byte array of input file
*        len    I/P     uint64_t        length of array
**************************************************************************/
        QtFastStartSTD::QtFastStart::QtFastStart(byte* in, uint64_t len)
        {
                this->inFile = new QtFastStartSTD::ArtificialFileStream(in, len);
                this->outFile = new QtFastStartSTD::ArtificialFileStream();
                QtFastStartSTD::QtFastStart::fastStartImpl();

        }

/***************************************************************************
* QtFastStartSTD::QtFastStart::~QtFastStart(void)
* Author: SkibbleBip
* Date: 08/02/2022
* Description: Destructor
*
* Parameters:
**************************************************************************/
        QtFastStartSTD::QtFastStart::~QtFastStart(void)
        {
                delete this->ftypAtom;
                delete this->moovAtom;
                delete this->inFile;
                delete this->outFile;
        }


/***************************************************************************
* void QtFastStartSTD::QtFastStart::fastStartImpl(void)
* Author: SkibbleBip
* Date: 08/02/2022
* Description: performs the implementation of converting the mp4 file into a faststart mp4
*
* Parameters:
**************************************************************************/
        void QtFastStartSTD::QtFastStart::fastStartImpl(void)
        {

                BYTEBUFFER::ByteBuffer atomBytes = BYTEBUFFER::ByteBuffer(ATOM_PREAMBLE_SIZE, BYTEBUFFER::B_ENDIAN);
                uint32_t atomType = 0;
                uint64_t atomSize = 0;
                uint64_t lastOffset;
                uint32_t moovAtomSize;
                uint64_t startOffset = 0;


                uint64_t orig = ATOM_PREAMBLE_SIZE;
                readAndFill(inFile, &atomBytes);
                while(orig == atomBytes.getLimit()){
                        atomSize = (uint32_t)atomBytes.getUint_32();
                        atomType = htobe32(atomBytes.getUint_32());
                        //hack for converting it into a usuable format


                        if(atomType == FTYP_ATOM){

                                uint32_t ftypAtomSize = atomSize;
                                ftypAtom = new BYTEBUFFER::ByteBuffer(ftypAtomSize, BYTEBUFFER::B_ENDIAN);
                                atomBytes.rewind();
                                ftypAtom->put(&atomBytes);

                                inFile->read(ftypAtom);

                                if(ftypAtom->getCapacity() < ftypAtomSize - ATOM_PREAMBLE_SIZE)
                                        break;
                                startOffset = inFile->getPosition();

                        }
                        else{
                                if(atomSize == 1){
                                        atomBytes.clear();
                                        readAndFill(inFile, &atomBytes);
                                        if(orig != atomBytes.getCapacity())
                                                break;
                                        atomSize = atomBytes.getUint_64();
                                        inFile->setPosition(inFile->getPosition() + atomSize - ATOM_PREAMBLE_SIZE * 2);

                                }
                                else{
                                        inFile->setPosition(inFile->getPosition() + atomSize - ATOM_PREAMBLE_SIZE);
                                }
                        }
                        if ((atomType != FREE_ATOM)
                                && (atomType != JUNK_ATOM)
                                && (atomType != MDAT_ATOM)
                                && (atomType != MOOV_ATOM)
                                && (atomType != PNOT_ATOM)
                                && (atomType != SKIP_ATOM)
                                && (atomType != WIDE_ATOM)
                                && (atomType != PICT_ATOM)
                                && (atomType != UUID_ATOM)
                                && (atomType != FTYP_ATOM)) {
#ifdef DEBUG
                                std::cerr << "encountered non-QT top-level atom (is this a QuickTime file?)" << std::endl;
#endif // DEBUG
                                break;
                        }
                         /* The atom header is 8 (or 16 bytes), if the atom size (which
                        * includes these 8 or 16 bytes) is less than that, we won't be
                        * able to continue scanning sensibly after this atom, so break. */
                        if (atomSize < 8)
                                break;
                        readAndFill(inFile, &atomBytes);

                }//while

                if(atomType != MOOV_ATOM){
#ifdef DEBUG
                        std::cerr << "Last atom in file was not moov atom" << std::endl;
#endif // DEBUG
                        inFile->setPosition(0);
                        inFile->transferTo(0, inFile->size(), outFile);
                        this->data = outFile->getByteArray();
                        return;
                }

                // moov atom was, in fact, the last atom in the chunk; load the whole moov atom

                moovAtomSize = atomSize;
                lastOffset = inFile->size() - moovAtomSize; // NOTE: assuming no extra data after moov, as qt-faststart.c
                moovAtom =  new BYTEBUFFER::ByteBuffer(moovAtomSize, BYTEBUFFER::B_ENDIAN);

                orig = moovAtom->getCapacity();
                readAndFill(inFile, moovAtom, lastOffset);
                if(orig != moovAtom->getCapacity()){
                        throw Malformed_Atom("Failed to read moov atom\n");
                }
                if(moovAtom->getUint_32(12) == CMOV_ATOM){
                        throw Compressed_Moov();
                }

                while(moovAtom->remaining() >= 8){
                        uint64_t atomHead = moovAtom->getPosition();
                        atomType = htobe32(moovAtom->getUint_32(atomHead + 4));
                        //printf("%.5s\n", &atomType);
                        if(!(atomType == STCO_ATOM || atomType == CO64_ATOM)){
                                moovAtom->setPosition(moovAtom->getPosition()+1);
                                continue;
                        }
                        atomSize = moovAtom->getUint_32(atomHead);
                        if(atomSize > moovAtom->remaining()){
                                throw Bad_Atom_Size();
                        }
                        moovAtom->setPosition(atomHead + 12); // skip size (4 bytes), type (4 bytes), version (1 byte) and flags (3 bytes)
                        if(moovAtom->remaining() < 4){
                                throw Malformed_Atom("Malformed atom\n");
                        }
                        // uint32_t, but assuming moovAtomSize is in int32 range, so this will be in int32 range
                        uint32_t offsetCount = moovAtom->getUint_32();
                        if (atomType == STCO_ATOM) {
#ifdef DEBUG
                                std::cout << "patching stco atom..." << std::endl;
#endif // DEBUG

                                if(moovAtom->remaining() < offsetCount * 4){
                                        throw Malformed_Atom("Bad atom size/element count\n");
                                }

                                for(uint32_t i = 0; i < offsetCount; i++){
                                        uint32_t currentOffset = moovAtom->getUint_32(moovAtom->getPosition());
                                        uint32_t newOffset = currentOffset + moovAtomSize;

                                        /*if (currentOffset < 0 && newOffset >= 0) {
#ifdef DEBUG                            //no clue if I even need this.
                                                std::cerr << "This is bug in original qt-faststart.c: "
                                                << "stco atom should be extended to co64 atom as new offset value overflows uint32, "
                                                << "but is not implemented." << std::endl;
#endif // DEBUG
                                                throw QT_BUG;
                                        }*/
                                        moovAtom->putUint_32(newOffset);

                                }

                        }
                        else if(atomType == CO64_ATOM){
#ifdef DEBUG
                                std::cout << "patching co64 atom..." << std::endl;
#endif // DEBUG

                                if(moovAtom->remaining() < offsetCount * 8){
                                        throw Malformed_Atom("Bad atom size/element count\n");
                                }
                                for(uint32_t i = 0; i < offsetCount; i++){
                                        uint64_t currentOffset = moovAtom->getUint_64(moovAtom->getPosition());
                                        moovAtom->putUint_64(currentOffset + moovAtomSize);
                                }
                        }

                }
                inFile->setPosition(startOffset);
                // seek after ftyp atom

                if(ftypAtom->getCapacity() != 0){
#ifdef DEBUG
                        std::cout << "writing ftyp atom..." << std::endl;
#endif // DEBUG
                        ftypAtom->rewind();
                        outFile->write(ftypAtom);
                }

                //dump new moov atom
#ifdef DEBUG
                std::cout << "writing moov atom..." << std::endl;
#endif // DEBUG
                moovAtom->rewind();
                outFile->write(moovAtom);


#ifdef DEBUG
                std::cout << "copying rest of file..." << std::endl;
#endif // DEBUG

                inFile->transferTo(startOffset, lastOffset - startOffset, outFile);

                this->data = outFile->getByteArray();
        }//end function





