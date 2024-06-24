# Clean previous builds
python setup.py clean --all
rm -rf build/ dist/ mypackage.egg-info
find mypackage -name "*.so" -type f -delete
rm -rf mypackage/__pycache__

# Build the extensions in place
python setup.py build_ext --inplace

# Install the package
pip install .



