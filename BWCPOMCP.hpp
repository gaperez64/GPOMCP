#ifndef AI_TOOLBOX_POMDP_BWCPOMCP_HEADER_FILE
#define AI_TOOLBOX_POMDP_BWCPOMCP_HEADER_FILE

#include <unordered_map>
#include <iostream>

#include <AIToolbox/POMDP/Types.hpp>
#include <AIToolbox/ProbabilityUtils.hpp>
#include <AIToolbox/Impl/Seeder.hpp>

#include "pomdp.h"

namespace AIToolbox {
    namespace POMDP {

#ifndef DOXYGEN_SKIP
        // This is done to avoid bringing around the enable_if everywhere.
        template <typename M, typename = typename std::enable_if<is_generative_model<M>::value>::type>
        class BWCPOMCP;
#endif

        /**
         * @brief This class represents the BWCPOMCP online planner using UCB1.
         *
         * This algorithm is an online planner for POMDPs. As an online
         * planner, it needs to have a generative model of the problem. This
         * means that it only needs a way to sample transitions, observations
         * and rewards from the model, but it does not need to know directly
         * the distribution probabilities for them.
         *
         * BWCPOMCP plans for a single belief at a time. It follows the logic of
         * Monte Carlo Tree Sampling, where a tree structure is build
         * progressively and action values are deduced as averages of the
         * obtained rewards over rollouts. If the number of sample episodes is
         * high enough, it is guaranteed to converge to the optimal solution.
         *
         * At each rollout, we follow each action and observation within the
         * tree from root to leaves. During this path we chose actions using an
         * algorithm called UCT. What this does is privilege the most promising
         * actions, while guaranteeing that in the limit every action will still
         * be tried an infinite amount of times.
         *
         * Once we arrive to a leaf in the tree, we then expand it with a
         * single new node, representing a new observation we just collected.
         * We then proceed outside the tree following a random policy, but this
         * time we do not track which actions and observations we actually
         * take/obtain. The final reward obtained by this random rollout policy
         * is used to approximate the values for all nodes visited in this
         * rollout inside the tree, before leaving it.
         *
         * Since BWCPOMCP expands a tree, it can reuse work it has done if
         * multiple action requests are done in order. To do so, it simply asks
         * for the action that has been performed and its respective obtained
         * observation. Then it simply makes that root branch the new root, and
         * starts again.
         *
         * In order to avoid performing belief updates between each
         * action/observation pair, which can be expensive, BWCPOMCP uses particle
         * beliefs. These approximate the beliefs at every step, and are used
         * to select states in the rollouts.
         *
         * A weakness of this implementation is that, as every particle
         * approximation of continuous values, it will lose particles in time.
         * To fight this a possibility is to implement a particle
         * reinvigoration method, which would introduce noise in the particle
         * beliefs in order to keep them "fresh" (possibly using domain
         * knowledge).
         */
        template <typename M>
        class BWCPOMCP<M> {
            public:
                using SampleBelief = std::vector<size_t>;

                struct BeliefNode;
                using BeliefNodes = std::unordered_map<size_t, BeliefNode>;

                struct ActionNode {
                    BeliefNodes children;
                    double V = 0.0;
                    unsigned N = 0;
                };
                using ActionNodes = std::vector<ActionNode>;

                struct BeliefNode {
                    BeliefNode() : N(0) {}
                    BeliefNode(size_t s) : belief(1, s), N(0) {}
                    BeliefNode(size_t s, double r, int o) : belief(1, s), N(0),
                                                            rem(r), obs(o) {}
                    ActionNodes children;
                    std::vector<int> support;
                    SampleBelief belief;
                    unsigned N;
                    double rem = 0.0;
                    int obs = -1;
                    std::vector<bool> _safe_cache;
                };

                /**
                 * @brief Basic constructor.
                 *
                 * @param m The POMDP model that BWCPOMCP will operate upon.
                 * @param beliefSize The size of the initial particle belief.
                 * @param iterations The number of episodes to run before completion.
                 * @param exp The exploration constant. This parameter is VERY
                 * important to determine the final BWCPOMCP performance.
                 * @param t The threshold for the worst-case value
                 */
                BWCPOMCP(const M& m, size_t beliefSize, unsigned iterations, double exp,
                         double t, BWC::POMDP* p, BWC::POMDP* b);

                /**
                 * @brief This function resets the internal graph and samples for the provided belief and horizon.
                 *
                 * In general it would be better if the belief did not contain
                 * any terminal states; although not necessary, it would
                 * prevent unnecessary work from being performed.
                 *
                 * @param b The initial belief for the environment.
                 * @param horizon The horizon to plan for.
                 *
                 * @return The best action.
                 */
                size_t sampleAction(const Belief& b, unsigned horizon);

                /**
                 * @brief This function uses the internal graph to plan.
                 *
                 * This function can be called after a previous call to
                 * sampleAction with a Belief. Otherwise, it will invoke it
                 * anyway with a random belief.
                 *
                 * If a graph is already present though, this function will
                 * select the branch defined by the input action and
                 * observation, and prune the rest. The search will be started
                 * using the existing graph: this should make search faster,
                 * and also not require any belief updates.
                 *
                 * NOTE: Currently there is no particle reinvigoration
                 * implemented, so for long horizons you can expect
                 * progressively degrading performances.
                 *
                 * @param a The action taken in the last timestep.
                 * @param o The observation received in the last timestep.
                 * @param horizon The horizon to plan for.
                 *
                 * @return The best action.
                 */
                size_t sampleAction(size_t a, size_t o, unsigned horizon);

                /**
                 * @brief This function sets the new size for initial beliefs created from sampleAction().
                 *
                 * Note that this parameter does not bound particle beliefs
                 * created within the tree by result of rollouts: only the ones
                 * directly created from true Beliefs.
                 *
                 * @param beliefSize The new particle belief size.
                 */
                void setBeliefSize(size_t beliefSize);

                /**
                 * @brief This function sets the number of performed rollouts in BWCPOMCP.
                 *
                 * @param iter The new number of rollouts.
                 */
                void setIterations(unsigned iter);

                /**
                 * @brief This function sets the new exploration constant for BWCPOMCP.
                 *
                 * This parameter is EXTREMELY important to determine BWCPOMCP
                 * performance and, ultimately, convergence. In general it is
                 * better to find it empirically, by testing some values and
                 * see which one performs best. Tune this parameter, it really
                 * matters!
                 *
                 * @param exp The new exploration constant.
                 */
                void setExploration(double exp);

                /**
                 * @brief This function returns the POMDP generative model being used.
                 *
                 * @return The POMDP generative model.
                 */
                const M& getModel() const;

                /**
                 * @brief This function returns a reference to the internal
                 * graph structure holding the results of rollouts.
                 *
                 * @return The internal graph.
                 */
                const BeliefNode& getGraph() const;

                /**
                 * @brief This function returns the initial particle size for converted Beliefs.
                 *
                 * @return The initial particle count.
                 */
                size_t getBeliefSize() const;

                /**
                 * @brief This function returns the number of iterations performed to plan for an action.
                 *
                 * @return The number of iterations.
                 */
                unsigned getIterations() const;

                /**
                 * @brief This function returns the currently set exploration constant.
                 *
                 * @return The exploration constant.
                 */
                double getExploration() const;

            private:
                const M& model_;
                size_t S, A, beliefSize_;
                unsigned iterations_, maxDepth_;
                double exploration_;
                double threshold_;
                BWC::POMDP* pomdp_;
                BWC::POMDP* belief_game_;

                SampleBelief sampleBelief_;
                BeliefNode graph_;

                mutable std::default_random_engine rand_;

                /**
                 * @brief This function starts the simulation process.
                 *
                 * This function simply calls simulate() for the number of
                 * times specified by BWCPOMCP's parameters. While doing so it
                 * builds a tree of explored outcomes, from which BWCPOMCP will
                 * then extract the best expected action for the current
                 * belief.
                 *
                 * @param horizon The horizon for which to plan.
                 *
                 * @return The best action to take given the final built tree.
                 */
                size_t runSimulation(unsigned horizon);

                /**
                 * @brief This function recursively simulates the model while building the tree.
                 *
                 * From the given belief node, state and horizon, this function
                 * selects an action based on UCT (so that estimated good
                 * actions are taken more often than estimated bad actions) and
                 * samples a new state, observation and reward. Based on the
                 * observation, the function detects whether it is at the end
                 * of the tree or not. If it is, it adds a new node to the tree
                 * and rollouts the rest of the episode. Otherwise it
                 * recursively traverses the tree.
                 *
                 * The states and rewards obtained on the way are used to
                 * update particle beliefs within the tree and the value
                 * estimations for those beliefs.
                 *
                 * @param b The tree node to simulate from.
                 * @param s The state from which we are simulating, possibly a particle of a previous particle belief.
                 * @param horizon The depth within the tree already reached.
                 *
                 * @return The discounted reward obtained from the simulation performed from here to the end.
                 */
                double simulate(BeliefNode & b, size_t s, unsigned horizon);

                /**
                 * @brief This function implements the rollout policy for BWCPOMCP.
                 *
                 * This function extracts some cumulative reward from a
                 * particular state, given that we have reached a particular
                 * horizon. The idea behind this function is to approximate the
                 * true value of the state; since this function is called when
                 * we are at the leaves of our tree, the only way for us to
                 * extract more information is to simply simulate the rest of
                 * the episode directly.
                 *
                 * However, in order to speed up the process and store only
                 * useful data, we avoid inserting every single state that we
                 * see here into the tree, preferring to add a single state at
                 * a time. This avoids wasting lots of computation and memory
                 * on states far from our root that we will probably never see
                 * again, while at the same time still getting an estimate for
                 * the rest of the simulation.
                 *
                 * @param s The state from which to start the rollout.
                 * @param horizon The horizon already reached while simulating inside the tree.
                 *
                 * @return An estimate return computed from simulating until max depth.
                 */
                double rollout(const BeliefNode &b, size_t s, unsigned horizon);


                /**
                 * @brief This function finds the best action based on value.
                 */
                size_t findBestA(BeliefNode &b);

                /**
                 * @brief This function finds the best action based on UCT.
                 *
                 * UCT gives a bonus to actions that have been tried very few
                 * times, in order to void thinking that a bad action is bad
                 * just because it got unlucky the few times that it tried it.
                 */
                size_t findBestBonusA(BeliefNode &b);

                /**
                 * @brief This function samples a given belief in order to produce a particle approximation of it.
                 *
                 * @param b The belief to be approximated.
                 *
                 * @return A particle belief approximating the input belief.
                 */
                SampleBelief makeSampledBelief(const Belief & b);
        };

        template <typename M>
        BWCPOMCP<M>::BWCPOMCP(const M& m, size_t beliefSize,
                              unsigned iter, double exp, double t,
                              BWC::POMDP* p,
                              BWC::POMDP* b) : model_(m),
                                               S(model_.getS()),
                                               A(model_.getA()),
                                               beliefSize_(beliefSize),
                                               iterations_(iter),
                                               exploration_(exp),
                                               graph_(),
                                               rand_(Impl::Seeder::getSeed()),
                                               threshold_(t),
                                               pomdp_(p),
                                               belief_game_(b) {}

        template <typename M>
        size_t BWCPOMCP<M>::sampleAction(const Belief& b, unsigned horizon) {
            // Reset graph
            graph_ = BeliefNode(A);
            graph_.children.resize(A);
            graph_.belief = makeSampledBelief(b);
            graph_.rem = threshold_;
            graph_.support = pomdp_->getStatesInBelief(b, -1);

            return runSimulation(horizon);
        }

        template <typename M>
        size_t BWCPOMCP<M>::sampleAction(size_t a, size_t o, unsigned horizon) {
            auto & obs = graph_.children[a].children;

            auto it = obs.find(o);
            if ( it == obs.end() ) {
                std::cerr << "Observation " << o
                          << " never experienced in simulation,"
                          << " restarting with uniform belief..\n";
                auto b = Belief(S);
                b.fill(1.0/S);
                return sampleAction(b, horizon);
            }

            // Here we need an additional step, because *it is contained by graph_.
            // If we just move assign, graph_ is first going to delete everything it
            // contains (included *it), and then we are going to move unallocated memory
            // into graph_! So we move *it outside of the graph_ hierarchy, so that
            // we can then assign safely.
            { auto tmp = std::move(it->second); graph_ = std::move(tmp); }

            if ( ! graph_.belief.size() ) {
                std::cerr << "BWCPOMCP Lost track of the belief, restarting with uniform..\n";
                auto b = Belief(S); b.fill(1.0/S);
                return sampleAction(b, horizon);
            }

            // We resize here in case we didn't have time to sample the new
            // head node. In this case, the new head may not have children.
            // This would break the UCT call.
            graph_.children.resize(A);

            return runSimulation(horizon);
        }

        template <typename M>
        size_t BWCPOMCP<M>::runSimulation(unsigned horizon) {
            if ( !horizon ) return 0;

            maxDepth_ = horizon;
            std::uniform_int_distribution<size_t> generator(0, graph_.belief.size()-1);

            for (unsigned i = 0; i < iterations_; ++i ) {
                simulate(graph_, graph_.belief.at(generator(rand_)), 0);
                // std::cout << "FULL SIMULATION COMPLETED!" << std::endl;
            }

            return findBestA(graph_);
        }

        template <typename M>
        double BWCPOMCP<M>::simulate(BeliefNode & b, size_t s, unsigned depth) {
            b.N++;

            size_t a = findBestBonusA(b);

            size_t s1, o; double rew;
            std::tie(s1, o, rew) = model_.sampleSOR(s, a);
            // std::cout << "SIM: played " << a << " and ontained observation " << o
            //           << " with reward " << rew << std::endl;
            double rem = (b.rem - rew) / model_.getDiscount();
            // std::cout << "SIM: new remainder = " << rem << std::endl;

            auto & aNode = b.children[a];

            {
                double futureRew = 0.0;
                // We need to append the node anyway to perform the belief
                // update for the next timestep.
                auto ot = aNode.children.find(o);
                if ( ot == std::end(aNode.children) ) {
                    aNode.children.emplace(std::piecewise_construct,
                                           std::forward_as_tuple(o),
                                           std::forward_as_tuple(s1, rem, o));
                    ot = aNode.children.find(o);
                    ot->second.support = pomdp_->postInObs(b.support, a, o);
                    // This stops automatically if we go out of depth
                    futureRew = rollout(ot->second, s1, depth + 1);
                }
                else {
                    ot->second.belief.push_back(s1);
                    // We only go deeper if needed (maxDepth_ is always at least 1).
                    if ( depth + 1 < maxDepth_ && !model_.isTerminal(s1) ) {
                        // Since most memory is allocated on the leaves,
                        // we do not allocate on node creation but only when
                        // we are actually descending into a node. If the node
                        // already has memory this should not do anything in
                        // any case.
                        ot->second.children.resize(A);
                        futureRew = simulate( ot->second, s1, depth + 1 );
                    }
                    // here, one would make ot->second update its rem to take
                    // the min between what it has and the local variable rem,
                    // however, for observable weights this is not necessary
                }

                rew += model_.getDiscount() * futureRew;
            }

            // Action update
            aNode.N++;
            aNode.V += ( rew - aNode.V ) / static_cast<double>(aNode.N);

            return rew;
        }

        template <typename M>
        double BWCPOMCP<M>::rollout(const BeliefNode &b, size_t s, unsigned depth) {
            double rew = 0.0, totalRew = 0.0, gamma = 1.0;
#if true
            // rolling out unsafely
            std::uniform_int_distribution<size_t> generator(0, A-1);
            for ( ; depth < maxDepth_; ++depth ) {
                std::tie( s, rew ) = model_.sampleSR( s, generator(rand_) );

                totalRew += gamma * rew;
                gamma *= model_.getDiscount();
            }
            return totalRew;
#endif
            // rolling out safely
            double rem = b.rem;
            size_t o = b.obs;
            std::vector<int> support = b.support;

            for ( ; depth < maxDepth_; ++depth ) {
                size_t a = belief_game_->sampleSafeActions(support, o, rem);
                std::tie( s, o, rew ) = model_.sampleSOR( s, a );
                
                rem = (rem - rew) / model_.getDiscount();
                support = pomdp_->postInObs(support, a, o);

                totalRew += gamma * rew;
                gamma *= model_.getDiscount();
            }
            return totalRew;
        }

        template <typename M>
        size_t BWCPOMCP<M>::findBestA(BeliefNode &b) {
            // std::cout << "instrumented findBestA called" << std::endl;
            std::vector<bool> safe;
            if (b._safe_cache.size() > 0)
                safe = b._safe_cache;
            else {
                safe = belief_game_->getSafeActions(b.support, b.obs, b.rem); 
                b._safe_cache = safe;
            }
            std::vector<int> indices(A);
            std::iota(indices.begin(), indices.end(), 0);

            std::sort(indices.begin(),
                      indices.end(),
                      [&b, &safe](int lhs, int rhs) {
                          return !safe[lhs] ||
                              (b.children[lhs].V < b.children[rhs].V);
                      });
            /* std::cout << "The best actions, in increasing order: ";
             * for (auto it = indices.begin(); it != indices.end(); ++it)
             *     std::cout << *it << " ";
             * std::cout << std::endl;
             */

            return indices.back();
        }

        template <typename M>
        size_t BWCPOMCP<M>::findBestBonusA(BeliefNode &b) {
            unsigned count = b.N;
            // Count here can be as low as 1.
            // Since log(1) = 0, and 0/0 = error, we add 1.0.
            double logCount = std::log(count + 1.0);
            // We use this function to produce a score for each action. This can be easily
            // substituted with something else to produce different BWCPOMCP variants.
            auto evaluationFunction = [this, logCount](const ActionNode & an){
                    return an.V + exploration_ * std::sqrt( logCount / an.N );
            };

            std::vector<bool> safe;
            if (b._safe_cache.size() > 0)
                safe = b._safe_cache;
            else {
                safe = belief_game_->getSafeActions(b.support, b.obs, b.rem); 
                b._safe_cache = safe;
            }
            std::vector<int> indices(A);
            std::iota(indices.begin(), indices.end(), 0);

            std::sort(indices.begin(),
                      indices.end(),
                      [&b, &safe, &evaluationFunction](int lhs, int rhs) {
                          return !safe[lhs] || (safe[rhs] &&
                              (evaluationFunction(b.children[lhs]) <
                               evaluationFunction(b.children[rhs])));
                      });
            // std::cout << "The best (with bonus) actions, in increasing order: ";
            // for (auto it = indices.begin(); it != indices.end(); ++it)
            //     std::cout << *it << " ";
            // std::cout << std::endl;
            
                   

            return indices.back();
        }

        template <typename M>
        typename BWCPOMCP<M>::SampleBelief BWCPOMCP<M>::makeSampledBelief(const Belief & b) {
            SampleBelief belief;
            belief.reserve(beliefSize_);

            for ( size_t i = 0; i < beliefSize_; ++i )
                belief.push_back(sampleProbability(S, b, rand_));

            return belief;
        }

        template <typename M>
        void BWCPOMCP<M>::setBeliefSize(size_t beliefSize) {
            beliefSize_ = beliefSize;
        }

        template <typename M>
        void BWCPOMCP<M>::setIterations(unsigned iter) {
            iterations_ = iter;
        }

        template <typename M>
        void BWCPOMCP<M>::setExploration(double exp) {
            exploration_ = exp;
        }

        template <typename M>
        const M& BWCPOMCP<M>::getModel() const {
            return model_;
        }

        template <typename M>
        const typename BWCPOMCP<M>::BeliefNode& BWCPOMCP<M>::getGraph() const {
            return graph_;
        }

        template <typename M>
        size_t BWCPOMCP<M>::getBeliefSize() const {
            return beliefSize_;
        }

        template <typename M>
        unsigned BWCPOMCP<M>::getIterations() const {
            return iterations_;
        }

        template <typename M>
        double BWCPOMCP<M>::getExploration() const {
            return exploration_;
        }
    }
}

#endif
