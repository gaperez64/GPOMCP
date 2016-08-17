#include <vector>
#include <set>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "z3++.h"
#include "dsgame.h"

using namespace z3;

Transition::Transition(int s, int a, int d, float w) {
    dest = d;
    action = a;
    source = s;
    weight = w;
}

int Transition::getSource() { return source; }

int Transition::getAction() { return action; }

int Transition::getDest() { return dest; }

float Transition::getWeight() { return weight; }

void Game::addTransition(Transition t) {
    int s = t.getSource();
    succ.resize(s + 1);
    int a = t.getAction();
    succ[s].resize(a + 1);
    succ[s][a].push_back(t);
}

void Game::addTransition(int s, int a, int d, float w) {
    addTransition(Transition(s, a, d, w));
}

void Game::addOrderVector(std::vector<int> order) {
    this->partial_orders.push_back(order);
}

std::set<int> Game::getStates() {
    std::set<int> s;
    for(unsigned i = 0; i < succ.size(); i++) {
        s.insert(i);
    }
    return s;
}

std::set<int> Game::availableActions(int state) {
    std::set<int> s;
    std::vector<std::list<Transition> > succ_state = succ[state];
    for(unsigned i = 0; i < succ_state.size(); i++) {
        if(succ_state[i].size() > 0)
            s.insert(i);
    }
    return s;
}

std::list<Transition> Game::post(int state, int action) {
    return succ[state][action];
}


expr fold(func_decl f, std::list<expr> e){
    std::list<expr>::iterator it = e.begin();
    expr res = *it;
    for(++it; it != e.end(); ++it)
        res = f(res, *it);
    return res;
}


expr disj(std::list<expr> e){
    std::list<expr>::iterator it = e.begin();
    expr res(*it);
    for(++it; it != e.end(); ++it)
        res = res || *it;
    return res;
}

std::vector<float> Game::solveGame(float discount_factor){
    char float_str [100];

    context c;

    sort R = c.real_sort();
    sort state_sort = c.int_sort();
    func_decl ds = c.function("ds", 1, &state_sort, R);

    func_decl max = c.function("max", R, R, R);
    func_decl min = c.function("min", R, R, R);
    expr lambda = c.real_const("lambda");
    expr x = c.real_const("x");
    expr y = c.real_const("y");

    std::set<int> states = this->getStates();
    solver s(c);
    sprintf(float_str, "%f", discount_factor);
    s.add(lambda == c.real_val(float_str));
    s.add(forall(x, y, max(x, y) == (ite(x < y, y, x))));
    s.add(forall(x, y, min(x, y) == (ite(x < y, x, y))));
    
    for (std::set<int>::iterator it = states.begin(); it != states.end(); ++it) {
        std::list<expr> state_expressions;
        std::set<int> actions = this->availableActions(*it);

        for (std::set<int>::iterator ait = actions.begin();
                ait != actions.end(); ++ait) {
            std::list<expr> state_action_expressions;
            std::list<Transition> successors = this->post(*it, *ait);

            for (std::list<Transition>::iterator sit = successors.begin();
                    sit != successors.end(); ++sit) {
                sprintf(float_str, "%f", sit->getWeight());
                state_action_expressions.push_back(c.real_val(float_str) +
                                                   (lambda * ds(sit->getDest())));
            }

            state_expressions.push_back(fold(min, state_action_expressions));
	    }

        s.add(ds(*it) == fold(max, state_expressions));
    }
    // we now add partial order information to help the SMT solver
    int opt_count = 0;
    if (this->partial_orders.size() > 0) {
        for (std::vector<std::vector<int> >::iterator i =
                this->partial_orders.begin(); i != this->partial_orders.end(); ++i) {
            std::vector<int> &order = *i;
            assert(order.size() >= 2);
            int prev = *(order.begin());
            for (std::vector<int>::iterator j = order.begin() + 1;
                    j != order.end(); ++j) {
                s.add(ds(*j) >= ds(prev));
                prev = *j;
                opt_count++;
            }
        }
    }
    std::cout << "Added " << opt_count << " additional inequalities" << std::endl;
    
    std::cout << "Equations : " << std::endl << s << "\n";
    assert(s.check() == sat);

    model m = s.get_model();
    //std::cout << "Model: " << std::endl << m << std::endl;

    set_param("pp.decimal", true);
    std::vector<float> result(states.size());
    for (std::set<int>::iterator it = states.begin(); it!= states.end(); ++it) {
        std::stringstream ss;
        ss << m.eval(ds(*it));
        result[*it] = std::stof(ss.str());
    }
    return result;        
}
