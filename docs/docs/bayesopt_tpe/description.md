---
sidebar_position: 2
---

# Description

Bayesian Optimization with Tree-structured Parzen Estimator (TPE) is a sequential model-based optimization algorithm which works by building models to predict optimal selection of candidate solutions. This algorithm is particularly useful for finding the minimum value of a function when the function evaluations are expensive and the search space is large and complex.

### Overview

1. **Objective Function**: The function we aim to minimize. This function takes a set of double parameters and returns a numerical value.

2. **Search Space**: Defined explicitly by upper and lower bound vectors. Each dimension of these vectors represents a different parameter, and the bounds define the range within which the algorithm searches for the optimal solution.

3. **Observation Set**: The set of all candidate solutions which we use to build our acquisition function. This set is divided into two subsets: "good" and "bad" solutions, based on their function values and a splitting algorithm. By default, we use the square root splitting algorithm as defined in Bergstra et al. This algorithm involves the threshold parameter, which is set to 0.5 by default. Smaller threshold values lead to more exploration, while larger values lead to more exploitation, as shown in Watanabe. If wanted, users can override this with a fixed splitting value, which defines the good set as a fixed percentage of the total number of observations. Users may also set a cap on the size of the "good" set.

4. **Kernel Density Estimator**: Kernel Density Estimators are functions which estimate a given function based on samples drawn from it. In Bayesian Optimization, we use KDEs to construct our acquisition function, which tells us the likelihood a given parameter will fall within the "good" set. Before building our KDEs, we first assign a weight to each candidate solution. This is done by using the function defined to lead to the Expected Improvement acquisition function in Song et al. This involves a bandwidth parameter, which is by default dynamically calculated using the heuristic provided in appendix C.3.3 of Watanabe - although users can override this with a fixed set of bandwidths (for each dimension) if needed. We then construct KDEs for the "good" and "bad" sets based off the Gaussian kernel for numerical parameters described in Watanabe. We use the multivariate Kernel used in Falkner et al. to enhance performance by capturing the interaction effect between parameters. 

5. **Acquisition Function**: The acquisition function gives the probability that a given parameter's  solution will fall within the "good" set. This function is defined as the ratio of the KDE of the "good" set to the sum of the KDEs of the "good" and "bad" sets. The acquisition function is used to balance exploration and exploitation, as described in Watanabe. This ratio includes a non-informative prior - the probability density function of a Gaussian distribution - which is scaled by the prior weight parameter. This parameter controls the balance between exploration and exploitation, with larger values leading to more exploration and smaller values leading to more exploitation. Additionally, it includes a parameter for the zeroth basis in the KDE, which is given as the mean of candidate weights for the "good" set, as described in Watanabe. To select our next set of parameters to evaluate, we draw random samples from the search space and evaluate the acquisition function at these points, selecting our best performing sample.

### The Optimization Process

1. **Initialization**:

   - Generate initial samples within the search space bounds.
   - Evaluate the objective function at these samples.
   - Initialize the "good" and "bad" sets based on the function values and splitting threshold.

2. **Iteration**:

   - Split the current samples into "good" and "bad" sets based on their function values and the splitting threshold.
   - Weigh candidates within each set, and build KDEs.
   - Calculate the acquisition function
   - Draw new samples from the search space and evaluate the acquisition function to select the next set of parameters to evaluate.
   - Evaluate the objective function at the new parameters and update the "good" and "bad" sets accordingly.
   - Update the bandwidth dynamically if not provided.

3. **Termination**:

   - Repeat the iteration process for the specified number of iterations.
   - Return the parameters that resulted in the minimum function value found during the optimization process.

### Intuition

- **Exploration vs. Exploitation**: The acquisition function helps balance the need to explore new regions of the search space (exploration) with the need to focus on promising regions (exploitation). The algorithm uses the KDE to model the probability density of good and bad samples, guiding the search towards areas with higher densities of good samples.

- **Kernel Density Estimation (KDE)**: KDE is used to estimate the probability density of the good and bad sets of samples. This estimation helps in calculating the acquisition function, which determines the next set of parameters to evaluate.

- **Non-Informative Prior**: This prior biases the search, ensuring that the algorithm does not focus too narrowly on the edges of the space.

- **Dynamic Bandwidth Calculation**: If the bandwidth is not provided, it is calculated dynamically based on a heuristic, ensuring that the KDE is appropriately scaled to the search space and number of samples.

