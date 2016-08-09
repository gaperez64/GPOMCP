#ifndef CASS_DRIVER_H
#define CASS_DRIVER_H

#include <string>
#include <map>
#include "cassandra-parser.h"

// tell flex the prototype of the lexer
#define YY_DECL \
    yy::CassParser::symbol_type yylex(CassDriver &driver)
// and instantiate it for the parser
YY_DECL;

class CassDriver {
    public:
        CassDriver();
        virtual ~CassDriver();
        int result;

        // Scan handling
        void beginScan();
        void endScan();
        bool trace_scanning;

        // parse file f, return 0 on success
        int parse(const std::string &f);
        std::string file;
        bool trace_parsing;

        // Error handling
        void error(const yy::location &l, const std::string &m);
        void error(const std::string &m);
};

#endif // CASS_DRIVER_H
