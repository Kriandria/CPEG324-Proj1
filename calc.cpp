#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <bitset>

#include "calc.h"

int skip = 0;

// Initialize all registers to 0
calc::calc(void) {
    this->r0 = this->r1 = this->r2 = this->r3 = 0;
    this->o0 = this->o1 = this->o2 = this->o3 = 0;
}

void calc::load(uint8_t d, uint8_t imm) {
    if (((imm >> 3) & 1) == 1) imm = 0xF0 | imm; //sign extends the bit, gives range -8 to 7
    switch(d) { //load imm into register, reset overflow tag
        case 0: r0 = imm; o0 = 0; break;
        case 1: r1 = imm; o1 = 0; break;
        case 2: r2 = imm; o2 = 0; break;
        case 3: r3 = imm; o3 = 0;
    }
}

void calc::add(uint8_t d, uint8_t s, uint8_t t, bool add) {
    unit8_t result, carry;
    bool overflow = false;

    switch(s) {
        case 0: s = r0;
            if (o0) { overflow = true;}
            break;
        case 1: s = r1;
            if (o1) { overflow = true;}
            break;
        case 2: s = r2;
            if (o2) { overflow = true;}
            break;
        case 3: s = r3;
            if (o3) { overflow = true;}
            break;
    }
    if (add) {
        switch(t) {
            case 0: t = r0;
                if (o0) { overflow = true;}
                break;
            case 1: t = r1;
                if (o1) { overflow = true;}
                break;
            case 2: t = r2;
                if (o2) { overflow = true;}
                break;
            case 3: t = r3;
                if (o3) { overflow = true;}
                break;
        }
        cout << bitset<8>(s) << " + " << bitset<8>(t) << endl;
    } else {    // subtraction
        // Load target register
        switch(t) {
            case 0:
                t = r0;
                if (r0 == 0x80) overflow = true;
                break;
            case 1:
                t = r1;
                if (r1 == 0x80) overflow = true;
                break;
            case 2:
                t = r2;
                if (r2 == 0x80) overflow = true;
                break;
            case 3:
                t = r3;
                if (r3 == 0x80) overflow = true;
        }
        cout << bitset<8>(s) << " - " << bitset<8>(t) << endl;
        // Flip sign of target register value
        t = ((t-1) ^ 0xFF);
    }

    // Calculate result and carry with a chain of half adders

    result = (s & 0x01) ^ (t & 0x01);
    carry = (s & 0x01) & (t & 0x01);
    
    result = ((s & 0x02) ^ (t & 0x02) ^ (carry << 1)) | result;
    carry = ((s & 0x02) & (t & 0x02)) | ((s & 0x02) & (carry << 1)) | ((t & 0x02) & (carry << 1));
    
    result = ((s & 0x04) ^ (t & 0x04) ^ (carry << 1)) | result;
    carry = ((s & 0x04) & (t & 0x04)) | ((s & 0x04) & (carry << 1)) | ((t & 0x04) & (carry << 1));
    
    result = ((s & 0x08) ^ (t & 0x08) ^ (carry << 1)) | result;
    carry = ((s & 0x08) & (t & 0x08)) | ((s & 0x08) & (carry << 1)) | ((t & 0x08) & (carry << 1));
    
    result = ((s & 0x10) ^ (t & 0x10) ^ (carry << 1)) | result;
    carry = ((s & 0x10) & (t & 0x10)) | ((s & 0x10) & (carry << 1)) | ((t & 0x10) & (carry << 1));
    
    result = ((s & 0x20) ^ (t & 0x20) ^ (carry << 1)) | result;
    carry = ((s & 0x20) & (t & 0x20)) | ((s & 0x20) & (carry << 1)) | ((t & 0x20) & (carry << 1));
    
    result = ((s & 0x40) ^ (t & 0x40) ^ (carry << 1)) | result;
    carry = ((s & 0x40) & (t & 0x40)) | ((s & 0x40) & (carry << 1)) | ((t & 0x40) & (carry << 1));
    
    result = ((s & 0x80) ^ (t & 0x80) ^ (carry << 1)) | result;
    carry = ((s & 0x80) & (t & 0x80)) | ((s & 0x80) & (carry << 1)) | ((t & 0x80) & (carry << 1));

/* If [ (source sign bit and target bit are the same)
        and (result sign bit is different) ]
            or overflow bit is set,
    then the overflow bit is set for the destination register.
 */
    if (((((s >> 7) & 1) == ((t >> 7) & 1)) && (((result >> 7) & 1) != ((s >> 7) & 1))) || overflow) {
        switch(d) {
            case 0: o0 = 1; break;
            case 1: o1 = 1; break;
            case 2: o2 = 1; break;
            case 3: o3 = 1;
        }
    }
    switch(d) {
        case 0: r0 = result; break;
        case 1: r1 = result; break;
        case 2: r2 = result; break;
        case 3: r3 = result;
    }
    return;
}

void calc::subtract(uint8_t d, uint8_t s, uint8_t t) {
    add(d, s, t, false);
}

// Skip one instrution
void calc::skip_1(uint8_t s, uint8_t t) {
    switch(s) {
        case 0: s = r0; break;
        case 1: s = r1; break;
        case 2: s = r2; break;
        case 3: s = r3;
    }
    switch(t) {
        case 0: t = r0; break;
        case 1: t = r1; break;
        case 2: t = r2; break;
        case 3: t = r3;
    }
    if (s == t) {
         skip = 1;
    }
}

// Skip two instructions
void calc::skip_2(uint8_t s, uint8_t t) {
    switch(s) {
        case 0: s = r0; break;
        case 1: s = r1; break;
        case 2: s = r2; break;
        case 3: s = r3;
    }
    switch(t) {
        case 0: t = r0; break;
        case 1: t = r1; break;
        case 2: t = r2; break;
        case 3: t = r3;
    }
    if (s == t) {
        skip = 2;
    }
}

// Print register value if no overflow,
//   otherwise print IntegerOverflow
void calc::print(uint8_t s) {
    switch(s) {
        case 0:
            if (o0) cout << "IntegerOverflow" << endl;
            else cout << bitset<8>(r0) << endl;
            break;
        case 1:
            if (o1) cout << "IntegerOverflow" << endl;
            else cout << bitset<8>(r1) << endl;
            break;
        case 2:
            if (o2) cout << "IntegerOverflow" << endl;
            else cout << bitset<8>(r2) << endl;
            break;
        case 3:
            if (o3) cout << "IntegerOverflow" << endl;
            else cout << bitset<8>(r3) << endl;
    }
}

// Read binary file byte by byte.
void calc::read_binary(const char* filename) {
    FILE*   fh   = fopen(filename,"r");
    uint8_t word = 0;

    // While there are instructions
    while ( fread(&word, sizeof(uint8_t), 1UL, fh) ) {

        if (skip != 0) { // check skip counter
            skip--;
            continue;
        }
    
        if (word >> 6 == 0b00) {                // LOAD Rs, Immediate
            load((word >> 4) & 0b11, word & 0xF);
        } else if (word >> 6 == 0b01) {         // ADD Rd, Rs, Rt
            add(word & 0b11, (word >> 4) & 0b11, (word >> 2) & 0b11, true);
        } else if (word >> 6 == 0b10) {         // SUB Rd, Rs, Rt
            subtract(word & 0b11, (word >> 4) & 0b11, (word >> 2) & 0b11);
        } else if (word >> 6 == 0b11) {         // SPECIAL (skip and print)
            if ((word & 0x01) == 0) {           // SKIP Rs, Rt, Offset
                if (((word >> 1) & 0x01) == 0) {        // SKIP Rs, Rt, 0
                    skip_1((word >> 4) & 0b11, (word >> 2) & 0b11);
                } else if (((word >> 1) & 0x01) == 1) { // SKIP Rs, Rt, 1
                    skip_2((word >> 4) & 0b11, (word >> 2) & 0b11);
                }
            } else if ((word & 0x01) == 1) {    // PRINT Rs
                print((word >> 4) & 0b11);
            }
        }
    }
    fclose(fh);
}
