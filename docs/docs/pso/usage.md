---
sidebar_position: 2
---

# Usage
## Import
```python
from quickopt.pso import pso
```

## `pso`

Uses particle swarm optimization to **maximize** a function of double inputs.

### Signature

```python
pso(funct, space_min, space_max, iterations=100, swarm_size=100, inertia=0.5, cognitive=1.0, social=2.0, clamp=0.1, verbose=1)
```

### Parameters

- `funct` - **function: (input: List[float]) -> float** : The function to be maximized. It should take a list of doubles as input and return a double.

- `space_min` - **List[float]** : Lower bound vector for the search space. Defines the minimum values for the parameters.

- `space_max` - **List[float]** : Upper bound vector for the search space. Defines the maximum values for the parameters.

- `iterations` - **int**, ___optional___ : The number of iterations to run the algorithm. Default is 100.

- `swarm_size` - **int**, ___optional___ : The number of particles in the swarm. Default is 100.

- `inertia` - **double**, ___optional___ : Inertia weight. Recommended to be in (0,1). Default is 0.5.

- `cognitive` - **double**, ___optional___ : Cognitive weight. Recommended to be in [1,3]. Default is 1.0.

- `social` - **double**, ___optional___ : Social weight. Recommended to be in [1,3]. Default is 2.0.

- `clamp` - **double**, ___optional___ : Clamping limit on velocity. Default is 0.1.

- `verbose` - **int**, ___optional___ : The verbosity level. `0` for final output, `1` for output at each iteration. Default is 1.

### Output

- `best_params` - **List[float]**: The best set of parameters found by the particle swarm optimization process.

### Notes

- Minimization can be achieved by returning the negative of objective function values.
- The `space_min` and `space_max` vectors must be of the same size.
- Ensure that the `inertia` is in (0,1), and the `cognitive` and `social` weights are in [1,3].
- Adjust the verbosity level with the `verbose` parameter to control the amount of output during the iterations.
