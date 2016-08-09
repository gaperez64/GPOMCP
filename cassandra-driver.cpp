#include "cassandra-driver.h"
#include "cassandra-parser.h"

CassDriver::CassDriver() : trace_scanning(false), trace_parsing(false) {
    // pass
}

CassDriver::~CassDriver() {
    // pass
}

int CassDriver::parse(const std::string &f) {
    this->file = f;
    beginScan();
    yy::CassParser parser(*this);
    parser.set_debug_level(trace_parsing);
    int res = parser.parse();
    std::cout << "Result = " << res << std::endl;
    endScan();
    return res;
}

void CassDriver::error(const yy::location &l, const std::string &m) {
    std::cerr << l << ": " << m << std::endl;
}

void CassDriver::error(const std::string &m) {
    std::cerr << m << std::endl;
}
