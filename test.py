from mypackage.evolution import evolve
import random

# Define the fitness function
def fitness(params):
    # Simple fitness function: sum of squares
    return -sum(x**2 for x in params)

# Define the mutation function
def mutate(params):
    # Mutate by adding a small random value to each parameter
    return [x + random.uniform(-0.1, 0.1) for x in params]

# Define the generate function
def generate():
    # Generate a list of 10 random values between -10 and 10
    return [random.uniform(-10, 10) for _ in range(10)]

# Testing evolve_double
def test_evolve_double():
    result = evolution.evolve_double(
        fitness=fitness,
        mutate=mutate,
        generate=generate,
        population_size=100,
        reproduction_ct=10,
        survivor_ct=10,
        mutation_rate=0.1,
        generations=100,
        verbose=1
    )
    print("Best solution (double):", result)

# Define the fitness function for integers
def fitness_int(params):
    return -sum(x**2 for x in params)

# Define the mutation function for integers
def mutate_int(params):
    return [x + random.randint(-1, 1) for x in params]

# Define the generate function for integers
def generate_int():
    return [random.randint(-10, 10) for _ in range(10)]

# Testing evolve_int
def test_evolve_int():
    result = evolution.evolve_int(
        fitness=fitness_int,
        mutate=mutate_int,
        generate=generate_int,
        population_size=100,
        reproduction_ct=10,
        survivor_ct=10,
        mutation_rate=0.1,
        generations=100,
        verbose=1
    )
    print("Best solution (int):", result)

# Define the fitness function for strings
def fitness_string(params):
    target = "evolutionary"
    return -sum((ord(c1) - ord(c2))**2 for c1, c2 in zip(params, target))

# Define the mutation function for strings
def mutate_string(params):
    params = list(params)
    idx = random.randint(0, len(params) - 1)
    params[idx] = chr(random.randint(32, 126))
    return ''.join(params)

# Define the generate function for strings
def generate_string():
    return ''.join(chr(random.randint(32, 126)) for _ in range(12))

# Testing evolve_string
def test_evolve_string():
    result = evolution.evolve_string(
        fitness=fitness_string,
        mutate=mutate_string,
        generate=generate_string,
        population_size=100,
        reproduction_ct=10,
        survivor_ct=10,
        mutation_rate=0.1,
        generations=100,
        verbose=1
    )
    print("Best solution (string):", result)

if __name__ == "__main__":
    test_evolve_double()
    test_evolve_int()
    test_evolve_string()
