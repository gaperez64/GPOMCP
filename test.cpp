#include <assert.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <AIToolbox/POMDP/Algorithms/POMCP.hpp>
#include <AIToolbox/POMDP/Types.hpp>
#include <AIToolbox/POMDP/Utils.hpp>

#include "pomdp.h"
#include "BWCPOMCP.hpp"


int main (int argc, char *argv[]) {
    if (argc < 5) {
        std::cerr << "Expected input parameters: the input POMDP file name, "
                  << "the worst-case lower bound "
                  << " the horizon to plan for "
                  << " and the number of tests to approx the exp value " << std::endl;
        std::cerr << "Call format:" << std::endl;
        std::cerr << argv[0] << " filename bound horizon n_tests" << std::endl;
        exit(1);
    }
    // recover the POMDP from file and parse horizon
    const float threshold = std::atof(argv[2]);
    const long max_timestep = std::atoi(argv[3]);
    const long n_tests = std::atoi(argv[4]);
    BWC::POMDP M(argv[1]);
    // check some of its properties to assert it is a valid MDP
    M.print(std::cout);
    assert(M.isValidMdp());
    assert(M.hasObsWeights());
    // obtain the initial belief
    AIToolbox::POMDP::Belief initial_belief = M.getInitialBelief();
    // obtain the worst-case value of the belief game
    std::cout << "Solving the game" << std::endl;
    BWC::POMDP N(M);
    N.solveGameBeliefConstruction();
    std::cout << "Done solving the game" << std::endl;
    // make the model in which we will simulate playing
    std::cout << "Start building AI-Toolbox model" << std::endl;
    auto model = M.makeModel();
    // create the model solver
    // create the model solver
    AIToolbox::POMDP::BWCPOMCP<decltype(model)> solver(
            model,
            1000,         // size of initial particle belief
            10000,        // number of episodes to run before completion
            100000000.0,  // the exploration constant
            threshold,    // the worst-case threshold
            &M,           // reference to original BWC POMDP
            &N);          // reference to belief-based game
    // simulate the game
    float average_reward = 0;
    for (long i_test = 0; i_test < n_tests; i_test++) {
        AIToolbox::POMDP::Belief new_belief, current_belief;
        size_t current_state, new_state, action, current_obs, new_obs;
        float reward;
        float total_reward = 0;
        float disc = M.getDiscFactor();
        current_belief = initial_belief;
        current_state = M.sampleInitialState();
        current_obs = -1;
        action = solver.sampleAction(current_belief,
                                     max_timestep); // horizon to plan for
        for (unsigned timestep = 0; timestep < max_timestep; ++timestep) {
            std::tie(new_state, new_obs, reward) =
                model.sampleSOR(current_state, action);
            std::cout << "played action " << M.getActionName(action)
                      << " from state " << M.getStateName(current_state)
                      << " and received state, obs, reward = "
                      << M.getStateName(new_state)
                      << ", " << M.getObsName(new_obs) << ", "
                      << reward << std::endl;
            new_belief = AIToolbox::POMDP::updateBeliefUnnormalized(model,
                                                                    current_belief,
                                                                    action, new_obs);
            total_reward += std::pow(disc, timestep) * reward;
            current_state = new_state;
            current_obs = new_obs;
            current_belief = new_belief;
            action = solver.sampleAction(action, current_obs,
                                         max_timestep - (timestep + 1));
        }
        std::cout << "Obtained an accum reward of: " << total_reward << std::endl;
        average_reward += total_reward;
    }
    average_reward = average_reward / n_tests;
    std::cout << "Obtained, in average, an ccum reward of: " << average_reward << std::endl;
    exit(EXIT_SUCCESS);
}
