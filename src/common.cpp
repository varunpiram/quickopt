// src/common.cpp
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(common, m) {
    py::class_<std::vector<double>>(m, "DoubleVector");
    py::class_<std::vector<int>>(m, "IntVector");
    py::class_<std::vector<std::string>>(m, "StringVector");
}
