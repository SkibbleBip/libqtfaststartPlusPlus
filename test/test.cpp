#include <iostream>
#include <string>
#include <getopt.h>

#include "QtFastStartCPP.hpp"

/***************************************************************************
* int main(int argc, char* argv[])
* Author: SkibbleBip
* Date: 08/21/2022
* Description: main function. Application takes in an optional input and output file names and reads and writes them.
*
* Parameters:
*        argc   I/P     int     number of arguments
*        argv   I/P     char* []        arguments passed (first element in name of application)
*        main   O/P     int     exit code. returns 0 on success, -1 for memory allocation and 1 for file formatting errors
**************************************************************************/
int main(int argc, char* argv[])
{

        FILE *input = NULL, *output = NULL;
        bool quiet = false;
        std::string inStr, outStr;
        int returnValue = 0;

        struct option long_options[] = {
                {"input",     required_argument, NULL, 'i'},
                {"output",    required_argument, NULL, 'o'},
                {"help",      no_argument,       NULL, 'h'},
                {"quiet",     no_argument,       NULL, 'q'},
                {NULL,      0,                   NULL, 0}

        };


        int ch;
        bool _exit = false;
        while( (ch = getopt_long(argc, argv, "i:o:hq", long_options, NULL)) != -1){
                switch(ch){
                        case 'i':{
                                input = fopen(optarg, "rb");
                                inStr = optarg;
                                break;
                        }
                        case 'o':{
                                output = fopen(optarg, "wb");
                                outStr = optarg;
                                break;
                        }
                        case 'q':{
                                quiet = true;
                                break;
                        }
                        case 'h':
                        default:
                        case ':':
                        case '?':{
                                _exit = true;
                                break;
                        }

                }
        }

        if(_exit){
                std::cerr << "Usage: " << argv[0] << " [--input -i ] INPUTFILE [--output -o ] OUTPUTFILE [--quiet -q]" << std::endl;
                return 1;
        }

        if(!input)
        //if the input was not set, then set the input to the stdin of the program
                input = stdin;
        else if(!quiet)
                std::cerr << "Input file: " << inStr << std::endl;
        if(!output)
        //ditto for output
                output = stdout;
        else if(!quiet)
                std::cerr << "Output file: " << outStr << std::endl;


        byte* totalFile = NULL;
        uint64_t totalFileSize = 0;
        byte tmp[1024];
        int r;
        while( (r = fread(tmp, sizeof(byte), 1024, input)) ){
                byte* tmpPtr = (byte*)realloc(totalFile, r + totalFileSize);
                if(!tmpPtr){
                        if(quiet)
                                std::cerr << "Failed to allocate input file" << std::endl;
                        free(totalFile);
                        return -1;
                }
                totalFile = tmpPtr;
                memcpy(&totalFile[totalFileSize], tmp, r);
                totalFileSize += r;
        }

        try{
                QtFastStartSTD::QtFastStart qtfs = QtFastStartSTD::QtFastStart(totalFile, totalFileSize);
                QtFastStartSTD::ArtificialFileStream result = qtfs.fastStart();

                fwrite(result.getByteArray(), sizeof(byte), result.size(), output);

        }catch(std::exception  const &e){
                if(!quiet)
                        std::cerr << "Failed to process file: " << e.what() << std::endl;
                returnValue = 1;
        }
        fclose(input);
        fclose(output);

        if(!quiet)
                std::cerr << "Completed" << std::endl;

        return returnValue;
}
