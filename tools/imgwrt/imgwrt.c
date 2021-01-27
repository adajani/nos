/************************************************************************
* Copyright (C) 2020 by Ahmad Dajani                                    *
*                                                                       *
* This file is part of NOS.                                             *
*                                                                       *
* NOS is free software: you can redistribute it and/or modify it        *
* under the terms of the GNU Lesser General Public License as published *
* by the Free Software Foundation, either version 3 of the License, or  *
* (at your option) any later version.                                   *
*                                                                       *
* NOS is distributed in the hope that it will be useful,                *
* but WITHOUT ANY WARRANTY* without even the implied warranty of        *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
* GNU Lesser General Public License for more details.                   *
*                                                                       *
* You should have received a copy of the GNU Lesser General Public      *
* License along with NOS.  If not, see <http://www.gnu.org/licenses/>.  *
************************************************************************/

/*@file imgwrt.c
* @author Ahmad Dajani <eng.adajani@gmail.com>
* @date 26 Jan 2021
* @brief Disk image writer
* @description Write binary files into disk image at specific LBA
*/

#include <stdio.h> /* printf. fprintf, stderr, FILE, fread, fclose, fwrite, fseek, feof, fputc */
#include <stdlib.h> /* exit, EXIT_SUCCESS, EXIT_FAILURE, atoi  */
#include <string.h> /* strcpy, stricmp */

#define VERSION "0.1"
/* @see include\kernel\disk.h */
#define SECTOR_SIZE 512
#define FILE_NAME_SIZE 80

struct Arguments {
    unsigned int logicalBlockAddress;
    char fileName[FILE_NAME_SIZE];
    char imageName[FILE_NAME_SIZE];
};

void usage(void) {
    printf("Disk image writer, version %s\n"
           "(c)2020, By Ahmad Dajani, eng.adajani@gmail.com\n"
           "Usage: imgwrt.exe -img [diskImageFile] -lba [logicalBlockAddress] -file [file]\n"
           "Where:\n"
           "    -img   [required]  Image file to write into.\n"
           "    -lba   [required]  Write at lba number.\n"
           "    -file  [required]  File to write into disk image.\n"
           "Example: Writing boot.bin file at lba 0 in disk image floppy.img\n"
           "    imgwrt.exe -img floppy.img -lba 0 -file boot.bin\n"
           , VERSION, SECTOR_SIZE);
}

int parseArguments(int argc, char* argv[], struct Arguments *arguments) {
    register unsigned int argumentIndex;

    if(argc != 7) {
        usage();
        return EXIT_FAILURE;
    }

    for(argumentIndex=1; argumentIndex<argc; argumentIndex++) {
        if(stricmp(argv[argumentIndex], "-img") == 0) {
            argumentIndex += 1;
            strcpy(arguments->imageName, argv[argumentIndex]);
        } else if(stricmp(argv[argumentIndex], "-lba") == 0) {
            argumentIndex += 1;
            arguments->logicalBlockAddress = atoi(argv[argumentIndex]);
        } else if(stricmp(argv[argumentIndex], "-file") == 0) {
            argumentIndex += 1;
            strcpy(arguments->fileName, argv[argumentIndex]);
        } else {
            fprintf(stderr, "Error: Unknown argument: %s.\n", argv[argumentIndex]);
            usage();
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    struct Arguments arguments;
    unsigned int imageFileOffset;
    FILE *imageFile;
    FILE *inFile;

    if(parseArguments(argc, argv, &arguments) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    /* Open an existing image file for update */
    imageFile = fopen(arguments.imageName, "rb+");
    if( imageFile == NULL) {
        fprintf(stderr, "Error: Cannot open image file: %s\n", arguments.imageName);
        return EXIT_FAILURE;
    }

    /* Open an inpute file for read only */
    inFile = fopen(arguments.fileName, "rb");
    if( inFile == NULL) {
        fprintf(stderr, "Error: Cannot open input file: %s\n", arguments.fileName);
        return EXIT_FAILURE;
    }

    /* Move the image file pointer into LBA */
    imageFileOffset = arguments.logicalBlockAddress * SECTOR_SIZE;
    fseek(imageFile, imageFileOffset, SEEK_SET);

    printf("Writing file:%s into image:%s @ lba %d ... ", arguments.fileName, arguments.imageName, arguments.logicalBlockAddress);

    /* TODO: 1. copy bulk of bytes instead of single byte copy.
             2. check the reserved sectors boundary in boot sector (kernel only).
    */
    while( !feof(inFile) ) {
        fputc(fgetc(inFile), imageFile);
    }

    fclose(imageFile);
    fclose(inFile);

    printf("Done\n");

    return EXIT_SUCCESS;
}