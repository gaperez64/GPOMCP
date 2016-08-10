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
    parser.set_debug_level(this->trace_parsing);
    int res = parser.parse();
    endScan();
    return res;
}

void CassDriver::error(const yy::location &l, const std::string &m) {
    std::cerr << l << ": " << m << std::endl;
}

void CassDriver::error(const std::string &m) {
    std::cerr << "error: " << m << std::endl;
}


void CassDriver::addTransition(ElemRef source, ElemRef action, ElemRef target,
                               float prob) {
    std::cout << "d(" << source.name << "," << action.name << ","
              << target.name << ")="
              << prob << std::endl;
}

void CassDriver::addWeight(ElemRef source, ElemRef action, ElemRef target,
ElemRef obs, float weight){}
void CassDriver::addObsTransition(ElemRef source, ElemRef action, ElemRef obs,
float prob){}
void CassDriver::setDiscount(float discount){}
void CassDriver::setWeightSign(int sign){}
void CassDriver::setStates(std::vector<std::string> states){}
void CassDriver::setActions(std::vector<std::string> actions){}
void CassDriver::setObservations(std::vector<std::string> observations){}
