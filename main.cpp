#include <iostream>
#include <cstdlib>
#include <fstream>

#include "calc.h"
#include "file_io.h"

using namespace std;

int main(int argc, char **argv) {
    
    if (argc != 2) {
        fprintf(stderr, "Usage: calc ASSEMBLY_FILE.\n");
        return 1;
    }

    calc c;
    
    read_assembly(argv[1], "binary.o");
    
    c.read_binary("binary.o");
    
    return 0;
}

