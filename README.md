# quickopt
A Python optimization toolkit developed in C++ geared towards multimodal functions.

# Documentation: [Click Here!](https://google.com)
**Most of the information is found in the documentation** - please refer to it! _Tutorials, examples, descriptions/breakdowns of algorithms, and references are found here._

## Installation
To install the package using PyPi, run the following command:
```
pip install quickopt
```

## Description
This package is a Python optimization toolkit developed in C++ geared towards global optimization problems, such as those encountered in hyperparameter tuning. It contains a variety of optimization algorithms, which are listed below. The package is designed to be easy to use and flexible, allowing users to use popular optimization methods out of the box in as little as one line of code, while also allowing users to customize the optimization process to suit their needs. A list of the optimization algorithms included in the package is provided below:

- Simulated Annealing: Compatible with `string`, `double`, and `integer` inputs

- Bayesian Optimization with Tree-Structured Parzen Estimators: Compatible with `double` inputs

- Genetic Algorithm: Compatible with `string`, `double`, and `integer` inputs

- Particle Swarm Optimization: Compatible with `double` inputs

with more to come soon!

## Quick Start
To use the package, import the desired optimization function from the package and run it with the desired parameters (described in detail in documentation). For example, to use the Particle Swarm Optimization algorithm for a double function, you would run the following code:

```python
import quickopt

def objective(params): # The function being optimized - this must take inputs as a list
        return sum(x**2 for x in params)
    space_min = [-1.0, -1.0]
    space_max = [1.0, 1.0]

result = quickopt.pso(funct=objective, space_min=space_min, space_max=space_max, iterations=10) # The optimization is run here

print(result)
```

Note that some functions have different definitions dependant on variable type. For example, the Simulated Annealing algorithm has different functions for double, integer, and string inputs. The functions are named `anneal_double`, `anneal_int`, and `anneal_string`, respectively. Objective functions must take in inputs as a list, as shown above.

## References:

Kirkpatrick, S., C. D. Gelatt, and M. P. Vecchi. 1983. “Optimization by Simulated Annealing.” Science 220 (4598): 671–80. https://doi.org/10.1126/science.220.4598.671.

Shuhei Watanabe, “Tree-Structured Parzen Estimator: Understanding Its Algorithm Components and Their Roles for Better Empirical Performance,” arXiv (Cornell University), January 1, 2023, https://doi.org/10.48550/arxiv.2304.11127.

Bergstra, James, Dan Yamins, and David D. Cox. “Making a Science of Model Search.” arXiv (Cornell University), January 1, 2012. https://doi.org/10.48550/arxiv.1209.5111.

Song, Jiaming, Lantao Yu, Willie Neiswanger, and Stefano Ermon. “A General Recipe for Likelihood-free Bayesian Optimization.” arXiv (Cornell University), January 1, 2022. https://doi.org/10.48550/arxiv.2206.13035.

Falkner, Stefan, Aaron Klein, and Frank Hutter. “BOHB: Robust and Efficient Hyperparameter Optimization at Scale.” arXiv (Cornell University), January 1, 2018. https://doi.org/10.48550/arxiv.1807.01774.

Baluja, Shumeet, and Rich Caruana. 1995. “Removing the Genetics From the Standard Genetic Algorithm.” In Elsevier eBooks, 38–46. https://doi.org/10.1016/b978-1-55860-377-6.50014-1.

Bonyadi, Mohammad Reza, and Zbigniew Michalewicz. 2017. “Particle Swarm Optimization for Single Objective Continuous Space Problems: A Review.” Evolutionary Computation 25 (1): 1–54. https://doi.org/10.1162/evco_r_00180.

## More
Contact: `varunpiram@gmail.com`
This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for details.
