#ifndef POMDP_H
#define POMDP_H

#include <tuple>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "AIToolbox/MDP/Model.hpp"
#include "AIToolbox/POMDP/Model.hpp"

namespace BWC {

class POMDP {
    protected:
        std::vector<std::string> states;
        std::vector<std::string> actions;
        std::vector<std::string> observations;
        std::map<std::tuple<int, int, int>, double> prob_delta;
        std::map<std::tuple<int, int>, double> prob_obs;
        std::map<std::tuple<int, int, int>, double> weight;
        std::map<int, double> initial_dist;
        double discount_factor;
        // mapping from states to sets of states
        // for when we make a subset construction
        std::map<std::tuple<std::vector<int>, int>, int> states_id;
        std::vector<std::tuple<std::vector<int>, int> > inv_states_id;
        // mapping from states to antagonistic values
        std::vector<double> a_value;

        std::map<std::string, int> state_id_cache;
        std::map<std::string, int> action_id_cache;
        std::map<std::string, int> observation_id_cache;

        POMDP();
        std::vector<int> post(int, int);
        std::vector<int> post(std::vector<int>, int);

        // cache
        std::map<std::tuple<std::vector<int>, int, int>,
                 std::vector<int> > _post_in_obs_cache;
        std::map<std::tuple<std::vector<int>, int>,
                 std::vector<double> > _min_fval_cache;
    public:
        POMDP(const std::string&);
        POMDP(const POMDP &other);
        POMDP(std::vector<std::string>,
              std::vector<std::string>,
              std::vector<std::string>,
              std::map<std::tuple<int, int, int>, double>,
              std::map<std::tuple<int, int>, double>,
              std::map<std::tuple<int, int, int>, double>,
              std::map<int, double>,
              double);
        std::vector<int> postInObs(std::vector<int>, int, int);
        void setStates(std::vector<std::string>);
        std::string getStateName(int);
        std::string getActionName(int);
        std::string getObsName(int);
        int getStateCount();
        void setActions(std::vector<std::string>);
        int getActionCount();
        void setObservations(std::vector<std::string>);
        int sampleInitialState();
        AIToolbox::POMDP::Belief getInitialBelief();
        std::vector<int> getStatesInBelief(const AIToolbox::POMDP::Belief&, int);
        double getAValueOfBelief(std::vector<int>, int);
        std::vector<bool> getSafeActions(std::vector<int>, int, double);
        int sampleSafeActions(std::vector<int>, int, double);

        void addTransition(int, int, int, double);
        void addObservationProb(int, int, double);
        void weightTransition(int, int, int, double);
        bool isValidMdp();
        bool hasObsWeights();
        bool hasInitialObs();
        void makeGameBeliefConstruction();
        std::vector<double> solveGameBeliefConstruction();
        bool hasDetObs();
        void makeObsDet();
        double getDiscFactor();
        int getStateId(const std::string&);
        int getActionId(const std::string&);
        int getObservationId(const std::string&);
        void setInitialDist(std::map<int, double>);
        void setDiscFactor(double);
        void print(std::ostream&);

        // method to implement in order to interface with AI-Toolbox
        AIToolbox::POMDP::Model<AIToolbox::MDP::Model> makeModel();
};

}

#endif // POMDP_H
