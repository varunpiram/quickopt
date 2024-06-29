---
sidebar_position: 1
---

# Usage

## `anneal_double`

Uses simulated annealing to **minimize** a function of double inputs.

### Signature

```python
anneal_double(funct, initial, neighbor, iterations=100, temperature=lambda iter: pow(0.999, iter), acceptance=lambda new_value, current_value, temperature: exp(-(new_value - current_value) / temperature), verbose=1)
```

### Parameters

- `funct` - **function: (input: List[float]) -> float** : The function to be minimized. It should take a list of doubles as input and return a double.

- `initial` - **List[float]** : The initial guess for the parameters. This is the starting point for the simulated annealing process.

- `neighbor` - **function: (input: List[float]) -> List[float]** : The neighbor function that generates a new set of parameters based on the current set. It should take a vector of doubles as input and return a new vector of doubles.

- `iterations` - **int**, ___optional___ : The number of iterations to run the algorithm. Default is 100.

- `temperature` - **function: (input: int) -> float**, ___optional___ : The temperature schedule that determines the temperature at each iteration. It should take an iteration number as input and return a temperature double. Default returns `0.999**iteration`.

- `acceptance` - **function: (input: float, float, float) -> float**, ___optional___ : Used to define a custom acceptance probability function, which determines whether to accept a new set of parameters based on the current set and temperature. It should take a new value, the current value, and the temperature - three doubles (`new_value`, `current_value`, `temperature`) - as inputs, and return a double in [0,1]. Default uses the Metropolis-Hastings algorithm in case T=1 and proposal distribution is symmetric. 

- `verbose` - **int**, ___optional___ : The verbosity level. `0` for final output, `1` for output at each iteration. Default is 1.

### Output

- `best_params` - **List[float]**: The best set of parameters found by the simulated annealing process.

### Notes

- Maximization can be achieved by returning the negative of objective function values.
- The `temperature` function should typically return a value in the range (0, 1] and must be decreasing over iterations.
- The `acceptance` function must return a value between 0 and 1.
- The `neighbor` function should handle boundary conditions if necessary.
- Adjust the verbosity level with the `verbose` parameter to control the amount of output during the iterations.

## `anneal_int`

Uses simulated annealing to **minimize** a function of integer inputs.

### Signature

```python
anneal_int(funct, initial, neighbor, iterations=100, temperature=lambda iter: pow(0.999, iter), acceptance=lambda new_value, current_value, temperature: exp(-(new_value - current_value) / temperature), verbose=1)
```

### Parameters

- `funct` - **function: (input: List[int]) -> float** : The function to be minimized. It should take a list of doubles as input and return a double.

- `initial` - **List[int]** : The initial guess for the parameters. This is the starting point for the simulated annealing process.

- `neighbor` - **function: (input: List[int]) -> List[float]** : The neighbor function that generates a new set of parameters based on the current set. It should take a vector of doubles as input and return a new vector of doubles.

- `iterations` - **int**, ___optional___ : The number of iterations to run the algorithm. Default is 100.

- `temperature` - **function: (input: int) -> float**, ___optional___ : The temperature schedule that determines the temperature at each iteration. It should take an iteration number as input and return a temperature double. Default returns `0.999**iteration`.

- `acceptance` - **function: (input: int, int, float) -> float**, ___optional___ : Used to define a custom acceptance probability function, which determines whether to accept a new set of parameters based on the current set and temperature. It should take a new value, the current value, and the temperature - three doubles (`new_value`, `current_value`, `temperature`) - as inputs, and return a double in [0,1]. Default uses the Metropolis-Hastings algorithm in case T=1 and proposal distribution is symmetric. 

- `verbose` - **int**, ___optional___ : The verbosity level. `0` for final output, `1` for output at each iteration. Default is 1.

### Output

- `best_params` - **List[int]**: The best set of parameters found by the simulated annealing process.

### Notes

- Maximization can be achieved by returning the negative of objective function values.
- The `temperature` function should typically return a value in the range (0, 1] and must be decreasing over iterations.
- The `acceptance` function must return a value between 0 and 1.
- The `neighbor` function should handle boundary conditions if necessary.
- Adjust the verbosity level with the `verbose` parameter to control the amount of output during the iterations.

## `anneal_string`

Uses simulated annealing to **minimize** a function of string inputs.

### Signature

```python
anneal_string(funct, initial, neighbor, iterations=100, temperature=lambda iter: pow(0.999, iter), acceptance=lambda new_value, current_value, temperature: exp(-(new_value - current_value) / temperature), verbose=1)
```

### Parameters

- `funct` - **function: (input: List[str]) -> float** : The function to be minimized. It should take a list of doubles as input and return a double.

- `initial` - **List[str]** : The initial guess for the parameters. This is the starting point for the simulated annealing process.

- `neighbor` - **function: (input: List[str]) -> List[float]** : The neighbor function that generates a new set of parameters based on the current set. It should take a vector of doubles as input and return a new vector of doubles.

- `iterations` - **int**, ___optional___ : The number of iterations to run the algorithm. Default is 100.

- `temperature` - **function: (input: int) -> float**, ___optional___ : The temperature schedule that determines the temperature at each iteration. It should take an iteration number as input and return a temperature double. Default returns `0.999**iteration`.

- `acceptance` - **function: (input: str, str, float) -> float**, ___optional___ : Used to define a custom acceptance probability function, which determines whether to accept a new set of parameters based on the current set and temperature. It should take a new value, the current value, and the temperature - three doubles (`new_value`, `current_value`, `temperature`) - as inputs, and return a double in [0,1]. Default uses the Metropolis-Hastings algorithm in case T=1 and proposal distribution is symmetric. 

- `verbose` - **int**, ___optional___ : The verbosity level. `0` for final output, `1` for output at each iteration. Default is 1.

### Output

- `best_params` - **List[str]**: The best set of parameters found by the simulated annealing process.

### Notes

- Maximization can be achieved by returning the negative of objective function values.
- The `temperature` function should typically return a value in the range (0, 1] and must be decreasing over iterations.
- The `acceptance` function must return a value between 0 and 1.
- The `neighbor` function should handle boundary conditions if necessary.
- Adjust the verbosity level with the `verbose` parameter to control the amount of output during the iterations.

