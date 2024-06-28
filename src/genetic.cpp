#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <type_traits>

namespace py = pybind11;

// Genetic algorithm function
template <typename T>
std::vector<T> genetic(
    py::function fitness, // Fitness function - takes in a set of parameters and returns a fitness value
    py::function mutate,  // Mutation function - takes in a set of parameters and returns a mutated set of parameters
    py::function generate, // Generate function - generates a set of parameters to be used in the initial population (ideally randomly)
    std::vector<std::vector<T>> seed = std::vector<std::vector<T>>(), // Seed population - a predefined set of parameters to be used in the initial population
    int population_size = 100, // Population size - more is better, but slower
    int reproduction_ct = 10,  // Number of individuals that will reproduce each generation - more leads to more diversity, but slower convergence
    int survivor_ct = 0, // Number of individuals that will survive each generation - more leads to more stability, but could harm diversity
    double mutation_rate = 0.1, // Mutation rate - the probability that a mutation will occur on a given individual
    int generations = 100, // Number of generations to run the algorithm for
    int verbose = 1 // Verbosity level - 0 for no output, 1 for average and top fitness each generation
) {
    
    // Individual class to store parameters and fitness value
    class Individual {
    public:
        std::vector<T> params; // Parameters for each individual
        double fitness_value; // Fitness of each individual

        Individual(const std::vector<T>& params, py::function& fitness)  // Constructor to calculate the fitness value automatically
            : params(params), fitness_value(fitness(params).template cast<double>()) {
        }
    };

    if (reproduction_ct > population_size) { // Check if reproduction count is greater than population size...
        throw std::invalid_argument("reproduction_ct cannot be greater than population_size"); // If so, throw an exception
    }

    std::vector<Individual> population; // Population vector - defined as a set of individuals
    population.reserve(population_size); // Reserve space for the population based on population size
    std::mt19937 rng(std::random_device{}()); // Random number generator
    std::uniform_int_distribution<> dist(0, population_size - 1); // Uniform distribution for random integers

    // Populate initial population
    while (population.size() < population_size) { // While the population is not full...
        if (!seed.empty() && population.size() < seed.size()) { // If there are parameters within the seed...
            population.emplace_back(seed[population.size()], fitness); // Add those parameters to the population as an individual (using emplace_back)
        } else {
            population.emplace_back(generate().template cast<std::vector<T>>(), fitness);  // Generate random parameters and add them to the population as an individual
        }
    }

    // Crossover function, taking in two individuals as well as distance and random number generator functions
    auto crossover = [&dist, &rng](const Individual& father, const Individual& mother) {

        if (father.params.size() != mother.params.size()) { // If the father and mother have different numbers of parameters...
            throw std::invalid_argument("Father and mother must have the same number of parameters!"); // Throw an exception
        }

        size_t param_len = father.params.size(); // Get the number of parameters of the parents
        int split = dist(rng) % param_len; // Randomly select a split point for the crossover
        std::vector<T> child_params; // Create a vector to store the child's parameters
        child_params.reserve(param_len); // Reserve space for the child's parameters based on the number of parameters of the parents
        for (size_t i = 0; i < param_len; ++i) { // For each specific parameter value...
            child_params.push_back(i < split ? father.params[i] : mother.params[i]); // Add the parameter value from the father/mother, depending on index
        }
        return child_params; // Return the child's parameters
    };

    auto compare = [](const Individual& a, const Individual& b) { // Lambda function to compare two individuals based on fitness value
        return a.fitness_value > b.fitness_value; // Return true if the fitness value of individual a is greater than that of individual b
    };

    for (int generation = 0; generation < generations; ++generation) { // For each generation...
        std::sort(population.begin(), population.end(), compare); // Sort the population based on fitness value

        if (verbose == 1) { // If verbose is set to 1...
            double total_fitness = std::accumulate( // Calculate the total fitness of the population
                population.begin(), population.end(), 0.0, // Start with a sum of 0.0
                [](double sum, const Individual& ind) { return sum + ind.fitness_value; }); // Add the fitness value of each individual to the sum
            double avg_fitness = total_fitness / population.size(); // Calculate the average fitness of the population  
            std::cout << "Generation " << generation // Output the generation number
                      << ": Average Fitness: " << avg_fitness // Output the average fitness
                      << ", Top Fitness: " << population[0].fitness_value << std::endl; // Output the fitness of the top individual
        }

        std::vector<Individual> parents(population.begin(), population.begin() + reproduction_ct); // Select the top individuals to be parents based on reproduction_ct

        std::vector<Individual> children; // Create a vector to store the children
        children.reserve(population_size - survivor_ct); // Reserve space for the children based on the population size and survivor count
        while (children.size() < (population_size - survivor_ct)) { // While the children vector is not full...
            int father_idx = dist(rng) % reproduction_ct; // Randomly select an index to get a father
            int mother_idx = dist(rng) % reproduction_ct; // Randomly select an index to get a mother
            children.emplace_back(crossover(parents[father_idx], parents[mother_idx]), fitness); // Adds a child to the children vector by crossing over the selected parents
        }

        std::vector<Individual> survivors(population.begin(), population.begin() + survivor_ct); // Select the top individuals to be survivors based on survivor_ct

        population.clear(); // Empty the population vector
        population.reserve(population_size); // Re-reserve space for the population vector
        population.insert(population.end(), survivors.begin(), survivors.end()); // Add in survivors to the population
        population.insert(population.end(), children.begin(), children.end()); // Add in children to the population

        for (auto& ind : population) { // For each individual in the population...
            if (std::uniform_real_distribution<>(0.0, 1.0)(rng) < mutation_rate) { // If the random number is less than the mutation rate...
                ind.params = mutate(ind.params).template cast<std::vector<T>>(); // Mutate the individual's parameters
                ind.fitness_value = fitness(ind.params).template cast<double>(); // Update the fitness value of the individual
            }
        }
    }

    return population[0].params; // Return the parameters of the top individual in the final population
}

PYBIND11_MODULE(genetic, m) { // Define the Python module
    // Define a function to optimize a function of doubles
    m.def("genetic_double", &genetic<double>,
        py::arg("fitness"), // Define the fitness function argument
        py::arg("mutate"), // Define the mutation function argument
        py::arg("generate"), // Define the generation function argument
        py::arg("seed") = std::vector<std::vector<double>>(), // Define the seed argument
        py::arg("population_size") = 100, // Define the population size argument with a default value of 100
        py::arg("reproduction_ct") = 10, // Define the reproduction count argument with a default value of 10
        py::arg("survivor_ct") = 0, // Define the survivor count argument with a default value of 0
        py::arg("mutation_rate") = 0.1, // Define the mutation rate argument with a default value of 0.1
        py::arg("generations") = 100, // Define the number of generations argument with a default value of 100
        py::arg("verbose") = 1, // Define the verbose argument with a default value of 1
        "Evolves a population of doubles over a number of generations using a fitness function and mutation function" // Define the docstring
    );

    // Define a function to optimize a function of integers
    m.def("genetic_int", &genetic<int>,
        py::arg("fitness"), // Define the fitness function argument
        py::arg("mutate"), // Define the mutation function argument
        py::arg("generate"), // Define the generation function argument
        py::arg("seed") = std::vector<std::vector<int>>(), // Define the seed argument
        py::arg("population_size") = 100, // Define the population size argument with a default value of 100
        py::arg("reproduction_ct") = 10, // Define the reproduction count argument with a default value of 10
        py::arg("survivor_ct") = 0, // Define the survivor count argument with a default value of 0
        py::arg("mutation_rate") = 0.1, // Define the mutation rate argument with a default value of 0.1
        py::arg("generations") = 100, // Define the number of generations argument with a default value of 100
        py::arg("verbose") = 1, // Define the verbose argument with a default value of 1
        "Evolves a population of integers over a number of generations using a fitness function and mutation function" // Define the docstring
    );

    // Define a function to optimize a function of strings
    m.def("genetic_string", &genetic<std::string>, 
        py::arg("fitness"), // Define the fitness function argument
        py::arg("mutate"), // Define the mutation function argument
        py::arg("generate"), // Define the generation function argument
        py::arg("seed") = std::vector<std::vector<std::string>>(), // Define the seed argument
        py::arg("population_size") = 100, // Define the population size argument with a default value of 100
        py::arg("reproduction_ct") = 10, // Define the reproduction count argument with a default value of 10
        py::arg("survivor_ct") = 0, // Define the survivor count argument with a default value of 0
        py::arg("mutation_rate") = 0.1, // Define the mutation rate argument with a default value of 0.1
        py::arg("generations") = 100, // Define the number of generations argument with a default value of 100
        py::arg("verbose") = 1, // Define the verbose argument with a default value of 1
        "Evolves a population of strings over a number of generations using a fitness function and mutation function" // Define the docstring
    );
}
