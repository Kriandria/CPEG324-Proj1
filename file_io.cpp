#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>

#include "file_io.h"


inline void fatal(const char* err_msg) {
    perror(err_msg);
    exit(errno);
}

/*
 * Safe version of fopen(3). If the call fails, generate a "graceful" 
 * failure message and exit the program. Otherwise, return the file
 * handle
 */
FILE* s_fopen(const char* path, const char* mode) {
    FILE* fh = fopen(path,mode);
    if (!fh)
        fatal("fopen");
    return fh;
}

inline size_t 
s_fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream) 
{
    size_t n_elt = fwrite(ptr,size,nmemb,stream);
    if (n_elt == 0) { // check if EOF or I/O error
        if ( ferror(stream) ) // *not* EOF
            fatal("fwrite"); 
    }
    return n_elt;
}

size_t 
s_fread(void* ptr, size_t size, size_t nmemb, FILE* stream) 
{
    size_t n_elt = fread(ptr,size,nmemb,stream);
    if (n_elt == 0) { // check if EOF or I/O error
        if ( ferror(stream) )  // *not* EOF
            fatal("fread");
    }
    return n_elt;
}

inline void 
usage(const char* progname)
{
    fprintf(stderr,"USAGE:\t '%s' filename\n", progname);
    exit(1);
}


/* Our implementation */

inline uint8_t
calculateReg(char *token) {
    uint8_t reg = (uint8_t) atoi(token);
    reg &= 0b11;
    return reg;
}

// takes a number from (7, -8)
inline uint8_t
calculateImm(char *token) {
    uint8_t value;
    if (*token == '-') {
        value = (uint8_t) atoi(token+1);
        value -= 1;     // subtract 1
        value ^= 0xF;   // flip bits
        value |= 0b1000;
        value &= 0x0F;
        return value;
    } else {
        value = (uint8_t) atoi(token);
        value &= 0b0111;
        return value;
    }
}

inline uint8_t
calculateBit(char *token) {
    uint8_t reg = (uint8_t) atoi(token);
    reg &= 0x01;
    return reg;
}

// REQUIRES THAT AMSEMBLY CODE HAVE A TRAILING NEW LINE!
// Reads from assembly file and writes to binary file
void
read_assembly(const char* read_filename, const char* write_filename) {
    FILE*   read_fh   = s_fopen(read_filename,"r");
    FILE*   write_fh   = s_fopen(write_filename,"w");
    
    char *token;
    char *command = NULL;
    uint8_t instruction;
    
    char letter = 0;
    char *line = (char*) calloc(MAX_INSTRUCTION_LENGTH, sizeof(char));
    int i = 0;


    while ( s_fread(&letter, sizeof(char), 1UL, read_fh) ) {
        if (letter == '\n') { // End of line
            // read assembly line
            token = strtok(line, " ,$#");
            command = token;
            if (strcmp(command, "LOAD") == 0)       instruction = 0b00000000;
            else if (strcmp(command, "ADD") == 0)   instruction = 0b01000000;
            else if (strcmp(command, "SUB") == 0)   instruction = 0b10000000;
            else if (strcmp(command, "SKIP") == 0)  instruction = 0b11000000;
            else if (strcmp(command, "PRINT") == 0) instruction = 0b11000001;
            else usage(command);
            token = strtok(NULL, " ,$#");
            for (i = 0; token != NULL; i++) {
                if (strcmp(command, "LOAD") == 0) {
                    if (i == 0) instruction |= calculateReg(token) << 4;
                    else if (i == 1) instruction |= calculateImm(token) << 0;
                } else if (strcmp(command, "ADD") == 0 || strcmp(command, "SUB") == 0) {
                    if (i == 0) instruction |= calculateReg(token) << 0;
                    else if (i == 1) instruction |= calculateReg(token) << 4;
                    else if (i == 2) instruction |= calculateReg(token) << 2;
                } else if (strcmp(command, "SKIP") == 0) {
                    if (i == 0) instruction |= calculateReg(token) << 4;
                    else if (i == 1) instruction |= calculateReg(token) << 2;
                    else if (i == 2) instruction |= calculateBit(token) << 1;
                } else if (strcmp(command, "PRINT") == 0) {
                    if (i == 0) instruction |= calculateReg(token) << 4;
                }
                token = strtok(NULL, " ,$#");
            }
//printf("instruction='%i'\n", instruction);  // debugging
            //write binary line
            s_fwrite(&instruction, sizeof(uint8_t), 1UL, write_fh);
//printf("file written\n");  // debugging
            // reset line
            line[0] = '\0';
            i = 0;
            continue;
        }
        line[i] = letter;
        line[++i] = '\0';
        
//printf("Read: %c\n", letter);  // debugging
    }

    fclose(read_fh);
    fclose(write_fh);
}