#include <iostream>
#include "cassandra-driver.h"

int main (int argc, char *argv[]) {
    CassDriver driver;
    if (argc < 2)
        exit(1);
    driver.parse(argv[1]);
    exit(EXIT_SUCCESS);
}
