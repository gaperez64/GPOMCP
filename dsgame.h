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
    std::vector<std::vector<std::list<Transition> > > succ;

    public:
        void addTransition(Transition t);
        void addTransition(int s, int a, int d, float w = 0);
        std::set<int> getStates();
        std::set<int> availableActions(int state);
        std::list<Transition> post(int state, int action);
        std::vector<float> solveGame(float);
};
