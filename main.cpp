#include <assert.h>
#include <iostream>
#include "pomdp.h"

int main (int argc, char *argv[]) {
    if (argc < 2)
        exit(1);
    POMDP p(argv[1]);
    assert(p.isValidMdp());
    p.print(std::cout);
    p.makeGameBeliefConstruction();
    p.print(std::cout);
    exit(EXIT_SUCCESS);
}
