---
sidebar_position: 1
---

# Usage

## `genetic_double`

Uses a genetic algorithm to **maximize** a function of double inputs.


### Signature

```python
genetic_double(fitness, mutate, generate, seed=[], population_size=100, reproduction_ct=10, survivor_ct=0, mutation_rate=0.1, generations=100, verbose=1)
```

### Parameters

- `fitness` - **function: (input: List[float]) -> float** : The fitness function. It should take a list of doubles as input and return a double representing the fitness value.

- `mutate` - **function: (input: List[float]) -> List[float]** : The mutation function. It should take a list of doubles as input and return a new list of doubles representing the mutated parameters.

- `generate` - **function: () -> List[float]** : The generate function. It generates a set of parameters to be used in the initial population, ideally randomly.

- `seed` - **List[List[float]]**, ___optional___ : Seed population. A predefined set of parameters to be used in the initial population. Default is an empty list.

- `population_size` - **int**, ___optional___ : The size of the population. More is better but slower. Default is 100.

- `reproduction_ct` - **int**, ___optional___ : The number of individuals that will reproduce each generation. More leads to more diversity but slower convergence. Default is 10.

- `survivor_ct` - **int**, ___optional___ : The number of individuals that will survive each generation. These individuals are not affected by mutation. More leads to more stability but could harm diversity. Default is 0.

- `mutation_rate` - **double**, ___optional___ : The probability that a mutation will occur on a given individual. Default is 0.1.

- `generations` - **int**, ___optional___ : The number of generations to run the algorithm. Default is 100.

- `verbose` - **int**, ___optional___ : The verbosity level. `0` for no output, `1` for average and top fitness each generation. Default is 1.

### Output

- `best_params` - **List[float]**: The best set of parameters found by the genetic algorithm.

### Notes

- Minimization can be achieved by returning the negative of objective function values.
- Ensure `reproduction_ct` is not greater than `population_size`.
- Search space boundaries should be implicitly defined within `generate` and `mutate` functions.
- The `generate` function should handle boundary conditions if necessary.
- Adjust the verbosity level with the `verbose` parameter to control the amount of output during the generations.

## `genetic_int`

Uses a genetic algorithm to **maximize** a function of integer inputs.

### Signature

```python
genetic_int(fitness, mutate, generate, seed=[], population_size=100, reproduction_ct=10, survivor_ct=0, mutation_rate=0.1, generations=100, verbose=1)
```

### Parameters

- `fitness` - **function: (input: List[int]) -> float** : The fitness function. It should take a list of integers as input and return a double representing the fitness value.

- `mutate` - **function: (input: List[int]) -> List[int]** : The mutation function. It should take a list of integers as input and return a new list of integers representing the mutated parameters.

- `generate` - **function: () -> List[int]** : The generate function. It generates a set of parameters to be used in the initial population, ideally randomly.

- `seed` - **List[List[int]]**, ___optional___ : Seed population. A predefined set of parameters to be used in the initial population. Default is an empty list.

- `population_size` - **int**, ___optional___ : The size of the population. More is better but slower. Default is 100.

- `reproduction_ct` - **int**, ___optional___ : The number of individuals that will reproduce each generation. More leads to more diversity but slower convergence. Default is 10.

- `survivor_ct` - **int**, ___optional___ : The number of individuals that will survive each generation. These individuals are not affected by mutation. More leads to more stability but could harm diversity. Default is 0.

- `mutation_rate` - **double**, ___optional___ : The probability that a mutation will occur on a given individual. Default is 0.1.

- `generations` - **int**, ___optional___ : The number of generations to run the algorithm. Default is 100.

- `verbose` - **int**, ___optional___ : The verbosity level. `0` for no output, `1` for average and top fitness each generation. Default is 1.

### Output

- `best_params` - **List[int]**: The best set of parameters found by the genetic algorithm.

### Notes

- Minimization can be achieved by returning the negative of objective function values.
- Ensure `reproduction_ct` is not greater than `population_size`.
- Search space boundaries should be implicitly defined within `generate` and `mutate` functions.
- The `generate` function should handle boundary conditions if necessary.
- Adjust the verbosity level with the `verbose` parameter to control the amount of output during the generations.

## `genetic_string`

Uses a genetic algorithm to **maximize** a function of string inputs.

### Signature

```python
genetic_string(fitness, mutate, generate, seed=[], population_size=100, reproduction_ct=10, survivor_ct=0, mutation_rate=0.1, generations=100, verbose=1)
```

### Parameters

- `fitness` - **function: (input: List[str]) -> float** : The fitness function. It should take a list of strings as input and return a double representing the fitness value.

- `mutate` - **function: (input: List[str]) -> List[str]** : The mutation function. It should take a list of strings as input and return a new list of strings representing the mutated parameters.

- `generate` - **function: () -> List[str]** : The generate function. It generates a set of parameters to be used in the initial population, ideally randomly.

- `seed` - **List[List[str]]**, ___optional___ : Seed population. A predefined set of parameters to be used in the initial population. Default is an empty list.

- `population_size` - **int**, ___optional___ : The size of the population. More is better but slower. Default is 100.

- `reproduction_ct` - **int**, ___optional___ : The number of individuals that will reproduce each generation. More leads to more diversity but slower convergence. Default is 10.

- `survivor_ct` - **int**, ___optional___ : The number of individuals that will survive each generation. These individuals are not affected by mutation. More leads to more stability but could harm diversity. Default is 0.

- `mutation_rate` - **double**, ___optional___ : The probability that a mutation will occur on a given individual. Default is 0.1.

- `generations` - **int**, ___optional___ : The number of generations to run the algorithm. Default is 100.

- `verbose` - **int**, ___optional___ : The verbosity level. `0` for no output, `1` for average and top fitness each generation. Default is 1.

### Output

- `best_params` - **List[str]**: The best set of parameters found by the genetic algorithm.

### Notes

- Minimization can be achieved by returning the negative of objective function values.
- Ensure `reproduction_ct` is not greater than `population_size`.
- Search space boundaries should be implicitly defined within `generate` and `mutate` functions.
- The `generate` function should handle boundary conditions if necessary.
- Adjust the verbosity level with the `verbose` parameter to control the amount of output during the generations.
