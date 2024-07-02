"""
A Python optimization toolkit focused on global optimization problems, featuring simple and customizable setup of various optimization algorithms implemented in C++.
"""

__version__ = '0.1.6'

from .common import DoubleVector, IntVector, StringVector
from .annealing import anneal_double, anneal_int, anneal_string
from .bayesopt_tpe import bayesopt_tpe
from .genetic import genetic_double, genetic_int, genetic_string
from .pso import pso
