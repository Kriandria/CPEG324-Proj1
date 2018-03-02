#ifndef FILE_IO_H
#define FILE_IO_H

const int MAX_INSTRUCTION_LENGTH = 100;

inline void fatal(const char* err_msg);

/*
 * Safe version of fopen(3). If the call fails, generate a "graceful" 
 * failure message and exit the program. Otherwise, return the file
 * handle
 */
FILE* s_fopen(const char* path, const char* mode);

inline size_t 
s_fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream);

size_t 
s_fread(void* ptr, size_t size, size_t nmemb, FILE* stream);

inline void 
usage(const char* progname);


/* Our implementation */

inline uint8_t
calculateReg(char *token);

// takes a number from (7, -8)
inline uint8_t
calculateImm(char *token);

inline uint8_t
calculateBit(char *token);

// REQUIRES THAT AMSEMBLY CODE HAVE A TRAILING NEW LINE!
void
read_assembly(const char* read_filename, const char* write_filename);

#endif /* FILE_IO_H */