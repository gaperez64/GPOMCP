#include <list>
#include <vector>

class Transition {
    protected:
        int source;
        int action;
        int dest;
        double weight;

    public: 
        Transition(int s, int a, int d, double w);
        int getSource();
        int getAction();
        int getDest();
        double getWeight();
};

class Game {
    protected:
        std::vector<std::vector<std::list<Transition> > > succ;
        // each vector here tells us that all states j > i will have
        // a higher value than state i
        std::vector<std::vector<int> > partial_orders;
        double biggest_weight;

    public:
        void addTransition(Transition t);
        void addTransition(int s, int a, int d, double w = 0);
        void addOrderVector(std::vector<int> order);
        std::set<int> getStates();
        std::set<int> availableActions(int state);
        std::list<Transition> post(int state, int action);
        std::vector<double> solveGameSMT(double);
        std::vector<double> solveGameValIter(double);
        std::vector<double> solveGame(double);
};
