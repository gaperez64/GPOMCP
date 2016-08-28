#ifndef CASS_DRIVER_H
#define CASS_DRIVER_H

#include <string>
#include <map>
#include "cassandra-parser.h"
#include "pomdp.h"

// tell flex the prototype of the lexer
#define YY_DECL \
    yy::CassParser::symbol_type yylex(CassDriver &driver)
// and instantiate it for the parser
YY_DECL;

class CassDriver {
    protected:
        POMDP* pomdp;
        double discount_factor;
        int weight_sign;

    public:
        CassDriver(POMDP*);
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

        // Building an MDP
        void addTransition(ElemRef source, ElemRef action, ElemRef target,
                           double prob);
        void addWeight(ElemRef source, ElemRef action, ElemRef target,
                       ElemRef obs, double weight);
        void addObsTransition(ElemRef action, ElemRef target, ElemRef obs,
                              double prob);
        void setDiscount(double discount);
        void setWeightSign(int sign);
        void setStates(std::vector<std::string> states);
        void setActions(std::vector<std::string> actions);
        void setObservations(std::vector<std::string> observations);
        void setInitialDist(std::vector<ElemRef> states);
        void setInitialDist(std::vector<double> probs);
};

#endif // CASS_DRIVER_H
