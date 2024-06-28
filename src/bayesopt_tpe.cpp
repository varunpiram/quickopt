#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <limits>
#include <type_traits>

namespace py = pybind11;

// Bayesian Optimization algorithm using Tree-structured Parzen Estimator (TPE)
std::vector<double> bayesopt_tpe(
    py::function funct, // The function to be *minimized*
    std::vector<double> space_min, // Lower bound vector for the search space
    std::vector<double> space_max, // Upper bound vector for the search space
    int iterations = 100, // Number of iterations to run the algorithm for
    int samples = 10, // Number of samples to draw per iteration
    int acquisition_samples = 100, // Number of samples to draw for acquisition function maximization
    int verbose = 1, // Verbosity level - 0 for final output, 1 for output at each iteration
    double threshold = 0.5, // Threshold value for the splitting function - unneccesary if split is provided
    int group_cap = 9999, // Group cap for the good set
    std::vector<double> bandwidth = std::vector<double>(), // Optional bandwidth vector for the KDE (overrides dynamic bandwidth calculation)
    int prior_weight = 1, // Prior weight parameter for Non-Informative Prior
    double split = -107.2931, // Optional splitting threshold (overrides dynamic threshold calculation)
    const std::vector<std::pair<std::vector<double>, double>>& seed = std::vector<std::pair<std::vector<double>, double>>() // Seed data for the algorithm
) {

    // Check for invalid inputs
    if (threshold > std::sqrt(samples)) { // If the threshold is greater than the square root of the number of samples...
        throw std::invalid_argument("Threshold value must be at most √(samples)!"); // Throw an exception
    }
    if (threshold <= 0) { // If the threshold is less than or equal to 0...
        throw std::invalid_argument("Threshold value must be positive!"); // Throw an exception
    }
    if (split != -107.2931 && (split < 0 || split > 1)) { // If the split value is not the default value and is not within the range [0, 1]...
        throw std::invalid_argument("Split value must be within 0 and 1!"); // Throw an exception
    }
    if (group_cap < 2) { // If the group cap is less than 2...
        throw std::invalid_argument("Group cap must be over 2!"); // Throw an exception
    }
    if (space_min.size() != space_max.size()) { // If the size of the lower bound vector is not equal to the size of the upper bound vector...
        throw std::invalid_argument("Lower and upper bound vectors must be of the same size!"); // Throw an exception
    }
    if (bandwidth.size() != 0 && bandwidth.size() != space_min.size()) { // If the bandwidth vector is not empty and is not the same size as the search space...
        throw std::invalid_argument("Bandwidth vector must be empty or of the same size as the search space!"); // Throw an exception
    }
    for (int i = 0; i < space_min.size(); i++) { // For each dimension in the search space...
        if (space_min[i] >= space_max[i]) { // If the lower bound is greater than or equal to the upper bound...
            throw std::invalid_argument("Lower bound must be less than upper bound for all dimensions!"); // Throw an exception
        }
    }
    if (iterations <= 0) { // If the number of iterations is less than or equal to 0...
        throw std::invalid_argument("Number of iterations must be positive!"); // Throw an exception
    }
    if (samples <= 0) { // If the number of samples is less than or equal to 0...
        throw std::invalid_argument("Number of samples must be positive!"); // Throw an exception
    }
    if (acquisition_samples <= 0) { // If the number of acquisition samples is less than or equal to 0...
        throw std::invalid_argument("Number of acquisition samples must be positive!"); // Throw an exception
    }
    

    bool dynam = false; // Set the dynamic bandwidth flag to false
    if (bandwidth.size() != space_min.size()){ // If the bandwidth vector is empty...
        dynam = true; // Set the dynamic bandwidth flag to true
        for (int i = 0; i < space_min.size(); i++) { // For each dimension in the search space...
            bandwidth.push_back(((space_max[i] - space_min[i]) / 5.0) * (pow(samples, (-1.0 / (space_min.size() + 4))))); // Calculate the initial bandwidth and add it to the bandwidth vector
            // This calculation is based on the heuristic in appendix C.3.3 of Watanbe 2023
        }
    }

    std::mt19937 rng(std::random_device{}()); // Initialize the random number generator

    class Candidate { // Define a Candidate class
    public: // Public access specifier
        std::vector<double> parameters; // Vector of parameters
        double value; // Value of the candidate
        double weights; // Weights of the candidate

        Candidate(const std::vector<double>& p, double val, double w) // Constructor for the Candidate class
            : parameters(p), value(val), weights(w) {} // Initialize the parameters, value, and weights
    };

    auto splitting = [threshold, split](double group_size) -> double { // Define a lambda function for splitting
        if (split < 0 || split > 1) { // If the split value is not within the range [0, 1] (eg. it isn't provided)... 
            return (threshold / std::sqrt(static_cast<double>(group_size))); // Return the threshold divided by the square root of the group size
            // This calculation is based on the square root splitting algorithm used in Bergstra et al. 2013
        }
        return split;
    };


    std::vector<Candidate> dataset; // Vector of candidates
    dataset.reserve(samples); // Reserve space for the candidates

    std::vector<std::uniform_real_distribution<double>> distributions; // Vector of uniform real distributions
    for (size_t j = 0; j < space_min.size(); j++) { // For each dimension in the search space...
        distributions.emplace_back(space_min[j], space_max[j]); // Add a uniform real distribution with the corresponding lower and upper bounds
    }

    double bestValue = std::numeric_limits<double>::max(); // Initialize the best value to the maximum double value
    std::vector<double> bestParameters(space_min.size(), 0); // Initialize the best parameters to zeros

    for (int i = 0; i < samples; i++) { // For each sample...
        if (i < seed.size()) { // If the sample is within the seed data...
            dataset.emplace_back(seed[i].first, seed[i].second, 0); // Add the seed data to the dataset
        } else { // If not...
            std::vector<double> parameters(space_min.size()); // Create a vector parameters of the same size as the search space
            for (size_t j = 0; j < space_min.size(); j++) { // For each dimension in the search space...
                parameters[j] = distributions[j](rng); // Generate a random parameter value using the corresponding distribution
            }
            double eval = funct(parameters).cast<double>(); // Evaluate the function at the generated parameters
            if (eval < bestValue) { // If the evaluation is better than the best value...
                bestValue = eval; // Update the best value
                bestParameters = parameters; // Update the best parameters
            }
            dataset.emplace_back(parameters, eval, 0); // Add the generated parameters, evaluation, and weights to the dataset as a candidate
        }
    }

    std::vector<Candidate> good; // Vector to hold set of good candidates
    std::vector<Candidate> bad; // Vector to hold set of bad candidates

    int toReserve = std::min(std::max(2, static_cast<int>(std::floor(static_cast<double>(samples) * (splitting(samples))))), group_cap); // Calculate the number of samples to reserve
    // Note we cap the number of samples to reserve at group_cap, and we ensure that at least 2 samples are reserved

    good.reserve(toReserve); // Reserve space for the good candidates
    bad.reserve(samples - toReserve); // Reserve space for the bad candidates

    std::unordered_map<int, double> denom_dict; // Memoization dictionary to hold denominator values in weight calculations

    auto denom_weight = [&denom_dict](std::vector<Candidate>& good) -> double { // Define a helper lambda function to calculate the denominator in the weight calculation

        int good_size = good.size(); // Get the size of the good candidates
        double yq = good.back().value; // Get the value of the last good candidate

        if (denom_dict.find(good_size) != denom_dict.end()) { // If the denominator value is already in the memoization dictionary...
            return denom_dict[good_size]; // Return the value from the dictionary
        }

        // If not...
        double denominator = 0; // Initialize the denominator to zero

        for (Candidate &c : good) { // For each good candidate...
            denominator += (1.0 / (1.0 + good_size)) * (yq - c.value); // Add on their denominator value
        }

        denom_dict[good_size] = denominator; // Add the denominator value to the memoization dictionary
        return denominator; // Return the denominator value
    };


    auto weight = [&denom_weight](bool isGood, std::vector<Candidate>& good, std::vector<Candidate>& bad, Candidate& c) -> double { // Define a lambda function to calculate the weight of a candidate
    // This weight is based on the expected improvement acqusition function shown in Song et al. 2022

        if (!isGood) { // If the candidate is in the bad set...
            return 1.0 / (bad.size() + 1); // Return the weight scaled by the size of the bad set
        }

        // If not...
        double denominator = denom_weight(good);  // Calculate the denominator value using our helper function
        return (good.back().value - c.value) / denominator; // Return the weight based on the expected improvement acquisition function
    };

    auto weight_base = [&denom_weight](std::vector<Candidate>& good) -> double { // Define a lambda function to calculate the weight for the zeroth basis in the KDE
    // This weight is based on the expected improvement acqusition function shown in Song et al. 2022

        double baseweight = 0; // Initialize the base weight to zero

        for (const Candidate& candidate : good) { // For each good candidate...
            baseweight += denom_weight(good) * candidate.weights; // Add on the weight of the candidate
        }

        return baseweight / good.size(); // Return the average weight of all good candidates
    };

    auto nonInformativePrior = [&space_min, &space_max](double param, int ind) -> double { // Define a lambda function to calculate the non-informative prior for an individual dimension
    // This prior is based on a Gaussian distribution with a mean at the midpoint of the search space and a standard deviation of the search space width
    // This is based on Watanabe 2023
        double left = space_min[ind]; // Get the lower bound of the search space
        double right = space_max[ind]; // Get the upper bound of the search space
        double mean = (left + right) / 2.0; // Calculate the mean of the search space
        double sigma = (right - left); // Calculate the standard deviation of the search space
        double exponent = -std::pow(param - mean, 2) / (2 * std::pow(sigma, 2)); // Calculate the exponent of the Gaussian distribution
        double pdf = (1.0 / (sigma * std::sqrt(2 * M_PI))) * std::exp(exponent); // Calculate the probability density function of the Gaussian distribution
        return pdf;
    };

    auto nIp = [&nonInformativePrior, prior_weight](const std::vector<double>& params) -> double { // Define a lambda function to calculate the non-informative prior for a set of parameters
        double runningProd = 1; // Initialize the running product to one
        for (size_t i = 0; i < params.size(); i++) { // For each dimension...
            runningProd *= nonInformativePrior(params[i], i); // Multiply the running product by the non-informative prior for that dimension
        }
        return runningProd * prior_weight; // Return the running product scaled by the prior weight
    };

    auto gaussianKernel = [](double x, double xn, double bw) -> double { // Define a lambda function to calculate the Gaussian kernel given a bandwidth hyperparameter
    // This kernel is described in Watanbe 2023

        return (1.0 / std::sqrt(2 * M_PI * bw * bw)) * // Calculate the Gaussian kernel
               std::exp(-std::pow(x - xn, 2) / (2 * bw * bw)); // Calculate the exponent of the Gaussian kernel
    };

    auto kde = [&gaussianKernel, &bandwidth](const std::vector<double>& params, const std::vector<Candidate>& candidates) -> double { // Define a lambda function to calculate the kernel density estimate
    // This is based on the multivariate kernel density estimation used in Falkner et al. 2018

        double runningSum = 0; // Initialize the running sum to zero
        for (const Candidate& c : candidates) { // For each candidate...
            double runningProd = 1; // Initialize the running product to one
            for (size_t i = 0; i < params.size(); i++) { // For each dimension...
                runningProd *= gaussianKernel(params[i], c.parameters[i], bandwidth[i]); // Multiply the running product by the Gaussian kernel
            } 
            runningSum += runningProd * c.weights; // Multiply the running product by the weight of the candidate and add it to the running sum
        }
        return runningSum / candidates.size(); // Return running sum scaled by the number of candidates
    };

    for (int i = 1; i < iterations; i++) { // For each iteration...

        toReserve = std::min(std::max(2, static_cast<int>(std::floor(dataset.size() * (splitting(dataset.size()))))), group_cap); // Recalculate the number of candidates to reserve

        std::sort(dataset.begin(), dataset.end(), [](const Candidate& a, const Candidate& b) { // Sort the dataset by value
            return a.value < b.value; // Lower values first
        });

        good.clear(); // Clear the good set
        bad.clear(); // Clear the bad set

        for (size_t j = 0; j < dataset.size(); j++) { // For each candidate in the dataset...
            if (j < static_cast<size_t>(toReserve)) { // If there is still space in the good set...
                good.push_back(dataset[j]); // Add the candidate to the good set
            } else { // If not...
                bad.push_back(dataset[j]); // Add the candidate to the bad set
            }
        }


        for (Candidate &c : good) { // For each candidate in the good set...
            c.weights = weight(true, good, bad, c); // Calculate the weight of the candidate
        }

        for (Candidate &c : bad) { // For each candidate in the bad set...
            c.weights = weight(false, good, bad, c); // Calculate the weight of the candidate
        }


        double w0 = weight_base(good); // Calculate the weight of the zeroth basis in the KDE

        auto acquisition = [&good, &bad, &kde, &nIp, w0](const std::vector<double>& params) -> double { // Define a lambda function to calculate the acquisition function's value for given parameters
            double badAcq = kde(params, bad); // Calculate the kernel density estimate for the bad candidates
            double goodAcq = kde(params, good); // Calculate the kernel density estimate for the good candidates
            double p0 = nIp(params); // Calculate the non-informative prior for the parameters
            return (goodAcq + w0 * p0) / (badAcq + w0 * p0); // Return the acquisition function's values
        };

        std::vector<std::vector<double>> acsamples; // Initialize the acquisition samples vector
        acsamples.reserve(acquisition_samples); // Reserve space for the acquisition samples

        for (int j = 0; j < acquisition_samples; j++) { // For each acquisition sample...
            std::vector<double> sample(space_min.size()); // Initialize the sample vector
            for (size_t k = 0; k < space_min.size(); k++) { // For each dimension...
                sample[k] = distributions[k](rng); // Generate a random value within the search space
            }
            acsamples.push_back(std::move(sample)); // Add the random sample to the acquisition samples
        }

        double bestAcq = -1 * std::numeric_limits<double>::max(); // Initialize the best acquisition value to negative infinity
        std::vector<double> bestSample; // Initialize the best sample vector

        for (const std::vector<double>& sample : acsamples) { // For each acquisition sample...
            double acq = acquisition(sample); // Calculate the acquisition function's value for the sample
            if (acq > bestAcq) { // If the acquisition value is better than the best acquisition value...
                bestAcq = acq; // Update the best acquisition value
                bestSample = sample; // Update the best sample (parameters)
            }
        }

        std::vector<double> newParameters(bestSample.begin(), bestSample.end()); // Initialize the new parameters vector using the best sample

        double eval = funct(newParameters).cast<double>(); // Evaluate the function at the new parameters
        if (eval < bestValue) { // If the evaluation is better than the best value...
            bestValue = eval; // Update the best value
            bestParameters = newParameters; // Update the best parameters
        }
        dataset.emplace_back(newParameters, eval, 0); // Add the new candidate to the dataset

        if (verbose == 1) { // If verbose output is enabled...
            std::cout << "Iteration " << i << ": " << eval << std::endl; // Print the iteration number and evaluation
            std::cout << "Best value: " << bestValue << std::endl; // Print the best value
        }

        if (dynam) { // If dynamic bandwidth is enabled...
            for (size_t i = 0; i < space_min.size(); i++) {  // For each dimension...
                bandwidth[i] = ((space_max[i] - space_min[i]) / 5.0) * (pow(dataset.size(), (-1.0 / (space_min.size() + 4)))); // Update the bandwidth
                // // This calculation is based on the heuristic in appendix C.3.3 of Watanbe 2023
            }
        }
    }

    return bestParameters; // Return the best parameters
}

PYBIND11_MODULE(bayesopt_tpe, m) { // Define the Python module
    m.def("bayesopt_tpe", &bayesopt_tpe, // Expose the bayesopt_tpe function to Python
        py::arg("funct"), // Define the function argument
        py::arg("space_min"), // Define the space_min argument
        py::arg("space_max"), // Define the space_max argument
        py::arg("iterations") = 100, // Define the iterations argument with a default value
        py::arg("samples") = 10, // Define the samples argument with a default value
        py::arg("acquisition_samples") = 100, // Define the acquisition_samples argument with a default value
        py::arg("verbose") = 1, // Define the verbose argument with a default value
        py::arg("threshold") = 0.5, // Define the threshold argument with a default value
        py::arg("group_cap") = 9999, // Define the group_cap argument with a default value
        py::arg("bandwidth") = std::vector<double>(), // Define the bandwidth argument with a default value
        py::arg("prior_weight") = 1, // Define the prior_weight argument with a default value
        py::arg("split") = -107.2931, // Define the split argument with a default value
        py::arg("seed") = std::vector<std::pair<std::vector<double>, double>>(), // Define the seed argument with a default value
        "Uses Bayesian Optimization with Tree-structured Parzen Estimator to find the minimum of a function" // Define the function's docstring
    );
}
