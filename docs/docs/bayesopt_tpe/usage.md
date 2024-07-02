---
sidebar_position: 1
---

# Usage
## Import
```python
from quickopt.bayesopt_tpe import bayesopt_tpe
```

## `bayesopt_tpe`

Uses Bayesian Optimization with Tree-structured Parzen Estimator (TPE) to **minimize** a double function.

### Signature

```python
bayesopt_tpe(funct, space_min, space_max, iterations=100, samples=10, acquisition_samples=100, verbose=1, threshold=0.5, group_cap=9999, bandwidth=[], prior_weight=1, split=-107.2931, seed=[])
```

### Parameters

- `funct` - **function: (input: List[float]) -> float** : The function to be minimized. It should take a list of doubles as input and return a double.

- `space_min` - **List[float]** : Lower bound vector for the search space. Defines the minimum values for the parameters.

- `space_max` - **List[float]** : Upper bound vector for the search space. Defines the maximum values for the parameters.

- `iterations` - **int**, ___optional___ : The number of iterations to run the algorithm. Default is 100.

- `samples` - **int**, ___optional___ : The number of samples to draw per iteration. Default is 10.

- `acquisition_samples` - **int**, ___optional___ : The number of samples to draw for acquisition function maximization. Default is 100.

- `verbose` - **int**, ___optional___ : The verbosity level. `0` for final output, `1` for output at each iteration. Default is 1.

- `threshold` - **double**, ___optional___ : Threshold value for the splitting function. Default is 0.5.

- `group_cap` - **int**, ___optional___ : Group cap for the good set. Default is 9999.

- `bandwidth` - **List[float]**, ___optional___ : Optional bandwidth vector for the KDE. If empty, dynamic bandwidth calculation is used.

- `prior_weight` - **int**, ___optional___ : Prior weight parameter for Non-Informative Prior. Default is 1.

- `split` - **double**, ___optional___ : Optional splitting threshold. Overrides dynamic threshold calculation.

- `seed` - **List[Tuple[List[float], float]]**, ___optional___ : Seed data for the algorithm.

### Output

- `best_params` - **List[float]**: The best set of parameters found by the Bayesian Optimization process.

### Notes

- Maximization can be achieved by returning the negative of objective function values.
- The `threshold` value must be positive and at most the square root of the number of samples.
- The `split` value must be within [0, 1] if provided; otherwise, dynamic calculation is used.
- The `space_min` and `space_max` vectors must be of the same size, with each element of `space_min` less than the corresponding element of `space_max`.
- The `bandwidth` vector, if provided, must be of the same size as the search space; otherwise, dynamic bandwidth calculation is used.
- Ensure the number of `iterations`, `samples`, and `acquisition_samples` are positive integers.
- The `seed` parameter must be a list of tuples, in which each tuple contains a list of parameters and the corresponding function value.
- Adjust the verbosity level with the `verbose` parameter to control the amount of output during the iterations.
