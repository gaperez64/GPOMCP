#include <iostream>
#include "cassandra-driver.h"

int main (int argc, char *argv[]) {
    int res = 0;
    CassDriver driver;
    if (argc < 2)
        exit(1);
    driver.parse(argv[1]);
    std::cout << driver.result << std::endl;
    res = 1;
    return res;
}
