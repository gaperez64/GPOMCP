#include <cmath>
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

Transition::Transition(int s, int a, int d, double w) {
    dest = d;
    action = a;
    source = s;
    weight = w;
}

int Transition::getSource() { return source; }

int Transition::getAction() { return action; }

int Transition::getDest() { return dest; }

double Transition::getWeight() { return weight; }

void Game::addTransition(Transition t) {
    int s = t.getSource();
    succ.resize(s + 1);
    int a = t.getAction();
    succ[s].resize(a + 1);
    succ[s][a].push_back(t);
    if (std::abs(t.getWeight()) > this->biggest_weight)
        this->biggest_weight = std::abs(t.getWeight());
}

void Game::addTransition(int s, int a, int d, double w) {
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

std::vector<double> Game::solveGameSMT(double discount_factor) {
    char double_str [100];

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
    sprintf(double_str, "%f", discount_factor);
    s.add(lambda == c.real_val(double_str));
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
                sprintf(double_str, "%f", sit->getWeight());
                state_action_expressions.push_back(c.real_val(double_str) +
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
    std::vector<double> result(states.size());
    for (std::set<int>::iterator it = states.begin(); it != states.end(); ++it) {
        std::stringstream ss;
        ss << m.eval(ds(*it));
        result[*it] = std::stof(ss.str());
    }
    return result;        
}

long gcd(long a, long b) {
    if (a == 0)
        return b;
    else if (b == 0)
        return a;

    if (a < b)
        return gcd(a, b % a);
    else
        return gcd(b, a % b);
}

std::tuple<long, long> toFraction(double input) {
    assert(input < 1);
    double frac = input;

    const long precision = 1000000000; // This is the accuracy.

    long gcd_ = gcd(frac * precision, precision);

    long denominator = precision / gcd_;
    long numerator = (frac * precision) / gcd_;
    return std::make_tuple(numerator, denominator);

}

std::vector<double> Game::solveGameValIter(double discount_factor) {
    std::set<int> states = this->getStates();
    long numerator, denominator;
    std::tie(numerator, denominator) = toFraction(discount_factor);
    double temp = 1.0;
    for (int i = 0; i < states.size(); i++)
        temp *= (std::pow(denominator, i) - std::pow(numerator, i));
    double D = temp * std::pow(denominator, states.size());
    double I = 2 + (std::log(this->biggest_weight) / std::log(2))
                + (std::log(denominator) / std::log(2))
                * ((states.size() * (states.size() + 3)) / 2)
                * -1
                * (std::log(2) / std::log(discount_factor));
    long ITER = std::ceil(I);
    std::cout << "Value iteration requires "
              << ITER
              << " iterations" << std::endl;
        
    std::vector<double> value(states.size(), 0.0);

    bool converged = false;
    for (long i = 0; i < ITER; i++) {
        converged = true;
        for (std::set<int>::iterator it = states.begin(); it != states.end(); ++it) {
            std::set<int> actions = this->availableActions(*it);
            std::vector<double> s_values;
            for (std::set<int>::iterator ait = actions.begin();
                    ait != actions.end(); ++ait) {
                std::list<Transition> successors = this->post(*it, *ait);
                std::vector<double> sa_values;
                for (std::list<Transition>::iterator sit = successors.begin();
                        sit != successors.end(); ++sit)
                    sa_values.push_back(sit->getWeight()
                                        + discount_factor * value[sit->getDest()]);
                s_values.push_back(*std::min_element(sa_values.begin(),
                                                     sa_values.end()));
            }
            double new_value = *std::max_element(s_values.begin(),
                                                s_values.end());
            if (new_value != value[*it]) {
                converged = false;
                value[*it] = new_value;
            }
        }
        if (converged) {
            std::cout << "Early exit because of convergence at interation "
                      << i << std::endl;
            break;
        }

        if ((i + 1) % 10 == 0)
            std::cout << "Iteration No.: "
                      << i << std::endl;
    }
    if (!converged)
        for (int i = 0; i < value.size(); i++)
            value[i] = (D * value[i] + 0.5) / D;
    return value;
}

std::vector<double> Game::solveGame(double discount_factor) {
    std::vector<double> sol1 = solveGameValIter(discount_factor);
    //std::vector<double> sol2 = solveGameSMT(discount_factor);
    //for (int i = 0; i < sol1.size(); i++)
    //    if (sol1[i] != sol2[i])
    //        std::cout << sol1[i] << " != " << sol2[i] << std::endl;
    //assert(sol1 == sol2);
    return sol1;
}
