#include <assert.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <AIToolbox/POMDP/Algorithms/POMCP.hpp>
#include <AIToolbox/POMDP/Types.hpp>
#include <AIToolbox/POMDP/Policies/Policy.hpp>
#include <AIToolbox/POMDP/Utils.hpp>

#include "pomdp.h"
#include "BWCPOMCP.hpp"


int main (int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Expected three input parameters: the input POMDP file name, "
                  << "the worst-case lower bound "
                  << "and the horizon to plan for" << std::endl;
        std::cerr << "Call format:" << std::endl;
        std::cerr << argv[0] << " filename bound horizon" << std::endl;
        exit(1);
    }
    // recover the POMDP from file and parse horizon
    const float threshold = std::atof(argv[2]);
    const long max_timestep = std::atoi(argv[3]);
    POMDP M(argv[1]);
    // check some of its properties to assert it is a valid MDP
    assert(M.isValidMdp());
    assert(M.hasObsWeights());
    // M.print(std::cout);
    // obtain the initial belief
    AIToolbox::POMDP::Belief new_belief, current_belief;
    current_belief = M.getInitialBelief();
    // obtain the worst-case value of the belief game
    std::cout << "Solving the game" << std::endl;
    POMDP N(M);
    N.solveGameBeliefConstruction();
    std::cout << "Done solving the game" << std::endl;
    // make the model in which we will simulate playing
    std::cout << "Start building AI-Toolbox model" << std::endl;
    auto model = M.makeModel();
    // create the model solver
    AIToolbox::POMDP::BWCPOMCP<decltype(model)> solver(
            model,
            1000,         // size of initial particle belief
            10000,        // number of episodes to run before completion
            100000000.0); // the exploration constant
    AIToolbox::POMDP::Policy policy(model.getS(), model.getA(), model.getO());
    // simulate the game
    size_t current_state, new_state, action, current_obs, new_obs;
    float reward;
    float total_reward = 0;
    float disc = M.getDiscFactor();
    current_state = M.sampleInitialState();
    current_obs = -1;
    solver.safe_actions =
        N.getSafeActions(M.getStatesInBelief(current_belief, current_obs),
                         current_obs, 0, 0, threshold);
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
        solver.safe_actions =
            N.getSafeActions(M.getStatesInBelief(current_belief, new_obs),
                             current_obs, timestep + 1, total_reward, threshold);
        action = solver.sampleAction(action, current_obs,
                                     max_timestep - (timestep + 1));
    }
    std::cout << "Obtained an accum reward of: " << total_reward << std::endl;
    exit(EXIT_SUCCESS);
}
