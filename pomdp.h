#ifndef POMDP_H
#define POMDP_H

#include <tuple>
#include <map>
#include <vector>
#include <string>
#include <iostream>

class POMDP {
    private:
        std::vector<std::string> states;
        std::vector<std::string> actions;
        std::vector<std::string> observations;
        std::map<std::tuple<int, int, int>, float> prob_delta;
        std::map<std::tuple<int, int>, float> prob_obs;
        std::map<std::tuple<int, int, int>, float> weight;
        std::map<int, float> initial_dist;
        float discount_factor;

        std::map<std::string, int> state_id_cache;
        std::map<std::string, int> action_id_cache;
        std::map<std::string, int> observation_id_cache;

        POMDP();

    public:
        POMDP(const std::string&);
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
        bool hasDetObs();
        void makeObsDet();
        float getDiscFactor();
        int getStateId(const std::string&);
        int getActionId(const std::string&);
        int getObservationId(const std::string&);
        void setInitialDist(std::map<int, float>);
        void setDiscFactor(float);
        void print(std::ostream&);
};

#endif // POMDP_H
