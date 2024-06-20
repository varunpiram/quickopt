from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext
from pybind11 import get_include

ext_modules = [
    Pybind11Extension(
        "mypackage.annealing",
        ["mypackage/annealing.cpp"],
        include_dirs=[
            get_include(),  # Path to pybind11 headers
        ],
        language='c++'
    ),
    Pybind11Extension(
        "mypackage.evolution",
        ["mypackage/evolution.cpp"],
        include_dirs=[
            get_include(),  # Path to pybind11 headers
        ],
        language='c++'
    ),
    Pybind11Extension(
        "mypackage.bayesopt_tpe",
        ["mypackage/bayesopt_tpe.cpp"],
        include_dirs=[
            get_include(),  # Path to pybind11 headers
        ],
        language='c++'
    ),
    Pybind11Extension(
        "mypackage.pso",
        ["mypackage/pso.cpp"],
        include_dirs=[
            get_include(),  # Path to pybind11 headers
        ],
        language='c++'
    ),
]

setup(
    name='mypackage',
    version='0.1.0',
    author='Varun Piram',
    cmdclass={'build_ext': build_ext},
    ext_modules=ext_modules,
)
