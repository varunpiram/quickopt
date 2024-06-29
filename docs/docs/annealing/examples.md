---
sidebar_position: 3
---

# Examples

## McCormick Function

The McCormick function is a multimodal function that is commonly used to test optimization algorithms. It is defined as `f(x,y) = sin(x + y) + (x - y)**2 - 1.5*x + 2.5*y + 1` along the domain `[-1.5, 4] x [-3, 4]`. The global minimum of this function is located at `f(-0.54719, -1.54719) = -1.9133`.

We implement the McCormick function as follows:
```python
import numpy as np
def objective(params):
    x, y = params
    return = np.sin(x + y) + (x - y)**2 - 1.5*x + 2.5*y + 1
```

We then set our initial guess to the center of the search space:
```python
initial_guess = [1.25, 0.5]
```

We define our neighbor function to perturb our current solution by a random normal variable with mean 0 and standard deviation 1:
```python
def neighbor(params):
    new_params = params[:]
    index = random.randint(0, len(params) - 1)
    new_params[index] += random.normalvariate(0, 1)
    new_params[0] = max(-1.5, min(4, new_params[0]))
    new_params[1] = max(-3, min(4, new_params[0]))
    return new_params
```

Finally, we run the simulated annealing algorithm. We choose to run the algorithm for 200 iterations:
```python
result = quickopt.anneal_double(funct=objective, initial=initial_guess, neighbor=neighbor, iterations=200, verbose=0)
```

The following is a heatmap of the optimization process, with each point representing a new optimal solution found by the algorithm:

![Simulated Annealing Optimization Heatmap](mc-annealing.png)


