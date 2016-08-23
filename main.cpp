#include <assert.h>
#include <iostream>
#include <cmath>
#include <AIToolbox/POMDP/Algorithms/POMCP.hpp>
#include <AIToolbox/POMDP/Types.hpp>
#include <AIToolbox/POMDP/Policies/Policy.hpp>
#include <AIToolbox/POMDP/Utils.hpp>

#include "pomdp.h"

int main (int argc, char *argv[]) {
    if (argc < 2)
        exit(1);
    // recover the POMDP from file
    POMDP M(argv[1]);
    // check some of its properties to assert it is a valid MDP
    assert(M.isValidMdp());
    M.print(std::cout);
    // obtain the initial belief
    AIToolbox::POMDP::Belief new_belief, current_belief;
    current_belief = M.getInitialBelief();
    // obtain the worst-case value of the belief game
    std::cout << "Solving the game" << std::endl;
    POMDP N(M);
    N.solveGameBeliefConstruction();
    std::cout << "Done solving the game" << std::endl;
    // make the model in which we will simulate playing
    const long max_timestep = 1000;
    std::cout << "Start building AI-Toolbox model" << std::endl;
    auto model = M.makeModel();
    // create the model solver
    AIToolbox::POMDP::POMCP<decltype(model)> solver(
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
    action = solver.sampleAction(current_belief,
                                 1000); // horizon to plan for
    for (unsigned timestep = 0; timestep < max_timestep; ++timestep) {
        std::tie(new_state, new_obs, reward) =
            model.sampleSOR(current_state, action);
        std::cout << "played action " << action
                  << " from state " << current_state
                  << " and received state, obs, reward = "
                  << new_state << ", " << new_obs << ", "
                  << reward << std::endl;
        new_belief = AIToolbox::POMDP::updateBelief(model, current_belief,
                                                    action, new_obs);
        current_state = new_state;
        current_obs = new_obs;
        total_reward += std::pow(disc, timestep) * reward;
        action = solver.sampleAction(action, current_obs, max_timestep - timestep);
    }
    std::cout << "Obtained an accum reward of: " << total_reward << std::endl;
    exit(EXIT_SUCCESS);
}
