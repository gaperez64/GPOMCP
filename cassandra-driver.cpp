#include "cassandra-driver.h"
#include "cassandra-parser.h"

CassDriver::CassDriver(POMDP* p) :
    trace_scanning(false), trace_parsing(false), pomdp(p) { }

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
                               double prob) {
    assert(source.type == ELEMREFTYPE_NAME);
    assert(target.type == ELEMREFTYPE_NAME);
    assert(action.type != ELEMREFTYPE_ID);

    std::vector<int>::iterator a_it;
    std::vector<int>::iterator a_end;
    if (action.type == ELEMREFTYPE_NAME) {
        std::vector<int> dummy_act_vector;
        dummy_act_vector.push_back(this->pomdp->getActionId(action.name));
        a_it = dummy_act_vector.begin();
        a_end = dummy_act_vector.end();
    } else if (action.type == ELEMREFTYPE_ALL) {
        std::vector<int> dummy_act_vector;
        for (int i = 0; i < this->pomdp->getActionCount(); i++)
            dummy_act_vector.push_back(i);
        a_it = dummy_act_vector.begin();
        a_end = dummy_act_vector.end();
    }

    for (; a_it != a_end; ++a_it) {
        this->pomdp->addTransition(this->pomdp->getStateId(source.name),
                                   *a_it,
                                   this->pomdp->getStateId(target.name),
                                   prob);
    }
}

void addWeightHelper2(POMDP* p, int source,
                      int action, ElemRef target, double weight) {
    if (target.type == ELEMREFTYPE_ALL)
        for (int i = 0; i < p->getStateCount(); i++)
            p->weightTransition(source, action, i, weight);
    else
        p->weightTransition(source, action,
                            p->getStateId(target.name),
                            weight);
}

void addWeightHelper1(POMDP* p, int source,
                      ElemRef action, ElemRef target, double weight) {
    if (action.type == ELEMREFTYPE_ALL)
        for (int i = 0; i < p->getActionCount(); i++)
            addWeightHelper2(p, source, i, target, weight);
    else
        addWeightHelper2(p, source, p->getActionId(action.name),
                         target, weight);
}

void CassDriver::addWeight(ElemRef source, ElemRef action, ElemRef target,
                           ElemRef obs, double weight) {
    assert(obs.type == ELEMREFTYPE_ALL);
    assert(source.type != ELEMREFTYPE_ID);
    assert(action.type != ELEMREFTYPE_ID);
    assert(target.type != ELEMREFTYPE_ID);

    if (source.type == ELEMREFTYPE_ALL)
        for (int i = 0; i < this->pomdp->getStateCount(); i++)
            addWeightHelper1(this->pomdp, i, action, target,
                             weight * this->weight_sign);
    else
        addWeightHelper1(this->pomdp, this->pomdp->getStateId(source.name),
                         action, target, weight * this->weight_sign);
}

void CassDriver::addObsTransition(ElemRef action, ElemRef target, ElemRef obs,
                                  double prob) {
    assert(action.type == ELEMREFTYPE_ALL);
    assert(target.type == ELEMREFTYPE_NAME);
    assert(obs.type == ELEMREFTYPE_NAME);
    this->pomdp->addObservationProb(this->pomdp->getStateId(target.name),
                                    this->pomdp->getObservationId(obs.name),
                                    prob);
}

void CassDriver::setDiscount(double discount) {
    this->pomdp->setDiscFactor(discount);
}

void CassDriver::setWeightSign(int sign) {
    this->weight_sign = sign;
}

void CassDriver::setStates(std::vector<std::string> states) {
    this->pomdp->setStates(states);
}

void CassDriver::setActions(std::vector<std::string> actions) {
    this->pomdp->setActions(actions);
}

void CassDriver::setObservations(std::vector<std::string> observations) {
    this->pomdp->setObservations(observations);
}

void CassDriver::setInitialDist(std::vector<ElemRef> states) {
    std::map<int, double> initial_dist;
    double denominator = states.size();
    for (std::vector<ElemRef>::iterator i = states.begin();
            i != states.end(); ++i) {
        assert(i->type == ELEMREFTYPE_NAME);
        initial_dist[this->pomdp->getStateId(i->name)] = 1.0/denominator;
    }
    this->pomdp->setInitialDist(initial_dist);
}

void CassDriver::setInitialDist(std::vector<double> probs) {
    assert(probs.size() == this->pomdp->getStateCount());
    std::map<int, double> initial_dist;
    for (int i = 0; i < probs.size(); i++)
        if (probs[i] > 0.0)
            initial_dist[i] = probs[i];
    this->pomdp->setInitialDist(initial_dist);
}
