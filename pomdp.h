#ifndef POMDP_H
#define POMDP_H

#include <tuple>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "AIToolbox/MDP/Model.hpp"
#include "AIToolbox/POMDP/Model.hpp"

class POMDP {
    protected:
        std::vector<std::string> states;
        std::vector<std::string> actions;
        std::vector<std::string> observations;
        std::map<std::tuple<int, int, int>, float> prob_delta;
        std::map<std::tuple<int, int>, float> prob_obs;
        std::map<std::tuple<int, int, int>, float> weight;
        std::map<int, float> initial_dist;
        float discount_factor;
        // mapping from states to sets of states
        // for when we make a subset construction
        std::map<std::tuple<std::vector<int>, int>, int> states_id;
        std::vector<std::tuple<std::vector<int>, int> > inv_states_id;

        std::map<std::string, int> state_id_cache;
        std::map<std::string, int> action_id_cache;
        std::map<std::string, int> observation_id_cache;

        POMDP();
        std::vector<int> post(std::vector<int>, int);

    public:
        POMDP(const std::string&);
        POMDP(const POMDP &other);
        POMDP(std::vector<std::string>,
              std::vector<std::string>,
              std::vector<std::string>,
              std::map<std::tuple<int, int, int>, float>,
              std::map<std::tuple<int, int>, float>,
              std::map<std::tuple<int, int, int>, float>,
              std::map<int, float>,
              float);
        void setStates(std::vector<std::string>);
        int getStateCount();
        void setActions(std::vector<std::string>);
        int getActionCount();
        void setObservations(std::vector<std::string>);

        void addTransition(int, int, int, float);
        void addObservationProb(int, int, float);
        void weightTransition(int, int, int, float);
        bool isValidMdp();
        bool hasObsWeights();
        bool hasInitialObs();
        void makeGameBeliefConstruction();
        std::vector<float> solveGameBeliefConstruction();
        bool hasDetObs();
        void makeObsDet();
        float getDiscFactor();
        int getStateId(const std::string&);
        int getActionId(const std::string&);
        int getObservationId(const std::string&);
        void setInitialDist(std::map<int, float>);
        void setDiscFactor(float);
        void print(std::ostream&);

        // method to implement in order to interface with AI-Toolbox
        AIToolbox::POMDP::Model<AIToolbox::MDP::Model> makeModel();
};

#endif // POMDP_H
