#include <tuple>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include "pomdp.h"
#include "dsgame.h"
#include "cassandra-driver.h"

POMDP::POMDP() : discount_factor(0.5) { }

POMDP::POMDP(const std::string &filename) : POMDP() {
    CassDriver driver(this);
    driver.parse(filename);
}

POMDP::POMDP(std::vector<std::string> S,
             std::vector<std::string> A,
             std::vector<std::string> O,
             std::map<std::tuple<int, int, int>, float> P,
             std::map<std::tuple<int, int>, float> PO,
             std::map<std::tuple<int, int, int>, float> W,
             std::map<int, float> I,
             float D) :
    states(S), actions(A), observations(O), prob_delta(P), prob_obs(PO),
    weight(W), initial_dist(I), discount_factor(D) { }

POMDP::POMDP(const POMDP &other) :
    POMDP(other.states, other.actions, other.observations, other.prob_delta,
          other.prob_obs, other.weight, other.initial_dist,
          other.discount_factor) { }

void POMDP::setStates(std::vector<std::string> S) {
    this->states = S;
}

int POMDP::getStateCount() {
    return this->states.size();
}

void POMDP::setActions(std::vector<std::string> A) {
    this->actions = A;
}

int POMDP::getActionCount() {
    return this->actions.size();
}

void POMDP::setObservations(std::vector<std::string> O) {
    this->observations = O;
}

void POMDP::addTransition(int source, int action, int target,
                          float prob) {
    assert(source < this->states.size());
    assert(target < this->states.size());
    this->prob_delta[std::make_tuple(source, action, target)] = prob;
}

void POMDP::addObservationProb(int state, int obs, float prob) {
    assert(state < this->states.size());
    assert(obs < this->observations.size());
    this->prob_obs[std::make_tuple(state, obs)] = prob;
}

void POMDP::weightTransition(int source, int action, int target,
                             float weight) {
    if (this->prob_delta.find(std::make_tuple(source, action, target)) ==
            this->prob_delta.end())
        return;
    this->weight[std::make_tuple(source, action, target)] = weight;
}

void POMDP::setDiscFactor(float d) {
    this->discount_factor = d;
}

float POMDP::getDiscFactor() {
    return this->discount_factor;
}

bool POMDP::isValidMdp() {
    // check #1: transition probs should add to 1 for every state x action
    std::map<std::tuple<int, int>, float> state_action_prob;
    for (int s = 0; s < this->states.size(); s++)
        for (int a = 0; a < this->actions.size(); a++)
            state_action_prob[std::make_tuple(s, a)] = 0;

    for (std::map<std::tuple<int, int, int>, float>::iterator P =
            this->prob_delta.begin(); P != this->prob_delta.end();
            ++P) {
        int s, a, t;
        std::tie(s, a, t) = P->first;
        state_action_prob[std::make_tuple(s, a)] += P->second;
    }

    for (int s = 0; s < this->states.size(); s++)
        for (int a = 0; a < this->actions.size(); a++)
            if (state_action_prob[std::make_tuple(s, a)] > 1) {
                std::cerr << "The probabilities for the state-action pair "
                          << this->states[s] << ","
                          << this->actions[a] << " sum to more than 1"
                          << std::endl;
                return false;
            }
    // check #2: every transition should have a weight
    for (std::map<std::tuple<int, int, int>, float>::iterator i =
            this->prob_delta.begin(); i != this->prob_delta.end(); ++i) {
        if (this->weight.find(i->first) == this->weight.end()) {
            int s, a, t;
            std::tie(s, a, t) = i->first;
            std::cerr << "The weight of transition "
                      << "(" << this->states[s] << ","
                      << this->actions[a] << ","
                      << this->states[t] << ")"
                      << " is not defined"
                      << std::endl;
            return false;
        }
    }
    // check #3: observation probs should add to 1 for every state
    std::map<std::tuple<int>, float> state_obs_prob;
    for (int s = 0; s < this->states.size(); s++)
        state_obs_prob[std::make_tuple(s)] = 0;

    for (std::map<std::tuple<int, int>, float>::iterator PO =
            this->prob_obs.begin(); PO != this->prob_obs.end();
            ++PO) {
        int s, o;
        std::tie(s, o) = PO->first;
        state_obs_prob[s] += PO->second;
    }

    for (int s = 0; s < this->states.size(); s++)
        if (state_obs_prob[s] > 1) {
            std::cerr << "The observation probabilities for state "
                      << this->states[s] << " sum to more than 1"
                      << std::endl;
            return false;
        }
    // TODO: what else can we check?
    return true;
}

bool POMDP::hasObsWeights() {
    std::vector<std::set<int> > same_obs_states;
    same_obs_states.resize(this->observations.size());
    for (std::map<std::tuple<int, int>, float>::iterator PO =
            this->prob_obs.begin(); PO != this->prob_obs.end();
            ++PO) {
        int s, o;
        std::tie(s, o) = PO->first;
        if (PO->second > 0.0)
            same_obs_states[o].insert(s);
    }
    // we now have all the states with any observation
    // let us iterate all rewards and, if we revisit an observation
    // transition, we assert that the weight is the same
    for (int cur_obs1 = 0; cur_obs1 < this->observations.size(); cur_obs1++) {
        for (int cur_obs2 = 0; cur_obs2 < this->observations.size(); cur_obs2++) {
            std::map<std::tuple<int, int, int>, float> obs_weight;
            for (std::map<std::tuple<int, int, int>, float>::iterator W =
                    this->weight.begin(); W != this->weight.end(); ++W) {
                int s, a, t;
                std::tie(s, a, t) = W->first;
                if ((same_obs_states[cur_obs1].find(s) !=
                        same_obs_states[cur_obs1].end()) &&
                        (same_obs_states[cur_obs2].find(t) !=
                         same_obs_states[cur_obs2].end())) {
                    // if the observation transition is not yet registered
                    // lets do that, otherwise, we can compare the registered weight
                    // and the current one
                    if (obs_weight.find(std::make_tuple(cur_obs1, a, cur_obs2)) ==
                            obs_weight.end()) {
                        obs_weight[std::make_tuple(cur_obs1, a, cur_obs2)] =
                            W->second;
                    } else if (obs_weight[std::make_tuple(cur_obs1, a, cur_obs2)] !=
                            W->second) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool POMDP::hasInitialObs() {
    std::vector<int> initial_obs;
    for (std::map<int, float>::iterator I = this->initial_dist.begin();
            I != this->initial_dist.end(); ++I)
        initial_obs.push_back(I->first);
    std::sort(initial_obs.begin(), initial_obs.end());

    std::vector<std::vector<int> > same_obs_states;
    same_obs_states.resize(this->observations.size());
    for (std::map<std::tuple<int, int>, float>::iterator PO =
            this->prob_obs.begin(); PO != this->prob_obs.end();
            ++PO) {
        int s, o;
        std::tie(s, o) = PO->first;
        if (PO->second > 0.0)
            same_obs_states[o].push_back(s);
    }

    for (int i = 0; i < this->observations.size(); i++) {
        std::sort(same_obs_states[i].begin(), same_obs_states[i].end());
        if (same_obs_states[i] == initial_obs)
            return true;
    }
    return false;
}

bool POMDP::hasDetObs() {
    for (std::map<std::tuple<int, int>, float>::iterator PO =
            this->prob_obs.begin(); PO != this->prob_obs.end();
            ++PO)
        if (PO->second != 1.0)
            return false;
    return true;
}

void POMDP::makeObsDet() {
    std::vector<std::string> new_states;
    std::map<std::tuple<int, int>, float> new_prob_obs;
    std::vector<std::vector<int> > newstates_per_state;
    std::vector<std::vector<int> > obs_per_state;
    obs_per_state.resize(this->states.size());
    int i = 0;
    for (std::map<std::tuple<int, int>, float>::iterator PO =
            this->prob_obs.begin(); PO != this->prob_obs.end();
            ++PO) {
        int s, o;
        std::tie(s, o) = PO->first;
        new_states.push_back(this->states[s] + ", obs: " +
                             this->observations[o]);
        newstates_per_state[s].push_back(i);
        obs_per_state[s].push_back(o);
        new_prob_obs[std::make_tuple(i, o)] = 1.0;
        i++;
    }
    // we now have a new set of states, we just need to update prob_delta,
    // prob_obs, weight, and initial_dist to be on the new set instead of the
    // old state set
    // we will do so by iterating over the original transitions and exploding
    // that times the size of observations per state
    std::map<std::tuple<int, int, int>, float> new_prob_delta;
    std::map<std::tuple<int, int, int>, float> new_weight;
    for (std::map<std::tuple<int, int, int>, float>::iterator P =
            this->prob_delta.begin(); P != this->prob_delta.end(); ++P) {
        int s, a, t;
        std::tie(s, a, t) = P->first;
        std::vector<int>::iterator NS1 = newstates_per_state[s].begin();
        std::vector<int>::iterator OS1 = obs_per_state[s].begin();
        while (NS1 != newstates_per_state[s].end()) {
            std::vector<int>::iterator NS2 = newstates_per_state[t].begin();
            std::vector<int>::iterator OS2 = obs_per_state[t].begin();
            while (NS2 != newstates_per_state[t].end()) {
                float w = this->weight[std::make_tuple(s, a, t)];
                new_prob_delta[std::make_tuple(*NS1, a, *NS2)] =
                    P->second * this->prob_obs[std::make_tuple(t, *OS2)]; 
                new_weight[std::make_tuple(*NS1, a, *NS2)] = w;
                ++NS2;
                ++OS2;
            }
            ++NS1;
            ++OS1;
        }
    }
    // generate the new initial distribution
    std::map<int, float> new_initial_dist;
    for (std::map<int, float>::iterator I = this->initial_dist.begin();
            I != this->initial_dist.end(); ++I) {
        std::vector<int>::iterator NS = newstates_per_state[I->first].begin();
        std::vector<int>::iterator OS = obs_per_state[I->first].begin();
        while (NS != newstates_per_state[I->first].end()) {
            new_initial_dist[*NS] = I->second *
                this->prob_obs[std::make_tuple(I->first, *OS)];
            ++NS;
            ++OS;
        }
    }
    // update all the internal variables
    std::swap(this->states, new_states);
    // actions stay the same
    // observations stay the same
    std::swap(this->prob_delta, new_prob_delta);
    std::swap(this->prob_obs, new_prob_obs);
    std::swap(this->weight, new_weight);
    std::swap(this->initial_dist, new_initial_dist);
}

std::vector<int> POMDP::post(std::vector<int> sources, int action) {
    std::vector<int> result;
    for (std::map<std::tuple<int, int, int>, float>::iterator i =
            this->prob_delta.begin(); i != this->prob_delta.end(); ++i) {
        int s, a, t;
        std::tie(s, a, t) = i->first;
        if (i->second > 0 && action == a &&
                std::find(sources.begin(), sources.end(), s) != sources.end())
            result.push_back(t);
    }
    return result;
}

std::vector<float> POMDP::solveGameBeliefConstruction() {
    this->makeGameBeliefConstruction();
    this->print(std::cout);
    Game g;
    for (std::map<std::tuple<int, int, int>, float>::iterator i =
            this->prob_delta.begin(); i != this->prob_delta.end(); ++i) {
        int s, a, t;
        std::tie(s, a, t) = i->first;
        std::cout << this->weight[i->first] << std::endl;
        g.addTransition(s, a, t, this->weight[i->first]);
    }
    std::vector<float> result = g.solveGame(this->discount_factor);
    for (int i = 0; i < result.size(); i++)
        std::cout << "DS(" << this->states[i] << ") = " << result[i] << std::endl;
    return result;
}

void POMDP::makeGameBeliefConstruction() {
    assert(this->hasObsWeights());
    std::set<int> initial_states;
    bool addInitialObs = !this->hasInitialObs();
    if (addInitialObs)
        this->observations.push_back("init");
    for (std::map<int, float>::iterator I = this->initial_dist.begin();
            I != this->initial_dist.end(); ++I) {
        if (addInitialObs)
            this->prob_obs[std::make_tuple(I->first,
                                           this->observations.size() - 1)] =
                1.0;
        initial_states.insert(I->first);
    }
    // create a map from observations to their states
    std::map<int, std::vector<int> > states_per_obs;
    for (std::map<std::tuple<int, int>, float>::iterator i =
            this->prob_obs.begin(); i != this->prob_obs.end(); ++i) {
        int s, obs;
        std::tie(s, obs) = i->first;
        states_per_obs[obs].push_back(s);
    }
    // we will now add a new state for each subset of an observation,
    // also, we'll create an observation per state and link them
    std::vector<std::string> new_states;
    std::vector<std::string> new_observations;
    std::map<int, float> new_initial_dist;
    std::map<std::tuple<int, int>, float> new_prob_obs;
    int state_count = 0;
    for (std::map<int, std::vector<int> >::iterator i =
            states_per_obs.begin(); i != states_per_obs.end(); ++i) {
        int n = i->second.size();
        assert(n < 64); // because of the trick below
        for (unsigned long j = 1; j < (1 << n); j++) {
            bool is_initial = true;
            std::string name;
            name += "{";
            for (int k = 0; k < i->second.size(); k++) {
                unsigned long mask = 1 << k;
                if ((j & mask) == mask) {
                    name += " " + this->states[i->second[k]];
                    if (initial_states.find(i->second[k]) ==
                            initial_states.end())
                        is_initial = false;
                }
            }
            name += " } <= " + this->observations[i->first];
            new_states.push_back(name);
            new_observations.push_back(name);
            new_prob_obs[std::make_tuple(state_count, state_count)] = 1.0;
            if (is_initial)
                new_initial_dist[state_count] = 1.0;
            state_count++;
        }
    }
    // create the new prob_delta and weight functions
    std::map<std::tuple<int, int, int>, float> new_prob_delta;
    std::map<std::tuple<int, int, int>, float> new_weight;
    state_count = 0;
    for (std::map<int, std::vector<int> >::iterator i =
            states_per_obs.begin(); i != states_per_obs.end(); ++i) {
        int n = i->second.size();
        for (unsigned long j = 1; j < (1 << n); j++) {
            std::vector<int> states;
            for (int k = 0; k < i->second.size(); k++) {
                unsigned long mask = 1 << k;
                if ((j & mask) == mask)
                    states.push_back(i->second[k]);
            }
            for (int action = 0; action < this->actions.size(); action++) {
                std::vector<int> targets = this->post(states, action);
                std::sort(targets.begin(), targets.end());
                // now we have to find the observation subsets which match this
                // set
                int state_count2 = 0;
                for (std::map<int, std::vector<int> >::iterator i2 =
                        states_per_obs.begin(); i2 != states_per_obs.end(); ++i2) {
                    int n2 = i2->second.size();
                    std::sort(i2->second.begin(), i2->second.end());
                    std::vector<int> intersection;
                    std::set_intersection(targets.begin(), targets.end(),
                                          i2->second.begin(), i2->second.end(),
                                          std::back_inserter(intersection));
                    for (unsigned long j2 = 1; j2 < (1 << n2); j2++) {
                        std::vector<int> target_states;
                        for (int k2 = 0; k2 < i2->second.size(); k2++) {
                            unsigned long mask2 = 1 << k2;
                            if ((j2 & mask2) == mask2)
                                target_states.push_back(i2->second[k2]);
                        }
                        std::sort(target_states.begin(), target_states.end());
                        if (intersection == target_states) {
                            // we know that the set of targets in this
                            // observation matches the target_state set,
                            // now we need to add an observation-level
                            // transition and weight it according to any of the
                            // concrete transitions
                            new_prob_delta[std::make_tuple(state_count,
                                                           action,
                                                           state_count2)] = 1.0;
                            for (std::map<std::tuple<int,
                                    int, int>, float>::iterator iw =
                                    this->weight.begin();
                                    iw != this->weight.end(); ++iw) {
                                int s, a, t;
                                std::tie(s, a ,t) = iw->first;
                                if (a == action &&
                                        std::find(
                                            states.begin(),
                                            states.end(),
                                            s) != states.end() &&
                                        std::find(
                                            target_states.begin(),
                                            target_states.end(),
                                            t) != states.end()) {
                                    new_weight[std::make_tuple(state_count,
                                                               a,
                                                               state_count2)] =
                                        this->weight[std::make_tuple(s, a ,t)];
                                    break;
                                }
                            }
                        }
                        state_count2++;
                    }
                }
            }
            state_count++;
        }
    }
    // update all the internal variables
    std::swap(this->states, new_states);
    // actions stay the same
    std::swap(this->observations, new_observations);
    std::swap(this->prob_delta, new_prob_delta);
    std::swap(this->prob_obs, new_prob_obs);
    std::swap(this->weight, new_weight);
    std::swap(this->initial_dist, new_initial_dist);
}

void POMDP::print(std::ostream &o) {
    o << "Discount factor: " << this->discount_factor << std::endl;
    o << this->states.size() << " States: " << std::endl;
    for (std::vector<std::string>::iterator i = this->states.begin();
            i != this->states.end(); ++i)
        o << *i << std::endl;
    o << "Initial distribution: " << std::endl;
    for (std::map<int, float>::iterator i = this->initial_dist.begin();
            i != this->initial_dist.end(); ++i)
        o << this->states[i->first] << " with prob " << i->second << std::endl;
    o << this->actions.size() << " Actions: " << std::endl;
    for (std::vector<std::string>::iterator i = this->actions.begin();
            i != this->actions.end(); ++i)
        o << *i << std::endl;
    o << this->observations.size() << " Observations: " << std::endl;
    for (std::vector<std::string>::iterator i = this->observations.begin();
            i != this->observations.end(); ++i)
        o << *i << std::endl;
    o << "State-observation mapping: " << std::endl;
    for (std::map<std::tuple<int, int>, float>::iterator i =
            this->prob_obs.begin(); i != this->prob_obs.end(); ++i) {
        int s, obs;
        std::tie(s, obs) = i->first;
        o << this->states[s] << " observed as "
          << this->observations[obs] << " with prob " << i->second << std::endl;
    }
    o << this->prob_delta.size() << " Transitions: " << std::endl;
    for (std::map<std::tuple<int, int, int>, float>::iterator i =
            this->prob_delta.begin(); i != this->prob_delta.end(); ++i) {
        int s, a, t;
        std::tie(s, a, t) = i->first;
        o << "(" << this->states[s] << ","
          << this->actions[a] << ","
          << this->states[t] << ") with prob "
          << i->second << " and weight "
          << this->weight[i->first] << std::endl;
    }
}

int getId(const std::vector<std::string> &list,
          const std::map<std::string, int> &cache,
          const std::string &name) {
    std::map<std::string, int>::const_iterator i = cache.find(name);
    if (i != cache.end())
        return i->second;
    for (int j = 0; j < list.size(); j++)
        if (list[j] == name)
            return j;
    return -1;
}

int POMDP::getStateId(const std::string &name) {
    return getId(this->states, this->state_id_cache, name);
}

int POMDP::getActionId(const std::string &name) {
    return getId(this->actions, this->action_id_cache, name);
}

int POMDP::getObservationId(const std::string &name) {
    return getId(this->observations, this->observation_id_cache, name);
}

void POMDP::setInitialDist(std::map<int, float> dist) {
    this->initial_dist = dist;
}

// method to implement in order to interface with AI-Toolbox
AIToolbox::POMDP::Model<AIToolbox::MDP::Model> makeModel() {
    // for a sample implementation look at:
    // https://github.com/Svalorzen/AI-Toolbox/blob/master/test/POMDP/Utils/TigerProblem.hpp
}
