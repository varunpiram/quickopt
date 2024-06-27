import mypackage
import numpy as np

def test_anneal_double():
    # Example test case for anneal_double
    print("Testing anneal_double...")
    # Define a simple objective function, initial guess, and neighbor function
    def objective(params):
        return sum(x**2 for x in params)
    initial_guess = [10.0, 10.0]
    def neighbor(params):
        return [x + np.random.uniform(-.0005,0) for x in params]
    def temperature(iter):
        return 1 / (iter + 1)
    def acceptance(new_val, current_val, temp):
        print("new_val:", new_val, "current_val:", current_val, "temp:", temp)
        print(mypackage.metropolis_hastings(new_val, current_val, temp))
        return 0.9
    
    result = mypackage.anneal_double(objective, initial_guess, neighbor, iterations=10000, verbose=1, temperature=temperature)
    print("Result:", result)

def test_bayesopt_tpe():
    print("Testing bayesopt_tpe...")

    def objective(params):

        params = np.array(params)

        if len(params) != 15:
            raise ValueError("The input array must have 15 elements.")
        
        # Component 1: A sinusoidal function
        comp1 = np.sum(np.sin(params))

        # Component 2: An exponential decay function
        comp2 = np.sum(np.exp(-params**2))

        # Component 3: A polynomial function
        comp3 = np.sum(params**3)

        # Component 4: Interaction terms
        comp4 = np.sum(np.outer(params, params).flatten())

        # Combining all components with different weights
        result = 0.2 * comp1 + 0.3 * comp2 - 0.4 * comp3 + 0.1 * comp4

        return result
        
    
    
    lower_bounds = [-5.0, -4.5, -3.0, -2.5, -5.0, -4.0, -3.5, -1.0, -2.0, -3.5, -4.5, -2.0, -1.5, -3.0, -4.0]
    upper_bounds = [5.0, 4.5, 3.0, 2.5, 5.0, 4.0, 3.5, 1.0, 2.0, 3.5, 4.5, 2.0, 1.5, 3.0, 4.0]


    result = mypackage.bayesopt_tpe(objective, lower_bounds, upper_bounds, iterations=100, samples=500, verbose=1)
    print("Result:", result)
    print("Best value:", objective(result))

    print(objective( [4.331888885679483, 4.015525820483823, -1.8196380191803363, -2.2239669971058285, 4.761185998106988, 2.82274528232427, -2.5489657182048475, 0.8582917656179292, -1.1498584772982157, -0.03635678022747646, 2.7468064795215206, -1.014327239777304, 0.07796845624754889, 2.1024165209624446, 3.5658330050631717]))

def test_evolve_double():
    print("Testing evolve_double...")
    def fitness(params):
        return -sum(x**2 for x in params)
    def mutate(params):
        return [x + 0.1 for x in params]
    def generate():
        return [0.5, 0.5]
    result = mypackage.evolve_double(fitness, mutate, generate, generations=10)
    print("Result:", result)

def test_pso():
    print("Testing pso...")
    def objective(params):
        return sum(x**2 for x in params)
    space_min = [-1.0, -1.0]
    space_max = [1.0, 1.0]

    result = mypackage.pso(objective, space_min, space_max, iterations=10)
    print("Result:", result)

if __name__ == "__main__":
    # test_anneal_double()
    test_bayesopt_tpe()
    # test_evolve_double()
    # test_pso()
    # print("All tests passed.")
