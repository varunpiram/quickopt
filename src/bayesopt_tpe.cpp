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

namespace py = pybind11;

template <typename T>
std::vector<T> bayesopt_tpe(
    py::function funct,
    const std::vector<T>& space_min,
    const std::vector<T>& space_max,
    int iterations = 100,
    int samples = 10,
    int acquisition_samples = 100,
    int verbose = 1,
    double threshold = 0.5,
    int group_cap = 9999,
    std::vector<double> bandwidth = std::vector<double>(),
    int prior_weight = 1,
    py::function splitting = py::cpp_function([](int key){
        if (key == 1234567890) {
            return 2 * 1234567890;
        }
        return key;
    }),
    py::function weight = py::cpp_function([](int key){
        if (key == 1234567890) {
            return 2 * 1234567890;
        }
        return key;
    }),
    py::function weight_base = py::cpp_function([](int key){
        if (key == 1234567890) {
            return 2 * 1234567890;
        }
        return key;
    }),
    const std::vector<std::pair<std::vector<double>, double>>& seed = std::vector<std::pair<std::vector<double>, double>>()
) {

    if (threshold > std::sqrt(samples)) {
        throw std::invalid_argument("Threshold value must be at most √(samples)!");
    }
    if (threshold <= 0) {
        throw std::invalid_argument("Threshold value must be positive!");
    }

    bool dynam = false;
    if (bandwidth.empty()){
        dynam = true;
        for (size_t i = 0; i < space_min.size(); i++) {
            bandwidth.push_back(((space_max[i] - space_min[i]) / 5) * (1 / (pow(samples, (space_min.size() * 4)))));
        }
    }

    std::mt19937 rng(std::random_device{}());

    class Candidate {
    public:
        std::vector<T> parameters;
        double value;
        double weight;

        Candidate(const std::vector<T>& p, double val, double w)
            : parameters(p), value(val), weight(w) {}
    };

    auto default_splitting = [threshold](double group_size) -> double {
        return threshold / std::sqrt(static_cast<double>(group_size));
    };

    if(splitting(1234567890).cast<int>() == 2 * 1234567890){
        splitting = py::cpp_function(default_splitting);
    }

    std::vector<Candidate> dataset;
    dataset.reserve(samples);
    std::vector<std::uniform_real_distribution<T>> distributions;
    for (size_t j = 0; j < space_min.size(); j++) {
        distributions.emplace_back(space_min[j], space_max[j]);
    }

    double bestValue = std::numeric_limits<double>::max();

    std::vector<T> bestParameters(space_min.size(), 0);

    for (int i = 0; i < samples; i++) {
        if (i < seed.size()) {
            dataset.emplace_back(seed[i].first, seed[i].second, 0);
        } else {
            std::vector<T> parameters(space_min.size());
            for (size_t j = 0; j < space_min.size(); j++) {
                parameters[j] = distributions[j](rng);
            }
            double eval = funct(parameters).template cast<double>();
            if (eval < bestValue) {
                bestValue = eval;
                bestParameters = parameters;
            }
            dataset.emplace_back(parameters, eval, 0);
        }
    }

    std::vector<Candidate> good;
    int toReserve = static_cast<int>(std::floor(samples / splitting(samples).template cast<double>()));
    good.reserve(toReserve);
    std::vector<Candidate> bad;
    bad.reserve(samples - toReserve);

    std::unordered_map<int, double> denom_dict;

    auto denom_weight = [&denom_dict](const std::vector<Candidate>& good) -> double {
        int good_size = good.size();
        double yq = good.back().value;

        if (denom_dict.find(good_size) != denom_dict.end()) {
            return denom_dict[good_size];
        }

        double denominator = 0;
        for (const Candidate& c : good) {
            denominator += (1.0 / (1.0 + good_size)) * (yq - c.value);
        }
        denom_dict[good_size] = denominator;
        return denominator;
    };

    auto weight_default = [&denom_weight](bool isGood, const std::vector<Candidate>& good, const std::vector<Candidate>& bad, const Candidate& c) -> double {
        if (!isGood) {
            return 1.0 / (bad.size() + 1);
        }
        double denominator = denom_weight(good);
        return (good.back().value - c.value) / denominator;
    };

    auto weight_base_default = [&denom_weight](const std::vector<Candidate>& good, const std::vector<Candidate>& bad) -> double {
        double baseweight = 0;
        for (const Candidate& candidate : good) {
            baseweight += denom_weight(good) * candidate.weight;
        }
        return baseweight / good.size();
    };

    if(weight(1234567890).cast<int>() == 2 * 1234567890){
        weight = py::cpp_function(weight_default);
    }


    if(weight_base(1234567890).cast<int>() == 2 * 1234567890){
        weight_base = py::cpp_function(weight_base_default);
    }

    auto nonInformativePrior = [&space_min, &space_max](double param, int ind) -> double {
        double left = space_min[ind];
        double right = space_max[ind];
        double mean = (left + right) / 2;
        double sigma = (right - left) / 6;
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

    auto gaussianKernel = [](double x, double xn, double bandwidth) -> double {
        return (1.0 / std::sqrt(2 * M_PI * bandwidth * bandwidth)) *
               std::exp(-std::pow(x - xn, 2) / (2 * bandwidth * bandwidth));
    };

    auto kde = [&gaussianKernel, &bandwidth](const std::vector<double>& params, const std::vector<Candidate>& candidates) -> double {
        double runningSum = 0;
        for (const Candidate& c : candidates) {
            double runningProd = 1;
            for (size_t i = 0; i < params.size(); i++) {
                runningProd *= gaussianKernel(params[i], c.parameters[i], bandwidth[i]);
            }
            runningSum += runningProd * c.weight;
        }
        return runningSum / candidates.size();
    };

    for (int i = 0; i < iterations; i++) {
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

        for (Candidate& c : good) {
            c.weight = weight(true, good, bad, c).template cast<double>();
        }
        for (Candidate& c : bad) {
            c.weight = weight(false, good, bad, c).template cast<double>();
        }

        double w0 = weight_base(good, bad).template cast<double>();

        auto acquisition = [&good, &bad, &kde, &nIp, w0](const std::vector<double>& params) -> double {
            double goodAcq = kde(params, good);
            double badAcq = kde(params, bad);
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

        double bestAcq = -std::numeric_limits<double>::max();
        std::vector<double> bestSample;
        for (const std::vector<double>& sample : acsamples) {
            double acq = acquisition(sample);
            if (acq > bestAcq) {
                bestAcq = acq;
                bestSample = sample;
            }
        }

        std::vector<T> newParameters(bestSample.begin(), bestSample.end());
        double eval = funct(newParameters).template cast<double>();
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
                bandwidth[i] = ((space_max[i] - space_min[i]) / 5) * (1 / (pow(dataset.size(), (space_min.size() * 4))));
            }
        }
    }

    return bestParameters;
}

PYBIND11_MODULE(bayesopt_tpe, m) {
    m.def("bayesopt_tpe", &bayesopt_tpe<double>,
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
        py::arg("splitting") = py::cpp_function([](int key){
        if (key == 1234567890) {
            return 2 * 1234567890;
        }
        return key;
    }),
        py::arg("weight") = py::cpp_function([](int key){
        if (key == 1234567890) {
            return 2 * 1234567890;
        }
        return key;
    }),
        py::arg("weight_base") = py::cpp_function([](int key){
        if (key == 1234567890) {
            return 2 * 1234567890;
        }
        return key;
    }),
        py::arg("seed") = std::vector<std::pair<std::vector<double>, double>>(),
        "Uses Bayesian Optimization with Tree-structured Parzen Estimator to find the minimum of a function"
    );
}
