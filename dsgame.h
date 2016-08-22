#include <list>
#include <vector>

class Transition {
    protected:
        int source;
        int action;
        int dest;
        float weight;

    public: 
        Transition(int s, int a, int d, float w);
        int getSource();
        int getAction();
        int getDest();
        float getWeight();
};

class Game {
    protected:
        std::vector<std::vector<std::list<Transition> > > succ;
        // each vector here tells us that all states j > i will have
        // a higher value than state i
        std::vector<std::vector<int> > partial_orders;
        float biggest_weight;

    public:
        void addTransition(Transition t);
        void addTransition(int s, int a, int d, float w = 0);
        void addOrderVector(std::vector<int> order);
        std::set<int> getStates();
        std::set<int> availableActions(int state);
        std::list<Transition> post(int state, int action);
        std::vector<float> solveGameSMT(float);
        std::vector<float> solveGameValIter(float);
        std::vector<float> solveGame(float);
};
