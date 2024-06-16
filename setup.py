from setuptools import setup, Extension
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
]
setup(
    name='mypackage',
    version='0.1.0',
    author='Varun Piram',
    author_email='varun_piram@brown.edu',
    description='A project using pybind11',
    long_description='',
    ext_modules=ext_modules,
    install_requires=['pybind11>=2.5.0'],
    setup_requires=['pybind11>=2.5.0'],
    cmdclass={'build_ext': build_ext},
    zip_safe=False,
)
