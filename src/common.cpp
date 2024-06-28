#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

// Define the common module
PYBIND11_MODULE(common, m) {
    py::class_<std::vector<double>>(m, "DoubleVector"); // Define a vector of doubles
    py::class_<std::vector<int>>(m, "IntVector"); // Define a vector of integers
    py::class_<std::vector<std::string>>(m, "StringVector"); // Define a vector of strings
}
