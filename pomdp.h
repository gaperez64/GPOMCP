#include <tuple>
#include <map>
#include <vector>
#include <string>

class POMDP {
    private:
        std::vector<std::string> states;
        std::vector<std::string> actions;
        std::vector<std::string> observations;
        std::map<std::tuple<int, int, int>, float> prob_delta;
        std::map<int, float> prob_obs;
        std::map<std::tuple<int, int, int>, float> weight;
        std::map<int, float> initial_dist;
        float discount_factor;

    public:
        POMDP();
        POMDP(std::vector<std::string> states,
              std::vector<std::string> actions,
              std::vector<std::string> observations,
              std::map<std::tuple<int, int, int>, float> prob_delta,
              std::map<int, float> prob_obs,
              std::map<std::tuple<int, int, int>, float> weight,
              std::map<int, float> initial_dist,
              float discount_factor);

        void addTransition(int source, int action, int target,
                           float prob, float weight);
        bool isValidMdp();
        bool hasObsWeights();
        bool hasDetObs();
        void makeObsDet();
}
