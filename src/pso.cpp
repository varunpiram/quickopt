#include <pybind11/pybind11.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>
#include <limits>

namespace py = pybind11;

// Simulated particle swarm optimization function - ONLY WORKS FOR DOUBLES! Can modify eventually if needed!
template <typename T>
std::vector<T> pso(
    py::function funct, // The function to be *maximized*
    std::vector<T> space_min, // Lower bound vector for the search space
    std::vector<T> space_max, // Upper bound vector for the search space
    int iterations = 100,  // Number of iterations to run the algorithm for
    int swarm_size = 100, // Number of particles in the swarm
    double inertia = 0.5, // Inertia weight - recommended be in (0,1)
    double cognitive = 1.0, // Cognitive weight - recommended be in [1,3]
    double social = 2.0, // Social weight - recommended be in [1,3]
    double clamp = 0.1, // Clamping limit on velocity
    int verbose = 1 // Verbosity level - 0 for final output, 1 for output at each iteration
) {

    // Warn unusual inputs
    if (inertia <= 0 || inertia >= 1) {  // If the inertia is outside the suggested limits...
        std::cerr << "Inertia outside suggested limits! Recommended be in (0,1)" << std::endl; // Prints a warning message
    }
    if (cognitive <= 1 || cognitive >= 3) { // If the cognitive factor is outside the suggested limits...
        std::cerr << "Cognition factor outside suggested limits! Recommended be in [1,3]" << std::endl; // Prints a warning message
    }
    if (social <= 1 || social >= 3) { // If the social factor is outside the suggested limits...
        std::cerr << "Social factor outside suggested limits! Recommended be in [1,3]" << std::endl; // Prints a warning message
    }

    // Check for invalid inputs
    if (space_min.size() != space_max.size()) { // If the space minimum and maximum vectors are not the same size...
        throw std::invalid_argument("Search space vectors must be of same size!"); // Throw an exception
    }

    // Particle class - represents each particle in the swarm, storing its parameters (position), velocity, and value
    class Particle {
    public:
        std::vector<T> position; // Position of each particle 
        std::vector<T> velocity; // Velocity of each particle

        std::vector<T> pb_pos; // Personal best position of each particle

        double value; // Value of each particle

        double pb_val; // Personal best value of each particle
        
        // Constructor to initialize position, velocity, and value
        Particle(const std::vector<T>& p, const std::vector<T>& v, double val)
            : position(p), velocity(v), pb_pos(p), value(val), pb_val(val) {}
    };

    // Initialize the swarm
    std::vector<Particle> swarm; // Vector to store the particles in the swarm
    std::mt19937 rng(std::random_device{}()); // Random number generator
    std::uniform_real_distribution<> dist_space(0.0, 1.0); // Uniform distribution for initial space
    std::uniform_real_distribution<> dist_velocity(-clamp, clamp); // Uniform distribution for initial velocity
    std::uniform_real_distribution<> dist_inertia(0.0, 1.0); // Uniform distribution for random factors

    std::vector<T> gb_pos; // Initialize the global best position value
    double gb_val = std::numeric_limits<double>::lowest(); // Initialize the global best value

    // Initialize the particles
    for (int i = 0; i < swarm_size; ++i) { // For every proposed particle in swarm size...
        std::vector<T> position; // Create a vector to store position
        for (int j = 0; j < space_min.size(); ++j) { // For every individual parameter in the function...
            position.push_back(space_min[j] + dist_space(rng) * (space_max[j] - space_min[j])); // Generate a random position (input) parameter within the search space
        }
        std::vector<T> velocity; // Create a vector to store velocity
        for (int j = 0; j < space_min.size(); ++j) { // For every individual parameter in the function...
            velocity.push_back(dist_velocity(rng)); // Generate a random velocity parameter within the clamping limit
        }

        double value = funct(position).template cast<double>(); // Calculate the value of the particle
        
        if(value > gb_val) { // If the value is greater than the global best...
            gb_val = value; // Update the global best value
            gb_pos = position; // Update the global best position
        }

        swarm.push_back(Particle(position, velocity, value)); // Add the particle to the swarm
    }

    // Run the optimization
    for (int i = 0; i < iterations; ++i){ // For every iteration...
        for (auto& particle : swarm) { // For each particle in the swarm...
            std::vector<T> proposed_velocity; // Create a vector to store the new velocity
            std::vector<T> proposed_position; // Create a vector to store the new position

            // Generate a new velocity for the particle, and then generate the particle's proposed position based off it
            for (int j = 0; j < space_min.size(); ++j) { // For every individual parameter in the function...
                double r1 = dist_inertia(rng); // Generate a random factor r1
                double r2 = dist_inertia(rng); // Generate a random factor r2
                double proposed_velo_param = inertia * particle.velocity[j] + cognitive * r1 * (particle.pb_pos[j] - particle.position[j]) + social * r2 * (gb_pos[j] - particle.position[j]); // Calculate the new velocity parameter based on the velocity formula
                proposed_velocity.push_back(std::min(std::max(proposed_velo_param, -clamp), clamp)); // Update the velocity by adding the new velocity parameter and using clamping limit

                double proposed_pos_param = particle.position[j] + proposed_velocity[j]; // Calculate the new position parameter based on the velocity
                proposed_position.push_back(std::min(std::max(proposed_pos_param, space_min[j]), space_max[j])); // Update the position by adding the new position parameter and using search space limits
            }

            double new_value = funct(proposed_position).template cast<double>(); // Calculate the value of the new position

            particle.value = new_value; // Update the particle's value
            particle.velocity = proposed_velocity; // Update the velocity
            particle.position = proposed_position; // Update the particle's position

            if (new_value > particle.pb_val) { // If the new value is greater than the current personal best value...
                particle.pb_pos = proposed_position; // Update the personal best position
                particle.pb_val = new_value; // Update the personal best value
                if (new_value > gb_val) { // If the new value is greater than the global best value...
                    gb_val = new_value; // Update the global best value
                    gb_pos = proposed_position; // Update the global best position
                }
            }
        }

        if (verbose == 1) { // If the user wants verbose output...
            std::cout << "Iteration: " << i << " Current Best: " << gb_val << std::endl; // Output the global best value
        }
    }

    std::cout << "Best Value: " << gb_val << std::endl; // Output the global best value
    return gb_pos; // Return the global best position
}

PYBIND11_MODULE(pso, m) { // Define the Python module

    // Define a function to maximize a function of doubles
    m.def("pso", &pso<double>,
        py::arg("funct"), // Define the function to be minimized
        py::arg("space_min"), // Define the lower bounds of the search space
        py::arg("space_max"), // Define the upper bounds of the search space
        py::arg("iterations") = 100, // Define the number of iterations
        py::arg("swarm_size") = 100, // Define the number of particles in the swarm
        py::arg("inertia") = 0.5, // Define the inertia weight
        py::arg("cognitive") = 1.0, // Define the cognitive weight
        py::arg("social") = 2.0, // Define the social weight
        py::arg("clamp") = 1, // Define the clamping limit on velocity
        py::arg("verbose") = 1, // Define the verbosity with a default value of 1
        "Uses particle swarm optimization to maximize a function of double inputs" // Define the docstring
    );
}
