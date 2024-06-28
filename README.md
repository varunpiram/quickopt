# quickopt
A Python optimization toolkit developed in C++ geared towards nonlocal optimization problems.

## Documentation: [Click Here](https://google.com)
Most of the information is found in the documentation - please refer to it! Tutorials, examples, descriptions/breakdowns of algorithms, and references are found here.

## Installation
To install the package using PyPi, run the following command:
```
pip install quickopt
```

## Description
This package is a Python optimization toolkit developed in C++ geared towards nonlocal optimization problems, such as those encountered in hyperparameter tuning. It contains a variety of optimization algorithms, which are listed below. The package is designed to be easy to use and flexible, allowing users to use popular optimization methods out of the box in as little as one line of code, while also allowing users to customize the optimization process to suit their needs. A list of the optimization algorithms included in the package is provided below:

- Simulated Annealing: Compatible with string, float, and integer inputs

- Bayesian Optimization with Tree-Structrued Parzen Estimators: Compatible with float inputs

- Genetic Algorithm: Compatible with string, float, and integer inputs

- Particle Swarm Optimization: Compatible with float inputs

with more to come soon!

## Quick Start
To use the package, simply import the desired optimization function from the package and run it with the desired parameters (described in detail in documentation). For example, to use the Particle Swarm Optimization algorithm for a double function, you would run the following code:

```python
import quickopt
def objective(params):
        return sum(x**2 for x in params)
    space_min = [-1.0, -1.0]
    space_max = [1.0, 1.0]
result = quickopt.pso(funct=objective, space_min=space_min, space_max=space_max, iterations=10)
print(result)
```

Note that some functions have different definitions dependant on variable type. For example, the Simulated Annealing algorithm has different functions for double, integer, and string inputs. The functions are named `anneal_double`, `anneal_int`, and `anneal_string`, respectively.

## References:

## More
Contact: `varunpiram@gmail.com`
This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for details.
