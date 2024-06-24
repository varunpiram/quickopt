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
        return 0.999**iter
    def acceptance(new_val, current_val, temp):
        print("new_val:", new_val, "current_val:", current_val, "temp:", temp)
        print(mypackage.metropolis_hastings(new_val, current_val, temp))
        return 0.9
    
    result = mypackage.anneal_double(objective, initial_guess, neighbor, iterations=10000, verbose=1)
    print("Result:", result)

def test_bayesopt_tpe():
    print("Testing bayesopt_tpe...")
    def objective(params):
        return sum(x**2 for x in params)
    space_min = [0.0, 0.0]
    space_max = [1.0, 1.0]
    result = mypackage.bayesopt_tpe(objective, space_min, space_max, iterations=17, samples=11, verbose=1)
    print("Result:", result)

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
    space_min = [0.0, 0.0]
    space_max = [1.0, 1.0]
    result = mypackage.pso(objective, space_min, space_max, iterations=10)
    print("Result:", result)

if __name__ == "__main__":
    #test_anneal_double()
    # test_bayesopt_tpe()
    test_evolve_double()
    # test_pso()
    # print("All tests passed.")
