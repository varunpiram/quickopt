---
sidebar_position: 2
---

# Description

Particle Swarm Optimization (PSO) is a computational method used for global optimization problems by iteratively improving a candidate solution with regard to a given measure of quality. It is inspired by social behavior in animals, ex. flocks of birds and schools of fish, and is particularly effective for problems with large and complex search spaces, and does not require differentiability.

### Overview

1. **Objective Function**: This function takes a set of double parameters and returns a numerical value.

2. **Search Space**: The set of all possible solutions for the optimization problem. This space is defined by lower and upper bound vectors for each parameter.

3. **Particles**: Individual candidate solutions within the swarm (set of candidate solutions). Each particle has a position representing a potential solution and a velocity dictating how it moves through the search space.

4. **Velocity Update**: The velocity of each particle is updated based on its own best-known position, the swarm's best-known position, and a random influence. This update is governed by inertia, cognitive, and social weights - representing the particles past position, personal best, and the swarm's global best, respectively. Their importance is scaled by parameters inputted by the user. Velocities may be clamped to prevent them from growing too large.

5. **Position Update**: The position of each particle is updated based on its current velocity. The new position is bounded by the search space limits.

6. **Tuning Parameters**: These parameters control the behavior of particles:
   - **Inertia**: Controls the influence of the particle's previous velocity.
   - **Cognitive**: Controls the influence of the particle's personal best-known position.
   - **Social**: Controls the influence of the swarm's best-known position.
   Inertia weight must generally be between 0 and 1, while Cognitive and Social weights are recommended to be between 1 and 3.

### The Optimization Process

1. **Initialization**:

   - Generate an initial swarm of particles with random positions within the search space and random velocities within the clamping limit.
   - Evaluate the fitness of each particle and determine the personal best position for each particle and the global best position for the swarm.

2. **Iteration**:

   - **Velocity Update**: Calculate the new velocity for each particle based on inertia, cognitive, and social components.
   - **Position Update**: Update the particle's position based on its new velocity, clamping the position within the search space bounds.
   - **Evaluation**: Assess the value of the new position. Update the personal best position and value for the particle if the new position is better. Update the global best position and value for the swarm if the new position is better than the current global best.

3. **Termination**:

   - Repeat the iteration process for the specified number of iterations.
   - Return the parameters of the particle with the best fitness in the final swarm.

### Intuition

- **Exploration vs. Exploitation**: The algorithm balances exploring new areas of the search space (via randomness in the velocity update) and exploiting known good solutions (via the cognitive and social components). This balance helps the algorithm avoid getting "stuck" at local optima while still searching regions of the search space which may be of high value.

- **Inertia Weight**: The inertia weight controls the momentum of the particles. A higher inertia weight favors exploration, while a lower inertia weight favors exploitation.

- **Cognitive and Social Weights**: These weights control the attraction of particles to their personal best positions and the global best position. Higher values increase the influence of these positions, leading to faster convergence but potentially less exploration.

- **Clamping Limit**: The clamping limit restricts the velocity of particles, preventing them from moving too fast and ensuring they stay within the search space.

