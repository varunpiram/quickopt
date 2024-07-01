from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import os
import pybind11

class get_pybind_include(object):
    """Helper class to determine the pybind11 include path.
    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked."""

    def __init__(self, user=False):
        self.user = user

    def __str__(self):
        return pybind11.get_include(self.user)

ext_modules = [
    Extension(
        'quickopt.common',
        ['src/common.cpp'],
        include_dirs=[
            get_pybind_include(),
            get_pybind_include(user=True),
        ],
        language='c++',
        extra_compile_args=['-std=c++17'],
    ),
    Extension(
        'quickopt.annealing',
        ['src/annealing.cpp'],
        include_dirs=[
            get_pybind_include(),
            get_pybind_include(user=True),
        ],
        depends=['src/common.cpp'],
        language='c++',
        extra_compile_args=['-std=c++17'],
    ),
    Extension(
        'quickopt.bayesopt_tpe',
        ['src/bayesopt_tpe.cpp'],
        include_dirs=[
            get_pybind_include(),
            get_pybind_include(user=True),
        ],
        depends=['src/common.cpp'],
        language='c++',
        extra_compile_args=['-std=c++17'],
    ),
    Extension(
        'quickopt.genetic',
        ['src/genetic.cpp'],
        include_dirs=[
            get_pybind_include(),
            get_pybind_include(user=True),
        ],
        depends=['src/common.cpp'],
        language='c++',
        extra_compile_args=['-std=c++17'],
    ),
    Extension(
        'quickopt.pso',
        ['src/pso.cpp'],
        include_dirs=[
            get_pybind_include(),
            get_pybind_include(user=True),
        ],
        depends=['src/common.cpp'],
        language='c++',
        extra_compile_args=['-std=c++17'],
    )
]

setup(
    name='quickopt',
    version='0.1.0',
    author='Varun Piram',
    author_email='varunpiram@gmail.com',
    description='A Python optimization toolkit focused on global optimization problems, featuring simple and customizable setup of various optimization algorithms implemented in C++.',
    long_description=open('README.md').read(),
    long_description_content_type='text/markdown',
    ext_modules=ext_modules,
    install_requires=[
        'pybind11>=2.5.0',
    ],
    cmdclass={'build_ext': build_ext},
    zip_safe=False,
)
