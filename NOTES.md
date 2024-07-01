# To set up development:

## Clean previous builds
python setup.py clean --all
rm -rf build/ dist/ quickopt.egg-info
find quickopt -name "*.so" -type f -delete
rm -rf quickopt/__pycache__

## Build the extensions in place
python setup.py build_ext --inplace

## Install the package
pip install -e .

# To edit:

## Rebuild the extensions in place
python setup.py build_ext --inplace



