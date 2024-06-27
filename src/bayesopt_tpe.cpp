#define PYBIND11_DETAILED_ERROR_MESSAGES

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

// #include <pybind11/functional.h>
// #include <pybind11/complex.h>
// #include <pybind11/chrono.h>


namespace py = pybind11;

std::vector<double> bayesopt_tpe(
    py::function funct,
    std::vector<double> space_min,
    std::vector<double> space_max,
    int iterations = 100,
    int samples = 10,
    int acquisition_samples = 100,
    int verbose = 1,
    double threshold = 0.5,
    int group_cap = 9999,
    std::vector<double> bandwidth = std::vector<double>(),
    int prior_weight = 1,
    double split = -107.2931,
    const std::vector<std::pair<std::vector<double>, double>>& seed = std::vector<std::pair<std::vector<double>, double>>()
) {


    if (threshold > std::sqrt(samples)) {
        throw std::invalid_argument("Threshold value must be at most √(samples)!");
    }
    if (threshold <= 0) {
        throw std::invalid_argument("Threshold value must be positive!");
    }
    if (split != -107.2931 && (split < 0 || split > 1)) {
        throw std::invalid_argument("Split value must be within 0 and 1!");
    }

    bool dynam = false;
    if (bandwidth.size() != space_min.size()){
        dynam = true;
        for (int i = 0; i < space_min.size(); i++) {
            bandwidth.push_back(((space_max[i] - space_min[i]) / 5.0) * (pow(samples, (-1.0 / (space_min.size() + 4)))));
        }
    }

    std::mt19937 rng(std::random_device{}());

    class Candidate {
    public:
        std::vector<double> parameters;
        double value;
        double weights;

        Candidate(const std::vector<double>& p, double val, double w)
            : parameters(p), value(val), weights(w) {}
    };

    auto splitting = [threshold, split](double group_size) -> double {
        if (split < 0 || split > 1) {
            return (threshold / std::sqrt(static_cast<double>(group_size)));
        }
        return split;
    };


    std::vector<Candidate> dataset;
    dataset.reserve(samples);

    std::vector<std::uniform_real_distribution<double>> distributions;
    for (size_t j = 0; j < space_min.size(); j++) {
        distributions.emplace_back(space_min[j], space_max[j]);
    }

    double bestValue = std::numeric_limits<double>::max();
    std::vector<double> bestParameters(space_min.size(), 0);

    for (int i = 0; i < samples; i++) {
        if (i < seed.size()) {
            dataset.emplace_back(seed[i].first, seed[i].second, 0);
        } else {
            std::vector<double> parameters(space_min.size());
            for (size_t j = 0; j < space_min.size(); j++) {
                parameters[j] = distributions[j](rng);
            }
            double eval = funct(parameters).cast<double>();
            if (eval < bestValue) {
                bestValue = eval;
                bestParameters = parameters;
            }
            dataset.emplace_back(parameters, eval, 0);
        }
    }

    std::vector<Candidate> good;
    std::vector<Candidate> bad;

    int toReserve = std::max(2, static_cast<int>(std::floor(static_cast<double>(samples) * (splitting(samples)))));

    good.reserve(toReserve);
    bad.reserve(samples - toReserve);

    std::unordered_map<int, double> denom_dict;

    auto denom_weight = [&denom_dict](std::vector<Candidate>& good) -> double {

        // Something's wrong here 


        int good_size = good.size();
        double yq = good.back().value;

        if (denom_dict.find(good_size) != denom_dict.end()) {
            return denom_dict[good_size];
        }

        double denominator = 0;

        for (Candidate &c : good) {
            denominator += (1.0 / (1.0 + good_size)) * (yq - c.value);
        }

        denom_dict[good_size] = denominator;
        return denominator;
    };

    auto weight = [&denom_weight](bool isGood, std::vector<Candidate>& good, std::vector<Candidate>& bad, Candidate& c) -> double {
        if (!isGood) {
            return 1.0 / (bad.size() + 1);
        }
        double denominator = denom_weight(good);
        return (good.back().value - c.value) / denominator;
    };

    auto weight_base = [&denom_weight](std::vector<Candidate>& good) -> double {
        double baseweight = 0;
        for (const Candidate& candidate : good) {

            baseweight += denom_weight(good) * candidate.weights;

        }

        return baseweight / good.size();
    };

    auto nonInformativePrior = [&space_min, &space_max](double param, int ind) -> double {
        double left = space_min[ind];
        double right = space_max[ind];
        double mean = (left + right) / 2.0;
        double sigma = (right - left) / 6.0;
        double exponent = -std::pow(param - mean, 2) / (2 * std::pow(sigma, 2));
        double pdf = (1.0 / (sigma * std::sqrt(2 * M_PI))) * std::exp(exponent);
        return pdf;
    };

    auto nIp = [&nonInformativePrior, prior_weight](const std::vector<double>& params) -> double {
        double runningProd = 1;
        for (size_t i = 0; i < params.size(); i++) {
            runningProd *= nonInformativePrior(params[i], i);
        }
        return runningProd * prior_weight;
    };

    auto gaussianKernel = [](double x, double xn, double bw) -> double {
        return (1.0 / std::sqrt(2 * M_PI * bw * bw)) *
               std::exp(-std::pow(x - xn, 2) / (2 * bw * bw));
    };

    auto kde = [&gaussianKernel, &bandwidth](const std::vector<double>& params, const std::vector<Candidate>& candidates) -> double {
        double runningSum = 0;
        for (const Candidate& c : candidates) {
            double runningProd = 1;
            for (size_t i = 0; i < params.size(); i++) {

                runningProd *= gaussianKernel(params[i], c.parameters[i], bandwidth[i]);
            }
            runningSum += runningProd * c.weights;
        }
        return runningSum / candidates.size();
    };

    for (int i = 1; i < iterations; i++) {
        toReserve = std::max(2, static_cast<int>(std::floor(dataset.size() * (splitting(dataset.size())))));

        std::sort(dataset.begin(), dataset.end(), [](const Candidate& a, const Candidate& b) {
            return a.value < b.value;
        });

        good.clear();
        bad.clear();

        for (size_t j = 0; j < dataset.size(); j++) {
            if (j < static_cast<size_t>(toReserve)) {
                good.push_back(dataset[j]);
            } else {
                bad.push_back(dataset[j]);
            }
        }


        for (Candidate &c : good) {
            c.weights = weight(true, good, bad, c);
        }

        for (Candidate &c : bad) {
            c.weights = weight(false, good, bad, c);


        }


        double w0 = weight_base(good);

        auto acquisition = [&good, &bad, &kde, &nIp, w0](const std::vector<double>& params) -> double {
            double badAcq = kde(params, bad);
            double goodAcq = kde(params, good);
            double p0 = nIp(params);
            return (goodAcq + w0 * p0) / (badAcq + w0 * p0);
        };

        std::vector<std::vector<double>> acsamples;
        acsamples.reserve(acquisition_samples);

        for (int j = 0; j < acquisition_samples; j++) {
            std::vector<double> sample(space_min.size());
            for (size_t k = 0; k < space_min.size(); k++) {
                sample[k] = distributions[k](rng);
            }

            acsamples.push_back(std::move(sample));
            
        }

        double bestAcq = -1 * std::numeric_limits<double>::max();
        std::vector<double> bestSample;

        for (const std::vector<double>& sample : acsamples) {

            double acq = acquisition(sample);

            if (acq > bestAcq) {
                bestAcq = acq;
                bestSample = sample;

            }
        }

        std::vector<double> newParameters(bestSample.begin(), bestSample.end());

        double eval = funct(newParameters).cast<double>();
        if (eval < bestValue) {
            bestValue = eval;
            bestParameters = newParameters;
        }
        dataset.emplace_back(newParameters, eval, 0);

        if (verbose == 1) {
            std::cout << "Iteration " << i << ": " << eval << std::endl;
            std::cout << "Best value: " << bestValue << std::endl;
        }

        if (dynam) {
            for (size_t i = 0; i < space_min.size(); i++) {
                bandwidth[i] = ((space_max[i] - space_min[i]) / 5.0) * (pow(dataset.size(), (-1.0 / (space_min.size() + 4))));
            }
        }
    }

    return bestParameters;
}

PYBIND11_MODULE(bayesopt_tpe, m) {
    m.def("bayesopt_tpe", &bayesopt_tpe,
        py::arg("funct"),
        py::arg("space_min"),
        py::arg("space_max"),
        py::arg("iterations") = 100,
        py::arg("samples") = 10,
        py::arg("acquisition_samples") = 100,
        py::arg("verbose") = 1,
        py::arg("threshold") = 0.5,
        py::arg("group_cap") = 9999,
        py::arg("bandwidth") = std::vector<double>(),
        py::arg("prior_weight") = 1,
        py::arg("split") = -107.2931,
        py::arg("seed") = std::vector<std::pair<std::vector<double>, double>>(),
        "Uses Bayesian Optimization with Tree-structured Parzen Estimator to find the minimum of a function"
    );
}
