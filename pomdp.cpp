#include <chrono>
#include <random>
#include <cmath>
#include <tuple>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include "pomdp.h"
#include "dsgame.h"
#include "cassandra-driver.h"

BWC::POMDP::POMDP() : discount_factor(0.5) { }

BWC::POMDP::POMDP(const std::string &filename) : POMDP() {
    CassDriver driver(this);
    driver.parse(filename);
}

BWC::POMDP::POMDP(std::vector<std::string> S,
             std::vector<std::string> A,
             std::vector<std::string> O,
             std::map<std::tuple<int, int, int>, double> P,
             std::map<std::tuple<int, int>, double> PO,
             std::map<std::tuple<int, int, int>, double> W,
             std::map<int, double> I,
             double D) :
    states(S), actions(A), observations(O), prob_delta(P), prob_obs(PO),
    weight(W), initial_dist(I), discount_factor(D) { }

BWC::POMDP::POMDP(const POMDP &other) :
    POMDP(other.states, other.actions, other.observations, other.prob_delta,
          other.prob_obs, other.weight, other.initial_dist,
          other.discount_factor) { }

void BWC::POMDP::setStates(std::vector<std::string> S) {
    this->states = S;
}

std::string BWC::POMDP::getStateName(int i) {
    assert(i < this->states.size());
    return this->states[i];
}

std::string BWC::POMDP::getActionName(int i) {
    assert(i < this->actions.size());
    return this->actions[i];
}

std::string BWC::POMDP::getObsName(int i) {
    assert(i < this->observations.size());
    return this->observations[i];
}

int BWC::POMDP::getStateCount() {
    return this->states.size();
}

void BWC::POMDP::setActions(std::vector<std::string> A) {
    this->actions = A;
}

int BWC::POMDP::getActionCount() {
    return this->actions.size();
}

void BWC::POMDP::setObservations(std::vector<std::string> O) {
    this->observations = O;
}

void BWC::POMDP::addTransition(int source, int action, int target,
                          double prob) {
    assert(source < this->states.size());
    assert(target < this->states.size());
    this->prob_delta[std::make_tuple(source, action, target)] = prob;
}

void BWC::POMDP::addObservationProb(int state, int obs, double prob) {
    assert(state < this->states.size());
    assert(obs < this->observations.size());
    this->prob_obs[std::make_tuple(state, obs)] = prob;
}

void BWC::POMDP::weightTransition(int source, int action, int target,
                             double weight) {
    assert(source >=0 && source < this->states.size());
    assert(action >=0 && action < this->actions.size());
    assert(target >=0 && target < this->states.size());

    if (this->prob_delta.find(std::make_tuple(source, action, target)) ==
            this->prob_delta.end()) {
        // assert(false);
        return;
    }
    this->weight[std::make_tuple(source, action, target)] = weight;
}

void BWC::POMDP::setDiscFactor(double d) {
    this->discount_factor = d;
}

double BWC::POMDP::getDiscFactor() {
    return this->discount_factor;
}

bool BWC::POMDP::isValidMdp() {
    // check #1: transition probs should add to 1 for every state x action
    std::map<std::tuple<int, int>, double> state_action_prob;
    for (int s = 0; s < this->states.size(); s++)
        for (int a = 0; a < this->actions.size(); a++)
            state_action_prob[std::make_tuple(s, a)] = 0;

    for (std::map<std::tuple<int, int, int>, double>::iterator P =
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
    for (std::map<std::tuple<int, int, int>, double>::iterator i =
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
    std::map<std::tuple<int>, double> state_obs_prob;
    for (int s = 0; s < this->states.size(); s++)
        state_obs_prob[std::make_tuple(s)] = 0;

    for (std::map<std::tuple<int, int>, double>::iterator PO =
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

bool BWC::POMDP::hasObsWeights() {
    std::vector<std::set<int> > same_obs_states;
    same_obs_states.resize(this->observations.size());
    for (std::map<std::tuple<int, int>, double>::iterator PO =
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
            std::map<std::tuple<int, int, int>, double> obs_weight;
            for (std::map<std::tuple<int, int, int>, double>::iterator W =
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

bool BWC::POMDP::hasInitialObs() {
    std::vector<int> initial_obs;
    for (std::map<int, double>::iterator I = this->initial_dist.begin();
            I != this->initial_dist.end(); ++I)
        initial_obs.push_back(I->first);
    std::sort(initial_obs.begin(), initial_obs.end());

    std::vector<std::vector<int> > same_obs_states;
    same_obs_states.resize(this->observations.size());
    for (std::map<std::tuple<int, int>, double>::iterator PO =
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

bool BWC::POMDP::hasDetObs() {
    for (std::map<std::tuple<int, int>, double>::iterator PO =
            this->prob_obs.begin(); PO != this->prob_obs.end();
            ++PO)
        if (PO->second != 1.0)
            return false;
    return true;
}

void BWC::POMDP::makeObsDet() {
    std::vector<std::string> new_states;
    std::map<std::tuple<int, int>, double> new_prob_obs;
    std::vector<std::vector<int> > newstates_per_state;
    std::vector<std::vector<int> > obs_per_state;
    obs_per_state.resize(this->states.size());
    int i = 0;
    for (std::map<std::tuple<int, int>, double>::iterator PO =
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
    std::map<std::tuple<int, int, int>, double> new_prob_delta;
    std::map<std::tuple<int, int, int>, double> new_weight;
    for (std::map<std::tuple<int, int, int>, double>::iterator P =
            this->prob_delta.begin(); P != this->prob_delta.end(); ++P) {
        int s, a, t;
        std::tie(s, a, t) = P->first;
        std::vector<int>::iterator NS1 = newstates_per_state[s].begin();
        std::vector<int>::iterator OS1 = obs_per_state[s].begin();
        while (NS1 != newstates_per_state[s].end()) {
            std::vector<int>::iterator NS2 = newstates_per_state[t].begin();
            std::vector<int>::iterator OS2 = obs_per_state[t].begin();
            while (NS2 != newstates_per_state[t].end()) {
                double w = this->weight[std::make_tuple(s, a, t)];
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
    std::map<int, double> new_initial_dist;
    for (std::map<int, double>::iterator I = this->initial_dist.begin();
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

std::vector<int> BWC::POMDP::post(int source, int action) {
    std::vector<int> result;
    for (std::map<std::tuple<int, int, int>, double>::iterator i =
            this->prob_delta.begin(); i != this->prob_delta.end(); ++i) {
        int s, a, t;
        std::tie(s, a, t) = i->first;
        if (i->second > 0 && action == a && s == source)
            result.push_back(t);
    }
    return result;
}

std::vector<int> BWC::POMDP::post(std::vector<int> sources, int action) {
    std::vector<int> result;
    for (std::map<std::tuple<int, int, int>, double>::iterator i =
            this->prob_delta.begin(); i != this->prob_delta.end(); ++i) {
        int s, a, t;
        std::tie(s, a, t) = i->first;
        if (i->second > 0 && action == a &&
                std::find(sources.begin(), sources.end(), s) != sources.end())
            result.push_back(t);
    }
    return result;
}

std::vector<double> BWC::POMDP::solveGameBeliefConstruction() {
    this->makeGameBeliefConstruction();
    this->print(std::cout);
    Game g;
    // add all transitions to game
    for (std::map<std::tuple<int, int, int>, double>::iterator i =
            this->prob_delta.begin(); i != this->prob_delta.end(); ++i) {
        int s, a, t;
        std::tie(s, a, t) = i->first;
        g.addTransition(s, a, t, this->weight[i->first]);
    }
    // provide some extra information about subsets being related
    std::vector<std::vector<int> > order_vectors;
    for (std::map<std::tuple<std::vector<int>, int>, int>::iterator i =
            this->states_id.begin(); i != this->states_id.end(); ++i) {
        std::vector<int> states;
        int obs;
        std::tie(states, obs) = i->first;
        // ignore the -1 observation
        if (obs == -1)
            continue;
        std::sort(states.begin(), states.end());
        bool found = false;
        for (std::vector<std::vector<int> >::iterator order = order_vectors.begin();
                order != order_vectors.end(); ++order) {
            std::vector<int> head_vector;
            std::vector<int> tail_vector;
            int to;
            std::tie(head_vector, to) =
                this->inv_states_id[order->front()];
            std::sort(head_vector.begin(), head_vector.end());
            if (std::includes(states.begin(), states.end(),
                              head_vector.begin(), head_vector.end())) {
                order->insert(order->begin(), i->second);
                found = true;
                break;
            }
            std::tie(tail_vector, to) =
                this->inv_states_id[order->back()];
            std::sort(tail_vector.begin(), tail_vector.end());
            if (std::includes(tail_vector.begin(), tail_vector.end(),
                              states.begin(), states.end())) {
                order->push_back(i->second);
                found = true;
                break;
            }
        }
        if (!found) {
            std::vector<int> temp;
            temp.push_back(i->second);
            order_vectors.push_back(temp);
        }
    }
    for (int i = 0; i < order_vectors.size(); i++) {
        if (order_vectors[i].size() >= 2) {
            g.addOrderVector(order_vectors[i]);
            std::cout << "Order vector:" << std::endl;
            for (int j = 0; j < order_vectors[i].size(); j++) {
                std::cout << this->states[order_vectors[i][j]] << " => ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << "Calling solver!" << std::endl;
    // we now call the solver
    std::vector<double> result = g.solveGame(this->discount_factor);
    assert(result.size() == this->states.size());
    this->a_value.resize(result.size());
    for (int i = 0; i < result.size(); i++) {
        this->a_value[i] = result[i];
        std::cout << "DS(" << this->states[i] << ") = " << result[i] << std::endl;
    }
    return result;
}

std::vector<int> BWC::POMDP::postInObs(std::vector<int> current_states, int action, int obs) {
    assert(current_states.size() > 0);
    // first, check the cache
    auto hit = this->_post_in_obs_cache.find(std::make_tuple(current_states, action, obs));
    if (hit != this->_post_in_obs_cache.end()) {
        return hit->second;
    }

    // std::cout << "Post in obs for (a,o) = (" << action << "," << obs << ")" << std::endl;
    // we first collect all states with the given observation
    std::vector<int> states_in_obs;
    for (std::map<std::tuple<int, int>, double>::iterator i =
            this->prob_obs.begin(); i != this->prob_obs.end(); ++i) {
        int s, o;
        std::tie(s, o) = i->first;
        if (o == obs && i->second > 0.0)
            states_in_obs.push_back(s);
    }
    std::vector<int> target_states = this->post(current_states, action);
    std::sort(target_states.begin(), target_states.end());
    std::sort(states_in_obs.begin(), states_in_obs.end());
    std::vector<int> intersection;
    std::set_intersection(target_states.begin(),
                          target_states.end(),
                          states_in_obs.begin(),
                          states_in_obs.end(),
                          std::back_inserter(intersection));
    assert(intersection.size() > 0);
    
    // save into cache, and return
    this->_post_in_obs_cache[std::make_tuple(current_states, action, obs)] = intersection;

    return intersection;
}
                           

void BWC::POMDP::makeGameBeliefConstruction() {
    assert(this->hasObsWeights());
    std::vector<int> initial_states;
    std::string initial_states_name;
    initial_states_name += "{";
    for (std::map<int, double>::iterator I = this->initial_dist.begin();
            I != this->initial_dist.end(); ++I) {
        initial_states.push_back(I->first);
        initial_states_name += " " + this->states[I->first];
    }
    std::sort(initial_states.begin(), initial_states.end());
    initial_states_name += " } <= init";
    // create a map from observations to their states
    std::map<int, std::vector<int> > states_per_obs;
    for (std::map<std::tuple<int, int>, double>::iterator i =
            this->prob_obs.begin(); i != this->prob_obs.end(); ++i) {
        int s, obs;
        std::tie(s, obs) = i->first;
        states_per_obs[obs].push_back(s);
    }
    for (std::map<int, std::vector<int> >::iterator spo =
            states_per_obs.begin(); spo != states_per_obs.end();
            ++spo)
        std::sort(spo->second.begin(), spo->second.end());
    // we will now add a new state for each subset of an observation,
    // also, we'll create an observation per state and link them
    int state_count = 0;
    std::vector<std::string> new_states;
    std::vector<std::string> new_observations;
    // we keep track of the fact a new state = (list of states, obs)
    // TECH NOTE: because of how we will fill new_states_id, the vectors
    // in the keys of the map will be sorted
    std::map<std::tuple<std::vector<int>, int>, int> new_states_id;
    std::vector<std::tuple<std::vector<int>, int> > inv_new_states_id;
    std::map<int, double> new_initial_dist;
    std::map<std::tuple<int, int>, double> new_prob_obs;
    // first, we add the initial state
    new_states.push_back(initial_states_name);
    new_observations.push_back(initial_states_name);
    new_initial_dist[state_count] = 1.0;
    new_prob_obs[std::make_tuple(state_count, state_count)] = 1.0;
    // we call the initial obs -1 because... it is not a real obs
    new_states_id[std::make_tuple(initial_states, -1)] = state_count;
    inv_new_states_id.push_back(std::make_tuple(initial_states, -1));
    state_count++;
    assert(inv_new_states_id.size() == state_count);
    // we now add subsets by doing a DFS on the belief game
    std::map<std::tuple<int, int, int>, double> new_prob_delta;
    std::map<std::tuple<int, int, int>, double> new_weight;
    std::set<int> to_process;
    to_process.insert(0); // adding the initial state
    while (!to_process.empty()) {
        int current_new_state = *(to_process.begin());
        std::vector<int> current_states;
        int obs;
        std::tie(current_states, obs) = inv_new_states_id[current_new_state];
        for (int action = 0; action < this->actions.size(); action++) {
            std::vector<int> target_states = this->post(current_states, action);
            // it might be that the state does not have an action-successor
            if (target_states.size() == 0)
                continue;
            std::sort(target_states.begin(), target_states.end());
            for (std::map<int, std::vector<int> >::iterator spo =
                    states_per_obs.begin(); spo != states_per_obs.end();
                    ++spo) {
                std::vector<int> intersection;
                std::set_intersection(target_states.begin(),
                                      target_states.end(),
                                      spo->second.begin(), spo->second.end(),
                                      std::back_inserter(intersection));
                // if there is no action-successor in this obs, we continue
                if (intersection.size() == 0)
                    continue;
                std::map<std::tuple<std::vector<int>, int>, int>::iterator
                    found_key = new_states_id.find(
                            std::make_tuple(intersection, spo->first));
                int new_target;
                if (found_key == new_states_id.end()) { // does the new target exist?
                    std::string name;
                    name += "{";
                    for (std::vector<int>::iterator j = intersection.begin();
                            j != intersection.end(); ++j) 
                        name += " " + this->states[*j];
                    name += " } <= " + this->observations[spo->first];
                    new_states.push_back(name);
                    new_observations.push_back(name);
                    new_prob_obs[std::make_tuple(state_count, state_count)] = 1.0;
                    new_states_id[(std::make_tuple(intersection,
                                                   spo->first))] = state_count;
                    inv_new_states_id.push_back(std::make_tuple(intersection,
                                                                spo->first));
                    // recall that this is the id of the req. new state
                    new_target = state_count;
                    to_process.insert(state_count);
                    state_count++;
                    assert(inv_new_states_id.size() == state_count);
                } else
                    new_target = found_key->second;
                new_prob_delta[std::make_tuple(current_new_state,
                                               action,
                                               new_target)] = 1.0;
                for (std::map<std::tuple<int,
                        int, int>, double>::iterator iw =
                        this->weight.begin();
                        iw != this->weight.end(); ++iw) {
                    int s, a, t;
                    std::tie(s, a ,t) = iw->first;
                    if (a == action &&
                            std::find(
                                current_states.begin(),
                                current_states.end(),
                                s) != current_states.end() &&
                            std::find(
                                target_states.begin(),
                                target_states.end(),
                                t) != target_states.end()) {
                        new_weight[std::make_tuple(current_new_state, a,
                                                   new_target)] =
                            this->weight[std::make_tuple(s, a ,t)];
                        break;
                    }
                }
            }
        }
        to_process.erase(current_new_state);        
    }
    // update all the internal variables
    std::swap(this->states, new_states);
    // actions stay the same
    std::swap(this->observations, new_observations);
    std::swap(this->prob_delta, new_prob_delta);
    std::swap(this->prob_obs, new_prob_obs);
    std::swap(this->weight, new_weight);
    std::swap(this->initial_dist, new_initial_dist);
    // update mappings for subset constructions
    std::swap(this->states_id, new_states_id);
    std::swap(this->inv_states_id, inv_new_states_id);
}

void BWC::POMDP::print(std::ostream &o) {
    o << "Discount factor: " << this->discount_factor << std::endl;
    o << this->states.size() << " States: " << std::endl;
    for (std::vector<std::string>::iterator i = this->states.begin();
            i != this->states.end(); ++i)
        o << *i << std::endl;
    o << "Initial distribution: " << std::endl;
    for (std::map<int, double>::iterator i = this->initial_dist.begin();
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
    for (std::map<std::tuple<int, int>, double>::iterator i =
            this->prob_obs.begin(); i != this->prob_obs.end(); ++i) {
        int s, obs;
        std::tie(s, obs) = i->first;
        o << this->states[s] << " observed as "
          << this->observations[obs] << " with prob " << i->second << std::endl;
    }
    o << this->prob_delta.size() << " Transitions: " << std::endl;
    for (std::map<std::tuple<int, int, int>, double>::iterator i =
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

int BWC::POMDP::getStateId(const std::string &name) {
    return getId(this->states, this->state_id_cache, name);
}

int BWC::POMDP::getActionId(const std::string &name) {
    return getId(this->actions, this->action_id_cache, name);
}

int BWC::POMDP::getObservationId(const std::string &name) {
    return getId(this->observations, this->observation_id_cache, name);
}

void BWC::POMDP::setInitialDist(std::map<int, double> dist) {
    this->initial_dist = dist;
}

// method to implement in order to interface with AI-Toolbox
AIToolbox::POMDP::Model<AIToolbox::MDP::Model> BWC::POMDP::makeModel() {
    size_t S, A, O;
    S = this->getStateCount();
    A = this->getActionCount();
    O = this->observations.size();
    AIToolbox::POMDP::Model<AIToolbox::MDP::Model> model(O, S, A);
    
    double d = this->getDiscFactor(); // is this good conversion?
    model.setDiscount(d);
    
    AIToolbox::Table3D transitions(boost::extents[S][A][S]);
    AIToolbox::Table3D rewards(boost::extents[S][A][S]);
    AIToolbox::Table3D local_observations(boost::extents[S][A][O]);
    
    for (int s = 0; s < S; ++s) {
    	for (int a = 0; a < A; ++a) {
    		for (int s1 = 0; s1 < S; ++s1) {
                if (this->prob_delta.find(std::make_tuple(s, a, s1)) ==
                        this->prob_delta.end()) {
                    transitions[s][a][s1] = 0.0;
                } else {
    			    transitions[s][a][s1] =
                        this->prob_delta[std::make_tuple(s, a, s1)];
                }
			}
		} 
	} 
	model.setTransitionFunction(transitions);
	
	for (int s = 0; s < S; ++s) {
		for (int a = 0; a < A; ++a) {
			for (int o = 0; o < O; ++o) {
                if (this->prob_obs.find(std::make_tuple(s, o)) ==
                        this->prob_obs.end())
                    local_observations[s][a][o] = 0.0;
                else
				    local_observations[s][a][o] =
                        this->prob_obs[std::make_tuple(s, o)];
			} 
		}
	}
    model.setObservationFunction(local_observations);
	
	for (int s = 0; s < S; ++s) {
		for (int a = 0; a < A; ++a) {
			for (int s1 = 0; s1 < S; ++s1) {
                if (this->weight.find(std::make_tuple(s, a, s1)) ==
                        this->weight.end())
                    rewards[s][a][s1] = 0.0;
                else
    			    rewards[s][a][s1] =
                        this->weight[std::make_tuple(s, a, s1)];
			}
		}
	}
    model.setRewardFunction(rewards);
    
    return model;
}

int BWC::POMDP::sampleInitialState() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::vector<double> probs;
    std::vector<int> prob_state;
    for (std::map<int, double>::iterator i = this->initial_dist.begin();
            i != this->initial_dist.end(); ++i) {
        probs.push_back(i->second);
        prob_state.push_back(i->first);
    }
    std::discrete_distribution<int> dist(probs.begin(), probs.end());
    int result = prob_state[dist(generator)];
    std::cout << "Randomly chosen initial state: " << this->states[result]
              << std::endl;
    return result;
}

AIToolbox::POMDP::Belief BWC::POMDP::getInitialBelief() {
    AIToolbox::POMDP::Belief initial(this->states.size());
    for (int i = 0; i < this->states.size(); i++) {
        if (this->initial_dist.find(i) == this->initial_dist.end())
            initial(i, 0) = 0.0;
        else
            initial(i, 0) = this->initial_dist[i];
    }
    return initial;
}

std::vector<int> BWC::POMDP::getStatesInBelief(const AIToolbox::POMDP::Belief &b, int obs) {
    // this vector will be, by construction, ordered
    std::vector<int> states_in_belief;
    std::cout << "Found states: ";
    for (int i = 0; i < this->states.size(); i++)
        if (b(i, 0) > 0.0) {
            states_in_belief.push_back(i);
            std::cout << "(" << i << ")" << this->states[i] << " ";
        }
    std::cout << std::endl;
    assert(states_in_belief.size() > 0); // otherwise we fucked up
    if (obs > -1) {
        std::cout << "Observation = " << this->observations[obs] << std::endl;
    }
    return states_in_belief;
}

double BWC::POMDP::getAValueOfBelief(std::vector<int> states_in_belief, int obs) {
    std::map<std::tuple<std::vector<int>, int>, int>::iterator i =
        this->states_id.find(std::make_tuple(states_in_belief, obs));
    if (i != this->states_id.end())
        return this->a_value[i->second];
    // If this is reached then either you have not computed the a_value vector
    // or you have provided a belief vector which is not a state in the current
    // belief construction (together with the obs)
    assert(false);
    return 0;
}

int BWC::POMDP::sampleSafeActions(std::vector<int> states_in_belief,
                                  int obs, double remain) {
    std::vector<bool> safe = this->getSafeActions(states_in_belief,
                                                  obs, remain);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::vector<double> probs;
    std::vector<int> prob_action;
    for (int i = 0; i < safe.size(); i ++)
        if (safe[i])
            prob_action.push_back(i);
    for (int i = 0; i < prob_action.size(); i++)
        probs.push_back(1.0 / prob_action.size());
    std::discrete_distribution<int> dist(probs.begin(), probs.end());
    int result = prob_action[dist(generator)];
    // std::cout << "Randomly chosen safe action: " << this->actions[result]
    //           << std::endl;
    return result;
}

std::vector<bool> BWC::POMDP::getSafeActions(std::vector<int> states_in_belief,
                                             int obs, double remain) {
    assert(states_in_belief.size() > 0);
    std::vector<bool> safe(this->actions.size(), true);

    // we now recover the worst (minimal) outcome of each action
    std::vector<double> min_vals;
    auto hit = this->_min_fval_cache.find(std::make_tuple(states_in_belief, obs));
    if (hit != this->_min_fval_cache.end()) {
        min_vals = hit->second;
    } else {
        std::map<std::tuple<std::vector<int>, int>, int>::iterator i =
            this->states_id.find(std::make_tuple(states_in_belief, obs));
        if (i == this->states_id.end()) {
            std::cout << "Failed to find the key: ( ";
            for (auto j = states_in_belief.begin(); j != states_in_belief.end(); ++j)
                std::cout << *j << " ";
            std::cout << "), " << obs << std::endl;
            std::cout << "Possibilities:" << std::endl;
            for (i = this->states_id.begin(); i != this->states_id.end(); ++i) {
                std::vector<int> vec;
                int o;
                std::tie(vec, o) = i->first;
                std::cout << "key: ( ";
                for (auto j = vec.begin(); j != vec.end(); ++j)
                    std::cout << *j << " ";
                std::cout << "), " << o;
                std::cout << std::endl;
            }
            // If this is reached then either you have not computed the a_value vector
            // or you have provided a belief vector which is not a state in the current
            // belief construction (together with the obs)
            assert(false);
            return std::vector<bool>();
        }

        // std::cout << "Computing safe actions with (aVal,remain) = ("
        //           << this->a_value[i->second] << "," << remain
        //           << ")" << std::endl;
        assert(this->a_value[i->second] >= remain);

        min_vals.assign(this->actions.size(), this->a_value[i->second]);
        for (int a = 0; a < this->actions.size(); a++) {
            std::vector<int> successors = this->post(i->second, a);
            for (std::vector<int>::iterator j = successors.begin();
                    j != successors.end(); ++j)
                min_vals[a] = std::min(min_vals[a], 
                    (this->weight[std::make_tuple(i->second, a, *j)] +
                     this->a_value[*j] * this->discount_factor));
        }
        // cache this information
        this->_min_fval_cache[std::make_tuple(states_in_belief, obs)] = min_vals;
    }

    for (int a = 0; a < this->actions.size(); a++) {
        safe[a] = (min_vals[a] >= remain);
        // if (!safe[a])
        //     std::cout << "Action (" << a << ") " << this->actions[a]
        //               << " is not safe to play now" << std::endl;
    }
    return safe;
}
