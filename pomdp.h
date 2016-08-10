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
        std::map<std::tuple<int, int>, float> prob_obs;
        std::map<std::tuple<int, int, int, int>, float> weight;
        std::map<int, float> initial_dist;
        float discount_factor;

    public:
        POMDP();
        POMDP(std::vector<std::string>,
              std::vector<std::string>,
              std::vector<std::string>,
              std::map<std::tuple<int, int, int>, float>,
              std::map<int, float>,
              std::map<std::tuple<int, int, int>, float>,
              std::map<int, float>,
              float);

        void addTransition(int, int, int, float, float);
        bool isValidMdp();
        bool hasObsWeights();
        bool hasDetObs();
        void makeObsDet();
        float getDiscFactor();
        void setDiscFactor(float);
}
