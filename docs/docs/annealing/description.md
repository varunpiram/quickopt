---
sidebar_position: 2
---

# Description

Simulated annealing is an optimization algorithm designed to find a global minimum in a large search space, particularly when the function to be minimized has many local minima. The technique is inspired by the annealing process in metallurgy, where a material is heated and then slowly cooled to remove defects and achieve a more stable structure.

### Overview

1. **Objective Function**: The function we aim to minimize. This function takes a set of parameters and returns a numerical value representing the "cost" or "energy" of that parameter set.

2. **Search Space**: The set of all possible solutions (parameter sets) for the optimization problem. Each point in this space corresponds to a potential solution. This is not explicitly defined within the implementation, and should instead be defined implicitly within the neighbor function.

3. **Temperature**: A control parameter dependant on iteration that starts high and gradually decreases. The temperature influences the likelihood of accepting worse solutions as the algorithm progresses, allowing the algorithm to explore the state space broadly at high temperatures and refine the search at low temperatures. Default temperature is set to `0.999**iteration`, but a custom function can be defined.

4. **Neighbor Function**: A function that generates a new candidate solution from the current solution by making a small change to the parameter set. This function defines the "neighborhood" of the current solution. This is to be defined by the user, and has a high impact on the algorithm's performance.

5. **Acceptance Criterion**: This criterion determines whether to accept a new candidate solution based on the change in the objective function value as well as the current temperature. Higher temperatures allow for a higher probability of accepting worse solutions, encouraging exploration, while lower temperatures focus on refining the best solutions found - exploitation. By default, the Metropolis-Hastings criterion (as shown in Kirkpatrick et al.) is used, which gives probability as an exponential function for worse solutions, while guaranteeing superior solutions are always accepted. Custom acceptance criteria can also be defined, although this isn't typically recommended.

### The Optimization Process

1. **Initialization**:

Generate an initial solution within the state space, and evaluate it. Define this as our current solution. Set the initial temperature to its maximum value.

2. **Iteration**:

Generate a candidate solution by perturbing the current solution using the neighbor function. Generate an acceptance probability for our new solution based on our current temperature, as well as the difference in values between the candidate and current solutions. If the candidate solution is accepted, update the current solution. Decrease the temperature according to the cooling schedule.

3. **Termination**:

Repeat the iteration process for the specified number of iterations, and return the parameters of the best candidate solution.

### Intuition

- **Exploration vs. Exploitation**: At high temperatures, the algorithm is more likely to accept worse solutions, allowing it to explore the state space broadly and avoid getting trapped in local minima. As the temperature decreases, the acceptance probability for worse solutions diminishes, leading the algorithm to focus more on refining the best solutions found (exploitation).

- **Metropolis-Hastings Criterion**: This criterion ensures that the algorithm has a controlled probability of accepting worse solutions, which decreases as the temperature drops. This probabilistic acceptance allows the algorithm to escape local minima and increases the likelihood of finding the global minimum.

- **Cooling Schedule**: The schedule for reducing the temperature is crucial for balancing exploration and exploitation. A rapid decrease in temperature may lead to premature convergence, while a slow decrease ensures thorough exploration but requires more iterations.
