# mypackage/__init__.py

"""
A simple package for applying functions to a list of numbers.
"""

__version__ = '0.1.0'

from .common import DoubleVector, IntVector, StringVector
from .annealing import anneal_double, anneal_int, anneal_string
from .bayesopt_tpe import bayesopt_tpe
from .evolution import evolve_double, evolve_int, evolve_string
from .pso import pso
