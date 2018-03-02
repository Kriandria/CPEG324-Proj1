
#ifndef CALC_H
#define CALC_H
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

typedef unsigned char unit8_t;

class calc{
    private:
        unit8_t r0; // Signed 8-bit valuse
        unit8_t r1;
        unit8_t r2;
        unit8_t r3;

        unit8_t o0; // Overflow flag
        unit8_t o1;
        unit8_t o2;
        unit8_t o3;

    public:
        calc();
        
        void load(unit8_t d, unit8_t imm);
        void add(unit8_t d, unit8_t s, unit8_t t, bool add);  // Functionality is nearly identical to subtract,
        void subtract(unit8_t d, unit8_t s, unit8_t t); // Therefor, subtract will just call add with functionality
        void skip_1(uint8_t s, uint8_t t);                                      // based around the 'add' boolean.
        void skip_2(uint8_t s, uint8_t t);
        void print(unit8_t s);
        void read_binary(const char* filename);
    
};
#endif