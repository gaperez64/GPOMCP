#include <tuple>
#include <map>
#include <vector>
#include <string>
#include <set>
#include "pomdp.h"

POMDP::POMDP() : discount_factor(0.5) { }

POMDP::POMDP(std::vector<std::string> S,
             std::vector<std::string> A,
             std::vector<std::string> O,
             std::map<std::tuple<int, int, int>, float> P,
             std::map<int, float> PO,
             std::map<std::tuple<int, int, int>, float> W,
             std::map<int, float> I,
             float D) :
    states(S), actions(A), observations(O), prob_delta(P), prob_obs(PO),
    weight(W), initial_dist(I), discount_factor(D) { }

void POMDP::addTransition(int source, int action, int target,
                          float prob, float weight) {
    int max_state = max(source, target);
    if (max_state >= this->states.size())
        this->states.resize(max_state + 1);
    if (action >= this->actions.size())
        this->actions.resize(action + 1);
    this->prob_delta[std::make_tuple(source, action, target)] = prob;
    this->weight[std::make_tuple(source, action, target)] = weight;
}

void POMDP::setDiscFactor(float d) {
    this->discount_factor = d;
}

float POMDP::getDiscFactor() {
    return this->discount_factor;
}

bool POMDP::isValidMdp() {
    std::map<std::tuple<int, int> > state_action_prob;
    for (std::vector<int>::iterator s = this->states.begin();
            s != this->states.end(); ++s)
        for (std::vector<int>::iterator a = this->actions.begin();
                a != this->actions.end(); ++a)
            state_action_prob[*s][*a] = 0;

    for (std::map<std::tuple<int, int, int>, float>:iterator P =
            this->prob_delta.begin(); P != this->prob_delta.end();
            ++P) {
        int s, a, t;
        std::tie(s, a, t) = P->first;
        state_action_prob[s][a] += P->second;
    }

    for (std::vector<int>::iterator s = this->states.begin();
            s != this->states.end(); ++s)
        for (std::vector<int>::iterator a = this->actions.begin();
                a != this->actions.end(); ++a)
            if (state_action_prob[*s][*a] > 1) {
                std::cerr << "The probabilities for the state-action pair "
                          << *s << "," << *a << " sum to more than 1"
                          << std::endl;
                return false;
            }
    return true;
    // TODO: what else can we check?
}

bool POMDP::hasObsWeights() {
    for (std::vector<int>::iterator O = this->observations.begin();
            O != this->observations.end(); ++O) {
        std::set same_obs_states;
        for (std::map<std::tuple<int, int>, float>::iterator PO =
                this->prob_obs.begin(); PO != this->prob_obs.end();
                ++PO) {
            int s, o;
            std::tie(s, o) = PO->first;
            if (PO->second > 0.0 && o == *O)
                same_obs_states.insert(s);
        }
        // we now have al the states with the current observation
        // let us iterate all rewards and, if we revisit an observation
        // transition, we assert that the weight is the same
        std::map<std::tuple<int, int, int>, float> obs_weight;
        for (std::map<std::tuple<int, int, int, int>, float::iterator W =
                this->weight.begin(); W != this->weight.end(); ++W) {
            int s, a, t, o;
            std::tie(s, a, t, o) = W->first;
            if (same_obs_states.find(s) != same_obs_states.end()) {
                // if the observation transition is not yet registered
                // lets do that, otherwise, we can compare the registered weight
                // and the current one
                if (obs_weight.find(std::make_tuple(*O, a, o)) ==
                        obs_weight.end()) {
                    obs_weight[std::make_tuple(*O, a, o)] = W->second;
                } else if (obs_weight[std::make_tuple(*O, a, o)] != W->second) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool POMDP::hasDetObs() {
    for (std::map<std::tuple<int, int> float>:iterator PO =
            this->prob_obs.begin(); PO != this->prob_obs.end();
            ++PO)
        if (PO->second != 1.0)
            return false;
    return true;
}

void makeObsDet() {
    std::vector<std::string> new_states;
    std::map<std::tuple<int, int>, float> new_prob_obs;
    std::vector<std::vector<int> > obs_of_state;
    obs_of_state.resize(this->states.size());
    int i = 0
    for (std::map<std::tuple<int, int>, float>::iterator PO =
            this->prob_obs.begin(); PO != this->prob_obs.end();
            ++PO) {
        int s, o;
        std::tie(s, o) = PO->first;
        new_states.push_back(this->states[s] + ", obs: " +
                             this->observations[o]);
        obs_of_state[s].push_back(o);
        new_prob_obs[std::make_tuple(i, o)] = 1.0;
        i++;
    }
    // we now have a new set of states, we just need to update prob_delta,
    // prob_obs, weight, and initial_dist to be on the new set instead of the
    // old state set
    // we will do so by iterating over the original transitions and exploding
    // that times the size of observations per state
    for (std::map<std::tuple<int, int, int>, float>::iterator P =
            this->prob_delta.begin(); P != this->prob_delta_end(); ++P) {
        int s, a, t;
        std::tie(s, a, t) = P->first;
        for (std::vector<int>::iterator O = obs_of_state[s].begin();
                O != obs_of_state[s].end(); ++O) {
        }
    }
}
